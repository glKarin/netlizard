#ifndef _KARIN_DEF_H
#define _KARIN_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define NEW(T) malloc(sizeof(T))
#define NEW_II(T, size) calloc(size, sizeof(T))
#define ZERO(ptr, T) memset(ptr, 0, sizeof(T))
#define ZERO_II(ptr, T, size) memset(ptr, 0, sizeof(T) * size)
#define ZERO_III(ptr) memset(ptr, 0, sizeof(*ptr))

#define MAX(num1, num2) (((num1) > (num2)) ? (num1) : (num2))
#define MIN(num1, num2) (((num1) < (num2)) ? (num1) : (num2))
#define ABS(num) (((num) >= 0) ? (num) : (-(num)))

#define ESIZE(type, size) (size)

#define BOOL_VALUE(cond) ((cond) ? NL_TRUE : NL_FALSE)

#define DELETE(ptr) { free(ptr); ptr = NULL; }

#define UNUSED(x) (void)(x);

#define BITS(x) (1 << (x))

#define CLAMP(t, min, max) MAX(min, MIN(t, max))

#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))
#define LENGTHOF COUNTOF

#define BITS_FALSE(b, t) (((b) & (t)) == 0)
#define BITS_TRUE(b, t) (((b) & (t)) != 0)

#define printfln(fmt, args...) printf(fmt"\n", ##args)

#define PUSH_BACK(T, arr, count, t) \
{ \
    T *_narr = (T *)calloc(count + 1, sizeof(T)); \
    if(arr) { \
        memcpy(_narr, arr, count * sizeof(T)); \
        free(arr); \
    } \
    _narr[count] = t; \
    count++; \
    arr = _narr; \
}

#ifdef __cplusplus
}
#endif

#endif // _KARIN_DEF_H
