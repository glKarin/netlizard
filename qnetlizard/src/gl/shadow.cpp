#include "shadow.h"

#include <QList>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/line.h"
#include "lib/mesa_gl_math.h"
#include "nlmath.h"

#define F_ZERO 0.1

#define Shadow_Vec3cmp Vector3_EqualsVector3
#define Shadow_Linecmp Line_EqualsLine

typedef QList<line_s> LineList;
typedef QList<vector3_s> Vector3List;

static int push_line(Vector3List &list, const line_s *lp)
{
    int has = 0;
    int o;

    for(o = 0; o < list.count; o++) // find in lines list
    {
        const line_s &line = list[o];
        if(line_equals_ignore_seq(lp, &line)) // if exists, remove this line, then continue
        {
            //printf("%d exist\n", i);
            has = 1;
#if 0
glDisable(GL_DEPTH_TEST);
glEnableClientState(GL_VERTEX_ARRAY);
glColor4f(1.0, 0.0, 1.0, 1.0);
glDepthFunc(GL_LEQUAL);
glLineWidth(4.0);
glPushMatrix();
{
GLfloat vs[6];
printf("%d %d\n", o, lines.count);
vs[0] = VECTOR_X(lp.a);
vs[1] = VECTOR_Y(lp.a);
vs[2] = VECTOR_Z(lp.a);

vs[3] = VECTOR_X(lp.b);
vs[4] = VECTOR_Y(lp.b);
vs[5] = VECTOR_Z(lp.b);
glVertexPointer(3, GL_FLOAT, 0, vs);
glDrawArrays(GL_LINES, 0, 2);
}
glPopMatrix();
glLineWidth(1.0);
glDepthFunc(GL_LESS);
glEnable(GL_DEPTH_TEST);
glColor4f(1.0, 1.0, 1.0, 1.0);
glDisableClientState(GL_VERTEX_ARRAY);
#endif
            break;
        }
    }

    if(has)
    {
        list.removeAt(o);
    }
    else // if not exists, add new line to list
    {
        list.push_back(*lp);
#if 0
glDisable(GL_DEPTH_TEST);
glEnableClientState(GL_VERTEX_ARRAY);
glColor4f(0.0, 0.0, 1.0, 1.0);
glDepthFunc(GL_LEQUAL);
glLineWidth(4.0);
glPushMatrix();
{
GLfloat vs[6];
printf("%d %d\n", o, lines.count);
lpptr = List_GetConstDataByIndexT(&lines, o, line_s);
vs[0] = VECTOR_X(lpptr->a);
vs[1] = VECTOR_Y(lpptr->a);
vs[2] = VECTOR_Z(lpptr->a);

vs[3] = VECTOR_X(lpptr->b);
vs[4] = VECTOR_Y(lpptr->b);
vs[5] = VECTOR_Z(lpptr->b);
glVertexPointer(3, GL_FLOAT, 0, vs);
glDrawArrays(GL_LINES, 0, 2);
}
glPopMatrix();
glLineWidth(1.0);
glDepthFunc(GL_LESS);
glEnable(GL_DEPTH_TEST);
glColor4f(1.0, 1.0, 1.0, 1.0);
glDisableClientState(GL_VERTEX_ARRAY);
#endif
    }
    return has;
}

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
    return(((compare_vector3(&l1->a, &l2->a) && compare_vector3(&l1->b, &l2->b)) || (compare_vector3(&l1->a, &l2->b) && compare_vector3(&l1->b, &l2->a))) ? 1 : 0);
}

vector3_s cale_light_direction(const vector3_s *v, const vector3_s *lightpos, int dirlight)
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

void Shadow_CaleTrans(GL_NETLizard_3D_Mesh *r, const GL_NETLizard_3D_Mesh *nl_mesh, int invert)
{
    GLmatrix nor_mat;
    GLmatrix mat;
	int count;
	unsigned int i, j;
	int m, n;
    GL_NETLizard_3D_Vertex *point;
	const GL_NETLizard_3D_Vertex *nl_vertex;
	GLushort *nl_indexs;
	const GL_NETLizard_3D_Material *nl_mat;
	vector3_s v;

	if(!r || !nl_mesh)
		return;

    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&nor_mat);
    memset(r, 0, sizeof(GL_NETLizard_3D_Mesh));
	count = 0;
	for(i = 0; i < nl_mesh->count; i++)
	{
		count += nl_mesh->materials[i].index_count;
	}
    r->count = 1;
    r->materials = calloc(1, GL_NETLizard_3D_Material);
    r->materials[0].mode = GL_TRIANGLES;
    r->materials[0].index_start = 0;
    r->materials[0].index_count = count;
    r->vertex_data.index_count = count;
    r->vertex_data.index = calloc(count, sizeof(GLushort));
    r->vertex_data.vertex_count = count;
    r->vertex_data.vertex = calloc(count, sizeof(GL_NETLizard_3D_Vertex));

	n = 0;
    nl_indexs = nl_mesh->vertex_data.index;
	for(i = 0; i < nl_mesh->count; i++)
	{
		nl_mat = nl_mesh->materials + i;
		for(j = nl_mat->index_start; j < nl_mat->index_count; j++)
		{
            point = r->vertex_data.vertex + n;
			m = nl_indexs[j];
            nl_vertex = nl_mesh->vertex_data.vertex + m;

            Mesa_glLoadIdentity(&mat);
            Mesa_glTranslate(&mat, nl_mesh->position[0], nl_mesh->position[1], nl_mesh->position[2]);
            Mesa_glRotate(&mat, nl_mesh->rotation[0], 1.0f, 0.0f, 0.0f);
            Mesa_glRotate(&mat, nl_mesh->rotation[1], 0.0f, 0.0f, 1.0f);

            NL::cale_normal_matrix(&nor_mat, &mat);

            Mesa_glTransform(point->position, nl_vertex->position, &mat);
            Mesa_glTransform(point->normal, nl_vertex->normal, &nor_mat);
            memcpy(point->texcoord, nl_vertex->texcoord, sizeof(GLfloat) * 2);
//            if(invert)
//            {
//                point->normal[0] = -point->normal[0];
//                point->normal[1] = -point->normal[1];
//                point->normal[2] = -point->normal[2];
//            }

            r->vertex_data.index[n] = n;
			n++;
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
#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glLineWidth(4);
	glPointSize(25);
	GLfloat nv[6];
	for(i = 0; i < r->count; i++)
	{
		vector3_s rv = VECTOR3V(r->points[i].vertex);
		nv[0] = VECTOR_X(rv);
		nv[1] = VECTOR_Y(rv);
		nv[2] = VECTOR_Z(rv);

		vector3_s rv2 = VECTOR3V(r->points[i].normal);
		Vector3_ScaleSelf(&rv2, 20);
		rv2 = Vector3_PlusVector3(&rv, &rv2);
		nv[3] = VECTOR_X(rv2);
		nv[4] = VECTOR_Y(rv2);
		nv[5] = VECTOR_Z(rv2);
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
#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glPointSize(20);
	GLfloat nv[3];
	for(i = 0; i < r->count; i++)
	{
	glColor4f(0.0, 1.0, 1.0, 1.0);
		vector3_s rv0 = VECTOR3(0, 0, 0);
		rv0 = Math3D_Vector3MultiplyMatrix44(&rv0, mat);
		nv[0] = VECTOR_X(rv0);
		nv[1] = VECTOR_Y(rv0);
		nv[2] = VECTOR_Z(rv0);
		glVertexPointer(3, GL_FLOAT, 0, nv);
		glDrawArrays(GL_POINTS, 0, 1);

	glColor4f(0.0, 1.0, 0.0, 1.0);
		vector3_s rv = VECTOR3V(r->points[i].vertex);
		nv[0] = VECTOR_X(rv);
		nv[1] = VECTOR_Y(rv);
		nv[2] = VECTOR_Z(rv);
		glVertexPointer(3, GL_FLOAT, 0, nv);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glPointSize(1);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

// cale shadow volume
void Shadow_MakeVolume(GL_NETLizard_3D_Mesh *r, const vector3_s *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method)
{
	int has;
    line_s lp;
    int i, k, n, o;
    GL_NETLizard_3D_Vertex *pa;
    GL_NETLizard_3D_Vertex *m;
	GLfloat *v;

	if(!r || !light || !mat || !mat->count)
		return;

    QList<line_s> lines;
    QList<vector3_s> tops;
    QList<vector3_s> bottoms;

    memset(r, 0, sizeof(GL_NETLizard_3D_Mesh));
    r->count = method == SHADOW_Z_FAIL ? 3 : 1; // top[1] bottom[2] caps, and side[0]
    r->materials = calloc(r->count, sizeof(GL_NETLizard_3D_Material));

    for(i = 0; i < mat->vertex_data.vertex_count / 3; i++) // cale triangle
	{
        pa = mat->vertex_data.vertex + (i * 3);
        v = pa->position;
		vector3_s nor = VECTOR3V(pa->normal);
		vector3_s p = VECTOR3V(v);
        vector3_s p2l = cale_light_direction(&p, light_position, dirlight);
#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0, 1.0, 0.0, 1.0);
	{
		GLfloat nv[6];
		nv[0] = VECTOR_X(p);
		nv[1] = VECTOR_Y(p);
		nv[2] = VECTOR_Z(p);

		nv[3] = VECTOR_X(light->position);
		nv[4] = VECTOR_Y(light->position);
		nv[5] = VECTOR_Z(light->position);
		glVertexPointer(3, GL_FLOAT, 0, nv);
		glDrawArrays(GL_LINES, 0, 2);
	}
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
        vector3_invertv(&p2l);
        float dot_p = vector3_dot(&p2l, &nor);
#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	if(dot_p > 0.0) // face to light source
		glColor4f(1.0, 0.0, 0.0, 1.0);
	else
		glColor4f(0.0, 1.0, 0.0, 1.0);
	{
		GLfloat nv[6];
		nv[0] = VECTOR_X(p);
		nv[1] = VECTOR_Y(p);
		nv[2] = VECTOR_Z(p);
		vector3_s p2 = p;
		vector3_s p3 = Vector3_Scale(&p2l, 200);
		p2 = Vector3_PlusVector3(&p2, &p3);

		nv[3] = VECTOR_X(p2);
		nv[4] = VECTOR_Y(p2);
		nv[5] = VECTOR_Z(p2);
		glVertexPointer(3, GL_FLOAT, 0, nv);
		glDrawArrays(GL_LINES, 0, 2);
	}
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
		//qDebug()<<i<<nor<<p<<dot_p;
		if(dot_p > 0.0) // face to light source
		{
			for(n = 0; n < 3; n++) // 3 points of triangle
            {
                int next_index = (n + 1) % 3;
                LINE_A_X(lp) = pa[n].position[0];
                LINE_A_Y(lp) = pa[n].position[1];
                LINE_A_Z(lp) = pa[n].position[2];
                LINE_B_X(lp) = pa[next_index].position[0];
                LINE_B_Y(lp) = pa[next_index].position[1];
                LINE_B_Z(lp) = pa[next_index].position[2];
                has = push_line(lines, &lp);
			}
			// top cap triangle
			if(method == SHADOW_Z_FAIL)
			{
                vector3_s vec = VECTOR3V(pa[0].position);
                tops.push_back(vec);
                VECTOR3_X(vec) = pa[1].position[0];
                VECTOR3_Y(vec) = pa[1].position[1];
                VECTOR3_Z(vec) = pa[1].position[2];
                tops.push_back(vec);
                VECTOR3_X(vec) = pa[2].position[0];
                VECTOR3_Y(vec) = pa[2].position[1];
                VECTOR3_Z(vec) = pa[2].position[2];
                tops.push_back(vec);
			}
		}
		else
		{
			// bottom cap triangle
			if(method == SHADOW_Z_FAIL)
			{
                vector3_s vec = VECTOR3V(pa[0].position);
                bottoms.push_back(vec);
                VECTOR3_X(vec) = pa[1].position[0];
                VECTOR3_Y(vec) = pa[1].position[1];
                VECTOR3_Z(vec) = pa[1].position[2];
                bottoms.push_back(vec);
                VECTOR3_X(vec) = pa[2].position[0];
                VECTOR3_Y(vec) = pa[2].position[1];
                VECTOR3_Z(vec) = pa[2].position[2];
                bottoms.push_back(vec);
			}
		}
	}
#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(4.0);
	glPushMatrix();
	{
		GLfloat vs[6];
		for(o = 0; o < lines.count; o++) // find in lines list
		{
			//printf("%d %d\n", o, lines.count);
			lpptr = List_GetConstDataByIndexT(&lines, o, line_s);
			vs[0] = VECTOR_X(lpptr->a);
			vs[1] = VECTOR_Y(lpptr->a);
			vs[2] = VECTOR_Z(lpptr->a);

			vs[3] = VECTOR_X(lpptr->b);
			vs[4] = VECTOR_Y(lpptr->b);
			vs[5] = VECTOR_Z(lpptr->b);
			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
	glPopMatrix();
	glLineWidth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif

	// cale sides of shadow volume
    int vcount = lines.size() * 6; // 2 triangles(6 points) every a line
    if(method == SHADOW_Z_FAIL)
    {
        vcount += tops.size();
        vcount += bottoms.size();
    }
    r->vertex_data.vertex_count = vcount;
    r->vertex_data.vertex = calloc(r->vertex_data.vertex_count, sizeof(GL_NETLizard_3D_Vertex));
    r->vertex_data.index_count = vcount;
    r->vertex_data.index = calloc(r->vertex_data.index_count, sizeof(GLushort));
	k = 0;
    m = r->vertex_data.vertex;
	// TODO: cale clock wise, now the lighting source must be above all cubes
    for(i = 0; i < lines.size(); i++)
	{
        const line_s &lpptr = lines[i];
		lpptr = List_GetDataByIndexT(&lines, i, line_s);
		// point lighting
        vector3_s dir_a = cale_light_direction(&(LINE_A(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_a, SHADOW_VOLUME_LENGTH);
        dir_a = vector3_add(&(LINE_A(lpptr)), &dir_a);
        vector3_s dir_b = cale_light_direction(&(LINE_B(lpptr)), light_position, dirlight);
        vector3_scalev(&dir_b, SHADOW_VOLUME_LENGTH);
        dir_b = vector3_add(&(LINE_A(lpptr)), &dir_b);

#if 0
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(4.0);
	glPushMatrix();
	{
		GLfloat vs[6];
			//printf("%d %d\n", o, lines.count);
			vs[0] = VECTOR_X(lpptr->a);
			vs[1] = VECTOR_Y(lpptr->a);
			vs[2] = VECTOR_Z(lpptr->a);

			vs[3] = VECTOR_X(dir_a);
			vs[4] = VECTOR_Y(dir_a);
			vs[5] = VECTOR_Z(dir_a);
			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_LINES, 0, 2);

			vs[0] = VECTOR_X(lpptr->b);
			vs[1] = VECTOR_Y(lpptr->b);
			vs[2] = VECTOR_Z(lpptr->b);

			vs[3] = VECTOR_X(dir_b);
			vs[4] = VECTOR_Y(dir_b);
			vs[5] = VECTOR_Z(dir_b);
			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_LINES, 0, 2);
	}
	glPopMatrix();
	glLineWidth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
#if 0
	//glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0, 1.0, 1.0, 0.2);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(4.0);
	glPushMatrix();
	{
		GLfloat vs[18];
			//printf("%d %d\n", o, lines.count);
			vs[0] = VECTOR_X(lpptr->a);
			vs[1] = VECTOR_Y(lpptr->a);
			vs[2] = VECTOR_Z(lpptr->a);

			vs[3] = VECTOR_X(dir_a);
			vs[4] = VECTOR_Y(dir_a);
			vs[5] = VECTOR_Z(dir_a);

			vs[6] = VECTOR_X(lpptr->b);
			vs[7] = VECTOR_Y(lpptr->b);
			vs[8] = VECTOR_Z(lpptr->b);

			vs[9] = VECTOR_X(dir_a);
			vs[10] = VECTOR_Y(dir_a);
			vs[11] = VECTOR_Z(dir_a);

			vs[12] = VECTOR_X(dir_b);
			vs[13] = VECTOR_Y(dir_b);
			vs[14] = VECTOR_Z(dir_b);

			vs[15] = VECTOR_X(lpptr->b);
			vs[16] = VECTOR_Y(lpptr->b);
			vs[17] = VECTOR_Z(lpptr->b);

			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glPopMatrix();
	glLineWidth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
		// triangle 1
		pa = m->points + k * 6;
		Shadow_Vector3ToArray(pa->vertex, &lpptr->a);
		pa->vertex[3] = 1;
		pa = m->points + k * 6 + 1;
		Shadow_Vector3ToArray(pa->vertex, &dir_a);
		pa->vertex[3] = SHADOW_VOLUME_FAR_W;
		pa = m->points + k * 6 + 2;
		Shadow_Vector3ToArray(pa->vertex, &lpptr->b);
		pa->vertex[3] = 1;

		// triangle 2
		pa = m->points + k * 6 + 3;
		Shadow_Vector3ToArray(pa->vertex, &dir_a);
		pa->vertex[3] = SHADOW_VOLUME_FAR_W;
		pa = m->points + k * 6 + 4;
		Shadow_Vector3ToArray(pa->vertex, &dir_b);
		pa->vertex[3] = SHADOW_VOLUME_FAR_W;
		pa = m->points + k * 6 + 5;
		Shadow_Vector3ToArray(pa->vertex, &lpptr->b);
		pa->vertex[3] = 1;

#if 0
	//glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0, 1.0, 1.0, 0.2);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(4.0);
	glPushMatrix();
	{
		GLfloat vs[18];
		// triangle 1
		pa = m->points + k * 6;
		memcpy(vs, pa->vertex, sizeof(GLfloat) * 3);
		pa = m->points + k * 6 + 1;
		memcpy(vs + 3, pa->vertex, sizeof(GLfloat) * 3);
		pa = m->points + k * 6 + 2;
		memcpy(vs + 6, pa->vertex, sizeof(GLfloat) * 3);

		// triangle 2
		pa = m->points + k * 6 + 3;
		memcpy(vs + 9, pa->vertex, sizeof(GLfloat) * 3);
		pa = m->points + k * 6 + 4;
		memcpy(vs + 12, pa->vertex, sizeof(GLfloat) * 3);
		pa = m->points + k * 6 + 5;
		memcpy(vs + 15, pa->vertex, sizeof(GLfloat) * 3);

			glVertexPointer(3, GL_FLOAT, 0, vs);
			glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glPopMatrix();
	glLineWidth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
		k++;
	}

	if(method == SHADOW_Z_FAIL)
	{
		// cale top cap of shadow volume
		// using triangles of the mesh faces to lighting source
		m = r->materials + 1;
		newmat(m, tops.count);
		for(k = 0; k < tops.count; k += 3)
		{
			const vector3_s *first = List_GetDataByIndexT(&tops, k, vector3_s);
			const vector3_s *sec = List_GetDataByIndexT(&tops, k + 1, vector3_s);
			const vector3_s *third = List_GetDataByIndexT(&tops, k + 2, vector3_s);
			pa = m->points + k;
			Shadow_Vector3ToArray(pa->vertex, first);
			pa = m->points + k + 1;
			Shadow_Vector3ToArray(pa->vertex, sec);
			pa = m->points + k + 2;
			Shadow_Vector3ToArray(pa->vertex, third);
		}

		// cale bottom cap of shadow volume
		// using triangles of the mesh not faces to lighting source
		m = r->materials + 2;
		newmat(m, bottoms.count);
		for(k = 0; k < bottoms.count; k += 3)
		{
			const vector3_s *first = List_GetDataByIndexT(&bottoms, k, vector3_s);
			const vector3_s *sec = List_GetDataByIndexT(&bottoms, k + 1, vector3_s);
			const vector3_s *third = List_GetDataByIndexT(&bottoms, k + 2, vector3_s);

			// point lighting
            vector3_s dir_a = cale_light_direction(first, &light->position, LIGHT_IS_DIRECTION(light));
			Vector3_ScaleSelf(&dir_a, LIGHT_IS_DIRECTION(light));
            vector3_s dir_b = cale_light_direction(sec, &light->position, LIGHT_IS_DIRECTION(light));
			Vector3_ScaleSelf(&dir_b, SHADOW_VOLUME_LENGTH);
            vector3_s dir_c = cale_light_direction(third, &light->position, LIGHT_IS_DIRECTION(light));
			Vector3_ScaleSelf(&dir_c, SHADOW_VOLUME_LENGTH);

			pa = m->points + k;
			Shadow_Vector3ToArray(pa->vertex, &dir_a);
			pa->vertex[3] = SHADOW_VOLUME_FAR_W;
			pa = m->points + k + 1;
			Shadow_Vector3ToArray(pa->vertex, &dir_b);
			pa->vertex[3] = SHADOW_VOLUME_FAR_W;
			pa = m->points + k + 2;
			Shadow_Vector3ToArray(pa->vertex, &dir_c);
			pa->vertex[3] = SHADOW_VOLUME_FAR_W;
		}
	}

#if 0
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	//glDisable(GL_CULL_FACE);
	//glDepthFunc(GL_LEQUAL);
	//glDisable(GL_BLEND);

	glEnableClientState(GL_VERTEX_ARRAY);

//printf("%d %d %d\n", lines.count, r->count, r->materials->count);
	r->materials->use_color = 1;
	r->materials->color[0] = 
	r->materials->color[1] = 
	r->materials->color[2] = 0.0;
	r->materials->color[3] = 0.2;
	glColor4f(1,0,0,1);
	rendermesh(r);
	//rendermat(r->materials);

#if ZFAIL_SHADOW
	glColor4f(0,1,0,1);
	rendermat(r->materials + 1);
	glColor4f(0,0,1,1);
	rendermat(r->materials + 2);
#endif

	glColor4f(1,1,1,1);

	glDepthFunc(GL_LESS);
	glLineWidth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif

__Exit:
	List_DeleteAll(&lines);
	if(method == SHADOW_Z_FAIL)
	{
		List_DeleteAll(&tops);
		List_DeleteAll(&bottoms);
	}
}

void Shadow_RenderVolume(const material_s *nl_mesh, const Light_Source_s *l, int m)
{
	mesh_s *vol;

	if(!nl_mesh || !l)
		return;

	vol = (mesh_s *)calloc(1, sizeof(mesh_s));

	Shadow_MakeVolume(vol, l, nl_mesh, m);

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

	glEnableClientState(GL_VERTEX_ARRAY);
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

		// 2-1: cale front faces of shadow volume
#ifdef _HARMATTAN_OPENGLES
		glStencilOp(GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glCullFace(GL_FRONT);
		rendermesh(vol);

		// 2-1: cale back faces of shadow volume
		glStencilOp(GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		glCullFace(GL_BACK);
		rendermesh(vol);
#else
		glDisable(GL_CULL_FACE);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		rendermesh(vol);
		glEnable(GL_CULL_FACE);
#endif

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

#if 0
		vol->materials->use_color = 1;
		vol->materials->color[0] = 
			vol->materials->color[1] = 
			vol->materials->color[2] = 0.0;
		vol->materials->color[3] = 0.2;
#endif
		// 2-1: cale front faces of shadow volume
#ifdef _HARMATTAN_OPENGLES
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
		glCullFace(GL_BACK);
		rendermesh(vol);

		// 2-1: cale back faces of shadow volume
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
		glCullFace(GL_FRONT);
		rendermesh(vol);
#else
		glDisable(GL_CULL_FACE);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
		rendermesh(vol);
		glEnable(GL_CULL_FACE);
#endif
	}
	//glDepthFunc(GL_LESS);
	glDisableClientState(GL_VERTEX_ARRAY);

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

__Exit:
	freemesh(vol);
	free(vol);
}

void Shadow_RenderItemShadow(const GL_NETLizard_3D_Item_Mesh *mesh, const Light_Source_s *light, int method)
{
	material_s m;
	glmatrix44_s mat;

	if(!mesh || !light)
		return;

	memset(&m, 0, sizeof(material_s));

	Matrix44_glIdentity(&mat);
#if 1
	Matrix44_glTranslateSelf(&mat, mesh->pos[0], mesh->pos[1], mesh->pos[2]);
	Matrix44_glRotateSelf(&mat, mesh->angle[0], 1.0f, 0.0f, 0.0f);
	Matrix44_glRotateSelf(&mat, mesh->angle[1], 0.0f, 0.0f, 1.0f);
#else
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mesh->pos[0], mesh->pos[1], mesh->pos[2]);
	glRotatef(mesh->angle[0], 1.0f, 0.0f, 0.0f);
	glRotatef(mesh->angle[1], 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat.x);
	glPopMatrix();
#endif

	Shadow_CaleTrans(&m, &mesh->item_mesh, &mat, 0);

	Shadow_RenderVolume(&m, light, method);

__Exit:
	freemat(&m);
}

void Shadow_RenderShadow(const GL_NETLizard_3D_Mesh *mesh, const Light_Source_s *light, int method)
{
	material_s m;

	if(!mesh || !light)
		return;

	memset(&m, 0, sizeof(material_s));

	Shadow_CaleTrans(&m, mesh, NULL, 1);

	Shadow_RenderVolume(&m, light, method);

__Exit:
	freemat(&m);
}

