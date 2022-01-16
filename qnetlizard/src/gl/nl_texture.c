#include "nl_texture.h"

#include <stdlib.h>

#include "gl_texture.h"
#include "netlizard.h"

unsigned new_texture_from_nl_file(texture_s *tex, const char *name)
{
    int len;

    char *data = nlReadAndHandlePNGFile(name, &len);
    if(!data)
        return GL_FALSE;

    GLboolean res = new_OpenGL_texture_2d_from_memory((unsigned char *)data, len, tex);
    free(data);
    return res;
}

unsigned new_texture_from_nl_v2_3d_file(texture_s *g_tex, const char *name)
{
    NETLizard_Texture png;
    NLboolean res = nlReadTextureV2File(name, &png);
    if(!res)
        return GL_FALSE;

    NLint len;
    NLuchar *data = png.format != NL_RGB ? nlMakePixelDataRGBA(&png, &len) : nlMakePixelDataRGB(&png, &len);

    GLboolean ok = GL_FALSE;
    if(data)
    {
        GLenum format = png.format != NL_RGB ? GL_RGBA : GL_RGB;
        ok = make_OpenGL_texture_2d((char *)data, len, png.width, png.height, format, g_tex);
        free(data);
    }
    nlDeleteNETLizardTexture(&png);
    return ok;
}

unsigned new_texture_from_nl_v3_3d_file(texture_s *g_tex, const char *name)
{
    NETLizard_Texture png;
    NLboolean res = nlReadTextureV3File(name, -1, &png);
    if(!res)
        return GL_FALSE;

    NLint len;
    NLuchar *data = png.format != NL_RGB ? nlMakePixelDataRGBA(&png, &len) : nlMakePixelDataRGB(&png, &len);

    GLboolean ok = GL_FALSE;
    if(data)
    {
        GLenum format = png.format != NL_RGB ? GL_RGBA : GL_RGB;
        ok = make_OpenGL_texture_2d((char *)data, len, png.width, png.height, format, g_tex);
        free(data);
    }
    nlDeleteNETLizardTexture(&png);
    return ok;
}

unsigned new_texture_from_nl_v3_3d_compress_file(texture_s *g_tex, const char *name)
{
    NETLizard_Texture png;
    NLboolean res = nlReadCompressTextureV3File(name, &png);
    if(!res)
        return GL_FALSE;

    GLboolean ok = GL_FALSE;
    if(png.format != NL_RGB)
    {
        NLint len;
        NLuchar *data = nlMakePixelDataRGBACompress(&png, &len);
        if(data)
        {
            GLenum format = png.format != NL_RGB ? GL_RGBA : GL_RGB;
            ok = make_OpenGL_texture_2d((char *)data, len, png.width, png.height, format, g_tex);
            free(data);
        }
    }

    nlDeleteNETLizardTexture(&png);
    return ok;
}

