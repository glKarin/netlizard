#ifndef KARIN_TEXTURE_V3_READER_H
#define KARIN_TEXTURE_V3_READER_H

#include "texture_reader.h"

NETLizard_Texture * nlReadTextureV3_File(const char *name, int i);
NETLizard_Texture * nlReadTextureV3_Memory(const array *data, int i);
int nlSaveImage_V3File(const char *from, int i, const char *to, int img_type);
int nlSaveImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type);
int nlSaveTextureV3_File(const char *from, int _i, const char *to);
int nlSaveTextureV3_Memory(const unsigned char *data, int _i, int width, int height, NETLizard_Texture_format formay, const char *to);

NETLizard_Texture * nlReadCompressTextureV3_File(const char *name);
NETLizard_Texture * nlReadCompressTextureV3_Memory(const array *data);
int nlSaveCompressImage_V3File(const char *from, const char *to, int img_type);
int nlSaveCompressImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type);

#endif
