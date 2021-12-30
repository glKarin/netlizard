#include "util.h"

#include "priv_local.h"

jfloat int_bits_to_float(jint i)
{
	union bit
	{
        jint i;
        jfloat f;
	} u;
	memset(&u, 0, sizeof(union bit));
	u.i = i;
	return u.f;
}

// 3D map general function
// ct3d h.a(int, int)
// clone3d h.b(int, int)
// egypt3d f.a(int, int);
jint marge_digit(jint paramInt1, jint paramInt2)
{
	if (paramInt1 < 0) {
		paramInt1 += 256;
	}
	if (paramInt2 < 0) {
		paramInt2 += 256;
	}
	return paramInt1 + (paramInt2 << 8); // * 256
}

jint sqrt_box(jint x, jint y, jint z)
{
    jint i2 = x * x + y * y + z * z;
    jint i3 = 65536;
    jint i5 = 0;
	while (i3 >>= 1 > 0) {
		if ((i5 + i3) * (i5 + i3) <= i2) {
			i5 += i3;
		}
	}
	return i5;
}

unsigned int rgb888_to_rgb332(unsigned int a)
{
    unsigned int ar = (unsigned int)(a << 8 >> 24);
    unsigned int ag = (unsigned int)(a << 16 >> 24);
    unsigned int ab = (unsigned int)(a << 24 >> 24);
    unsigned int r = (unsigned int)floor((double)ar / (double)255 * (double)7) ;
    unsigned int g = (unsigned int)floor((double)ag / (double)255 * (double)7);
    unsigned int b = (unsigned int)floor((double)ab / (double)255 * (double)3);
    r = (unsigned int)floor((double)r / (double)7 * (double)255);
    g = (unsigned int)floor((double)g / (double)7 * (double)255);
    b = (unsigned int)floor((double)b / (double)3 * (double)255);
    return 0xff000000 | (r << 16) | (g << 8) | b;
}

int nlvasprintf(char **ptr, const char *fmt, va_list ap)
{
    int length = vsnprintf(NULL, 0, fmt, ap);
    *ptr = NULL;
    if(length < 0)
        return length;
    char *str = calloc(length + 1, sizeof(char));
    va_list ap_copy;
    va_copy(ap_copy, ap);
    length = vsnprintf(str, length + 1, fmt, ap_copy);
    va_end(ap_copy);
    if(length < 0)
        free(str);
    else
        *ptr = str;
    return length;
    // res = vfwprintf(file, str, ap); fwprintf(file, L"\n");
}

int nlasprintf(char **ptr, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int length = nlvasprintf(ptr, fmt, ap);
    va_end(ap);
    return length;
}
