#include "io.h"

#include <stdio.h>

jlong file_get_contents(const char *name, array *array)
{
    FILE *file;
    long l;
    size_t ret;

    file = fopen(name, "rb");
    if(!file)
        return -1;
    ret = 0;
    fseek(file, 0, SEEK_END);
    l = ftell(file);
    if(l > 0)
    {
        fseek(file, 0, SEEK_SET);
        new_array(array, 1, l);
        ret = fread(array->array, 1, l, file);
    }
    fclose(file);

    return ret;
}

jlong file_size(const char *name)
{
    FILE *file;

    file = fopen(name, "rb");
    if(!file)
        return -1;
    fseek(file, 0, SEEK_END);
    long l = ftell(file);
    fclose(file);
    return l;
}


jlong file_get_contents_s(const char *name, array *array)
{
    FILE *file;
    jlong ret;
    char *ptr;
    size_t rlen;
    size_t arr_len;

    file = fopen(name, "rb");
    if(!file)
        return -1;

    ret = 0;
    ptr = (char *)array->array;
    arr_len = array->size * array->length;
    while(!feof(file))
    {
        rlen = fread(ptr, 1, arr_len - ret, file);
        if(rlen <= 0)
            break;
        ret += rlen;
        ptr += rlen;
    }
    fclose(file);

    return ret;
}

jlong file_put_contents(const char *name, const array *array)
{
    FILE *file;
    size_t ret;

    file = fopen(name, "wb");
    if(!file)
        return -1;
    ret = 0;
    ret = fwrite(array->array, array->size, array->length, file);
    fclose(file);

    return ret;
}
