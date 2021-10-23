#include "nl_gl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nl_texture.h"
#include "gl_texture.h"

GLboolean NETLizard_ReadFont(GL_NETLizard_Font *fnt, const char *map_file, const char *tex_file)
{
	if(!map_file || !tex_file)
        return GL_FALSE;

    NETLizard_Font o;
    NLboolean res = nlReadNETLizardFontFile(map_file, &o);
    if(!res)
        return GL_FALSE;

    texture_s tex;

    res = new_OpenGL_texture_2d(&tex, tex_file);
    if(!res)
	{
        delete_NETLizard_Font(&o);
        return GL_FALSE;
	}

    memset(fnt, 0, sizeof(GL_NETLizard_Font));
	fnt->tex = tex;
    fnt->private_m = o.private_m;
    fnt->private_l = o.private_l;
    fnt->private_e = o.private_e;
    fnt->char_count = o.char_count;
    fnt->offset = calloc(o.offset.count, sizeof(GLbyte));
    int8_t *offset = (int8_t *)(o.offset.data);
	int i;
    for(i = 0; i < o.offset.count; i++)
	{
		fnt->offset[i] = offset[i];
	}
    fnt->char_map_count = o.char_raster_map.count;
    fnt->char_map = calloc(o.char_raster_map.count, sizeof(GL_NETLizard_Font_Char));
    NETLizard_Font_Char_Raster *f = (NETLizard_Font_Char_Raster *)(o.char_raster_map.data);
    for(i = 0; i < o.char_raster_map.count; i++)
    {
		GL_NETLizard_Font_Char *c = fnt->char_map + i;
		c->width = (GLfloat)f[i].width;
		c->height = (GLfloat)f[i].height;
		c->x_stride = (GLfloat)f[i].x_stride;
		c->y_stride = (GLfloat)f[i].y_stride;

        GL_NETLizard_3D_Vertex *vertexs = calloc(4, sizeof(GL_NETLizard_3D_Vertex));
		GLfloat vertex[] = {
			0.0, 0.0,
			c->width, 0.0,
			0.0, c->height,
			c->width, c->height
		};
		GLfloat texcoord[] = {
            (GLfloat)f[i].x / tex.width, ((GLfloat)f[i].y + c->height) / tex.height,
            ((GLfloat)f[i].x + c->width) / tex.width, ((GLfloat)f[i].y + c->height) / tex.height,
            (GLfloat)f[i].x / tex.width, ((GLfloat)f[i].y) / tex.height,
            ((GLfloat)f[i].x + c->width) / tex.width, ((GLfloat)f[i].y) / tex.height
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

        c->vertex_data.vertex = vertexs;
        c->vertex_data.vertex_count = 4;
        c->vertex_data.index = indexes;
        c->vertex_data.index_count = 6;
    }
    delete_NETLizard_Font(&o);
    return GL_TRUE;
}

GLvoid delete_GL_NETLizard_Font(GL_NETLizard_Font *fnt)
{
	if(!fnt)
		return;
    if(fnt->tex.texid)
	{
        if(glIsTexture(fnt->tex.texid))
            glDeleteTextures(1, &fnt->tex.texid);
	}
	free(fnt->offset);
	GLuint i;
	for(i = 0; i < fnt->char_map_count; i++)
	{
        GL_NETLizard_Font_Char *c = fnt->char_map + i;
        if(c->vertex_data.vertex)
            free(c->vertex_data.vertex);
        if(c->vertex_data.index)
            free(c->vertex_data.index);
    }
}

GLvoid NETLizard_RenderFontChar(const GL_NETLizard_Font *fnt, GLuint i)
{
	if(!fnt)
		return;
	if(i >= fnt->char_map_count)
		return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if(fnt->tex.texid)
        glBindTexture(GL_TEXTURE_2D, fnt->tex.texid);
    const GL_NETLizard_Font_Char *c = fnt->char_map + i;
    glVertexPointer(3, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), c->vertex_data.vertex[0].position);
    glTexCoordPointer(2, GL_FLOAT, sizeof(GL_NETLizard_3D_Vertex), c->vertex_data.vertex[0].texcoord);
	glPushMatrix();
    {
        glTranslatef(0.0, c->y_stride, 0.0);
        glDrawElements(GL_TRIANGLES, c->vertex_data.index_count, GL_UNSIGNED_SHORT, c->vertex_data.index);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

GLint NETLizard_GetFontIndex(const GL_NETLizard_Font *fnt, int paramInt)
{
	if(!fnt)
		return -1;
	int ch = paramInt;
	int n = ch;
	if (ch == 946) {
		n = 223;
	}
	if ((n >= 192) && (n - 192 < fnt->offset_count))
	{
		if ((ch = fnt->offset[(n - 192)]) < 0)
		{
			ch = -ch;
			n = 100;
			if (fnt->private_e) {
				n = 201;
			}
			return n + ch;
		}
		n = 100;
		if (fnt->private_e) {
			n = 165;
		}
		return n + ch;
	}
	if (n > 848) {
		n = ch - 848;
	}
	if ((n >= 97) && (n <= 122))
	{
		if (fnt->private_l) {
			n += 35;
		} else {
			n -= 32;
		}
	}
	else if ((n >= 192) && (n <= 223)) {
		n -= 96;
	} else if ((n >= 224) && (n <= 255))
	{
		if (fnt->private_m) {
			n -= 61;
		} else {
			n -= 128;
		}
	}
	else if ((n >= 154) && (n <= 191)) {
		n -= 26;
	}
	n -= 32;
	if ((n < 0) || (n >= fnt->char_count)) {
		n = 0;
	}
	return n;
}
