#include "netlizard.h"

#include "priv_local.h"
#include "soil/image.h"

// ct 3d - class h
// specnaz 3d - class ?
// ct 3d ep2 - class ?

static int_array class_h__function_b_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format);
static byte_array class_h__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const byte_array *data, jint paramBoolean, jint paramInt1, jint paramInt2, jint paramInt3, jint paramInt4, int *width, int *height);
static void class_h__function_a_1byte_array__swap(byte paramArrayOfByte[]);

NLboolean nlReadTextureV2File(const char *name, NETLizard_Texture *tex)
{
    array arr;
    int len;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlLoadTextureV2Data(arr.array, arr.length, tex);
    delete_array(&arr);
    return res;
}

NLboolean nlLoadTextureV2Data(const char *data, NLsizei length, NETLizard_Texture *tex)
{
    if(!nlIsNL3DTextureV2(data, length))
    {
        nlflogfln(NL_LOG_ERR, "Not NETLizard 3D texture v2");
        return NL_FALSE;
    }

    ZERO(tex, NETLizard_Texture);

    int_array arr = class_h__function_b_1byte_array__color_map((const byte *)data, &tex->format);
    tex->color_map.data = (NLint *)arr.array;
    tex->color_map.count = arr.length;

    byte_array data_arr;
    make_array(&data_arr, 1, length, data);
    arr = class_h__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(&data_arr, 0, 0, 0, 0, 0, &tex->width, &tex->height);
    tex->color_index.data = (NLuchar *)arr.array;
    tex->color_index.count = arr.length;
    tex->type = NL_TEXTURE_3D_ENGINE_V2;
    //class_h__function_a_1byte_array__swap((byte *)(tex->color_index));

    return NL_TRUE;
}

NLboolean nlConvertTextureV2FileToImageFile(const char *from, const char *to, int img_type)
{
    NETLizard_Texture tex;
    NLboolean res = nlReadTextureV2File(from, &tex);
    if(!res)
        return NL_FALSE;
    res = nlSaveTextureV2DataToImageFile(&tex, to, img_type);
    nlDeleteNETLizardTexture(&tex);
	return res;
}

NLboolean nlSaveTextureV2DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type)
{
    if(tex->type != NL_TEXTURE_3D_ENGINE_V2)
        return NL_FALSE;
    NLuchar *data = nlMakePixelDataRGBA(tex, NULL);
    if(!data)
        return NL_FALSE;
    int channel = tex->format != NL_RGB ? SOIL_LOAD_RGBA : SOIL_LOAD_RGBA;
    NLboolean res = SOIL_save_image(to, img_type, tex->width, tex->height, channel, data);
    free(data);
    return res;
}

NLboolean nlSavePixelDataToTextureV2File(const NLuchar *data,  int width, int height, NETLizard_Texture_format format, const char *to)
{
    FILE *file;

    file = fopen(to, "wb");
	if(!file)
        return NL_FALSE;

	const int max_index_count = 255 * 255;
	const int max_map_count = 256;
	long index_len = width * height;
	if(index_len > max_index_count)
		index_len = max_index_count;
	byte *index = NEW_II(byte, index_len);
    uint32_t *map = NULL;
	int len = 0;
	int over = 0;
	int i;
	for(i = 0; i < index_len; i++)
	{
        uint32_t color;
		if(format != NL_RGB)
            color = 0xff000000 + (((uint32_t)data[i * 4]) << 16) + (((uint32_t)data[i * 4 + 1]) << 8) + ((uint32_t)data[i * 4 + 2]);
        //color = (((uint32_t)data[i * 4 + 3]) << 24) + (((uint32_t)data[i * 4]) << 16) + (((uint32_t)data[i * 4 + 1]) << 8) + ((uint32_t)data[i * 4 + 2]);
		else
            color = (((uint32_t)data[i * 3]) << 16) + (((uint32_t)data[i * 3 + 1]) << 8) + ((uint32_t)data[i * 3 + 2]);
		if(!map)
		{
            map = NEW_II(uint32_t, len + 1);
			map[len] = color;
			len++;
		}
		else
		{
			int j;
			int cur = len;
			for(j = 0; j < len; j++)
			{
				if(map[j] == color)
				{
					cur = j;
					break;
				}
			}
			if(cur >= len)
			{
				if(len >= max_map_count)
				{
					over = 1;
					break;
				}
                uint32_t *tmp = NEW_II(uint32_t, len + 1);
                memcpy(tmp, map, sizeof(uint32_t) * len);
				tmp[len] = color;
				free(map);
				map = tmp;
				len++;
			}
		}
	}

	if(over)
        nllogfln("Color map'size is over 256. Need to compress.");
	else
        nllogfln("Color map'size is less 256. Don't need compress.");

	free(map);
	len = 0;
	map = NULL;

	for(i = 0; i < index_len; i++)
	{
        uint32_t color;
		if(format != NL_RGB)
            color = 0xff000000 + (((uint32_t)data[i * 4]) << 16) + (((uint32_t)data[i * 4 + 1]) << 8) + ((uint32_t)data[i * 4 + 2]);
        //color = (((uint32_t)data[i * 4 + 3]) << 24) + (((uint32_t)data[i * 4]) << 16) + (((uint32_t)data[i * 4 + 1]) << 8) + ((uint32_t)data[i * 4 + 2]);
		else
            color = (((uint32_t)data[i * 3]) << 16) + (((uint32_t)data[i * 3 + 1]) << 8) + ((uint32_t)data[i * 3 + 2]);

		color = rgb888_to_rgb332(color);
		if(len == max_map_count)
		{
			index[i] = len - 1;
			continue;
		}
		if(!map)
		{
            map = NEW_II(uint32_t, len + 1);
			map[len] = color;
			index[i] = len;
			len++;
		}
		else
		{
			int j;
			int cur = len;
			if(over)
			{
				for(j = 0; j < len; j++)
				{
					if(map[j] == color)
					{
						cur = j;
						break;
					}
				}
			}
			else
			{
				for(j = 0; j < len; j++)
				{
					if(map[j] == color)
					{
						cur = j;
						break;
					}
				}
			}
			if(cur < len)
			{
				index[i] = cur;
			}
			else
			{
                uint32_t *tmp = NEW_II(uint32_t, len + 1);
                memcpy(tmp, map, sizeof(uint32_t) * len);
				tmp[len] = color;
				free(map);
				map = tmp;
				index[i] = len;
				len++;
			}
		}
	}

	//class_h__function_a_1byte_array__swap(index);

    nllogf("Make v2 PNG Color map'size->%d", len);
	int i2 = 3; // 3
	int i3 = format != NL_RGB ? 1 : 0; // 4
	char c;
    const char magic[] = NL_TEXTURE_V2_HEADER_MAGIC;
    fwrite(magic + i, ESIZE(char, 1), 3, file);
	c = (char)i3;
    fwrite(&c, ESIZE(char, 1), 1, file);
	c = (char)len;
    fwrite(&c, ESIZE(char, 1), 1, file);

	// 1 color
	int i1 = len;
	if (i3 == 0)
	{
		int i4 = 0;
		while (i4 < i1)
		{
			//arrayOfInt[i4] = ((paramArrayOfByte[(++i2)] << 16) + (paramArrayOfByte[(++i2)] << 8) + paramArrayOfByte[(++i2)]);
            uint8_t r = (uint8_t)(map[i4] << 8 >> 24);
            uint8_t g = (uint8_t)(map[i4] << 16 >> 24);
            uint8_t b = (uint8_t)(map[i4] << 24 >> 24);
            fwrite(&r, ESIZE(unsigned char, 1), 1, file);
            fwrite(&g, ESIZE(unsigned char, 1), 1, file);
            fwrite(&b, ESIZE(unsigned char, 1), 1, file);
			i4++;
		}
	}
	else
	{
		int i5;
		for (i5 = 0; i5 < i1; i5++) {
            uint8_t a = (uint8_t)(map[i5] >> 24);
			if (a != 0)
			{
				//arrayOfInt[i5] = (-16777216 + (paramArrayOfByte[(++i2)] << 16) + (paramArrayOfByte[(++i2)] << 8) + paramArrayOfByte[(++i2)]);
                uint8_t r = (uint8_t)(map[i5] << 8 >> 24);
                uint8_t g = (uint8_t)(map[i5] << 16 >> 24);
                uint8_t b = (uint8_t)(map[i5] << 24 >> 24);
                fwrite(&a, ESIZE(unsigned char, 1), 1, file);
                fwrite(&r, ESIZE(unsigned char, 1), 1, file);
                fwrite(&g, ESIZE(unsigned char, 1), 1, file);
                fwrite(&b, ESIZE(unsigned char, 1), 1, file);
			}
			else
			{
				c = 0;
                fwrite(&c, ESIZE(unsigned char, 1), 1, file);
                fwrite(&c, ESIZE(unsigned char, 1), 1, file);
                fwrite(&c, ESIZE(unsigned char, 1), 1, file);
                fwrite(&c, ESIZE(unsigned char, 1), 1, file);
			}
		}
	}

	// 2 index
	unsigned int i5 = width;
	unsigned int i6 = height;
    nllogf("Make v2 PNG->width: %d, height: %d\n", i5, i6);
    c = (uint8_t)(i5);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
    c = (uint8_t)(i6);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
	int i7;
	c = 0;
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
	i = i5 * i6;
	for (i7 = 0; i7 < i; i7++) {
        c = (uint8_t)index[i7];
        fwrite(&c, ESIZE(unsigned char, 1), 1, file);
	}
	fflush(file);
	fclose(file);
	free(index);
	free(map);
    return NL_TRUE;
}

NLboolean nlConvertImageFileToTextureV2File(const char *from, const char *to)
{
	int channel;
	int width;
	int height;
	NETLizard_Texture_format format;
	unsigned char *data = SOIL_load_image(from, &width, &height, &channel, SOIL_LOAD_AUTO);
    nllogfln("Read image->width: %d, height: %d", width, height);
	if(!data)
        return NL_FALSE;
	if(channel == SOIL_LOAD_RGB)
		format = NL_RGB;
	else if(channel == SOIL_LOAD_RGBA)
		format = NL_RGBA;
	else
	{
		free(data);
        return NL_FALSE;
	}
    NLboolean res = nlSavePixelDataToTextureV2File(data, width, height, format, to);
	free(data);
	return res;
}



/* ******** static ******** */

int_array class_h__function_b_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format)
{
    jint i2 = 3;
    jint i3 = 0;
	if (paramArrayOfByte[3] == 1) {
		i3 = 1;
	}
    jint i1;
	i2++;
	if ((i1 = paramArrayOfByte[4]) < 0) {
		i1 += 256;
	}
    nllogfln("NETLizard 3D engine v2 texture color map'size->%d", i1);
    int_array arr;
    new_array(&arr, ESIZE(int32_t, 4), i1);
    int32_t *arrayOfInt = (int32_t *)(arr.array);
	if (i3 == 0)
	{
        jint i4 = 0;
		//tmpTernaryOp = paramArrayOfByte;
		while (i4 < i1)
		{
			arrayOfInt[i4] = ((paramArrayOfByte[(++i2)]) << 16 | (paramArrayOfByte[(++i2)]) << 8 | paramArrayOfByte[(++i2)]);
			i4++;
		}
	}
	else
	{
        jint i5;
		for (i5 = 0; i5 < i1; i5++) {
			if (paramArrayOfByte[(++i2)] != 0)
			{
				arrayOfInt[i5] = (-16777216 + ((paramArrayOfByte[(++i2)]) << 16) + ((paramArrayOfByte[(++i2)]) << 8) + paramArrayOfByte[(++i2)]);
			}
			else
			{
				i2 += 3;
				arrayOfInt[i5] = 0;
			}
		}
	}
	if(format)
	{
		if(i3 != 0)
			*format = NL_RGBA;
		else
			*format = NL_RGB;
	}
	return arr;
}

byte_array class_h__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const byte_array *data, jint paramBoolean, jint paramInt1, jint paramInt2, jint paramInt3, jint paramInt4, int *width, int *height)
{
    jint i2 = 0;
    jint i3 = 0;
	byte *paramArrayOfByte = (byte *)(data->array);
	if (paramArrayOfByte[3] == 1) {
		i3 = 1;
	}
    jint i4 = 0;
	if ((i4 = paramArrayOfByte[4]) < 0) {
		i4 += 256;
	}
	if (i3 != 0) {
		i2 = 4 + i4 * 4;
	} else {
		i2 = 4 + i4 * 3;
	}
	//printf("____%d,%d\n", i2, i4);
	byte *arrayOfByte = NULL;
	size_t length = 0;
	int w;
	int h;
    jint i5;
    jint i6;
    jint i7;
	if (!paramBoolean)
	{
		i5 = paramArrayOfByte[(++i2)];
		i6 = paramArrayOfByte[(++i2)];
		if (i5 < 0) {
			i5 += 256;
		}
		if (i6 < 0) {
			i6 += 256;
		}
		if (i5 == 0) {
			i5 += 256;
		}
		if (i6 == 0) {
			i6 += 256;
		}
        jint i1;
		arrayOfByte = NEW_II(byte, (i1 = i5 * i6));
        nllogfln("NETLizard 3D engine v2 texture->width: %d, height: %d", i5, i6);
		for (i7 = 0; i7 < i1; i7++) {
			arrayOfByte[i7] = paramArrayOfByte[(++i2)];
		}
		length = i1;
		w = i5;
		h = i6;
	}
	else
	{
		i5 = paramArrayOfByte[(++i2)];
		i2++;
		i6 = 0;
		if (i5 < 0) {
			i5 += 256;
		}
		i7 = 0;
		arrayOfByte = NEW_II(byte, paramInt3 * paramInt4);
		i2 += paramInt2 * i5 + paramInt1;
		i2++;
        jint i9;
		for (i9 = 0; i9 < paramInt4; i9++)
		{
            jint i8 = i9 * i5 + i2;
            jint i10;
			for (i10 = 0; i10 < paramInt3; i10++)
			{
				arrayOfByte[i7] = paramArrayOfByte[(i8 + i10)];
				i7++;
			}
		}
		length = paramInt3 * paramInt4;
		w = paramInt4;
		h = paramInt3;
	}
    byte_array arr;
    make_array(&arr, 1, length, arrayOfByte);
	if(width)
		*width = w;
	if(height)
        *height = h;
	return arr;
}

void class_h__function_a_1byte_array__swap(byte paramArrayOfByte[])
{
    jint i2;
    jint i3;
	for (i2 = 63; i2 >= 0; i2--) {
		for (i3 = 31; i3 >= 0; i3--)
		{
			byte i1 = paramArrayOfByte[(i3 * 64 + i2)];
			paramArrayOfByte[(i3 * 64 + i2)] = paramArrayOfByte[((63 - i3) * 64 + i2)];
			paramArrayOfByte[((63 - i3) * 64 + i2)] = i1;
		}
	}
}
