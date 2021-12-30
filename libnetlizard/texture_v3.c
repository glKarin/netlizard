#include "netlizard.h"

#include "priv_local.h"

NLboolean nlIsNL3DTextureV3(const char *data, NLsizei length)
{
    if(length < 3)
        return NL_FALSE;
    const char F[] = NL_TEXTURE_V3_HEADER_MAGIC; // BIP
    int res = memcmp(data, F, 3);
    return BOOL_VALUE(res == 0);
}

NLboolean nlIsNL3DTextureV3File(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[3] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 3, data);
    len = file_get_contents_s(name, &arr);
    if(len == 3)
        ret = nlIsNL3DTextureV3((char *)arr.array, arr.length);
    return ret;
}
