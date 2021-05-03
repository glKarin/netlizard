#include "netlizard.h"

#include "priv_local.h"
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

