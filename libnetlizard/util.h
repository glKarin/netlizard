#ifndef _KARIN_UTIL_H
#define _KARIN_UTIL_H

#include "jport.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

jfloat int_bits_to_float(jint i);
jint marge_digit(jint paramInt1, jint paramInt2);
jint sqrt_box(jint x, jint y, jint z);
unsigned int rgb888_to_rgb332(unsigned int a);

int nlvasprintf(char **ptr, const char *fmt, va_list ap);
int nlasprintf(char **ptr, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
