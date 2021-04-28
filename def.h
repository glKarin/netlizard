#ifndef _KARIN_DEF_H
#define _KARIN_DEF_H

#include <stddef.h>

#define NEW(T) malloc(sizeof(T))
#define NEW_II(T, size) calloc(size, sizeof(T))
#define ZERO(ptr, T) memset(ptr, 0, sizeof(T))
#define ZERO_II(ptr, T, size) memset(ptr, 0, sizeof(T) * size)
#define ZERO_III(ptr) memset(ptr, 0, sizeof(*ptr))

#define MAX(num1, num2) (((num1) > (num2)) ? (num1) : (num2))
#define MIN(num1, num2) (((num1) < (num2)) ? (num1) : (num2))
#define ABS(num) (((num) >= 0) ? (num) : (-(num)))

#endif // _KARIN_DEF_H
