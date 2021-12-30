#include "netlizard.h"

#include "priv_local.h"

#define NL_FONT_FILE "fnt.png"
#define NL_FONT_CF_FILE "cf.png"

typedef struct _class__o__font
{
    jint a;
    jint d;
    boolean bool__e;
    T_array(jshort[]) short_2_array_6x__f__map;
    byte_array byte_array__h__offset;
    jint int__i__char_count;
    boolean bool__l;
    boolean bool__m;
} class__o__font;

static class__o__font * class__o__font___construct(class__o__font *fnt);
static class__o__font clone3d_class_o__function_a_void_return_class_o(const byte_array *arr);
static jint clone3d_class_o__function_a_1int_return_int__get_char_map_index(const class__o__font *o, jint paramInt);

NLboolean nlLoadNETLizardFontData(const char *data, NLsizei size, NETLizard_Font *fnt)
{
    if(size <= 0)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, size, data);

    class__o__font res = clone3d_class_o__function_a_void_return_class_o(&arr);
    class__o__font *o = &res;

    ZERO(fnt, NETLizard_Font);
    fnt->private_m = o->bool__m;
    fnt->private_e = o->bool__e;
    fnt->private_l = o->bool__l;
    fnt->char_count = o->int__i__char_count;
    strcpy(fnt->fnt_file, NL_FONT_FILE);
    fnt->offset.count = o->byte_array__h__offset.length;
    fnt->offset.data = NEW_II(NLchar, o->byte_array__h__offset.length);
    memcpy(fnt->offset.data, o->byte_array__h__offset.array, o->byte_array__h__offset.length);
    fnt->char_raster_map.count = o->short_2_array_6x__f__map.length;
    fnt->char_raster_map.data = NEW_II(NETLizard_Font_Char_Raster, o->short_2_array_6x__f__map.length);
    int i;
    jshort **f = (jshort **)(o->short_2_array_6x__f__map.array);
    for(i = 0; i < o->short_2_array_6x__f__map.length; i++)
    {
        NETLizard_Font_Char_Raster *c = (NETLizard_Font_Char_Raster *)(fnt->char_raster_map.data) + i;
        c->x = f[i][0];
        c->y = f[i][1];
        c->width = f[i][2];
        c->height = f[i][3];
        c->x_stride = f[i][4];
        c->y_stride = f[i][5];
        free(f[i]);
    }

    delete_array(&o->short_2_array_6x__f__map);
    delete_array(&o->byte_array__h__offset);

    return NL_TRUE;
}

NLboolean nlReadNETLizardFontFile(const char *map_file, NETLizard_Font *fnt)
{
    array arr;
    int len = file_get_contents(map_file, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlLoadNETLizardFontData(arr.array, len, fnt);
    delete_array(&arr);
    return res;
}

void nlDeleteNETLizardFont(NETLizard_Font *fnt)
{
    free(fnt->char_raster_map.data);
    fnt->char_raster_map.count = 0;
    free(fnt->offset.data);
    fnt->offset.count = 0;
}

class__o__font * class__o__font___construct(class__o__font *fnt)
{
	ZERO(fnt, class__o__font);
  fnt->bool__e = jtrue;
  //fnt->short_2_array_6x__f__map;
  int8_t h[] = { 0, 4, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 10, 0, 0, 0, 0, 0, 12, 0, 0, 2, 0, 0, 0, 16, 0, 3, 17, 0, 1, 0, -4, 0, 0, 0, 0, 0, 0, 0, -8, 0, 0, 0, -10, 0, 0, 0, 0, 0, -12, 0, 0, -2, 0, 0, 0, -16, 0, -3, -17, -1, 0, 0, 0, 5, -5, 19, -19, 20, -20, 0, 0, 0, 0, 6, -6, 7, -7, 0, 0, 0, 0, 0, 0, 0, 0, 21, -21, 9, -9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, -22, 23, -23, 0, 0, 11, -11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, -13, 24, -24, 0, 0, 0, 0, 14, -14, 0, 0, 15, -15, 0, 0, 0, 0, 0, 0, 0, 0, 27, -27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, -25, 26, -26, 18, -18, 0, 0, 0 };
  new_array_with_data(&fnt->byte_array__h__offset, ESIZE(char, 1), sizeof(h) / sizeof(int8_t), h, 0);
  fnt->int__i__char_count = 127;
  fnt->bool__l = jtrue;
  fnt->bool__m = jtrue;
	return fnt;
}

// cf.png
class__o__font clone3d_class_o__function_a_void_return_class_o(const byte_array *arr)
{
    class__o__font res;
    class__o__font___construct(&res);
    class__o__font *o = &res;

	byte *arrayOfByte = (byte *)(arr->array);
    jint i4 = arr->length / 6;
	if (o->int__i__char_count < i4) {
		o->int__i__char_count = i4;
    }
    new_array(&o->short_2_array_6x__f__map, sizeof(jshort *), o->int__i__char_count + 2);
    jshort **f = (jshort **)(o->short_2_array_6x__f__map.array);
    f[o->int__i__char_count] = NEW_II(jshort, 6);
    f[o->int__i__char_count + 1] = NEW_II(jshort, 6);
    jint i2 = 0;
    jint i1;
	for (i1 = 0; i1 < o->int__i__char_count; i1++)
	{
        f[i1] = NEW_II(jshort, 6);
        jint n;
		for (n = 0; n < 6; n++)
		{
            f[i1][n] = ((jshort)(signed char)arrayOfByte[i2]);
			i2++;
		}
		if (f[i1][0] < 0)
		{
            jint tmp141_140 = 0;
            jshort *tmp141_139 = f[i1];
            tmp141_139[tmp141_140] = ((jshort)(tmp141_139[tmp141_140] + 256));
		}
		if (f[i1][1] < 0)
		{
            jint tmp167_166 = 1;
            jshort *tmp167_165 = f[i1];
            tmp167_165[tmp167_166] = ((jshort)(tmp167_165[tmp167_166] + 256));
		}
		if (i2 >= arr->length) {
			break;
		}
	}
	if (f[0][2] == 0) {
		f[0][2] = f[16][2];
	}
	if (f[0][2] == 0) {
		f[0][2] = f[33][2];
	}
	f[0][3] = 0;
	if (i4 < 146)
	{
        o->bool__l = jfalse;
        o->bool__m = jfalse;
        o->bool__e = jfalse;
	}
	else
	{
        o->bool__l = jtrue;
        o->bool__m = jtrue;
        o->bool__e = jtrue;
	}
	o->d = f[clone3d_class_o__function_a_1int_return_int__get_char_map_index(o, 1062)][3];
    jint n = 0;
    jlong l1 = 0L;
	for (i1 = 65; i1 <= 90; i1++)
	{
		l1 += f[clone3d_class_o__function_a_1int_return_int__get_char_map_index(o, i1)][3];
		if (n < f[clone3d_class_o__function_a_1int_return_int__get_char_map_index(o, i1)][3]) {
			n = f[clone3d_class_o__function_a_1int_return_int__get_char_map_index(o, i1)][3];
		}
	}
    o->d = ((jint)(l1 / 26L));
	o->d = n;
	n = 100;
	i1 = -100;
    for (i2 = 0; i2 < o->short_2_array_6x__f__map.length; i2++)
	{
		int i3 = -f[i2][5];
		i4 = -(f[i2][3] + f[i2][5]);
		if (i1 < i3) {
			i1 = i3;
		}
		if (n > i4) {
			n = i4;
		}
	}
	int i3 = i1 - n;
	if (o->d > i3) {
		o->d = i3;
	}
	if ((n = (i4 = i3 - o->d >> 1) - i1) != 0) {
        for (i1 = 0; i1 < o->short_2_array_6x__f__map.length; i1++)
		{
            jint tmp529_528 = 5;
            jshort *tmp529_527 = f[i1];
            tmp529_527[tmp529_528] = ((jshort)(tmp529_527[tmp529_528] - n));
		}
	}
	n = 100;
	i1 = -100;
	for (i2 = 65; i2 <= 90; i2++)
	{
		i3 = clone3d_class_o__function_a_1int_return_int__get_char_map_index(o, i2);
		i4 = -f[i3][5];
		i3 = -(f[i3][3] + f[i3][5]);
		if (i1 < i4) {
			i1 = i4;
		}
		if (n > i3) {
			n = i3;
		}
	}
	i3 = i1 - n;
	if ((n = (i4 = o->d - i3 >> 1) - i1) != 0) {
        for (i3 = 0; i3 < o->short_2_array_6x__f__map.length; i3++)
		{
            jint tmp668_667 = 5;
            jshort *tmp668_666 = f[i3];
            tmp668_666[tmp668_667] = ((jshort)(tmp668_666[tmp668_667] - n));
		}
	}
	o->a = o->d;
	//localo1.g = null;
    return res;
}

jint clone3d_class_o__function_a_1int_return_int__get_char_map_index(const class__o__font *o, jint paramInt)
{
    jint n = paramInt;
	if (paramInt == 946) {
		n = 223;
	}
    int8_t *h = (int8_t *)(o->byte_array__h__offset.array); // signed char
    if ((n >= 192) && (n - 192 < o->byte_array__h__offset.length))
	{
		if ((paramInt = h[(n - 192)]) < 0)
		{
			paramInt = -paramInt;
			n = 100;
			if (o->bool__e) {
				n = 201;
			}
			return n + paramInt;
		}
		n = 100;
		if (o->bool__e) {
			n = 165;
		}
		return n + paramInt;
	}
	if (n > 848) {
		n = paramInt - 848;
	}
	if ((n >= 97) && (n <= 122))
	{
		if (o->bool__l) {
			n += 35;
		} else {
			n -= 32;
		}
	}
	else if ((n >= 192) && (n <= 223)) {
		n -= 96;
	} else if ((n >= 224) && (n <= 255))
	{
		if (o->bool__m) {
			n -= 61;
		} else {
			n -= 128;
		}
	}
	else if ((n >= 154) && (n <= 191)) {
		n -= 26;
	}
	n -= 32;
	if ((n < 0) || (n >= o->int__i__char_count)) {
		n = 0;
	}
	return n;
}

