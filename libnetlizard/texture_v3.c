#include "netlizard.h"

#include "priv_local.h"

NLboolean nlIsNL3DV3Texture(const char *data)
{
    const char F[] = NL_TEXTURE_V3_MAGIC_HEADER; // BIP
    int res = memcmp(data, F, sizeof(F));
    return res == 0;
}

NLboolean nlIsNL3DV3TextureFile(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[3] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 3, data);
    len = file_get_contents_s(name, &arr);
    if(len == 3)
        ret = nlIsNL3DV3Texture((char *)arr.array);
    return ret;
}
