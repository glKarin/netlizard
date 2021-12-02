#include "stencil_shadow.h"

// using cpp::std
#include <list>
#include <vector>

#include <QDebug>
//#include <QGLContext>

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
#define SHADOW_CAP_OFFSET 0
#define SHADOW_VOLUME_FAR_W 1
#define SHADOW_MASK 1 //
#define SHADOW_MASK_LIGHT 0 //

#define VEC3CMP vector3_equals // compare_vector3
#define LINECMP line_equals_ignore_seq // compare_line_segment

typedef std::list<line_t> LineList;
typedef std::vector<vector3_t> Vector3List;
typedef std::vector<triangle_t> TriangleList;
typedef struct _Shadow_Volume_s
{
    LineList lines;
    TriangleList tops;
    TriangleList bottoms;
} Shadow_Volume_s;

static int compare_vector3(const vector3_t *a, const vector3_t *b)
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

static int compare_line_segment(const line_t *l1, const line_t *l2)
{
    if(!l1 || !l2)
        return 0;
    return(((compare_vector3(&LINEV_A(l1), &LINEV_A(l2)) && compare_vector3(&LINEV_B(l1), &LINEV_B(l1))) || (compare_vector3(&LINEV_A(l1), &LINEV_B(l2)) && compare_vector3(&LINEV_B(l1), &LINEV_A(l2)))) ? 1 : 0);
}

static GLboolean shadow_volume_is_empty(const Shadow_Volume_s *sv)
{
    return sv->lines.empty()
            //&& sv->tops.isEmpty() && sv->bottoms.isEmpty()
            ;
}

static GLboolean shadow_volume_need_cap(int method)
{
    return method == SHADOW_Z_FAIL/* || 1*/ ? GL_TRUE : GL_FALSE;
}

#define UP_NORMAL_LIMIT 0.866025
static GLboolean normal_is_up_down(const float normal[3])
{
    //return normal[2] > 0 || normal[2] < 0;
    //return normal[2] != 0;
    return normal[2] > 0 || normal[2] < -UP_NORMAL_LIMIT;
}

static int push_edge_line(LineList &list, const line_t *lp)
{
    int has = 0;
    LineList::iterator itor;

    for(itor = list.begin(); itor != list.end(); ++itor) // find in lines list
    {
        const line_t &line = *itor;
        if(LINECMP(lp, &line))
        {
            //printf("%d exist\n", i);
            has = 1;
            break;
        }
    }

    if(has) // if exists, remove this line
    {
        list.erase(itor);
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
        LineList::const_iterator itor;
        for(itor = lines.begin(); itor != lines.end(); ++itor)
        {
            const line_t &lpptr = *itor;
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

static vector3_t cale_light_direction(const vector3_t *v, const vector3_t *lightpos, int dirlight)
{
	int d;
	vector3_t r = VECTOR3(0, 0, 0);

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

static GLboolean cale_mesh_transform(GL_NETLizard_3D_Mesh *r, const GL_NETLizard_3D_Mesh *nl_mesh, int invert)
{
    GLmatrix nor_mat;
    GLmatrix mat;
	unsigned int i, j;
    int m;
    GL_NETLizard_3D_Vertex *point;
	const GL_NETLizard_3D_Vertex *nl_vertex;
	GLushort *nl_indexs;
    const GL_NETLizard_3D_Material *nl_mat;

    if(!r || !nl_mesh || nl_mesh->count == 0)
        return GL_FALSE;

    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&nor_mat);
    memset(r, 0, sizeof(GL_NETLizard_3D_Mesh));
    r->count =  nl_mesh->count;
    r->materials = (GL_NETLizard_3D_Material *)calloc(nl_mesh->count, sizeof(GL_NETLizard_3D_Material));
    for(i = 0; i < nl_mesh->count; i++) // must GL_TRIANGLE
    {
        r->materials[i].index_start = nl_mesh->materials[i].index_start;
        r->materials[i].index_count = nl_mesh->materials[i].index_count;
        r->materials[i].mode = GL_TRIANGLES;
        r->materials[i].tex_index = 0;
        r->materials[i].index = (GLushort *)calloc(nl_mesh->materials[i].index_count, sizeof(GLushort));
        memcpy(r->materials[i].index, nl_mesh->materials[i].index, nl_mesh->materials[i].index_count * sizeof(GLushort));
    }
//    r->vertex_data.index_count = nl_mesh->vertex_data.index_count;
//    r->vertex_data.index = (GLushort *)calloc(nl_mesh->vertex_data.index_count, sizeof(GLushort));
//    memcpy(r->vertex_data.index, nl_mesh->vertex_data.index, sizeof(GLushort) * nl_mesh->vertex_data.index_count);
    r->vertex_data.vertex_count = nl_mesh->vertex_data.vertex_count;
    r->vertex_data.vertex = (GL_NETLizard_3D_Vertex *)calloc(nl_mesh->vertex_data.vertex_count, sizeof(GL_NETLizard_3D_Vertex));

    Mesa_glLoadIdentity(&mat);
    Mesa_glTranslate(&mat, nl_mesh->position[0], nl_mesh->position[1], nl_mesh->position[2]);
    Mesa_glRotate(&mat, nl_mesh->rotation[0], 1.0f, 0.0f, 0.0f);
    Mesa_glRotate(&mat, nl_mesh->rotation[1], 0.0f, 0.0f, 1.0f);

    NL::cale_normal_matrix(nor_mat, mat);

	for(i = 0; i < nl_mesh->count; i++)
	{
		nl_mat = nl_mesh->materials + i;
        for(j = 0; j < nl_mat->index_count; j++)
        {
            m = nl_mat->index[j];
            nl_vertex = nl_mesh->vertex_data.vertex + m;
            point = r->vertex_data.vertex + m;

            Mesa_glTransform(point->position, nl_vertex->position, &mat);
            Mesa_glTransform_row(point->normal, nl_vertex->normal, &nor_mat);
            //memcpy(point->texcoord, nl_vertex->texcoord, sizeof(GLfloat) * 2);
            if(invert)
            {
                point->normal[0] = -point->normal[0];
                point->normal[1] = -point->normal[1];
                point->normal[2] = -point->normal[2];
            }
		}
    }

    if(invert)
    {
        for(i = 0; i < r->count; i++)
        {
            nl_mat = r->materials + i;
            nl_indexs = nl_mat->index;
            for(j = 0; j < nl_mat->index_count; j += 3)
            {
                m = nl_indexs[j];
                nl_indexs[j] = nl_indexs[j + 1];
                nl_indexs[j + 1] = m;
            }
        }
    }

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
        vector3_t rv = VECTOR3V(r->vertex_data.vertex[i].position);
        nv[0] = VECTOR3_X(rv);
        nv[1] = VECTOR3_Y(rv);
        nv[2] = VECTOR3_Z(rv);

        vector3_t rv2 = VECTOR3V(r->vertex_data.vertex[i].normal);
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
    return GL_TRUE;
}

// cale shadow volume
static GLboolean cale_mesh_volume_data(Shadow_Volume_s *r, const vector3_t *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method, int invert)
{
    int has;
    unsigned int i, j, n;
    const GL_NETLizard_3D_Vertex *point;
    GL_NETLizard_3D_Material *m;
    const GLfloat *v;
    GLushort index1, index2, index3;

    if(!r || !light_position || !mat || !mat->count)
        return GL_FALSE;

    const GLboolean Cap = shadow_volume_need_cap(method);

    for(i = 0; i < mat->count; i++)
    {
        m = mat->materials + i;
        for(j = 0; j < m->index_count; j += 3)
        {
            index1 = m->index[j];
            index2 = m->index[j + 1];
            index3 = m->index[j + 2];
            const GL_NETLizard_3D_Vertex *pa[] = {
                mat->vertex_data.vertex + index1,
                mat->vertex_data.vertex + index2,
                mat->vertex_data.vertex + index3,
            };

            point = pa[0];
            if(invert == SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR) // when render scene, cull up and down plane face.
            {
                if(normal_is_up_down(point->normal))
                    continue;
            }

            v = point->position;
            vector3_t nor = VECTOR3V(point->normal);
            vector3_t p = VECTOR3V(v);
            vector3_t p2l = cale_light_direction(&p, light_position, dirlight);
            vector3_invertv(&p2l);
            float dot_p = vector3_dot(&p2l, &nor);

            if(dot_p > 0.0) // face to light source
            {
                for(n = 0; n < 3; n++) // 3 points of triangle
                {
                    int next_index = (n + 1) % 3;

                    line_t lp = LINEV(pa[n]->position, pa[next_index]->position);
                    has = push_edge_line(r->lines, &lp);
                }
                // top cap triangle
                if(Cap)
                {
                    triangle_t tri = TRIANGLEV(pa[0]->position, pa[1]->position, pa[2]->position);
                    r->tops.push_back(tri);
                }
            }
            else
            {
                // bottom cap triangle
                // because some model not cap, so using face light vertex
                if(Cap)
                {
                    triangle_t tri = TRIANGLEV(pa[0]->position, pa[1]->position, pa[2]->position);
                    r->bottoms.push_back(tri);
                }
            }
        }
    }

//#define _TEST_RENDER_EDGE_LINES
#ifdef _TEST_RENDER_EDGE_LINES
    render_edge_lines(r->lines);
#endif
    return GL_TRUE;
}

static GLboolean make_shadow_volume_mesh(GL_NETLizard_3D_Mesh *r, const vector3_t *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method, int invert)
{
    unsigned int i;
    int o, q, n;
    GL_NETLizard_3D_Material *m;
    GL_NETLizard_3D_Vertex *vd, *pa;

    if(!r || !light_position || !mat || !mat->count)
        return GL_FALSE;

    Shadow_Volume_s volume;
    if(!cale_mesh_volume_data(&volume, light_position, dirlight, mat, method, invert))
        return GL_FALSE;
    if(shadow_volume_is_empty(&volume))
        return GL_FALSE;

    const LineList &lines = volume.lines;
    const TriangleList &tops = volume.tops;
    const TriangleList &bottoms = volume.tops; // using top caps

    const GLboolean Cap = shadow_volume_need_cap(method);

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
//    r->vertex_data.index_count = vcount;
//    r->vertex_data.index = (GLushort *)calloc(r->vertex_data.index_count, sizeof(GLushort));
    m = r->materials;
    m->mode = GL_TRIANGLES;
    m->index_start = 0;
    m->index_count = lines.size() * 6;
    m->index = (GLushort *)calloc(m->index_count, sizeof(GLushort));
    o = 0;
    vd = r->vertex_data.vertex;
	// TODO: cale clock wise, now the lighting source must be above all cubes
    LineList::const_iterator itor;
    i = 0;
    for(itor = lines.begin(); itor != lines.end(); ++itor)
    {
        line_t lpptr = *itor;
		// point lighting
        vector3_t dir_a = cale_light_direction(&(LINE_A(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_a, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
        dir_a = vector3_add(&(LINE_A(lpptr)), &dir_a);

        vector3_t dir_b = cale_light_direction(&(LINE_B(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_b, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
        dir_b = vector3_add(&(LINE_B(lpptr)), &dir_b);

        if(SHADOW_CAP_OFFSET)
        {
            vector3_t dir_offset_a = cale_light_direction(&(LINE_A(lpptr)), light_position, dirlight);
            vector3_scalev(&dir_offset_a, SHADOW_CAP_OFFSET);
            LINE_A(lpptr) = vector3_add(&(LINE_A(lpptr)), &dir_offset_a);

            vector3_t dir_offset_b = cale_light_direction(&(LINE_B(lpptr)), light_position, dirlight);
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
        pa = vd + i * 6;
        pa->position[0] = LINE_A_X(lpptr);
        pa->position[1] = LINE_A_Y(lpptr);
        pa->position[2] = LINE_A_Z(lpptr);
        pa = vd + i * 6 + 1;
        pa->position[0] = VECTOR3_X(dir_a);
        pa->position[1] = VECTOR3_Y(dir_a);
        pa->position[2] = VECTOR3_Z(dir_a);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + i * 6 + 2;
        pa->position[0] = LINE_B_X(lpptr);
        pa->position[1] = LINE_B_Y(lpptr);
        pa->position[2] = LINE_B_Z(lpptr);

		// triangle 2
        pa = vd + i * 6 + 3;
        pa->position[0] = VECTOR3_X(dir_a);
        pa->position[1] = VECTOR3_Y(dir_a);
        pa->position[2] = VECTOR3_Z(dir_a);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + i * 6 + 4;
        pa->position[0] = VECTOR3_X(dir_b);
        pa->position[1] = VECTOR3_Y(dir_b);
        pa->position[2] = VECTOR3_Z(dir_b);
        //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = vd + i * 6 + 5;
        pa->position[0] = LINE_B_X(lpptr);
        pa->position[1] = LINE_B_Y(lpptr);
        pa->position[2] = LINE_B_Z(lpptr);

        for(q = 0; q < 6; q++)
        {
            n = i * 6 + q;
            m->index[n] = n;
        }
        o += 6;
        i++;
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
        m->index = (GLushort *)calloc(m->index_count, sizeof(GLushort));

        for(i = 0; i < tops.size(); i++)
		{
            const triangle_t &tri = tops[i];

            if(SHADOW_CAP_OFFSET)
            {
                vector3_t dir_a = cale_light_direction(&(TRIANGLE_A(tri)), light_position, dirlight);
                vector3_scalev(&dir_a, SHADOW_CAP_OFFSET);
                dir_a = vector3_add(&(TRIANGLE_A(tri)), &dir_a);

                vector3_t dir_b = cale_light_direction(&(TRIANGLE_B(tri)), light_position, dirlight);
                vector3_scalev(&dir_b, SHADOW_CAP_OFFSET);
                dir_b = vector3_add(&(TRIANGLE_B(tri)), &dir_b);

                vector3_t dir_c = cale_light_direction(&(TRIANGLE_C(tri)), light_position, dirlight);
                vector3_scalev(&dir_c, SHADOW_CAP_OFFSET);
                dir_c = vector3_add(&(TRIANGLE_C(tri)), &dir_c);

                pa = vd + i * 3;
                pa->position[0] = VECTOR3_X(dir_a);
                pa->position[1] = VECTOR3_Y(dir_a);
                pa->position[2] = VECTOR3_Z(dir_a);
                //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
                pa = vd + i * 3 + 1;
                pa->position[0] = VECTOR3_X(dir_b);
                pa->position[1] = VECTOR3_Y(dir_b);
                pa->position[2] = VECTOR3_Z(dir_b);
                //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
                pa = vd + i * 3 + 2;
                pa->position[0] = VECTOR3_X(dir_c);
                pa->position[1] = VECTOR3_Y(dir_c);
                pa->position[2] = VECTOR3_Z(dir_c);
            }
            else
            {
                pa = vd + i * 3;
                pa->position[0] = TRIANGLE_A_X(tri);
                pa->position[1] = TRIANGLE_A_Y(tri);
                pa->position[2] = TRIANGLE_A_Z(tri);
                pa = vd + i * 3 + 1;
                pa->position[0] = TRIANGLE_B_X(tri);
                pa->position[1] = TRIANGLE_B_Y(tri);
                pa->position[2] = TRIANGLE_B_Z(tri);
                pa = vd + i * 3 + 2;
                pa->position[0] = TRIANGLE_C_X(tri);
                pa->position[1] = TRIANGLE_C_Y(tri);
                pa->position[2] = TRIANGLE_C_Z(tri);
            }

            for(q = 0; q < 3; q++)
            {
                m->index[i * 3 + q] = o + q;
            }
            o += 3;
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
        m->index = (GLushort *)calloc(m->index_count, sizeof(GLushort));

        for(i = 0; i < bottoms.size(); i++)
        {
            const triangle_t &tri = bottoms[i];

            vector3_t dir_a = cale_light_direction(&(TRIANGLE_A(tri)), light_position, dirlight);
            vector3_scalev(&dir_a, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_a = vector3_add(&(TRIANGLE_A(tri)), &dir_a);

            vector3_t dir_b = cale_light_direction(&(TRIANGLE_B(tri)), light_position, dirlight);
            vector3_scalev(&dir_b, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_b = vector3_add(&(TRIANGLE_B(tri)), &dir_b);

            vector3_t dir_c = cale_light_direction(&(TRIANGLE_C(tri)), light_position, dirlight);
            vector3_scalev(&dir_c, SHADOW_VOLUME_LENGTH + SHADOW_CAP_OFFSET);
            dir_c = vector3_add(&(TRIANGLE_C(tri)), &dir_c);

            qSwap(dir_a, dir_b);

            pa = vd + i * 3;
            pa->position[0] = VECTOR3_X(dir_a);
            pa->position[1] = VECTOR3_Y(dir_a);
            pa->position[2] = VECTOR3_Z(dir_a);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
            pa = vd + i * 3 + 1;
            pa->position[0] = VECTOR3_X(dir_b);
            pa->position[1] = VECTOR3_Y(dir_b);
            pa->position[2] = VECTOR3_Z(dir_b);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;
            pa = vd + i * 3 + 2;
            pa->position[0] = VECTOR3_X(dir_c);
            pa->position[1] = VECTOR3_Y(dir_c);
            pa->position[2] = VECTOR3_Z(dir_c);
            //pa->vertex[3] = SHADOW_VOLUME_FAR_W;

            for(q = 0; q < 3; q++)
            {
                m->index[i * 3 + q] = o + q;
            }
            o += 3;
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
    return GL_TRUE;
}

static GLboolean render_shadow_volume_mesh(const GL_NETLizard_3D_Mesh *nl_mesh, const vector3_t *light_position, const int dirlight, int m, int invert)
{
    if(!nl_mesh || !light_position)
        return GL_FALSE;

    glStencilOpSeparateProc glStencilOpSeparate = NULL;
    //glStencilOpSeparate = (glStencilOpSeparateProc)QGLContext::currentContext()->getProcAddress("glStencilOpSeparate");

    GL_NETLizard_3D_Mesh vol;
    memset(&vol, 0, sizeof(GL_NETLizard_3D_Mesh));
    if(!make_shadow_volume_mesh(&vol, light_position, dirlight, nl_mesh, m, invert))
        return GL_FALSE;
    if(vol.count == 0)
        return GL_TRUE;

	// render
	// 1: get depth buffer of scene
#if 0
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

#ifndef SHADOW_MASK
    // 3: final render scene again
#endif
    //glCullFace(GL_BACK);

    delete_GL_NETLizard_3D_Mesh(&vol);
    return GL_TRUE;
}

void NETLizard_RenderMeshShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_t *light_position, int dirlight, int method, int invert)
{
    if(!mesh || !light_position)
		return;

    GL_NETLizard_3D_Mesh m;
    memset(&m, 0, sizeof(GL_NETLizard_3D_Mesh));

    if(cale_mesh_transform(&m, mesh, invert))
        render_shadow_volume_mesh(&m, light_position, dirlight, method, invert);

    delete_GL_NETLizard_3D_Mesh(&m);
}

