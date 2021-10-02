#ifndef KARIN_TEXTURE_V3_READER_H
#define KARIN_TEXTURE_V3_READER_H

#include "texture_reader.h"

NETLizard_Texture * nlReadTextureV3File(const char *name, int i);
NETLizard_Texture * nlLoadTextureV3Data(const array *data, int i);
int nlConvertTextureV3FileToImageFile(const char *from, int i, const char *to, int img_type);
int nlSaveTextureV3DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type);
int nlConvertImageFileToTextureV3File(const char *from, int _i, const char *to);
int nlSavePixelDataToTextureV3File(const unsigned char *data, int _i, int width, int height, NETLizard_Texture_format formay, const char *to);

NETLizard_Texture * nlReadCompressTextureV3File(const char *name);
NETLizard_Texture * nlLoadCompressTextureV3Data(const array *data);
int nlConvertTextureV3CompressFileToImageFile(const char *from, const char *to, int img_type);
int nlSaveTextureV3CompressDataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type);

#endif
