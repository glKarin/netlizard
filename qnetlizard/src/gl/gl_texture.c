#include "gl_texture.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <GL/glu.h>

#include "libnetlizard/soil/image.h"

#define SCALE_OUT 3
#define SCALE_IN 2
#define NO_SCALE 0
#define SCALE_AUTO 1

static void * OpenGLU_ScaleImage(GLsizei width, GLsizei height, GLenum format, GLenum type, const void *datain, GLenum scale, GLsizei *rw, GLsizei *rh)
{
    if(scale == NO_SCALE)
        return NULL;
    double wn = log2((double)width);
    double w = 0.0;
    modf(wn, &w);
    double hn = log2((double)height);
    double h = 0.0;
    modf(hn, &h);
    if(w == wn && h == hn)
        return NULL;
    GLsizei fw = width;
    GLsizei fh = height;
    if(w != wn)
    {
        double n = (scale == SCALE_IN ? floor(wn) : (scale == SCALE_OUT ? ceil(wn) : round(wn)));
        fw = (GLsizei)pow(2, n);
        if(rw)
            *rw = fw;
    }
    if(h != hn)
    {
        double n = (scale == SCALE_IN ? floor(hn) : (scale == SCALE_OUT ? ceil(hn) : round(hn)));
        fh = (GLsizei)pow(2, n);
        if(rh)
            *rh = fh;
    }
    int bytes;
    int components;

#if 1
    if(0)
#else
    if ( type == GL_UNSIGNED_SHORT_5_6_5 || type == GL_UNSIGNED_SHORT_4_4_4_4
            || type == GL_UNSIGNED_SHORT_4_4_4_4_REV
            || type == GL_UNSIGNED_SHORT_5_5_5_1
         )
#endif
    {
        components = 1;
    }
    else
    {
        switch(format) {
            case GL_RGB:
                components = 3;
            case GL_LUMINANCE_ALPHA:
                components = 2;
            case GL_RGBA:
#if 0
            case GL_BGRA:
#endif
                components = 4;
            default:
                components = 1;
        }
    }

    switch(type) {
        case GL_UNSIGNED_SHORT:
            bytes = sizeof(GLushort);
        case GL_SHORT:
            bytes = sizeof(GLshort);
        case GL_UNSIGNED_BYTE:
            bytes = sizeof(GLubyte);
        case GL_BYTE:
            bytes = sizeof(GLbyte);
        case GL_FLOAT:
            bytes = sizeof(GLfloat);
#if 0
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            bytes = sizeof(GLushort);
#endif
        default:
            bytes = 4;
    }
    int size = bytes * fw * fh * components;
    char *dataout = calloc(1, size);
    gluScaleImage(format, width, height, type, datain, fw, fh, type, dataout);
    if(rw)
        *rw = fw;
    if(*rh)
        *rh = fh;
    return dataout;
}

static void OpenGL_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels, GLenum scale, GLsizei *rw, GLsizei *rh)
{
    if(scale == NO_SCALE)
    {
        gluBuild2DMipmaps(target, internalformat, width, height, format, type, pixels);
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
        return;
    }

    GLsizei fw = width;
    GLsizei fh = height;
    void *data = OpenGLU_ScaleImage(width, height, format, type, pixels, SCALE_AUTO, &fw, &fh);
    if(data)
    {
        gluBuild2DMipmaps(target, format, fw, fh, format, type, data);
        glTexImage2D(GL_TEXTURE_2D, 0, format, fw, fh, 0, format, type, data);
        if(rw)
            *rw = fw;
        if(rh)
            *rh = fh;
        free(data);
    }
    else
    {
        gluBuild2DMipmaps(target, internalformat, width, height, format, type, pixels);
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }
}

GLboolean new_OpenGL_texture_2d_from_memory(const unsigned char *d, unsigned int len, texture_s *g_tex)
{
    if(!d)
        return GL_FALSE;
    int channel;
    int width = 0;
    int height = 0;
    unsigned char *data = SOIL_load_image_from_memory(d, len, &width, &height, &channel, SOIL_LOAD_AUTO);
    if(!data)
    {
        fprintf(stderr, "Unable to load texture file.\n");
        return GL_FALSE;
    }

    GLenum format = 0;
    switch(channel)
    {
        case SOIL_LOAD_L:
            format = GL_LUMINANCE;
            break;
        case SOIL_LOAD_LA:
            format = GL_LUMINANCE_ALPHA;
            break;
        case SOIL_LOAD_RGB:
            format = GL_RGB;
            break;
        case SOIL_LOAD_RGBA:
            format = GL_RGBA;
            break;
        default:
            break;
    }

    if(format == 0)
    {
        fprintf(stderr, "Unsupport format of texture file.\n");
        free(data);
        return GL_FALSE;
    }

    // SOIL_load_OGL_texture()
    make_OpenGL_texture_2d(data, 0, width, height, format, g_tex);

    free(data);

    //printf("tex %d: width->%d, height->%d, channel->%d(%s)\n", g_tex->texid, g_tex->width, g_tex->height, channel, (channel == 1 ? "luminance" : (channel == 2 ? "luminance-alpha" : (channel == 3 ? "RGB" : "RGBA"))));
    return GL_TRUE;
}

GLboolean make_OpenGL_texture_2d(const char *data, int len, int width, int height, GLenum format, texture_s *g_tex)
{
    memset(g_tex, 0, sizeof(texture_s));
    g_tex->width = width;
    g_tex->height = height;
    g_tex->format = format;

    glGenTextures(1, &(g_tex->texid));
    glBindTexture(GL_TEXTURE_2D, g_tex->texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#if 0
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
#if 1
    OpenGL_TexImage2D(GL_TEXTURE_2D, 0, g_tex->format, g_tex->width, g_tex->height, 0, g_tex->format, GL_UNSIGNED_BYTE, data, SCALE_AUTO, NULL, NULL);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, g_tex->format, g_tex->width, g_tex->height, 0, g_tex->format, GL_UNSIGNED_BYTE, data);
#endif

    glBindTexture(GL_TEXTURE_2D, 0);

    return GL_TRUE;
}

GLboolean new_OpenGL_texture_2d(texture_s *g_tex, const char *dds)
{
    if(!dds)
        return GL_FALSE;
    int channel;
    int width = 0;
    int height = 0;
    unsigned char *data = SOIL_load_image((char *)dds, &width, &height, &channel, SOIL_LOAD_AUTO);
    if(data == NULL)
    {
        fprintf(stderr, "Unable to load texture file.\n");
        return GL_FALSE;
    }

    GLenum format = 0;
    switch(channel)
    {
        case SOIL_LOAD_L:
            format = GL_LUMINANCE;
            break;
        case SOIL_LOAD_LA:
            format = GL_LUMINANCE_ALPHA;
            break;
        case SOIL_LOAD_RGB:
            format = GL_RGB;
            break;
        case SOIL_LOAD_RGBA:
            format = GL_RGBA;
            break;
        default:
            break;
    }
    if(format == 0)
    {
        fprintf(stderr, "Unsupport format of texture file.\n");
        free(data);
        return GL_FALSE;
    }

    make_OpenGL_texture_2d(data, 0, width, height, format, g_tex);

    free(data);
    return GL_TRUE;
}
