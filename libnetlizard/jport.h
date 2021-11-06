#ifndef _KARIN_JPORT_H
#define _KARIN_JPORT_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_DATA(arr) (arr).array
#define ARRAY_NULL(arr) memset(&(arr), 0, sizeof(array))

#define ARRAY_DATA_T(arr, T) ((T *)ARRAY_DATA(arr))

#define jtrue 1
#define jfalse 0

typedef uint8_t jboolean;
typedef int8_t jbyte;
typedef uint16_t jchar;
typedef int16_t jshort;
typedef int32_t jint;
typedef int64_t jlong;
typedef float jfloat;
typedef double jdouble;

typedef unsigned char byte; // unsigned
typedef unsigned char boolean;

struct array_s
{
    void *array;
    size_t length;
    size_t size;
    unsigned char alloc;
};

typedef struct array_s array;
typedef struct array_s jarray;

typedef struct array_s boolean_array;
typedef struct array_s byte_array;
typedef struct array_s char_array;
typedef struct array_s short_array;
typedef struct array_s int_array;
typedef struct array_s long_array;
typedef struct array_s float_array;
typedef struct array_s double_array;
#define T_array(T) array

typedef struct array_s strings;
typedef struct array_s jstring;
typedef struct array_s wstrings;

void reset_array(array *arr);
void delete_array(array *arr);
void zero_array(array *arr);
void init_array(array *arr, void *data, size_t data_size);
array * alloc_array(size_t size, size_t length); // heap
array * new_array(array *arr, size_t size, size_t length); // heap
array * new_array_with_data(array *arr, size_t size, size_t length, const void *data, size_t data_size); // heap
array * make_array(array *arr, size_t size, size_t length, void *data); // stack
size_t array_size(const array *arr);
int array_is_empty(const array *arr);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_JPORT_H
