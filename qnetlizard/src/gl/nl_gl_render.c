#include "nl_gl.h"

#include <stdlib.h>

#include "math/vector3.h"

static GLboolean NETLizard_NotCullFace(int item_type)
{
    if((item_type & NL_3D_ITEM_TYPE_THIN)
            )
        return GL_TRUE;
    return GL_FALSE;
}

static GLboolean NETLizard_NotRenderDirectly(int item_type)
{
    if((item_type & NL_3D_ITEM_TYPE_SKYBOX)
            )
        return GL_TRUE;
    return GL_FALSE;
}

GLvoid NETLizard_RenderGL3DModel(const GL_NETLizard_3D_Model *model)
{
	if(!model)
		return;

	if(model->meshes)
	{
		GLuint i;
		for(i = 0; i < model->count; i++)
        {
            const GL_NETLizard_3D_Mesh *m = model->meshes + i;
            NETLizard_RenderGL3DMesh(m, model->texes);
#if 0
            if(model->item_meshes)
			{
				GLuint j;
				for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++) 
                {
					GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                    if(!im->materials) // REDO
                        continue;
                    NETLizard_RenderGL3DMesh(im, model->texes);
				}
			}
#endif
		}
	}

    if(model->item_meshes)
	{
		GLuint i;
		for(i = 0; i < model->item_count; i++)
        {
            const GL_NETLizard_3D_Item_Mesh *m = model->item_meshes + i;
            if(NETLizard_NotRenderDirectly(m->item_type))
                continue;
            NETLizard_RenderGL3DMesh(m, model->texes);
		}
	}
}

GLvoid NETLizard_RenderGL3DMapModel(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count)
{
    if(!model || !model->meshes)
        return;

    GLuint c = scene ? count : model->count;
    GLuint i;
    for(i = 0; i < c; i++)
    {
        int s = scene ? scene[i] : i;
        if(s >= 0 && s < model->count)
        {
            const GL_NETLizard_3D_Mesh *m = model->meshes + s;
            NETLizard_RenderGL3DMesh(m, model->texes);
            if(model->item_meshes)
            {
                GLuint j;
                for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++)
                {
                    const GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                    if(NETLizard_NotRenderDirectly(im->item_type))
                        continue;
                    NETLizard_RenderGL3DMesh(im, model->texes);
                }
            }
        }
    }
}

GLvoid NETLizard_RenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count)
{
    if(!model || !model->meshes)
        return;

    GLuint c = scene ? count : model->count;
    GLuint i;
    for(i = 0; i < c; i++)
    {
        int s = scene ? scene[i] : i;
        if(s >= 0 && s < model->count)
        {
            const GL_NETLizard_3D_Mesh *m = model->meshes + s;
            NETLizard_RenderGL3DMesh(m, model->texes);
        }
    }
}

GLvoid NETLizard_RenderGL3DMapModelItem(const GL_NETLizard_3D_Model *model, GLint *items, GLuint count)
{
    if(!model || !model->item_meshes)
        return;

    GLuint c = items ? count : model->item_count;
    GLuint i;
    for(i = 0; i < c; i++)
    {
        int s = items ? items[i] : i;
        if(s >= 0 && s < model->item_count)
        {
            const GL_NETLizard_3D_Mesh *im = model->item_meshes + s;
            if(NETLizard_NotRenderDirectly(im->item_type))
                continue;
            NETLizard_RenderGL3DMesh(im, model->texes);
        }
    }
}

GLvoid NETLizard_RenderGL3DItemModel(const GL_NETLizard_3D_Item_Model *m)
{
	if(!m)
		return;
    NETLizard_RenderGL3DItemMesh(&(m->item_mesh), &m->tex);
}

GLvoid NETLizard_RenderGL3DModelFrameAnimation(const GL_NETLizard_3D_Model *m, const NETLizard_3D_Frame_Animation *config, GLuint anim, GLuint frame)
{
    if(!m || !config)
		return;
    if(anim >= NL_FRAME_ANIMATION_TOTAL || frame < 0)
        return;
    const NETLizard_3D_Frame_Animation *c = config + anim;
    if(frame >= c->count)
        return;
    int f = c->begin_frame + frame;
    GL_NETLizard_3D_Mesh *mesh = m->item_meshes + f;
    NETLizard_RenderGL3DMesh(mesh, m->texes);
    //NETLizard_RenderGL3DItemMesh(mesh, m->texes[mesh->tex_index[0]]);
}

GLvoid NETLizard_RenderGL3DMesh(const GL_NETLizard_3D_Mesh *m, texture_s **const texes)
{
    if(!m || !m->materials)
		return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLboolean notcullface = NETLizard_NotCullFace(m->item_type);
    if(notcullface)
        glDisable(GL_CULL_FACE);

    glPushMatrix();
    {
        glTranslatef(m->position[0], m->position[1], m->position[2]);
        glRotatef(m->rotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(m->rotation[1], 0.0f, 0.0f, 1.0f);
        glRotatef(m->rotation[2], 0.0f, 1.0f, 0.0f);

        glNormalPointer(GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].normal);
        glTexCoordPointer(2, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].texcoord);
        glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].position);
        GLuint j;
        for(j = 0; j < m->count; j++)
        {
            if(texes && m->materials[j].tex_index >= 0 && texes[m->materials[j].tex_index])
                glBindTexture(GL_TEXTURE_2D, texes[m->materials[j].tex_index]->texid);
            glDrawElements(/*GL_TRIANGLES*/m->materials[j].mode, m->materials[j].index_count, GL_UNSIGNED_SHORT, m->materials[j].index);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    glPopMatrix();
    if(notcullface)
        glEnable(GL_CULL_FACE);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

GLvoid NETLizard_RenderGL3DItemMesh(const GL_NETLizard_3D_Mesh *m, const texture_s *tex)
{
	if(!m)
		return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if(tex)
        glBindTexture(GL_TEXTURE_2D, tex->texid);

    glNormalPointer(GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].normal);
    glTexCoordPointer(2, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].texcoord);
    glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].position);
    GLuint j;
    for(j = 0; j < m->count; j++)
    {
        glDrawElements(/*GL_TRIANGLES*/m->materials[j].mode, m->materials[j].index_count, GL_UNSIGNED_SHORT, m->materials[j].index);
    }

    if(tex)
        glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

