#ifndef _KARIN_IMAGE_H
#define _KARIN_IMAGE_H

// SOIL

enum
{
    SOIL_SAVE_TYPE_TGA = 0,
    SOIL_SAVE_TYPE_BMP = 1,
    SOIL_SAVE_TYPE_DDS = 2
};

enum
{
    SOIL_LOAD_AUTO = 0,
    SOIL_LOAD_L = 1,
    SOIL_LOAD_LA = 2,
    SOIL_LOAD_RGB = 3,
    SOIL_LOAD_RGBA = 4
};

int
    SOIL_save_image
    (
        const char *filename,
        int image_type,
        int width, int height, int channels,
        const unsigned char *const data
    );

unsigned char*
    SOIL_load_image
    (
        const char *filename,
        int *width, int *height, int *channels,
        int force_channels
    );

unsigned char*
    SOIL_load_image_from_memory
    (
        const unsigned char *const buffer,
        int buffer_length,
        int *width, int *height, int *channels,
        int force_channels
    );

#endif // _KARIN_IMAGE_H
