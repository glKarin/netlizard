#include "nl_gl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nl_texture.h"

static char * itostr(int i)
{
    int o = 1;
    int num = i;
    if(i < 0)
    {
        o = 0;
        num = -num;
    }
    int size = 1;
    int base = 10;
    for(; num / base; base *= 10)
        size++;
    if(o == 0)
        size++;
    char *str = (char *)malloc(size + 1);
    memset(str, '\0', size + 1);
    sprintf(str, "%d", i);
    return str;
}

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
        nlDeleteNETLizardFont(&o);
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
    GLfloat w = 0;
    GLfloat h = 0;
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
        c->index_data.index = indexes;
        c->index_data.index_count = 6;
        c->index_data.mode = GL_TRIANGLES;

        if(i == 0)
        {
            w = c->width;
            h = c->height;
        }
        else
        {
            if(w < c->width)
                w = c->width;
            if(h < c->height)
                h = c->height;
        }
    }
    fnt->width = /*(GLuint)*/w;
    fnt->height = /*(GLuint)*/h;

    nlDeleteNETLizardFont(&o);
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
        if(c->index_data.index)
            free(c->index_data.index);
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
        glDrawElements(c->index_data.mode, c->index_data.index_count, GL_UNSIGNED_SHORT, c->index_data.index);
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

// render
void NETLizard_FontRenderString(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char *str)
{
    if(!fnt || !str)
        return;

    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor4f(r, g, b, a);
        glPushMatrix();
        {
            glTranslatef(x, y, 0.0);
            size_t len = strlen(str);
            unsigned int i;
            for(i = 0; i < len; i++)
            {
                GLint index = NETLizard_GetFontIndex(fnt, str[i]);
                if(index == -1)
                    index = 0;
                NETLizard_RenderFontChar(fnt, index);
                glTranslatef(fnt->char_map[index].width + fnt->char_map[index].x_stride, 0.0, 0.0);
            }
        }
        glPopMatrix();
    }
    glPopAttrib();
}

void NETLizard_FontRenderDigit(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint num)
{
    if(!fnt)
        return;
    char *str = itostr(num);
    NETLizard_FontRenderString(fnt, x, y, r, g, b, a, str);
    free(str);
}

void NETLizard_FontRenderChar(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, char ch)
{
    if(!fnt)
        return;

    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor4f(r, g, b, a);
        glPushMatrix();
        {
            glTranslatef(x, y, 0.0);
            GLint index = NETLizard_GetFontIndex(fnt, ch);
            if(index == -1)
                index = 0;
            NETLizard_RenderFontChar(fnt, index);
        }
        glPopMatrix();
    }
    glPopAttrib();
}

GLfloat NETLizard_FontGetStringWidth(const GL_NETLizard_Font *fnt, const char *str)
{
    if(!fnt || !str)
        return 0;
    size_t len = strlen(str);
    GLfloat w = 0.0;
    size_t i;
    for(i = 0; i < len; i++)
    {
        GLint index = NETLizard_GetFontIndex(fnt, str[i]);
        if(index == -1)
            index = 0;
        w += fnt->char_map[index].width;
        if(i < len - 1)
            w += fnt->char_map[index].x_stride;
    }
    return w;
}

GLint NETLizard_FontGetCharCountOfWidth(const GL_NETLizard_Font *fnt, GLfloat width, const char *str)
{
    if(!fnt || !str)
        return -1;
    if(width <= 0.0)
        return -1;
    size_t len = strlen(str);
    GLfloat w = 0.0;
    size_t i;
    for(i = 0; i < len; i++)
    {
        GLint index = NETLizard_GetFontIndex(fnt, str[i]);
        if(index == -1)
            index = 0;
        w += fnt->char_map[index].width;
        if(i < len - 1)
            w += fnt->char_map[index].x_stride;
        if(w >= width)
            break;
    }
    return i;
}

int NETLizard_FontGetDigitCenterPosition(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, int num, GLfloat *rx, GLfloat *ry)
{
    if(!fnt || !rx || !ry)
        return 0;
    char *str = itostr(num);
    int r = NETLizard_FontGetStringCenterPosition(fnt, x, y, w, h, str, rx, ry);
    free(str);
    return r;
}

int NETLizard_FontGetStringCenterPosition(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, const char *str, GLfloat *rx, GLfloat *ry)
{
    if(!fnt || !str || (!rx && !ry))
        return 0;
    GLfloat len = NETLizard_FontGetStringWidth(fnt, str);
    if(len <= 0.0)
        return 0;
    if(rx)
        *rx = x + (w / 2 - len / 2);
    if(ry)
        *ry = y + (h / 2 - fnt->height / 2);
    return 1;
}
