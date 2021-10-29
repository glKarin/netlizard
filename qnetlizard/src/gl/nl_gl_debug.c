#include "nl_gl_debug.h"

#include <stdlib.h>

#include "lib/vector3.h"
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

void NETLizard_DebugRenderGL3DModel(const GL_NETLizard_3D_Model *model, GL_NETLizard_Debug_Render_Mesh_f func)
{
    if(!model || !func)
		return;

	if(model->meshes)
	{
		GLuint i;
		for(i = 0; i < model->count; i++)
		{
			GL_NETLizard_3D_Mesh *m = model->meshes + i;
            func(m);
            if(model->item_meshes && 0)
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
		}
	}

    if(model->item_meshes)
	{
		GLuint i;
		for(i = 0; i < model->item_count; i++)
        {
			GL_NETLizard_3D_Item_Mesh *m = model->item_meshes + i;
            if(!m->materials) // REDO
				continue;
			if(m->item_type == Item_Box_Type)
                continue;
            func(m);
		}
	}
}

void NETLizard_DebugRenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count, GL_NETLizard_Debug_Render_Mesh_f func)
{
    if(!model)
        return;

    GLboolean all = !scene || count == 0;

    if(model->meshes)
    {
        if(all)
        {
            GLuint i;
            for(i = 0; i < model->count; i++)
            {
                GL_NETLizard_3D_Mesh *m = model->meshes + i;
                func(m);
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
            }
        }
        else
        {
            GLint c = model->count;
            GLuint i;
            for(i = 0; i < count; i++)
            {
                if(scene[i] >= 0 && scene[i] < c)
                {
                    GL_NETLizard_3D_Mesh *m = model->meshes + scene[i];
                    func(m);
                    if(model->item_meshes)
                    {
                        GLuint j;
                        for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++)
                        {
                            GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                            if(!im->materials) // REDO
                                continue;
                            if(im->item_type == Item_Box_Type)
                                continue;
                            func(im);
                        }
                    }
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

                    vector3_s v = VECTOR3V(vertex->normal);
                    vector3_scalev(&v, NORMAL_LENGTH);
                    vector3_s p = VECTOR3V(vertex->position);
                    vector3_addv_self(&p, &v);
                    GLfloat vs[] = {
                        vertex->position[0], vertex->position[1], vertex->position[2],
                        VECTOR3_X(p), VECTOR3_Y(p), VECTOR3_Z(p),
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

void NETLizard_DebugRenderGL3DMapModelBound(const GL_NETLizard_3D_Model *model)
{
    if(!model)
        return;

    bound_s bound;
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
