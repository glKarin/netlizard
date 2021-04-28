#include "util.h"

#include "priv_local.h"

float int_bits_to_float(int i)
{
	union bit
	{
		int i;
		float f;
	} u;
	memset(&u, 0, sizeof(union bit));
	u.i = i;
	return u.f;
}

// 3D map general function
// ct3d h.a(int, int)
// clone3d h.b(int, int)
// egypt3d f.a(int, int);
int marge_digit(int paramInt1, int paramInt2)
{
	if (paramInt1 < 0) {
		paramInt1 += 256;
	}
	if (paramInt2 < 0) {
		paramInt2 += 256;
	}
	return paramInt1 + (paramInt2 << 8); // * 256
}

int sqrt_box(int x, int y, int z)
{
	int i2 = x * x + y * y + z * z;
	int i3 = 65536;
	int i5 = 0;
	while (i3 >>= 1 > 0) {
		if ((i5 + i3) * (i5 + i3) <= i2) {
			i5 += i3;
		}
	}
	return i5;
}
