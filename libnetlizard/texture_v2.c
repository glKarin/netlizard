#include "netlizard.h"

#include "priv_local.h"

NLboolean nlIsNL3DV2Texture(const char *data)
{
    const char F[] = NL_TEXTURE_V2_MAGIC_HEADER; // &&&
    int res = memcmp(data, F, sizeof(F));
    return res == 0;
}

NLboolean nlIsNL3DV2TextureFile(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[3] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 3, data);
    len = file_get_contents_s(name, &arr);
    if(len == 3)
        ret = nlIsNL3DV2Texture((char *)arr.array);
    return ret;
}
