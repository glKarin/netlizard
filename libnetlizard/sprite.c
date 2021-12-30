#include "netlizard.h"

#include "priv_local.h"

// clone 3d is cu.png.
// sprite texture file is u0.png, other 3D game like this, u\d.png file.
// ccc.png + ccu.png

static T_array(jshort[6]) clone3d_class_g__function_M_void__read_sprite_map(const byte_array *arr);

NLint nlGetSpiritFileCount(const char *file)
{
    jlong len = file_size(file);
    return len / 6;
}

NLint nlGetSpiritDataCount(const char *data, NLsizei length)
{
    UNUSED(data);
    return length / 6;
}

NLboolean nlReadSpiritMapFile(const char *file, NETLizard_Sprite *ret)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(file, &arr);
    if(res <= 0)
        return -1;

    b = nlLoadSpiritMapData(arr.array, arr.length, ret);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadSpiritMapData(const char *data, NLsizei len, NETLizard_Sprite *ret)
{
    array arr;
    make_array(&arr, 1, len, data);

    T_array(jshort[6]) qarr = clone3d_class_g__function_M_void__read_sprite_map(&arr);
    if(qarr.length == 0)
        return NL_FALSE;

    const int Length = qarr.length;
    ret->data = NEW_II(NETLizard_Sprite_Cell, Length);
    jshort (*q)[6] = (jshort (*)[6])(qarr.array);
    int i;
    for(i = 0; i < Length; i++)
    {
        NETLizard_Sprite_Cell *sprite = ret->data + i;
        sprite->x = q[i][0];
        sprite->y = q[i][1];
        sprite->width = q[i][2];
        sprite->height = q[i][3];
        sprite->private_4 = q[i][4];
        sprite->private_5 = q[i][5];
    }
    delete_array(&qarr);

    ret->count = Length;

    return NL_TRUE;
}

void nlDeleteNETLizardSprite(NETLizard_Sprite *sprite)
{
    free(sprite->data);
    sprite->count = 0;
}

/* static */
T_array(jshort[6]) clone3d_class_g__function_M_void__read_sprite_map(const array *arr)
{
	byte *arrayOfByte = (byte *)(arr->array);
    jint i1;
    jint i5 = (jshort)(i1 = (arr->length / 6));
    T_array(jshort[6]) rarr;
    new_array(&rarr, sizeof(jshort[6]), i5);
    jshort (*q)[6] = (jshort (*)[6])(rarr.array);
    jint i2;
	for (i2 = 0; i2 < i1; i2++)
	{
        //q[i2] = ((jshort *)(rarr.array)) + (i2 * 6);
        jint i4 = i2 * 6;
        jint i3;
		for (i3 = 0; i3 < 6; i3++)
		{
            q[i2][i3] = ((jshort)arrayOfByte[(i4 + i3)]);
			if (q[i2][i3] < 0)
			{
                jint tmp76_75 = i3;
                jshort *tmp76_74 = q[i2];
                tmp76_74[tmp76_75] = ((jshort)(tmp76_74[tmp76_75] + 256));
			}
		}
	}
	return rarr;
}

