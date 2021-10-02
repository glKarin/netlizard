#include "netlizard.h"

#include "priv_local.h"

char * nlReadAndHandlePNGFile(const char *name, NLint *rlen)
{
    array arr;
    jlong len;
    char *data;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NULL;

    if(rlen)
        *rlen = array_size(&arr);
    data = nlLoadAndHandlePNGData(arr.array, array_size(&arr));
    delete_array(&arr);

    return data;
}

char * nlLoadAndHandlePNGData(const char *arr, NLint len)
{
    if(nlIsPNG(arr, len))
    {
        nlprintf("Encode NETLizard 2D image PNG\n");
    }
    else if(nlIsNLPNG(arr, len))
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

NLboolean nlConvertAndHandlePNGFile(const char *from, const char *to)
{
    char *data;
    NLint rlen;
    NLboolean res;

    data = nlReadAndHandlePNGFile(from, &rlen);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, rlen, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);

    return res;
}

NLboolean nlSaveAndHandlePNGData(const char *in_data, NLint len, const char *to)
{
    char *data;
    NLboolean res;

    data = nlLoadAndHandlePNGData(in_data, len);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, len, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);
    return res;
}

