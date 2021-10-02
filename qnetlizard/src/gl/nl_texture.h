#ifndef _KARIN_NL_TEXTURE_H
#define _KARIN_NL_TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gl_texture.h"

typedef GLboolean (*new_netlizard_texture_from_file)(texture_s *tex, const char *name);

GLboolean new_texture_from_nl_file(texture_s *tex, const char *name);
GLboolean new_texture_from_nl_v2_3d_file(texture_s *tex, const char *name);
GLboolean new_texture_from_nl_v3_3d_file(texture_s *tex, const char *name);
GLboolean new_texture_from_nl_v3_3d_compress_file(texture_s *tex, const char *name);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_NL_TEXTURE_H
