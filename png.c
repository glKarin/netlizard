#include "netlizard.h"

#include "priv_local.h"

NLboolean nlIsPNG(const char *data)
{
    const unsigned char PNG_Dec[] = {
        137, 80, 78, 71, 13, 10, 26, 10
    };
    /*
    const unsigned char PNG_Hex[] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };
    */

    int res = memcmp(data, PNG_Dec, sizeof(PNG_Dec));
    return res == 0;
}

NLboolean nlIsPNGFile(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[7] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 7, data);
    len = file_get_contents_s(name, &arr);
    if(len == 7)
        ret = nlIsPNG((char *)arr.array);
    return ret;
}

NLboolean nlIsNLPNG(const char *data)
{
    const unsigned char NL_PNG_Dec[] = {
        237, 53, 40, 32, 101, 99, 112, 97
    };

    int res = memcmp(data, NL_PNG_Dec, sizeof(NL_PNG_Dec));
    return res == 0;
}

NLboolean nlIsNLPNGFile(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[7] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 7, data);
    len = file_get_contents_s(name, &arr);
    if(len == 7)
        ret = nlIsNLPNG((char *)arr.array);
    return ret;
}

// NETLizard 2D游戏/老的3D游戏的png加密/解密
char * nlEncodeDecodeData(const char *arr, char *data, NLsizei length)
{
    memcpy(data, arr, length);

    return nlEncodeDecodeThisData(data, length);
}

char * nlEncodeDecodeThisData(char *arr, NLsizei length)
{
    int paramInt = NL_ENCODE_DECODE_FACTOR; // 100
    int i2;

    for (i2 = 0; i2 < length; i2++)
    {
        ((unsigned char *)arr)[i2] = ((unsigned char *)arr)[i2] ^ paramInt;
        paramInt++;
        if (paramInt > 255) {
            paramInt = 0;
        }
    }
    return arr;
}

char * nlHandlePNG_File2Memory(const char *name, NLint *rlen)
{
    array arr;
    jlong len;
    char *data;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NULL;

    if(rlen)
        *rlen = array_size(&arr);
    data = nlHandlePNG_Memory2Memory(arr.array, array_size(&arr));
    delete_array(&arr);

    return data;
}

char * nlHandlePNG_Memory2Memory(const char *arr, NLint len)
{
    if(nlIsPNG(arr))
    {
        nlprintf("Encode NETLizard 2D image PNG\n");
    }
    else if(nlIsNLPNG(arr))
    {
        nlprintf("Decode NETLizard 2D image PNG\n");
    }
    else
    {
        nlprintf("Unsupported PNG\n");
        return NULL;
    }

    char *res = NEW_II(char, len);
    return nlEncodeDecodeData(arr, res, len);
}

NLboolean nlHandlePNG_File2File(const char *from, const char *to)
{
    char *data;
    NLint rlen;
    NLboolean res;

    data = nlHandlePNG_File2Memory(from, &rlen);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, rlen, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);

    return res;
}

NLboolean nlHandlePNG_Memory2File(const char *in_data, NLint len, const char *to)
{
    char *data;
    NLboolean res;

    data = nlHandlePNG_Memory2Memory(in_data, len);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, len, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);
    return res;
}

