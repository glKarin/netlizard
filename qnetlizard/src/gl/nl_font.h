#ifndef _KARIN_NL_FONT_H
#define _KARIN_NL_FONT_H

#include "nl_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _font_s
{
    GL_NETLizard_Font netlizard;
	GLuint width;
	GLuint height;
} font_s;

GLboolean new_netlizard_font(font_s *fnt, const char *mapf, const char *texf);
void delete_font(font_s *f);

void Font_RenderString(const font_s *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char *str);
void Font_RenderDigit(const font_s *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint num);
void Font_RenderChar(const font_s *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, char ch);

GLfloat Font_GetStringWidth(const font_s *f, const char *str);
GLint Font_GetCharCountOfWidth(const font_s *f, GLfloat width, const char *str);

int Font_GetDigitCenterPosition(const font_s *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, int num, GLfloat *rx, GLfloat *ry);
int Font_GetStringCenterPosition(const font_s *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, const char *str, GLfloat *rx, GLfloat *ry);

#ifdef __cplusplus
}
#endif
#endif
