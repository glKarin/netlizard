#include "nl_gl.h"

#include <stdlib.h>

#include "lib/vector3.h"

void NETLizard_RenderGL3DModel(const GL_NETLizard_3D_Model *model)
{
	if(!model)
		return;

	if(model->meshes)
	{
		GLuint i;
		for(i = 0; i < model->count; i++)
		{
			GL_NETLizard_3D_Mesh *m = model->meshes + i;
            NETLizard_RenderGL3DMesh(m, model->texes);
            if(model->item_meshes && 0)
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
            NETLizard_RenderGL3DMesh(m, model->texes);
		}
	}
}

void NETLizard_RenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count)
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
                NETLizard_RenderGL3DMesh(m, model->texes);
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
                    NETLizard_RenderGL3DMesh(m, model->texes);
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
                            NETLizard_RenderGL3DMesh(im, model->texes);
                        }
                    }
                }
            }
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
	if(!m || !texes)
		return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glPushMatrix();
    {
        glTranslatef(m->position[0], m->position[1], m->position[2]);
        glRotatef(m->rotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(m->rotation[1], 0.0f, 0.0f, 1.0f);

        glNormalPointer(GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].normal);
        glTexCoordPointer(2, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].texcoord);
        glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), m->vertex_data.vertex[0].position);
        GLuint j;
        for(j = 0; j < m->count; j++)
        {
            if(m->materials[j].tex_index >= 0 && texes[m->materials[j].tex_index])
                glBindTexture(GL_TEXTURE_2D, texes[m->materials[j].tex_index]->texid);
            glDrawElements(/*GL_TRIANGLES*/m->materials[j].mode, m->materials[j].index_count, GL_UNSIGNED_SHORT, m->vertex_data.index + m->materials[j].index_start);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    glPopMatrix();

#if 0 // render point and normal
	glDisable(GL_DEPTH_TEST);
	glLineWidth(4);
	glPointSize(25);
	GLfloat nv[6];
	int i;
	for(i = 0; i < m->count; i++)
	{
		int j;
		int vcount = m->materials[i].index_count;
		for(j = 0; j < vcount; j++)
		{
            int start = m->vertex_data.index[m->materials[i].index_start + j];
            vector3_s rv = VECTOR3V(m->vertex_data.vertex[start].position);
            nv[0] = VECTOR3_X(rv);
            nv[1] = VECTOR3_Y(rv);
            nv[2] = VECTOR3_Z(rv);

            vector3_s rv2 = VECTOR3V(m->vertex_data.vertex[start].normal);
            vector3_scalev(&rv2, 20);
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
	}
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1);
	glPointSize(1);
	glColor4f(1.0, 1.0, 1.0, 1.0);
#endif

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
        glDrawElements(/*GL_TRIANGLES*/m->materials[j].mode, m->materials[j].index_count, GL_UNSIGNED_SHORT, m->vertex_data.index + m->materials[j].index_start);
    }

    if(tex)
        glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

