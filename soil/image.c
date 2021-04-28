#include "image.h"

#include "image_dxt.h"
#include "stb_image_aug.h"

const char *result_string_pointer = "SOIL initialized";

int
    SOIL_save_image
    (
        const char *filename,
        int image_type,
        int width, int height, int channels,
        const unsigned char *const data
    )
{
    int save_result;

    /*	error check	*/
    if( (width < 1) || (height < 1) ||
        (channels < 1) || (channels > 4) ||
        (data == NULL) ||
        (filename == NULL) )
    {
        return 0;
    }
    if( image_type == SOIL_SAVE_TYPE_BMP )
    {
        save_result = stbi_write_bmp( filename,
                width, height, channels, (void*)data );
    } else
    if( image_type == SOIL_SAVE_TYPE_TGA )
    {
        save_result = stbi_write_tga( filename,
                width, height, channels, (void*)data );
    } else
    if( image_type == SOIL_SAVE_TYPE_DDS )
    {
        save_result = save_image_as_DDS( filename,
                width, height, channels, (const unsigned char *const)data );
    } else
    {
        save_result = 0;
    }
    if( save_result == 0 )
    {
        result_string_pointer = "Saving the image failed";
    } else
    {
        result_string_pointer = "Image saved";
    }
    return save_result;
}


unsigned char*
    SOIL_load_image
    (
        const char *filename,
        int *width, int *height, int *channels,
        int force_channels
    )
{
    unsigned char *result = stbi_load( filename,
            width, height, channels, force_channels );
    if( result == NULL )
    {
        result_string_pointer = stbi_failure_reason();
    } else
    {
        result_string_pointer = "Image loaded";
    }
    return result;
}

unsigned char*
    SOIL_load_image_from_memory
    (
        const unsigned char *const buffer,
        int buffer_length,
        int *width, int *height, int *channels,
        int force_channels
    )
{
    unsigned char *result = stbi_load_from_memory(
                buffer, buffer_length,
                width, height, channels,
                force_channels );
    if( result == NULL )
    {
        result_string_pointer = stbi_failure_reason();
    } else
    {
        result_string_pointer = "Image loaded from memory";
    }
    return result;
}
