#include "nl_gl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nl_texture.h"
#include "gl_texture.h"

GLboolean NETLizard_ReadSpirit(GL_NETLizard_Sprite *sprite, const char *map_file, const char *tex_file)
{
	if(!map_file || !tex_file)
        return GL_FALSE;

    NETLizard_Sprite s;
    NLboolean res = nlReadSpiritMapFile(map_file, &s);
    if(!res)
        return GL_FALSE;

    texture_s tex;

	new_netlizard_texture_from_file new_image_texture;
    NETLizard_Texture_Type type = nlGetPNGFileType(tex_file);
	switch(type)
	{
        case NL_TEXTURE_3D_ENGINE_V3_COMPRESS:
			new_image_texture = new_texture_from_nl_v3_3d_compress_file;
			break;
        case NL_TEXTURE_3D_ENGINE_V3:
			new_image_texture = new_texture_from_nl_v3_3d_file;
			break;
        case NL_TEXTURE_3D_ENGINE_V2:
			new_image_texture = new_texture_from_nl_v2_3d_file;
			break;
        case NL_TEXTURE_ENCODE_PNG:
			new_image_texture = new_texture_from_nl_file;
			break;
        case NL_TEXTURE_NORMAL_PNG:
			new_image_texture = new_OpenGL_texture_2d;
			break;
		default:
			new_image_texture = new_OpenGL_texture_2d;
			break;
	}

    res = new_image_texture(&tex, tex_file);
    if(!res)
    {
        delete_NETLizard_Sprite(&s);
        return GL_FALSE;
    }

    memset(sprite, 0, sizeof(GL_NETLizard_Sprite));
    sprite->tex = tex;
    sprite->sprite_count = s.count;
    sprite->sprites = calloc(s.count, sizeof(GL_NETLizard_Sprite_Cell));
	GLuint i;
    for(i = 0; i < s.count; i++)
	{
        NETLizard_Sprite_Cell *cell = s.data + i;
        GL_NETLizard_Sprite_Cell *glcell = sprite->sprites + i;
        glcell->width = cell->width;
        glcell->height = cell->height;
        glcell->private_4 = cell->private_4;
        glcell->private_5 = cell->private_5;

        GL_NETLizard_3D_Vertex *vertexs = calloc(4, sizeof(GL_NETLizard_3D_Vertex));
		GLfloat vertex[] = {
            -cell->width / 2, -cell->height / 2,
            cell->width / 2, -cell->height / 2,
            -cell->width / 2, cell->height / 2,
            cell->width / 2, cell->height / 2
		};
		GLfloat texcoord[] = {
            (GLfloat)cell->x / tex.width, ((GLfloat)cell->y + cell->height) / tex.height,
            ((GLfloat)cell->x + cell->width) / tex.width, ((GLfloat)cell->y + cell->height) / tex.height,
            (GLfloat)cell->x / tex.width, ((GLfloat)cell->y) / tex.height,
            ((GLfloat)cell->x + cell->width) / tex.width, ((GLfloat)cell->y)  / tex.height
        };
        GLushort *indexes = calloc(6, sizeof(GLushort));
        indexes[0] = 0;
        indexes[1] = 1;
        indexes[2] = 2;
        indexes[3] = 2;
        indexes[4] = 1;
        indexes[5] = 3;

        int j;
        for(j = 0; j < 4; j++)
        {
            vertexs[j].position[0] = vertex[j * 2];
            vertexs[j].position[1] = vertex[j * 2 + 1];
            vertexs[j].position[2] = 0;
            vertexs[j].texcoord[0] = texcoord[j * 2];
            vertexs[j].texcoord[1] = texcoord[j * 2 + 1];
        }

        glcell->vertex_data.vertex = vertexs;
        glcell->vertex_data.vertex_count = 4;
        glcell->index_data.index = indexes;
        glcell->index_data.index_count = 6;
        glcell->index_data.mode = GL_TRIANGLES;
    }
    delete_NETLizard_Sprite(&s);
    return GL_TRUE;
}

GLvoid delete_GL_NETLizard_Sprite(GL_NETLizard_Sprite *s)
{
	if(!s)
		return;
    if(s->tex.texid)
	{
        if(glIsTexture(s->tex.texid))
            glDeleteTextures(1, &s->tex.texid);
    }
    GLuint i;
    for(i = 0; i < s->sprite_count; i++)
    {
        GL_NETLizard_Sprite_Cell *c = s->sprites + i;
        if(c->vertex_data.vertex)
            free(c->vertex_data.vertex);
        if(c->index_data.index)
            free(c->index_data.index);
    }
	free(s->sprites);
}

GLvoid NETLizard_RenderSpirit(const GL_NETLizard_Sprite *s, GLuint i)
{
	if(!s)
		return;
	if(i >= s->sprite_count)
		return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if(s->tex.texid)
        glBindTexture(GL_TEXTURE_2D, s->tex.texid);
	const GL_NETLizard_Sprite_Cell *c = s->sprites + i;
    glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), c->vertex_data.vertex[0].position);
    glTexCoordPointer(2, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), c->vertex_data.vertex[0].texcoord);
    //glPushMatrix();
    {
        glDrawElements(c->index_data.mode, c->index_data.index_count, GL_UNSIGNED_SHORT, c->index_data.index);
	}
    //glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
