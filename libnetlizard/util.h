#ifndef _KARIN_UTIL_H
#define _KARIN_UTIL_H

#include "jport.h"

#ifdef __cplusplus
extern "C" {
#endif

jfloat int_bits_to_float(jint i);
jint marge_digit(jint paramInt1, jint paramInt2);
jint sqrt_box(jint x, jint y, jint z);
unsigned int rgb888_to_rgb332(unsigned int a);

#ifdef __cplusplus
}
#endif

#endif
