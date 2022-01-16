#ifndef _KARIN_NL_TEXTURE_H
#define _KARIN_NL_TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

struct _texture_s;

typedef unsigned (*new_netlizard_texture_from_file)(struct _texture_s *tex, const char *name);

unsigned new_texture_from_nl_file(struct _texture_s *tex, const char *name);
unsigned new_texture_from_nl_v2_3d_file(struct _texture_s *tex, const char *name);
unsigned new_texture_from_nl_v3_3d_file(struct _texture_s *tex, const char *name);
unsigned new_texture_from_nl_v3_3d_compress_file(struct _texture_s *tex, const char *name);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_NL_TEXTURE_H
