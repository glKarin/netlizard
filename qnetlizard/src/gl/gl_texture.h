#ifndef _KARIN_GL_TEXTURE_H
#define _KARIN_GL_TEXTURE_H

#include <GL/gl.h>

typedef struct _texture_s
{
    GLuint texid;
    int width;
    int height;
    GLenum format;
} texture_s;

GLboolean new_OpenGL_texture_2d_from_memory(const unsigned char *d, unsigned int len, texture_s *tex);
GLboolean new_OpenGL_texture_2d(texture_s *g_tex, const char *dds);

GLboolean make_OpenGL_texture_2d(const char *d, int len, int width, int height, GLenum format, texture_s *tex);

#endif // _KARIN_GL_TEXTURE_H
