#include "nl_font.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

GLboolean new_netlizard_font(font_s *fnt, const char *mapf, const char *texf)
{
	if(!mapf || !texf)
        return GL_FALSE;
    GL_NETLizard_Font nf;
    GLboolean res = NETLizard_ReadFont(&nf, mapf, texf);
    if(!res)
        return GL_FALSE;

    GLfloat w = 0;
    GLfloat h = 0;
    GLuint i;
    for(i = 0; i < nf.char_map_count; i++)
    {
        if(i == 0)
        {
            w = nf.char_map[i].width;
            h = nf.char_map[i].height;
        }
        else
        {
            if(w < nf.char_map[i].width)
                w = nf.char_map[i].width;
            if(h < nf.char_map[i].height)
                h = nf.char_map[i].height;
        }
    }
    fnt->width = (GLuint)w;
    fnt->height = (GLuint)h;

    fnt->netlizard = nf;
    return GL_TRUE;
}

void delete_font(font_s *f)
{
	if(!f)
        return;
    delete_GL_NETLizard_Font(&f->netlizard);
}

void Font_RenderString(const font_s *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char *str)
{
    if(!fnt || !str)
		return;

    const GL_NETLizard_Font *f = &fnt->netlizard;
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
                GLint index = NETLizard_GetFontIndex(f, str[i]);
				if(index == -1)
					index = 0;
				NETLizard_RenderFontChar(f, index);
                glTranslatef(f->char_map[index].width + f->char_map[index].x_stride, 0.0, 0.0);
			}
		}
		glPopMatrix();
	}
	glPopAttrib();
}

void Font_RenderDigit(const font_s *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint num)
{
    if(!fnt)
        return;
	char *str = itostr(num);
    Font_RenderString(fnt, x, y, r, g, b, a, str);
	free(str);
}

void Font_RenderChar(const font_s *fnt, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, char ch)
{
    if(!fnt)
		return;

    const GL_NETLizard_Font *f = &fnt->netlizard;
	glPushAttrib(GL_CURRENT_BIT);
	{
		glColor4f(r, g, b, a);
		glPushMatrix();
		{
			glTranslatef(x, y, 0.0);
			GLint index = NETLizard_GetFontIndex(f, ch);
			if(index == -1)
				index = 0;
			NETLizard_RenderFontChar(f, index);
		}
		glPopMatrix();
	}
	glPopAttrib();
}

GLfloat Font_GetStringWidth(const font_s *fnt, const char *str)
{
    if(!fnt || !str)
		return 0;
    size_t len = strlen(str);
    GLfloat w = 0.0;
    size_t i;
    const GL_NETLizard_Font *f = &fnt->netlizard;
    for(i = 0; i < len; i++)
    {
        GLint index = NETLizard_GetFontIndex(f, str[i]);
        if(index == -1)
            index = 0;
        w += f->char_map[index].width;
        if(i < len - 1)
            w += f->char_map[index].x_stride;
    }
    return w;
}

GLint Font_GetCharCountOfWidth(const font_s *fnt, GLfloat width, const char *str)
{
    if(!fnt || !str)
		return -1;
	if(width <= 0.0)
		return -1;
    size_t len = strlen(str);
    GLfloat w = 0.0;
    size_t i;
    const GL_NETLizard_Font *f = &fnt->netlizard;
    for(i = 0; i < len; i++)
    {
        GLint index = NETLizard_GetFontIndex(f, str[i]);
        if(index == -1)
            index = 0;
        w += f->char_map[index].width;
        if(i < len - 1)
            w += f->char_map[index].x_stride;
        if(w >= width)
            break;
    }
    return i;
}

int Font_GetDigitCenterPosition(const font_s *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, int num, GLfloat *rx, GLfloat *ry)
{
	if(!fnt || !rx || !ry)
		return 0;
	char *str = itostr(num);
	int r = Font_GetStringCenterPosition(fnt, x, y, w, h, str, rx, ry);
	free(str);
	return r;
}

int Font_GetStringCenterPosition(const font_s *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, const char *str, GLfloat *rx, GLfloat *ry)
{
	if(!fnt || !str || (!rx && !ry))
		return 0;
	GLfloat len = Font_GetStringWidth(fnt, str);
	if(len <= 0.0)
		return 0;
	if(rx)
		*rx = x + (w / 2 - len / 2);
	if(ry)
		*ry = y + (h / 2 - fnt->height / 2);
	return 1;
}
