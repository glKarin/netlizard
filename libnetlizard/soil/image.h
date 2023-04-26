#ifndef _KARIN_IMAGE_H
#define _KARIN_IMAGE_H

// SOIL

#if defined(__WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__) || defined(_WIN32)
#    define NLSOIL_EXPORT __declspec(dllexport)
#    define NLSOIL_IMPORT __declspec(dllimport)
#    define NLSOIL_HIDDEN
#else
#  define NLSOIL_EXPORT     __attribute__((visibility("default")))
#  define NLSOIL_IMPORT     __attribute__((visibility("default")))
#  define NLSOIL_HIDDEN     __attribute__((visibility("hidden")))
#endif

#if defined(_NL_STATIC)
#define NLSOILAPI
#elif defined(_NL_DLL)
#  define NLSOILAPI NLSOIL_EXPORT
#else
#  define NLSOILAPI NLSOIL_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

NLSOILAPI int
    SOIL_save_image
    (
        const char *filename,
        int image_type,
        int width, int height, int channels,
        const unsigned char *const data
    );

NLSOILAPI unsigned char*
    SOIL_load_image
    (
        const char *filename,
        int *width, int *height, int *channels,
        int force_channels
    );

NLSOILAPI unsigned char*
    SOIL_load_image_from_memory
    (
        const unsigned char *const buffer,
        int buffer_length,
        int *width, int *height, int *channels,
        int force_channels
    );

#ifdef __cplusplus
}
#endif

#endif // _KARIN_IMAGE_H
