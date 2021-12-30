#include "netlizard.h"

#include "priv_local.h"
#include "soil/image.h"

// egpyt 3d - class s
// clone 3d - class g
// ct 3d ep3 - class ?

#define class_g__function_b_1byte_array__color_map(byte_array, format_ptr) \
	class_s__function_a_1byte_array__color_map(byte_array, format_ptr)
#define class_g__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(byte_array, bool1, int1, int2, int3, int4, width_ptr, height_ptr) \
	class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(byte_array, bool1, int1, int2, int3, int4, width_ptr, height_ptr)
#define class_g__function_a_1byte_array_2byte__get_width_height_depth(byte_array, what) \
	class_s__function_a_1byte_array_2byte__get_width_height_depth(byte_array, what)
#define class_g__no_function_1byte_array_2int__swap(byte_array, int1) \
	class_s__function_a_1byte_array_2int__swap(byte_array, int1)

static int_array class_s__function_a_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format);
static byte_array class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const byte_array *data, jint paramBoolean, jint paramInt1, jint paramInt2, jint paramInt3, jint paramInt4, int *width, int *height);
static void class_s__function_a_1byte_array_2int__swap(byte paramArrayOfByte[], jint paramInt);
static jint class_s__function_a_1byte_array_2byte__get_width_height_depth(byte paramArrayOfByte[], byte paramByte);

NLboolean nlReadTextureV3File(const char *name, NLint i1, NETLizard_Texture *tex)
{
    array arr;
    int len;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlLoadTextureV3Data(arr.array, arr.length, i1, tex);
    delete_array(&arr);
    return res;
}

NLboolean nlLoadTextureV3Data(const char *data, NLsizei length, NLint i1, NETLizard_Texture *tex)
{
    if(!nlIsNL3DTextureV3(data, length))
    {
        nlflogfln(NL_LOG_ERR, "Not NETLizard 3D texture v3");
        return NL_FALSE;
    }

    ZERO(tex, NETLizard_Texture);
    int_array arr = class_s__function_a_1byte_array__color_map((const byte *)data, &tex->format);
    tex->color_map.data = (NLint *)arr.array;
    tex->color_map.count = arr.length;

    byte_array data_arr;
    make_array(&data_arr, 1, length, data);
    arr = class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(&data_arr, 0, 0, 0, 0, 0, &tex->width, &tex->height);
    tex->color_index.data = arr.array;
    tex->color_index.count = arr.length;
    tex->type = NL_TEXTURE_3D_ENGINE_V3;

    if (i1 > 6) {
        class_s__function_a_1byte_array_2int__swap((byte *)(tex->color_index.data), 7);
    }
    else if(i1 < 0)
    {
    }
    else
    {
        class_s__function_a_1byte_array_2int__swap((byte *)(tex->color_index.data), 8);
    }
    tex->depth = 1;
    return NL_TRUE;
}

// fp*.png 主视角武器
NLboolean nlLoadCompressTextureV3Data(const char *data, NLsizei length, NETLizard_Texture *tex)
{
    if(!nlIsNL3DTextureV3(data, length))
    {
        nlflogfln(NL_LOG_ERR, "Not NETLizard 3D texture v3 compress");
        return NL_FALSE;
    }

    ZERO(tex, NETLizard_Texture);

    int_array arr = class_s__function_a_1byte_array__color_map((const byte *)data, &tex->format);
    tex->color_map.data = (NLint *)arr.array;
    tex->color_map.count = arr.length;

    byte_array data_arr;
    make_array(&data_arr, 1, length, data);
    arr = class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(&data_arr, 0, 0, 0, 0, 0, &tex->width, &tex->height);
    tex->color_index.data = arr.array;
    tex->color_index.count = arr.length;

    tex->width = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 0);
    tex->height = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 1);
    tex->depth = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 2);
    tex->type = NL_TEXTURE_3D_ENGINE_V3_COMPRESS;

    return NL_TRUE;
}

NLboolean nlReadCompressTextureV3File(const char *name, NETLizard_Texture *tex)
{
    array arr;
    int len;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlLoadCompressTextureV3Data(arr.array, arr.length, tex);
    delete_array(&arr);
    return res;
}

NLboolean nlConvertTextureV3FileToImageFile(const char *from, NLint i, const char *to, int img_type)
{
    NETLizard_Texture tex;
    NLboolean res = nlReadTextureV3File(from, i, &tex);
    if(!res)
        return NL_FALSE;
    res = nlSaveTextureV3DataToImageFile(&tex, to, img_type);
    nlDeleteNETLizardTexture(&tex);
    return res;
}

NLboolean nlSaveTextureV3DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type)
{
    if(tex->type != NL_TEXTURE_3D_ENGINE_V3)
        return NL_FALSE;
    NLuchar *data = nlMakePixelDataRGBA(tex, NULL);
    int channel = tex->format != NL_RGB ? SOIL_LOAD_RGBA : SOIL_LOAD_RGBA;
    NLboolean res = SOIL_save_image(to, img_type, tex->width, tex->height, channel, data);
    free(data);
    return res;
}

NLboolean nlSavePixelDataToTextureV3File(const NLuchar *data, NLint _i, int width, int height, NETLizard_Texture_format format, const char *to)
{
    FILE *file;

    file = fopen(to, "wb");
    if(!file)
        return NL_FALSE;

	const unsigned int max_index_count = UINT_MAX; // TODO
	const int max_map_count = 256;
	unsigned int index_len = width * height;
	if(index_len > max_index_count)
		index_len = max_index_count;
	byte *index = NEW_II(byte, index_len);
    uint32_t *map = NULL;
	int len = 0;
	int over = 0;
	unsigned int i;
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
        nllogfln("Color map'size is less 256. Don't need to compress.");

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
			if(cur != len)
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

	if (_i > 6) {
		class_s__function_a_1byte_array_2int__swap(index, 7);
	} else if(_i < 0){
	}
	else
	{
		class_s__function_a_1byte_array_2int__swap(index, 8);
	}

    nllogfln("Make v3 PNG color map'size->%d", len);
	int j = 3; // 3
	int m4 = 0; // 4
	int k = format != NL_RGB ? 1 : 0; // 5
	char c;
    const char magic[] = NL_TEXTURE_V3_HEADER_MAGIC;
    fwrite(magic + i, ESIZE(char, 1), 3, file);
	c = (char)k;
    fwrite(&c, ESIZE(char, 1), 1, file);
	c = (char)m4;
    fwrite(&c, ESIZE(char, 1), 1, file);
	c = (char)len;
    fwrite(&c, ESIZE(char, 1), 1, file);

	// 1 color
	i = len;
	if (k == 0)
	{
		int m = 0;
		while (m < i)
		{
			//arrayOfInt[m] = ((paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
            uint8_t r = (uint8_t)(map[m] << 8 >> 24);
            uint8_t g = (uint8_t)(map[m] << 16 >> 24);
            uint8_t b = (uint8_t)(map[m] << 24 >> 24);
            fwrite(&r, ESIZE(unsigned char, 1), 1, file);
            fwrite(&g, ESIZE(unsigned char, 1), 1, file);
            fwrite(&b, ESIZE(unsigned char, 1), 1, file);
			m++;
		}
	}
	else
	{
		int n;
		for (n = 0; n < i; n++) {
            uint8_t a = (uint8_t)(map[n] >> 24);
			if (a != 0)
			{
				//arrayOfInt[n] = (0xFF000000 | (paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
                uint8_t r = (uint8_t)(map[n] << 8 >> 24);
                uint8_t g = (uint8_t)(map[n] << 16 >> 24);
                uint8_t b = (uint8_t)(map[n] << 24 >> 24);
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
	unsigned int i1 = width;
	unsigned int i2 = height;
    nllogfln("Make v3 PNG->width: %d, height: %d", i1, i2);
    c = (uint8_t)(i1 << 16 >> 24);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
    c = (uint8_t)(i1 << 24 >> 24);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
    c = (uint8_t)(i2 << 16 >> 24);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
    c = (uint8_t)(i2 << 24 >> 24);
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
	//int ii1 = (i1 << 16 >> 24 << 8) | (i1 << 24 >> 24);
	//int ii2 = (i2 << 16 >> 24 << 8) | (i2 << 24 >> 24);
	//printf("s2 %d, %d\n", ii1, ii2);
	int i3;
	c = 0;
    fwrite(&c, ESIZE(unsigned char, 1), 1, file);
	if (m4 == 0)
	{
		i = i1 * i2;
		for (i3 = 0; i3 < i; i3++) {
            c = (uint8_t)index[i3];
            fwrite(&c, ESIZE(unsigned char, 1), 1, file);
		}
	}
	else
	{
		c = 0;
        fwrite(&c, ESIZE(unsigned char, 1), 1, file);
		i = width * height;
		for (i3 = 0; i3 < i; i3++) {
            c = (uint8_t)index[i3];
            fwrite(&c, ESIZE(unsigned char, 1), 1, file);
		}
	}
	fflush(file);
	fclose(file);
	free(index);
	free(map);
    return NL_TRUE;
}

NLboolean nlConvertImageFileToTextureV3File(const char *from, NLint _i, const char *to)
{
	int channel;
	int width;
	int height;
	NETLizard_Texture_format format;
	unsigned char *data = SOIL_load_image(from, &width, &height, &channel, SOIL_LOAD_AUTO);
    nllogfln("Read image->width: %d, height: %d", width, height);
	if(!data)
		return 0;
	if(channel == SOIL_LOAD_RGB)
		format = NL_RGB;
	else if(channel == SOIL_LOAD_RGBA)
		format = NL_RGBA;
	else
	{
		free(data);
		return 0;
	}
	int res = nlSavePixelDataToTextureV3File(data, _i, width, height, format, to);
	free(data);
	return res;
}

NLboolean nlConvertTextureV3CompressFileToImageFile(const char *from, const char *to, int img_type)
{
    NETLizard_Texture tex;
    NLboolean res = nlReadCompressTextureV3File(from, &tex);
    if(!res)
        return NL_FALSE;
    res = nlSaveTextureV3CompressDataToImageFile(&tex, to, img_type);
    nlDeleteNETLizardTexture(&tex);
    return res;
}

NLboolean nlSaveTextureV3CompressDataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type)
{
    NLuchar *data = nlMakePixelDataRGBACompress(tex, NULL);
    int channel = tex->format != NL_RGB ? SOIL_LOAD_RGBA : SOIL_LOAD_RGBA;
    NLboolean res = SOIL_save_image(to, img_type, tex->width, tex->height, channel, data);
    free(data);
    return res;
}


/* ******** static ******** */

int_array class_s__function_a_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format)
{
    jint j = 3;
    jint k = 0;
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
	j++;
    jint i;
	j++;
	if ((i = paramArrayOfByte[5]) < 0) {
		i += 256;
	}
    nllogfln("NETLizard 3D engine v3 texture color map'size->%d", i);
    int_array arr;
    new_array(&arr, ESIZE(jint, 4), i);
    jint *arrayOfInt = (jint *)(arr.array);
	if (k == 0)
	{
        jint m = 0;
		//tmpTernaryOp = paramArrayOfByte;
		while (m < i)
		{
			arrayOfInt[m] = ((paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
			m++;
		}
	}
	else
	{
        jint n;
		for (n = 0; n < i; n++) {
			if (paramArrayOfByte[(++j)] != 0)
			{
				arrayOfInt[n] = (0xFF000000 | (paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
			}
			else
			{
				j += 3;
				arrayOfInt[n] = 0;
			}
		}
	}
	if(format)
	{
		if(k != 0)
			*format = NL_RGBA;
		else
			*format = NL_RGB;
	}
	return arr;
}

byte_array class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const byte_array *data, jint paramBoolean, jint paramInt1, jint paramInt2, jint paramInt3, jint paramInt4, int *width, int *height)
{
    jint j = 0;
    jint k = 0;
	byte *paramArrayOfByte = (byte *)(data->array);
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
    jint m = 0;
	if (paramArrayOfByte[4] == 1) {
		m = 1;
	}
    jint n;
	if ((n = paramArrayOfByte[5]) < 0) {
		n += 256;
	}
	if (k != 0) {
		j = 5 + n * 4;
	} else {
		j = 5 + n * 3;
	}
	//printf("____%d,%d\n", n, j);
	byte *arrayOfByte = NULL;
	size_t length = 0;
	int w;
	int h;
    jint i1;
    jint i2;
    jint i3;
	if (!paramBoolean)
	{
		i1 = paramArrayOfByte[(++j)] << 8 | paramArrayOfByte[(++j)];
		i2 = paramArrayOfByte[(++j)] << 8 | paramArrayOfByte[(++j)];
		if (i1 < 0) {
			i1 += 256;
		}
		if (i2 < 0) {
			i2 += 256;
		}
		j++;
        jint i;
        nllogfln("NETLizard 3D engine v3 texture->width: %d, height: %d", i1, i2);
		if (m == 0)
		{
			arrayOfByte = NEW_II(byte, (i = i1 * i2));
			for (i3 = 0; i3 < i; i3++) {
				arrayOfByte[i3] = paramArrayOfByte[(++j)];
			}
			length = i;
			w = i1;
			h = i2;
		}
		else
		{
			j++;
			arrayOfByte = NEW_II(byte, (i = data->length - j));
			for (i3 = 0; i3 < i; i3++) {
				arrayOfByte[i3] = paramArrayOfByte[(j++)];
			}
			length = i;
			w = h = (size_t)sqrt(i);
		}
	}
	else
	{
		i1 = 0;
		i3 = paramArrayOfByte[(++j)] << 8 | paramArrayOfByte[(++j)];
		j++;
		j++;
		if (i3 < 0) {
			i3 += 256;
		}
		arrayOfByte = NEW_II(byte, paramInt3 * paramInt4);
		j += paramInt2 * i3 + paramInt1;
		j++;
		j++;
        jint i4;
		for (i4 = 0; i4 < paramInt4; i4++)
		{
			i2 = i4 * i3 + j;
            jint i5;
			for (i5 = 0; i5 < paramInt3; i5++)
			{
				arrayOfByte[i1] = paramArrayOfByte[(i2 + i5)];
				i1++;
			}
		}
		length = paramInt3 * paramInt4;
		w = paramInt4;
		h = paramInt3;
	}
    array arr;
    make_array(&arr, 1, length, arrayOfByte);
	if(width)
		*width = w;
	if(height)
        *height = h;
	return arr;
}

void class_s__function_a_1byte_array_2int__swap(byte paramArrayOfByte[], jint paramInt)
{
    jint j;
    jint k = (j = (1 << paramInt) - 1) >> 1;
    jint m;
    jint n;
	for (m = j; m >= 0; m--) {
		for (n = k; n >= 0; n--)
		{
            jint i = paramArrayOfByte[(n * (j + 1) + m)];
			paramArrayOfByte[(n * (j + 1) + m)] = paramArrayOfByte[((j - n) * (j + 1) + m)];
			paramArrayOfByte[((j - n) * (j + 1) + m)] = i;
		}
	}
}

jint class_s__function_a_1byte_array_2byte__get_width_height_depth(byte paramArrayOfByte[], byte paramByte)
{
    jint j = 0;
    jint k = 0;
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
	if (paramByte == 2) {
		return paramArrayOfByte[4];
	}
    jint i;
	if ((i = paramArrayOfByte[5]) < 0) {
		i += 256;
	}
	if (k != 0) {
		j = 5 + i * 4;
	} else {
		j = 5 + i * 3;
	}
    jint m;
    jint n;
	if (paramByte == 0)
	{
        if ((m = (jshort)paramArrayOfByte[(++j)]) < 0) {
            m = (jshort)(m + 256);
		}
        if ((n = (jshort)paramArrayOfByte[(++j)]) < 0) {
            n = (jshort)(n + 256);
		}
		i = m << 8 | n;
	}
	if (paramByte == 1)
	{
		j += 2;
        if ((m = (jshort)paramArrayOfByte[(++j)]) < 0) {
            m = (jshort)(m + 256);
		}
        if ((n = (jshort)paramArrayOfByte[(++j)]) < 0) {
            n = (jshort)(n + 256);
		}
		i = m << 8 | n;
	}
	return i;
}
