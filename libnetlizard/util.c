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
