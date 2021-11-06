#include "nl_shadow_render.h"

#include <QDebug>
#include <string.h>
#include <stdio.h>

#include "stencil_shadow.h"

#define SHADOW_BEGIN \
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); \
	glDepthMask(GL_FALSE); \
	glEnable(GL_STENCIL_TEST); \
    glStencilFunc(GL_ALWAYS, 0, ~0U); \
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); \
    glDisable(GL_BLEND); \
    glDisable(GL_ALPHA_TEST); \
    glDisable(GL_FOG); \
    glEnable(GL_POLYGON_OFFSET_FILL); \
    glPolygonOffset(0.1, 0.1); \
    glClear(GL_STENCIL_BUFFER_BIT);


#define SHADOW_END \
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); \
	glDepthMask(GL_TRUE); \
	glDisable(GL_STENCIL_TEST); \
	glCullFace(GL_BACK); \
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); \
    glEnable(GL_BLEND); \
    glStencilFunc(GL_ALWAYS, 0, ~0U); \
    glDisable(GL_POLYGON_OFFSET_FILL); \
    glPolygonOffset(0, 0); \
    glEnable(GL_ALPHA_TEST);

#define SHADOW_MASK_Z 1
#define SHADOW_MASK_W 5000
static void render_shadow_mask(void)
{
    const GLfloat mask_color[] = {
#if SHADOW_MASK_LIGHT
        1.0, 1.0, 1.0, 0.1
#else
            0.0, 0.0, 0.0, 1 - 0.618
    };
#endif
    GLfloat mask[] = {
        -SHADOW_MASK_W, -SHADOW_MASK_W, -SHADOW_MASK_Z,
        SHADOW_MASK_W, -SHADOW_MASK_W, -SHADOW_MASK_Z,
        -SHADOW_MASK_W, SHADOW_MASK_W, -SHADOW_MASK_Z,
        SHADOW_MASK_W, SHADOW_MASK_W, -SHADOW_MASK_Z
    };

    GLfloat cur_color[4];
    glGetFloatv(GL_CURRENT_COLOR, cur_color);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDepthMask(GL_FALSE);
#if SHADOW_MASK_LIGHT
    glStencilFunc(GL_EQUAL, 0, ~0U);
#else
    glStencilFunc(GL_NOTEQUAL, 0, ~0U);
#endif


    glPushMatrix();
    glLoadIdentity();
    glColor4fv(mask_color);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mask);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor4fv(cur_color);

    glPopMatrix();
    glDisable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0U);
    glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
}

void NETLizard_RenderNETLizardModelSceneShadow(const GL_NETLizard_3D_Model *map_model, const int scenes[], unsigned int count, const vector3_t *light_position, int dirlight, int method, int render_mask)
{
	GLuint i;
	GLint c;
	GLuint j;
	GL_NETLizard_3D_Mesh *m;
    GL_NETLizard_3D_Item_Mesh *im;

    if(!map_model || !light_position)
        return;

    GLboolean all = !scenes || count == 0;

    if(all)
    {
        NETLizard_RenderNETLizardModelShadow(map_model, light_position, dirlight, method, render_mask);
    }
    else
    {
        if((render_mask & NETLIZARD_SHADOW_RENDER_SCENE_WALL) || (render_mask & NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR))
        {
            int invert = (render_mask & NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR) ? SHADOW_INVERT_ALL : SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR;
            SHADOW_BEGIN
            {
                c = map_model->count;
                for(i = 0; i < count; i++)
                {
                    if(scenes[i] < 0 && scenes[i] >= c)
                        continue;
                    m = map_model->meshes + scenes[i];
                    NETLizard_RenderMeshShadow(m, light_position, dirlight, method, invert);
                }
            }
            SHADOW_END
            render_shadow_mask();
        }
        if(render_mask & NETLIZARD_SHADOW_RENDER_ITEM)
        {
            SHADOW_BEGIN
            {
                c = map_model->count;
                for(i = 0; i < count; i++)
                {
                    if(scenes[i] < 0 && scenes[i] >= c)
                        continue;
                    m = map_model->meshes + scenes[i];
                    for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++)
                    {
                        im = map_model->item_meshes + j;
                        if(!im->materials) // REDO
                            continue;
                        if(im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
                            continue;
                        NETLizard_RenderMeshShadow(im, light_position, dirlight, method, 0);
                    }
                }
            }
            SHADOW_END
            render_shadow_mask();
        }
    }
}

void NETLizard_RenderNETLizardModelShadow(const GL_NETLizard_3D_Model *map_model, const vector3_t *light_position, int dirlight, int method, int render_mask)
{
	GLuint i;
	GLuint j;
	GL_NETLizard_3D_Mesh *m;
	GL_NETLizard_3D_Item_Mesh *im;

    if(!map_model || !light_position)
        return;

    if((render_mask & NETLIZARD_SHADOW_RENDER_SCENE_WALL) || (render_mask & NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR))
    {
        SHADOW_BEGIN
        {
            int invert = (render_mask & NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR) ? SHADOW_INVERT_ALL : SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR;
            for(i = 0; i < map_model->count; i++)
            {
                m = map_model->meshes + i;
                NETLizard_RenderMeshShadow(m, light_position, dirlight, method, invert);
            }
        }
        SHADOW_END
        render_shadow_mask();
    }
    if(render_mask & NETLIZARD_SHADOW_RENDER_ITEM)
    {
        SHADOW_BEGIN
        {
            for(i = 0; i < map_model->count; i++)
            {
                m = map_model->meshes + i;
                for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++)
                {
                    im = map_model->item_meshes + j;
                    if(!im->materials) // REDO
                        continue;
                    if(im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
                        continue;
                    NETLizard_RenderMeshShadow(im, light_position, dirlight, method, 0);
                }
            }
        }
        SHADOW_END
        render_shadow_mask();
    }
}

#if 0
void Shadow_RenderNETLizard3DAnimationModel(const GL_NETLizard_3D_Animation_Model *m, int a, int f, const float pos[3], float xangle, float yangle, const Light_Source_s *light, int method)
{
	GL_NETLizard_3D_Item_Mesh item_mesh;
	NETLizard_3D_Role_Animation *animation;
	GL_NETLizard_3D_Mesh *mesh;
	unsigned int anim;
	unsigned int frame;

	if(!m || !light)
		return;

	SHADOW_BEGIN

	anim = a;
	frame = f;
	ZERO(&item_mesh, GL_NETLizard_3D_Item_Mesh);

	if(anim >= m->anim_count)
		anim = 0;
	animation = m->animations + anim;
	if(frame > animation->end - animation->begin)
		frame = 0;
	mesh = m->meshes + animation->begin + frame;

	if(pos)
	{
		item_mesh.pos[0] = pos[0];
		item_mesh.pos[1] = pos[1];
		item_mesh.pos[2] = pos[2];
	}
	item_mesh.angle[0] = xangle;
	item_mesh.angle[1] = yangle;

	item_mesh.item_mesh = *mesh;

	Shadow_RenderItemShadow(&item_mesh, light, method);

	SHADOW_END
}

void Shadow_RenderNETLizard3DItemMesh(const GL_NETLizard_3D_Item_Mesh *m, const Light_Source_s *light, int method)
{
	if(!m || !light)
		return;

	SHADOW_BEGIN

	Shadow_RenderItemShadow(m, light, method);

	SHADOW_END
}
#endif

void NETLizard_RenderNETLizard3DMeshShadow(const GL_NETLizard_3D_Mesh *m, const vector3_t *light_position, int dirlight, int method)
{
    if(!m || !light_position)
		return;

    SHADOW_BEGIN

    NETLizard_RenderMeshShadow(m, light_position, dirlight, method, 0);

    SHADOW_END
            render_shadow_mask();
}
