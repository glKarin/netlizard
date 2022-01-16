#ifndef _KARIN_GL_TEXTURE_H
#define _KARIN_GL_TEXTURE_H

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _texture_s
{
    GLuint texid;
    int width;
    int height;
    GLenum format;
} texture_s;

unsigned new_OpenGL_texture_2d_from_memory(const unsigned char *d, unsigned int len, texture_s *tex);
unsigned new_OpenGL_texture_2d(texture_s *g_tex, const char *dds);

unsigned make_OpenGL_texture_2d(const char *d, int len, int width, int height, GLenum format, texture_s *tex);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_GL_TEXTURE_H
