#include "jport.h"

#include "def.h"

#include <stdlib.h>
#include <string.h>

void delete_array(array *arr)
{
    if(arr->alloc)
    {
        if(arr->array)
            free(arr->array);
    }
    arr->array = NULL;
    arr->length = 0;
    arr->size = 0;
}

void reset_array(array *arr)
{
    memset(arr, 0, sizeof(array));
    return;
    arr->alloc = 0;
    arr->array = NULL;
    arr->length = 0;
    arr->size = 0;
}

void zero_array(array *arr)
{
    arr->array = memset(arr->array, 0, arr->size * arr->length);
}

void init_array(array *arr, void *data, size_t data_size)
{
    zero_array(arr);
    if(data)
    {
        size_t dsize = data_size > 0 ? data_size : arr->size * arr->length;
        memcpy(arr->array, data, dsize);
    }
}

array * make_array(array *arr, size_t size, size_t length, void *data)
{
    arr->array = data;
    arr->length = length;
    arr->size = size;
    arr->alloc = 0;
    return arr;
}

array * alloc_array(size_t size, size_t length)
{
    void *ptr;
    array *arr;

    arr = NEW(array);
    ptr = calloc(length, size);
    arr->array = ptr;
    arr->length = length;
    arr->size = size;
    arr->alloc = 1;
    return arr;
}

array * new_array(array *arr, size_t size, size_t length)
{
    return new_array_with_data(arr, size, length, NULL, 0);
}

array * new_array_with_data(array *arr, size_t size, size_t length, const void *data, size_t data_size)
{
    void *ptr;
    size_t len = length * size;

    ptr = len ? calloc(length, size) : NULL;
    arr->array = ptr;
    arr->length = length;
    arr->size = size;
    arr->alloc = 1;
    if(data)
    {
        size_t dsize = data_size > 0 ? data_size : size * length;
        memcpy(arr->array, data, dsize);
    }
    return arr;
}

size_t array_size(const array *arr)
{
    return arr->size * arr->length;
}

int array_is_empty(const array *arr)
{
    return arr->length == 0;
}
