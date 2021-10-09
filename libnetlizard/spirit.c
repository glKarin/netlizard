#include "netlizard.h"

#include "priv_local.h"

static T_array(jshort[6]) clone3d_class_g__function_M_void__read_spirit_map(const array *arr);

// clone 3d is cu.png.
// spirit texture file is u0.png, other 3D game like this, u\d.png file.
int nlReadSpiritMap(const char *file, NETLizard_Spirit **ret)
{
    array arr;
    int len = file_get_contents(file, &arr);
    if(len <= 0)
        return -1;

    int res = nlLoadSpiritMapData(arr.array, arr.length, ret);
    delete_array(&arr);
    return res;
}

int nlLoadSpiritMapData(const char *data, NLsizei len, NETLizard_Spirit **ret)
{
    array arr;
    make_array(&arr, 1, len, data);
    T_array(jshort[6]) qarr = clone3d_class_g__function_M_void__read_spirit_map(&arr);
    len = qarr.length;
    NETLizard_Spirit *spirit = NEW_II(NETLizard_Spirit, len);
    short **q = (short **)(qarr.array);
    int i;
    for(i = 0; i < len; i++)
    {
        spirit[i].x = q[i][0];
        spirit[i].y = q[i][1];
        spirit[i].width = q[i][2];
        spirit[i].height = q[i][3];
        spirit[i].private_4 = q[i][4];
        spirit[i].private_5 = q[i][5];
        free(q[i]);
    }
    delete_array(&qarr);

    *ret = spirit;
    return len;
}

T_array(jshort[6]) clone3d_class_g__function_M_void__read_spirit_map(const array *arr)
{
	byte *arrayOfByte = (byte *)(arr->array);
	int i1;
    int i5 = (jshort)(i1 = (arr->length / 6));
    T_array(jshort[6]) rarr;
    new_array(&rarr, sizeof(jshort *), i5);
    jshort **q = (jshort **)(rarr.array);
	int i2;
	for (i2 = 0; i2 < i1; i2++)
	{
        q[i2] = NEW_II(jshort, 6);
		int i4 = i2 * 6;
		int i3;
		for (i3 = 0; i3 < 6; i3++)
		{
            q[i2][i3] = ((jshort)arrayOfByte[(i4 + i3)]);
			if (q[i2][i3] < 0)
			{
				int tmp76_75 = i3;
                jshort *tmp76_74 = q[i2];
                tmp76_74[tmp76_75] = ((jshort)(tmp76_74[tmp76_75] + 256));
			}
		}
	}
	return rarr;
}

