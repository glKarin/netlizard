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
