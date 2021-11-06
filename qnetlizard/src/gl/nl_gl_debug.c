#include "nl_gl_debug.h"

#include <stdlib.h>

#include "lib/vector3.h"
#include "lib/line.h"
#include "lib/bound.h"
#include "nl_util.h"

#define BEGIN_DEBUG_RENDER \
    glDepthMask(GL_FALSE); \
    glDisable(GL_DEPTH_TEST); \
    glDisable(GL_ALPHA_TEST); \
    glDisable(GL_BLEND); \
    glDisable(GL_TEXTURE_2D); \
    glDisable(GL_CULL_FACE); \
    glLineWidth(2); \
    glPointSize(6); \
    glEnable(GL_LINE_SMOOTH); \
    glEnable(GL_POINT_SMOOTH); \
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); \
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); \
    GLfloat _cur_color[4]; \
    glGetFloatv(GL_CURRENT_COLOR, _cur_color);

#define END_DEBUG_RENDER \
    glDepthMask(GL_TRUE); \
    glEnable(GL_DEPTH_TEST); \
    glEnable(GL_ALPHA_TEST); \
    glEnable(GL_BLEND); \
    glEnable(GL_TEXTURE_2D); \
    glEnable(GL_CULL_FACE); \
    glLineWidth(1); \
    glPointSize(1); \
    glDisable(GL_LINE_SMOOTH); \
    glDisable(GL_POINT_SMOOTH); \
    glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE); \
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE); \
    glColor4fv(_cur_color);

#define NORMAL_LENGTH 100
#define POINT_COLOR 0,0,1,1
#define LINE_COLOR 1,0,0,1

#define BITS_FALSE(b, t) (((b) & (t)) == 0)
#define BITS_TRUE(b, t) (((b) & (t)) != 0)

void NETLizard_DebugRenderGL3DModel(const GL_NETLizard_3D_Model *model, GLuint type, GL_NETLizard_Debug_Render_Mesh_f func)
{
    if(!model || !func)
		return;
    if(BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_ITEM) && BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_SCENE))
        return;

    if(model->meshes && BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_SCENE))
	{
		GLuint i;
		for(i = 0; i < model->count; i++)
		{
            const GL_NETLizard_3D_Mesh *m = model->meshes + i;
            func(m);
#if 0
            if(model->item_meshes)
			{
				GLuint j;
				for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++) 
                {
					GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                    if(!im->materials) // REDO
                        continue;
                    func(im);
				}
			}
#endif
		}
	}

    if(model->item_meshes && BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_ITEM))
	{
		GLuint i;
		for(i = 0; i < model->item_count; i++)
        {
            const GL_NETLizard_3D_Item_Mesh *m = model->item_meshes + i;
            if(!m->materials) // REDO
				continue;
            if(m->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
                continue;
            func(m);
		}
	}
}

void NETLizard_DebugRenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count, GLuint type, GL_NETLizard_Debug_Render_Mesh_f func)
{
    if(!model)
        return;
    if(BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_ITEM) && BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_SCENE))
        return;

    if(!model->meshes)
        return;

    GLuint c = scene ? count : model->count;
    GLuint i;
    for(i = 0; i < c; i++)
    {
        int s = scene ? scene[i] : i;
        if(s >= 0 && s < model->count)
        {
            const GL_NETLizard_3D_Mesh *m = model->meshes + s;
            if(BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_SCENE))
                func(m);

            if(model->item_meshes && BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_ITEM))
            {
                GLuint j;
                for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++)
                {
                    const GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                    if(!im->materials) // REDO
                        continue;
                    if(im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
                        continue;
                    func(im);
                }
            }
        }
    }
}

GLvoid NETLizard_DebugRenderGL3DMeshVertexNormal(const GL_NETLizard_3D_Mesh *m)
{
    if(!m)
		return;

    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);

        glPushMatrix();
        {
            glTranslatef(m->position[0], m->position[1], m->position[2]);
            glRotatef(m->rotation[0], 1.0f, 0.0f, 0.0f);
            glRotatef(m->rotation[1], 0.0f, 0.0f, 1.0f);

            GLuint j;
            GLuint i;

            for(j = 0; j < m->count; j++)
            {
                const GL_NETLizard_3D_Material *mat = m->materials + j;
                for(i = 0; i < mat->index_count; i++)
                {
                    GLushort index = mat->index[i];
                    const GL_NETLizard_3D_Vertex *vertex = m->vertex_data.vertex + index;

                    glColor4f(POINT_COLOR);
                    glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), vertex->position);
                    glDrawArrays(GL_POINTS, 0, 1);

                    line_t line;
                    ray_t ray = RAYV(vertex->position, vertex->normal);
                    line_ray_to_line(&line, &ray, NORMAL_LENGTH);
                    GLfloat vs[] = {
                        LINE_A_X(line), LINE_A_Y(line), LINE_A_Z(line),
                        LINE_B_X(line), LINE_B_Y(line), LINE_B_Z(line),
                    };
                    glColor4f(LINE_COLOR);
                    glVertexPointer(3, GL_FLOAT, 0, vs);
                    glDrawArrays(GL_LINES, 0, 2);
                }
            }
        }
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}

GLvoid NETLizard_DebugRenderGL3DMeshBound(const GL_NETLizard_3D_Mesh *m)
{
    if(!m)
        return;

    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
        {
            glTranslatef(m->position[0], m->position[1], m->position[2]);
            glRotatef(m->rotation[0], 1.0f, 0.0f, 0.0f);
            glRotatef(m->rotation[1], 0.0f, 0.0f, 1.0f);

            GLfloat vs[] = {
                m->box.min[0], m->box.min[1], m->box.min[2],
                m->box.min[0], m->box.min[1], m->box.max[2],
                m->box.min[0], m->box.max[1], m->box.min[2],
                m->box.min[0], m->box.max[1], m->box.max[2],

                m->box.max[0], m->box.min[1], m->box.min[2],
                m->box.max[0], m->box.min[1], m->box.max[2],
                m->box.max[0], m->box.max[1], m->box.min[2],
                m->box.max[0], m->box.max[1], m->box.max[2],
            };

            GLushort index[] = {
                0, 1,
                0, 2,
                1, 3,
                2, 3,

                4, 5,
                4, 6,
                5, 7,
                6, 7,

                0, 4,
                1, 5,
                2, 6,
                3, 7,
            };
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glColor4f(LINE_COLOR);
            glDrawElements(GL_LINES, sizeof(index) / sizeof(index[0]), GL_UNSIGNED_SHORT, index);
            glColor4f(POINT_COLOR);
            glDrawArrays(GL_POINTS, 0, 8);
        }
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}

GLvoid NETLizard_DebugRenderGL3DMeshPlane(const GL_NETLizard_3D_Mesh *m)
{
    if(!m)
        return;
    if(!m->plane)
        return;

    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);

        glPushMatrix();
        {
            glTranslatef(m->position[0], m->position[1], m->position[2]);
            glRotatef(m->rotation[0], 1.0f, 0.0f, 0.0f);
            glRotatef(m->rotation[1], 0.0f, 0.0f, 1.0f);

            GLuint j;

            for(j = 0; j < m->plane_count; j++)
            {
                const GL_NETLizard_3D_Plane *plane = m->plane + j;

                glColor4f(POINT_COLOR);
                glVertexPointer(3, GL_FLOAT, 0, plane->position);
                glDrawArrays(GL_POINTS, 0, 1);

                line_t line;
                ray_t ray = RAYV(plane->position, plane->normal);
                line_ray_to_line(&line, &ray, NORMAL_LENGTH);
                GLfloat vs[] = {
                    LINE_A_X(line), LINE_A_Y(line), LINE_A_Z(line),
                    LINE_B_X(line), LINE_B_Y(line), LINE_B_Z(line),
                };
                glColor4f(LINE_COLOR);
                glVertexPointer(3, GL_FLOAT, 0, vs);
                glDrawArrays(GL_LINES, 0, 2);
            }
        }
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}

GLvoid NETLizard_DebugRenderGL3DMapModelBSP(const GL_NETLizard_3D_Model *m)
{
    if(!m)
        return;
    if(!m->bsp_data)
        return;


    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);

        GLuint i;
        for(i = 0; i < m->bsp_count; i++)
        {
            const GL_NETLizard_BSP_Tree_Node *node = m->bsp_data + i;

            GLushort index[] = {
                0, 1,
                3, 2,
            };
            glVertexPointer(3, GL_FLOAT, 0, node->plane);
            glColor4f(LINE_COLOR);
            glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, index);
            glColor4f(POINT_COLOR);
            glDrawArrays(GL_POINTS, 0, 4);

            bound_t bound;
            vector3_t vecs[] = {
                VECTOR3V(node->plane[0]),
                VECTOR3V(node->plane[1]),
                VECTOR3V(node->plane[2]),
                VECTOR3V(node->plane[3]),
            };
            bound_make_with_vertors(&bound, vecs, 4);
            vector3_t center;
            bound_center(&bound, &center);
            glColor4f(POINT_COLOR);
            glVertexPointer(3, GL_FLOAT, 0, VECTOR3_V(center));
            glDrawArrays(GL_POINTS, 0, 1);

            line_t line;
            ray_t ray = RAYV(VECTOR3_V(center), node->normal);
            line_ray_to_line(&line, &ray, NORMAL_LENGTH);
            GLfloat vs[] = {
                LINE_A_X(line), LINE_A_Y(line), LINE_A_Z(line),
                LINE_B_X(line), LINE_B_Y(line), LINE_B_Z(line),
            };
            glColor4f(LINE_COLOR);
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glDrawArrays(GL_LINES, 0, 2);
        }

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}

void NETLizard_DebugRenderGL3DMapModelBound(const GL_NETLizard_3D_Model *model)
{
    if(!model)
        return;

    bound_t bound;
    NETLizard_GetNETLizard3DMapBound(model, NULL, 0, &bound);
    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
        {
            GLfloat vs[] = {
                BOUND_MIN_X(bound), BOUND_MIN_Y(bound), BOUND_MIN_Z(bound),
                BOUND_MIN_X(bound), BOUND_MIN_Y(bound), BOUND_MAX_Z(bound),
                BOUND_MIN_X(bound), BOUND_MAX_Y(bound), BOUND_MIN_Z(bound),
                BOUND_MIN_X(bound), BOUND_MAX_Y(bound), BOUND_MAX_Z(bound),

                BOUND_MAX_X(bound), BOUND_MIN_Y(bound), BOUND_MIN_Z(bound),
                BOUND_MAX_X(bound), BOUND_MIN_Y(bound), BOUND_MAX_Z(bound),
                BOUND_MAX_X(bound), BOUND_MAX_Y(bound), BOUND_MIN_Z(bound),
                BOUND_MAX_X(bound), BOUND_MAX_Y(bound), BOUND_MAX_Z(bound),
            };

            GLushort index[] = {
                0, 1,
                0, 2,
                1, 3,
                2, 3,

                4, 5,
                4, 6,
                5, 7,
                6, 7,

                0, 4,
                1, 5,
                2, 6,
                3, 7,
            };
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glColor4f(LINE_COLOR);
            glDrawElements(GL_LINES, sizeof(index) / sizeof(index[0]), GL_UNSIGNED_SHORT, index);
            glColor4f(POINT_COLOR);
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glDrawArrays(GL_POINTS, 0, 8);
        }
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}
