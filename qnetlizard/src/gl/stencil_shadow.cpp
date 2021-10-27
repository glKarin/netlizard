#include "stencil_shadow.h"

#include <QList>
#include <QDebug>
#include <QGLContext>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "glk.h"

#include "lib/line.h"
#include "lib/triangle.h"
#include "lib/mesa_gl_math.h"
#include "nlmath.h"

#define F_ZERO 0.1
#define SHADOW_VOLUME_LENGTH 5000
#define SHADOW_CAP_OFFSET 0.1
#define SHADOW_VOLUME_FAR_W 1
#define SHADOW_MASK 1 //
#define SHADOW_MASK_LIGHT 0 //

#define VEC3CMP vector3_equals // compare_vector3
#define LINECMP line_equals_ignore_seq // compare_line_segment

typedef QList<line_s> LineList;
typedef QList<vector3_s> Vector3List;
typedef QList<triangle_s> TriangleList;
typedef struct _Shadow_Volume_s
{
    LineList lines;
    TriangleList tops;
    TriangleList bottoms;
} Shadow_Volume_s;

static int compare_vector3(const vector3_s *a, const vector3_s *b)
{
    float xd, yd, zd;

    if(!a || !b)
        return 0;
    xd = VECTOR3V_X(a) - VECTOR3V_X(b);
    yd = VECTOR3V_Y(a) - VECTOR3V_Y(b);
    zd = VECTOR3V_Z(a) - VECTOR3V_Z(b);

    return(
            (xd <= F_ZERO && xd >= -F_ZERO) &&
            (yd <= F_ZERO && yd >= -F_ZERO) &&
            (zd <= F_ZERO && zd >= -F_ZERO)
            );
}

static int compare_line_segment(const line_s *l1, const line_s *l2)
{
    if(!l1 || !l2)
        return 0;
    return(((compare_vector3(&LINEV_A(l1), &LINEV_A(l2)) && compare_vector3(&LINEV_B(l1), &LINEV_B(l1))) || (compare_vector3(&LINEV_A(l1), &LINEV_B(l2)) && compare_vector3(&LINEV_B(l1), &LINEV_A(l2)))) ? 1 : 0);
}

static int push_edge_line(LineList &list, const line_s *lp)
{
    int has = 0;
    int o;

    for(o = 0; o < list.size(); o++) // find in lines list
    {
        const line_s &line = list[o];
        if(LINECMP(lp, &line))
        {
            //printf("%d exist\n", i);
            has = 1;
            break;
        }
    }

    if(has) // if exists, remove this line
    {
        list.removeAt(o);
    }
    else // if not exists, add new line to list
    {
        list.push_back(*lp);
    }
    return has;
}

static void render_edge_lines(const LineList &lines)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat color[4];
    glGetFloatv(GL_CURRENT_COLOR, color);
    glColor4f(0.0, 0.0, 1.0, 1.0);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(2);
    glPushMatrix();
    {
        GLfloat vs[6];
        int o;
        for(o = 0; o < lines.size(); o++) // find in lines list
        {
            const line_s &lpptr = lines[o];
            vs[0] = VECTOR3_X(lpptr.a);
            vs[1] = VECTOR3_Y(lpptr.a);
            vs[2] = VECTOR3_Z(lpptr.a);

            vs[3] = VECTOR3_X(lpptr.b);
            vs[4] = VECTOR3_Y(lpptr.b);
            vs[5] = VECTOR3_Z(lpptr.b);
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
    glPopMatrix();
    glLineWidth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glColor4fv(color);
    glDisableClientState(GL_VERTEX_ARRAY);
}

static vector3_s cale_light_direction(const vector3_s *v, const vector3_s *lightpos, int dirlight)
{
	int d;
	vector3_s r = VECTOR3(0, 0, 0);

	if(!lightpos)
		return r;

	d = !v ? 1 : dirlight;

	if(d)
	{
        r = vector3_normalize(lightpos);
        vector3_invertv(&r);
	}
	else
	{
		r = *v;
        r = vector3_direction(lightpos, v);
	}

	return r;
}

static void cale_mesh_transform(GL_NETLizard_3D_Mesh *r, const GL_NETLizard_3D_Mesh *nl_mesh, int invert)
{
    GLmatrix nor_mat;
    GLmatrix mat;
	unsigned int i, j;
    int m;
    GL_NETLizard_3D_Vertex *point;
	const GL_NETLizard_3D_Vertex *nl_vertex;
	GLushort *nl_indexs;
    const GL_NETLizard_3D_Material *nl_mat;

	if(!r || !nl_mesh)
		return;

    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&nor_mat);
    memset(r, 0, sizeof(GL_NETLizard_3D_Mesh));
    r->count =  nl_mesh->count;
    r->materials = (GL_NETLizard_3D_Material *)calloc( nl_mesh->count, sizeof(GL_NETLizard_3D_Material));
    for(i = 0; i < nl_mesh->count; i++) // must GL_TRIANGLE
    {
        r->materials[i].index_start = nl_mesh->materials[i].index_start;
        r->materials[i].index_count = nl_mesh->materials[i].index_count;
        r->materials[i].mode = GL_TRIANGLES;
        r->materials[i].tex_index = 0;
    }
    r->vertex_data.index_count = nl_mesh->vertex_data.index_count;
    r->vertex_data.index = (GLushort *)calloc(nl_mesh->vertex_data.index_count, sizeof(GLushort));
    memcpy(r->vertex_data.index, nl_mesh->vertex_data.index, sizeof(GLushort) * nl_mesh->vertex_data.index_count);
    r->vertex_data.vertex_count = nl_mesh->vertex_data.vertex_count;
    r->vertex_data.vertex = (GL_NETLizard_3D_Vertex *)calloc(nl_mesh->vertex_data.vertex_count, sizeof(GL_NETLizard_3D_Vertex));

    nl_indexs = nl_mesh->vertex_data.index;
	for(i = 0; i < nl_mesh->count; i++)
	{
		nl_mat = nl_mesh->materials + i;
        for(j = 0; j < nl_mat->index_count; j++)
        {
            m = nl_indexs[nl_mat->index_start + j];
            nl_vertex = nl_mesh->vertex_data.vertex + m;
            point = r->vertex_data.vertex + m;

            Mesa_glLoadIdentity(&mat);
            Mesa_glTranslate(&mat, nl_mesh->position[0], nl_mesh->position[1], nl_mesh->position[2]);
            Mesa_glRotate(&mat, nl_mesh->rotation[0], 1.0f, 0.0f, 0.0f);
            Mesa_glRotate(&mat, nl_mesh->rotation[1], 0.0f, 0.0f, 1.0f);

            NL::cale_normal_matrix(nor_mat, mat);

            Mesa_glTransform(point->position, nl_vertex->position, &mat);
            Mesa_glTransform_row(point->normal, nl_vertex->normal, &nor_mat);
            //memcpy(point->texcoord, nl_vertex->texcoord, sizeof(GLfloat) * 2);
//            if(invert)
//            {
//                point->normal[0] = -point->normal[0];
//                point->normal[1] = -point->normal[1];
//                point->normal[2] = -point->normal[2];
//            }
		}
    }

//	if(invert)
//	{
//		for(i = 0; i < (unsigned int)r->count; i += 3)
//		{
//			NL_SWAP(r->points[i + 1], r->points[i + 2], point_s);
//		}
//	}

    Mesa_FreeGLMatrix(&mat);
    Mesa_FreeGLMatrix(&nor_mat);
//#define _TEST_RENDER_NORMAL
#ifdef _TEST_RENDER_NORMAL
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1);
    glPointSize(2);
	GLfloat nv[6];
    for(i = 0; i < r->vertex_data.vertex_count; i++)
    {
        vector3_s rv = VECTOR3V(r->vertex_data.vertex[i].position);
        nv[0] = VECTOR3_X(rv);
        nv[1] = VECTOR3_Y(rv);
        nv[2] = VECTOR3_Z(rv);

        vector3_s rv2 = VECTOR3V(r->vertex_data.vertex[i].normal);
        vector3_scalev(&rv2, 200);
        rv2 = vector3_add(&rv, &rv2);
        nv[3] = VECTOR3_X(rv2);
        nv[4] = VECTOR3_Y(rv2);
        nv[5] = VECTOR3_Z(rv2);
		glVertexPointer(3, GL_FLOAT, 0, nv);
		glColor4f(1.0, 0.0, 1.0, 1.0);
		glDrawArrays(GL_LINES, 0, 2);
		glColor4f(0.0, 0.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1);
	glPointSize(1);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

// cale shadow volume
static void cale_mesh_volume_data(Shadow_Volume_s *r, const vector3_s *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method)
{
    int has;
    int i, j, n;
    const GL_NETLizard_3D_Vertex *point;
    GL_NETLizard_3D_Material *m;
    const GLfloat *v;
    GLushort index1, index2, index3;

    if(!r || !light_position || !mat || !mat->count)
		return;

    const GLboolean Cap = method == SHADOW_Z_FAIL || 1 ? GL_TRUE : GL_FALSE;

    for(i = 0; i < mat->count; i++)
    {
        m = mat->materials + i;
        for(j = 0; j < m->index_count; j += 3)
        {
            index1 = mat->vertex_data.index[m->index_start + j];
            index2 = mat->vertex_data.index[m->index_start + j + 1];
            index3 = mat->vertex_data.index[m->index_start + j + 2];
            const GL_NETLizard_3D_Vertex *pa[] = {
                mat->vertex_data.vertex + index1,
                mat->vertex_data.vertex + index2,
                mat->vertex_data.vertex + index3,
            };

            point = pa[0];
            v = point->position;
            vector3_s nor = VECTOR3V(point->normal);
            vector3_s p = VECTOR3V(v);
            vector3_s p2l = cale_light_direction(&p, light_position, dirlight);
            vector3_invertv(&p2l);
            float dot_p = vector3_dot(&p2l, &nor);

            if(dot_p > 0.0) // face to light source
            {
                for(n = 0; n < 3; n++) // 3 points of triangle
                {
                    int next_index = (n + 1) % 3;

                    line_s lp = LINEV(pa[n]->position, pa[next_index]->position);
                    has = push_edge_line(r->lines, &lp);
                }
                // top cap triangle
                if(Cap)
                {
                    triangle_s tri = TRIANGLEV(pa[0]->position, pa[1]->position, pa[2]->position);
                    r->tops.push_back(tri);
                }
            }
            else
            {
                // bottom cap triangle
                // because some model not cap, so using face light vertex
#if 0
                if(Cap)
                {
                    triangle_s tri = TRIANGLEV(pa[0]->position, pa[1]->position, pa[2]->position);
                    r->bottoms.push_back(tri);
                }
#endif
            }
        }
    }

//#define _TEST_RENDER_EDGE_LINES
#ifdef _TEST_RENDER_EDGE_LINES
    render_edge_lines(lines);
#endif
}

static void make_shadow_volume_mesh(GL_NETLizard_3D_Mesh *r, const vector3_s *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method)
{
    int i, k, o;
    GL_NETLizard_3D_Material *m;
    GL_NETLizard_3D_Vertex *vd, *pa;

    if(!r || !light_position || !mat || !mat->count)
        return;

    Shadow_Volume_s volume;
    cale_mesh_volume_data(&volume, light_position, dirlight, mat, method);

    const LineList &lines = volume.lines;
    const TriangleList &tops = volume.tops;
    const TriangleList &bottoms = volume.tops; // using top caps

    const GLboolean Cap = method == SHADOW_Z_FAIL || 1 ? GL_TRUE : GL_FALSE;

    memset(r, 0, sizeof(GL_NETLizard_3D_Mesh));
    r->count = Cap ? 3 : 1; // top[1] bottom[2] caps, and side[0]
    r->materials = (GL_NETLizard_3D_Material *)calloc(r->count, sizeof(GL_NETLizard_3D_Material));
	// cale sides of shadow volume
    int vcount = lines.size() * 6; // 2 triangles(6 points) every a line
    if(Cap)
    {
        vcount += tops.size() * 3;
        vcount += bottoms.size() * 3;
    }
    r->vertex_data.vertex_count = vcount;
    r->vertex_data.vertex = (GL_NETLizard_3D_Vertex *)calloc(r->vertex_data.vertex_count, sizeof(GL_NETLizard_3D_Vertex));
    r->vertex_data.index_count = vcount;
    r->vertex_data.index = (GLushort *)calloc(r->vertex_data.index_count, sizeof(GLushort));
    m = r->materials;
    m->mode = GL_TRIANGLES;
    m->index_start = 0;
    m->index_count = lines.size() * 6;
	k = 0;
    o = 0;
    vd = r->vertex_data.vertex;
	// TODO: cale clock wise, now the lighting source must be above all cubes
    for(i = 0; i < lines.size(); i++)
    {
        line_s lpptr = lines[i];
		// point lighting
        vector3_s dir_a = cale_light_direction(&(LINE_A(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_a, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
        dir_a = vector3_add(&(LINE_A(lpptr)), &dir_a);

        vector3_s dir_b = cale_light_direction(&(LINE_B(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_b, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
        dir_b = vector3_add(&(LINE_B(lpptr)), &dir_b);

        if(SHADOW_CAP_OFFSET)
        {
            vector3_s dir_offset_a = cale_light_direction(&(LINE_A(lpptr)), light_position, dirlight);
            vector3_scalev(&dir_offset_a, SHADOW_CAP_OFFSET);
            LINE_A(lpptr) = vector3_add(&(LINE_A(lpptr)), &dir_offset_a);

            vector3_s dir_offset_b = cale_light_direction(&(LINE_B(lpptr)), light_position, dirlight);
            vector3_scalev(&dir_offset_b, SHADOW_CAP_OFFSET);
            LINE_B(lpptr) = vector3_add(&(LINE_B(lpptr)), &dir_offset_b);
        }

//#define _TEST_RENDER_SHADOW_VOLUME
#ifdef _TEST_RENDER_SHADOW_VOLUME
	glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0, 1.0, 0.0, 0.8);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
	glPushMatrix();
    {
		GLfloat vs[18];
			//printf("%d %d\n", o, lines.count);
            vs[0] = VECTOR3_X(lpptr.a);
            vs[1] = VECTOR3_Y(lpptr.a);
            vs[2] = VECTOR3_Z(lpptr.a);

            vs[3] = VECTOR3_X(dir_a);
            vs[4] = VECTOR3_Y(dir_a);
            vs[5] = VECTOR3_Z(dir_a);

            vs[6] = VECTOR3_X(lpptr.b);
            vs[7] = VECTOR3_Y(lpptr.b);
            vs[8] = VECTOR3_Z(lpptr.b);

            vs[9] = VECTOR3_X(dir_a);
            vs[10] = VECTOR3_Y(dir_a);
            vs[11] = VECTOR3_Z(dir_a);

            vs[12] = VECTOR3_X(dir_b);
            vs[13] = VECTOR3_Y(dir_b);
            vs[14] = VECTOR3_Z(dir_b);

            vs[15] = VECTOR3_X(lpptr.b);
            vs[16] = VECTOR3_Y(lpptr.b);
            vs[17] = VECTOR3_Z(lpptr.b);

			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_TRIANGLES, 0, 6);
	}
    glPopMatrix();
	glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
		// triangle 1
        pa = vd + k * 6;
        pa->position[0] = LINE_A_X(lpptr);
        pa->position[1] = LINE_A_Y(lpptr);
        pa->position[2] = LINE_A_Z(lpptr);
        pa = vd + k * 6 + 1;
        pa->position[0] = VECTOR3_X(dir_a);
        pa->position[1] = VECTOR3_Y(dir_a);
        pa->position[2] = VECTOR3_Z(dir_a);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + k * 6 + 2;
        pa->position[0] = LINE_B_X(lpptr);
        pa->position[1] = LINE_B_Y(lpptr);
        pa->position[2] = LINE_B_Z(lpptr);

		// triangle 2
        pa = vd + k * 6 + 3;
        pa->position[0] = VECTOR3_X(dir_a);
        pa->position[1] = VECTOR3_Y(dir_a);
        pa->position[2] = VECTOR3_Z(dir_a);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + k * 6 + 4;
        pa->position[0] = VECTOR3_X(dir_b);
        pa->position[1] = VECTOR3_Y(dir_b);
        pa->position[2] = VECTOR3_Z(dir_b);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + k * 6 + 5;
        pa->position[0] = LINE_B_X(lpptr);
        pa->position[1] = LINE_B_Y(lpptr);
        pa->position[2] = LINE_B_Z(lpptr);

        int six = 6 + o;
        for(; o < six; o++)
        {
            r->vertex_data.index[o] = o;
        }
		k++;
	}

    if(Cap)
	{
        // cale top cap of shadow volume
        // using triangles of the mesh faces to lighting source
        vd = r->vertex_data.vertex + o;
        m = r->materials + 1;
        m->mode = GL_TRIANGLES;
        m->index_count = tops.size() * 3;
        m->index_start = o;

        m = r->materials + 1;
        for(k = 0; k < tops.size(); k++)
		{
            const triangle_s &tri = tops[k];

            if(SHADOW_CAP_OFFSET)
            {
                vector3_s dir_a = cale_light_direction(&(TRIANGLE_A(tri)), light_position, dirlight);
                vector3_scalev(&dir_a, SHADOW_CAP_OFFSET);
                dir_a = vector3_add(&(TRIANGLE_A(tri)), &dir_a);

                vector3_s dir_b = cale_light_direction(&(TRIANGLE_B(tri)), light_position, dirlight);
                vector3_scalev(&dir_b, SHADOW_CAP_OFFSET);
                dir_b = vector3_add(&(TRIANGLE_B(tri)), &dir_b);

                vector3_s dir_c = cale_light_direction(&(TRIANGLE_C(tri)), light_position, dirlight);
                vector3_scalev(&dir_c, SHADOW_CAP_OFFSET);
                dir_c = vector3_add(&(TRIANGLE_C(tri)), &dir_c);

                pa = vd + k * 3;
                pa->position[0] = VECTOR3_X(dir_a);
                pa->position[1] = VECTOR3_Y(dir_a);
                pa->position[2] = VECTOR3_Z(dir_a);
                //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
                pa = vd + k * 3 + 1;
                pa->position[0] = VECTOR3_X(dir_b);
                pa->position[1] = VECTOR3_Y(dir_b);
                pa->position[2] = VECTOR3_Z(dir_b);
                //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
                pa = vd + k * 3 + 2;
                pa->position[0] = VECTOR3_X(dir_c);
                pa->position[1] = VECTOR3_Y(dir_c);
                pa->position[2] = VECTOR3_Z(dir_c);
            }
            else
            {
                pa = vd + k * 3;
                pa->position[0] = TRIANGLE_A_X(tri);
                pa->position[1] = TRIANGLE_A_Y(tri);
                pa->position[2] = TRIANGLE_A_Z(tri);
                pa = vd + k * 3 + 1;
                pa->position[0] = TRIANGLE_B_X(tri);
                pa->position[1] = TRIANGLE_B_Y(tri);
                pa->position[2] = TRIANGLE_B_Z(tri);
                pa = vd + k * 3 + 2;
                pa->position[0] = TRIANGLE_C_X(tri);
                pa->position[1] = TRIANGLE_C_Y(tri);
                pa->position[2] = TRIANGLE_C_Z(tri);
            }

            int three = 3 + o;
            for(; o < three; o++)
            {
                r->vertex_data.index[o] = o;
            }
//#define _TEST_RENDER_SHADOW_VOLUME_TOP
#ifdef _TEST_RENDER_SHADOW_VOLUME_TOP
            glEnableClientState(GL_VERTEX_ARRAY);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glPushMatrix();
            {
                GLfloat vs[9];
                vs[0] = TRIANGLE_A_X(tri);
                vs[1] = TRIANGLE_A_Y(tri);
                vs[2] = TRIANGLE_A_Z(tri);

                vs[3] = TRIANGLE_B_X(tri);
                vs[4] = TRIANGLE_B_Y(tri);
                vs[5] = TRIANGLE_B_Z(tri);

                vs[6] = TRIANGLE_C_X(tri);
                vs[7] = TRIANGLE_C_Y(tri);
                vs[8] = TRIANGLE_C_Z(tri);

                glVertexPointer(3, GL_FLOAT, 0, vs);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            glPopMatrix();
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glDisableClientState(GL_VERTEX_ARRAY);
#endif
		}

        // cale bottom cap of shadow volume
        // using triangles of the mesh not faces to lighting source

        vd = r->vertex_data.vertex + o;
        m = r->materials + 2;
        m->mode = GL_TRIANGLES;
        m->index_count = bottoms.size() * 3;
        m->index_start = o;

        for(k = 0; k < bottoms.size(); k++)
        {
            const triangle_s &tri = bottoms[k];

            vector3_s dir_a = cale_light_direction(&(TRIANGLE_A(tri)), light_position, dirlight);
            vector3_scalev(&dir_a, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_a = vector3_add(&(TRIANGLE_A(tri)), &dir_a);

            vector3_s dir_b = cale_light_direction(&(TRIANGLE_B(tri)), light_position, dirlight);
            vector3_scalev(&dir_b, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_b = vector3_add(&(TRIANGLE_B(tri)), &dir_b);

            vector3_s dir_c = cale_light_direction(&(TRIANGLE_C(tri)), light_position, dirlight);
            vector3_scalev(&dir_c, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_c = vector3_add(&(TRIANGLE_C(tri)), &dir_c);

            qSwap(dir_a, dir_b);

            pa = vd + k * 3;
            pa->position[0] = VECTOR3_X(dir_a);
            pa->position[1] = VECTOR3_Y(dir_a);
            pa->position[2] = VECTOR3_Z(dir_a);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
            pa = vd + k * 3 + 1;
            pa->position[0] = VECTOR3_X(dir_b);
            pa->position[1] = VECTOR3_Y(dir_b);
            pa->position[2] = VECTOR3_Z(dir_b);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
            pa = vd + k * 3 + 2;
            pa->position[0] = VECTOR3_X(dir_c);
            pa->position[1] = VECTOR3_Y(dir_c);
            pa->position[2] = VECTOR3_Z(dir_c);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;

            int three = 3 + o;
            for(; o < three; o++)
            {
                r->vertex_data.index[o] = o;
            }
//#define _TEST_RENDER_SHADOW_VOLUME_BOTTOM
#ifdef _TEST_RENDER_SHADOW_VOLUME_BOTTOM
            glEnableClientState(GL_VERTEX_ARRAY);
            glColor4f(0.0, 0.0, 1.0, 1);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glPushMatrix();
            {
                GLfloat vs[9];
                vs[0] = VECTOR3_X(dir_a);
                vs[1] = VECTOR3_Y(dir_a);
                vs[2] = VECTOR3_Z(dir_a);

                vs[3] = VECTOR3_X(dir_b);
                vs[4] = VECTOR3_Y(dir_b);
                vs[5] = VECTOR3_Z(dir_b);

                vs[6] = VECTOR3_X(dir_c);
                vs[7] = VECTOR3_Y(dir_c);
                vs[8] = VECTOR3_Z(dir_c);

                glVertexPointer(3, GL_FLOAT, 0, vs);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            glPopMatrix();
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glDisableClientState(GL_VERTEX_ARRAY);
#endif
		}
	}
}

static void render_shadow_volume_mesh(const GL_NETLizard_3D_Mesh *nl_mesh, const vector3_s *light_position, const int dirlight, int m)
{
    if(!nl_mesh || !light_position)
        return;

    glStencilOpSeparateProc glStencilOpSeparate = NULL;
    //glStencilOpSeparate = (glStencilOpSeparateProc)QGLContext::currentContext()->getProcAddress("glStencilOpSeparate");

    GL_NETLizard_3D_Mesh vol;
    make_shadow_volume_mesh(&vol, light_position, dirlight, nl_mesh, m);

	// render
	// 1: get depth buffer of scene
#if 0
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	if(render_count == 0)
	{
		glDisable(GL_BLEND);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		//rendermat(nl_mesh);
	}
#endif
	//goto __Exit;

    //glEnableClientState(GL_VERTEX_ARRAY);
	//glDepthFunc(GL_LEQUAL);

	if(m == SHADOW_Z_FAIL)
	{
		// 2: Z-Fail
#ifdef GL_ARB_depth_clamp
		glEnable(GL_DEPTH_CLAMP);
#endif
#if 0
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, ~0U);
#endif

        if(glStencilOpSeparate)
        {
            glDisable(GL_CULL_FACE);
            glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
            glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
            NETLizard_RenderGL3DMesh(&vol, 0);
            glEnable(GL_CULL_FACE);
        }
        else
        {
            // 2-1: cale front faces of shadow volume
            glStencilOp(GL_KEEP, GL_INCR_WRAP, GL_KEEP);
            glCullFace(GL_FRONT);
            NETLizard_RenderGL3DMesh(&vol, 0);

            // 2-1: cale back faces of shadow volume
            glStencilOp(GL_KEEP, GL_DECR_WRAP, GL_KEEP);
            glCullFace(GL_BACK);
            NETLizard_RenderGL3DMesh(&vol, 0);
        }

#ifdef GL_ARB_depth_clamp
		glDisable(GL_DEPTH_CLAMP);
#endif
	}
	else
	{
		// 2: Z-Pass
#if 0
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, ~0U);
#endif

        if(glStencilOpSeparate)
        {
            glDisable(GL_CULL_FACE);
            glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
            glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
            NETLizard_RenderGL3DMesh(&vol, 0);
            glEnable(GL_CULL_FACE);
        }
        else
        {
            // 2-1: cale front faces of shadow volume
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
            glCullFace(GL_BACK);
            NETLizard_RenderGL3DMesh(&vol, 0);

            // 2-1: cale back faces of shadow volume
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
            glCullFace(GL_FRONT);
            NETLizard_RenderGL3DMesh(&vol, 0);
        }
	}
	//glDepthFunc(GL_LESS);
    //glDisableClientState(GL_VERTEX_ARRAY);

#if 0
#if SHADOW_MASK
#if 0	
	if(render_count != -1)
	{
		// 3: final render scene again
		glCullFace(GL_BACK);
		glDepthFunc(GL_EQUAL); // GL_LESS will not pass depth-testing
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0, ~0U);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glEnable(GL_BLEND);

		glDisable(GL_DEPTH_TEST);
		Shadow_RenderMask();

		// 4: reset OpenGL state
		glDisable(GL_STENCIL_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
#else
		glCullFace(GL_BACK);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDisable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDepthMask(GL_TRUE);
#endif
#else
	if(render_count != -1)
	{
		// 3: final render scene again
		glCullFace(GL_BACK);
		glDepthFunc(GL_EQUAL); // GL_LESS will not pass depth-testing
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 0, ~0U);
		glBlendFunc(GL_ONE, GL_ONE);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glEnable(GL_BLEND);

		rendermat(nl_mesh);

		// 4: reset OpenGL state
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDepthFunc(GL_LESS);
	}
#endif
#endif
    //glCullFace(GL_BACK);

__Exit:
    delete_GL_NETLizard_3D_Mesh(&vol);
}

void NETLizard_RenderMeshShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_s *light_position, int dirlight, int method)
{
    if(!mesh || !light_position)
		return;

    GL_NETLizard_3D_Mesh m;

    cale_mesh_transform(&m, mesh, 0);

    render_shadow_volume_mesh(&m, light_position, dirlight, method);

    delete_GL_NETLizard_3D_Mesh(&m);
}

