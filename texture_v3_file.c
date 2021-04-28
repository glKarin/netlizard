#include "netlizard.h"

#include "priv_local.h"
#include "soil/image.h"
#include "texture.h"

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

static array class_s__function_a_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format);
static array class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const array *data, int paramBoolean, int paramInt1, int paramInt2, int paramInt3, int paramInt4, int *width, int *height);
static void class_s__function_a_1byte_array_2int__swap(byte paramArrayOfByte[], int paramInt);
static int class_s__function_a_1byte_array_2byte__get_width_height_depth(byte paramArrayOfByte[], byte paramByte);

NLboolean nlReadTextureV3_File(const char *name, int i1, NETLizard_Texture *tex)
{
    array arr;
    int len;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlReadTextureV3_Memory(arr.array, arr.length, i1, tex);
    delete_array(&arr);
    return res;
}

NLboolean nlReadTextureV3_Memory(const char *data, NLsizei length, int i1, NETLizard_Texture *tex)
{
    if(!nlIsNL3DV3Texture(data))
    {
        nlprintf("Not NETLizard 3D texture v3\n");
        return NL_FALSE;
    }

    ZERO(tex, NETLizard_Texture);
    array arr = class_s__function_a_1byte_array__color_map(data, &tex->format);
    tex->color_map.data = (NLint *)arr.array;
    tex->color_map.length = arr.length;

    array data_arr;
    make_array(&data_arr, 1, length, data);
    arr = class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(&data_arr, 0, 0, 0, 0, 0, &tex->width, &tex->height);
    tex->color_index.data = arr.array;
    tex->color_index.length = arr.length;

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
NLboolean nlReadCompressTextureV3_Memory(const char *data, NLsizei length, NETLizard_Texture *tex)
{
    if(!nlIsNL3DV3Texture(data))
    {
        nlprintf("Not NETLizard 3D texture v3 compress\n");
        return NL_FALSE;
    }

    ZERO(tex, NETLizard_Texture);

    array arr = class_s__function_a_1byte_array__color_map(data, &tex->format);
    tex->color_map.data = (NLint *)arr.array;
    tex->color_map.length = arr.length;

    array data_arr;
    make_array(&data_arr, 1, length, data);
    arr = class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(&data_arr, 0, 0, 0, 0, 0, &tex->width, &tex->height);
    tex->color_index.data = arr.array;
    tex->color_index.length = arr.length;

    tex->width = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 0);
    tex->height = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 1);
    tex->depth = class_s__function_a_1byte_array_2byte__get_width_height_depth((byte *)(data_arr.array), 2);

    return NL_TRUE;
}

NLboolean nlReadCompressTextureV3_File(const char *name, NETLizard_Texture *tex)
{
    array arr;
    int len;

    len = file_get_contents(name, &arr);
    if(len <= 0)
        return NL_FALSE;
    NLboolean res = nlReadCompressTextureV3_Memory(arr.array, arr.length, tex);
    delete_array(&arr);
    return res;
}

NLboolean nlSaveImage_V3File(const char *from, int i, const char *to, int img_type)
{
    NETLizard_Texture tex;
    NLboolean res = nlReadTextureV3_File(from, i, &tex);
    if(!res)
        return NL_FALSE;
    res = nlSaveImage_V3Memory(&tex, to, img_type);
    delete_NETLizard_Texture(&tex);
    return res;
}

NLboolean nlSaveImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type)
{
    NLuchar *data = nlMakeOpenGLTextureDataRGBA(tex, NULL);
    int channel = tex->format != NL_RGB ? SOIL_LOAD_RGBA : SOIL_LOAD_RGBA;
    NLboolean res = SOIL_save_image(to, img_type, tex->width, tex->height, channel, data);
    free(data);
    return res;
}

NLboolean nlSaveTextureV3_Memory(const NLuchar *data, int _i, int width, int height, NETLizard_Texture_format format, const char *to)
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
	unsigned int *map = NULL;
	int len = 0;
	int over = 0;
	unsigned int i;
	for(i = 0; i < index_len; i++)
	{
		unsigned color;
		if(format != NL_RGB)
			color = 0xff000000 + (((unsigned int)data[i * 4]) << 16) + (((unsigned int)data[i * 4 + 1]) << 8) + ((unsigned int)data[i * 4 + 2]);
		//color = (((unsigned int)data[i * 4 + 3]) << 24) + (((unsigned int)data[i * 4]) << 16) + (((unsigned int)data[i * 4 + 1]) << 8) + ((unsigned int)data[i * 4 + 2]);
		else
			color = (((unsigned int)data[i * 3]) << 16) + (((unsigned int)data[i * 3 + 1]) << 8) + ((unsigned int)data[i * 3 + 2]);
		if(!map)
		{
			map = NEW_II(unsigned int, len + 1);
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
				unsigned int *tmp = NEW_II(unsigned int, len + 1);
				memcpy(tmp, map, sizeof(unsigned int) * len);
				tmp[len] = color;
				free(map);
				map = tmp;
				len++;
			}
		}
	}

	if(over)
		nlprintf("Color map'size is over 256. Need to compress.\n")
	else
		nlprintf("Color map'size is less 256. Don't need to compress.\n")

	free(map);
	len = 0;
	map = NULL;

	for(i = 0; i < index_len; i++)
	{
		unsigned int color;
		if(format != NL_RGB)
			color = 0xff000000 + (((unsigned int)data[i * 4]) << 16) + (((unsigned int)data[i * 4 + 1]) << 8) + ((unsigned int)data[i * 4 + 2]);
		//color = (((unsigned int)data[i * 4 + 3]) << 24) + (((unsigned int)data[i * 4]) << 16) + (((unsigned int)data[i * 4 + 1]) << 8) + ((unsigned int)data[i * 4 + 2]);
		else
			color = (((unsigned int)data[i * 3]) << 16) + (((unsigned int)data[i * 3 + 1]) << 8) + ((unsigned int)data[i * 3 + 2]);
		
		color = rgb888_to_rgb332(color);
		if(len == max_map_count)
		{
			index[i] = len - 1;
			continue;
		}
		if(!map)
		{
			map = NEW_II(unsigned int, len + 1);
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
				unsigned int *tmp = NEW_II(unsigned int, len + 1);
				memcpy(tmp, map, sizeof(unsigned int) * len);
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

	nlprintf("Make v3 PNG color map'size->%d\n", len);
	int j = 3; // 3
	int m4 = 0; // 4
	int k = format != NL_RGB ? 1 : 0; // 5
	char c;
	char *magic = "BIP";
	for(i = 0; i < j; i++)
		fwrite(magic + i, sizeof(char), 1, file);
	c = (char)k;
	fwrite(&c, sizeof(char), 1, file);
	c = (char)m4;
	fwrite(&c, sizeof(char), 1, file);
	c = (char)len;
	fwrite(&c, sizeof(char), 1, file);

	// 1 color
	i = len;
	if (k == 0)
	{
		int m = 0;
		while (m < i)
		{
			//arrayOfInt[m] = ((paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
			unsigned char r = (unsigned char)(map[m] << 8 >> 24);
			unsigned char g = (unsigned char)(map[m] << 16 >> 24);
			unsigned char b = (unsigned char)(map[m] << 24 >> 24);
			fwrite(&r, sizeof(unsigned char), 1, file);
			fwrite(&g, sizeof(unsigned char), 1, file);
			fwrite(&b, sizeof(unsigned char), 1, file);
			m++;
		}
	}
	else
	{
		int n;
		for (n = 0; n < i; n++) {
			unsigned char a = (unsigned char)(map[n] >> 24);
			if (a != 0)
			{
				//arrayOfInt[n] = (0xFF000000 | (paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
				unsigned char r = (unsigned char)(map[n] << 8 >> 24);
				unsigned char g = (unsigned char)(map[n] << 16 >> 24);
				unsigned char b = (unsigned char)(map[n] << 24 >> 24);
				fwrite(&a, sizeof(unsigned char), 1, file);
				fwrite(&r, sizeof(unsigned char), 1, file);
				fwrite(&g, sizeof(unsigned char), 1, file);
				fwrite(&b, sizeof(unsigned char), 1, file);
			}
			else
			{
				c = 0;
				fwrite(&c, sizeof(unsigned char), 1, file);
				fwrite(&c, sizeof(unsigned char), 1, file);
				fwrite(&c, sizeof(unsigned char), 1, file);
				fwrite(&c, sizeof(unsigned char), 1, file);
			}
		}
	}

	// 2 index
	unsigned int i1 = width;
	unsigned int i2 = height;
	nlprintf("Make v3 PNG->width: %d, height: %d\n", i1, i2);
	c = (unsigned char)(i1 << 16 >> 24);
	fwrite(&c, sizeof(unsigned char), 1, file);
	c = (unsigned char)(i1 << 24 >> 24);
	fwrite(&c, sizeof(unsigned char), 1, file);
	c = (unsigned char)(i2 << 16 >> 24);
	fwrite(&c, sizeof(unsigned char), 1, file);
	c = (unsigned char)(i2 << 24 >> 24);
	fwrite(&c, sizeof(unsigned char), 1, file);
	//int ii1 = (i1 << 16 >> 24 << 8) | (i1 << 24 >> 24);
	//int ii2 = (i2 << 16 >> 24 << 8) | (i2 << 24 >> 24);
	//printf("s2 %d, %d\n", ii1, ii2);
	int i3;
	c = 0;
	fwrite(&c, sizeof(unsigned char), 1, file);
	if (m4 == 0)
	{
		i = i1 * i2;
		for (i3 = 0; i3 < i; i3++) {
			c = (unsigned char)index[i3];
			fwrite(&c, sizeof(unsigned char), 1, file);
		}
	}
	else
	{
		c = 0;
		fwrite(&c, sizeof(unsigned char), 1, file);
		i = width * height;
		for (i3 = 0; i3 < i; i3++) {
			c = (unsigned char)index[i3];
			fwrite(&c, sizeof(unsigned char), 1, file);
		}
	}
	fflush(file);
	fclose(file);
	free(index);
	free(map);
    return NL_TRUE;
}

NLboolean nlSaveTextureV3_File(const char *from, int _i, const char *to)
{
	int channel;
	int width;
	int height;
	NETLizard_Texture_format format;
	unsigned char *data = SOIL_load_image(from, &width, &height, &channel, SOIL_LOAD_AUTO);
	nlprintf("Read image->width: %d, height: %d\n", width, height);
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
	int res = nlSaveTextureV3_Memory(data, _i, width, height, format, to);
	free(data);
	return res;
}

/* ******** static ******** */

array class_s__function_a_1byte_array__color_map(const byte paramArrayOfByte[], NETLizard_Texture_format *format)
{
	int j = 3;
	int k = 0;
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
	j++;
	int i;
	j++;
	if ((i = paramArrayOfByte[5]) < 0) {
		i += 256;
	}
	nlprintf("NETLizard 3D engine v3 texture color map'size->%d\n", i);
    array arr;
    new_array(&arr, sizeof(jint), i);
    int *arrayOfInt = (int *)(arr.array);
	if (k == 0)
	{
		int m = 0;
		//tmpTernaryOp = paramArrayOfByte;
		while (m < i)
		{
			arrayOfInt[m] = ((paramArrayOfByte[(++j)] & 0xFF) << 16 | (paramArrayOfByte[(++j)] & 0xFF) << 8 | paramArrayOfByte[(++j)] & 0xFF);
			m++;
		}
	}
	else
	{
		int n;
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

array class_s__function_a_1byte_array_2bool_3int_4int_5int_6int__color_index(const array *data, int paramBoolean, int paramInt1, int paramInt2, int paramInt3, int paramInt4, int *width, int *height)
{
	int j = 0;
	int k = 0;
	byte *paramArrayOfByte = (byte *)(data->array);
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
	int m = 0;
	if (paramArrayOfByte[4] == 1) {
		m = 1;
	}
	int n;
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
	int i1;
	int i2;
	int i3;
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
		int i;
		nlprintf("NETLizard 3D engine v3 texture->width: %d, height: %d\n", i1, i2);
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
		int i4;
		for (i4 = 0; i4 < paramInt4; i4++)
		{
			i2 = i4 * i3 + j;
			int i5;
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

void class_s__function_a_1byte_array_2int__swap(byte paramArrayOfByte[], int paramInt)
{
	int j;
	int k = (j = (1 << paramInt) - 1) >> 1;
	int m;
	int n;
	for (m = j; m >= 0; m--) {
		for (n = k; n >= 0; n--)
		{
			int i = paramArrayOfByte[(n * (j + 1) + m)];
			paramArrayOfByte[(n * (j + 1) + m)] = paramArrayOfByte[((j - n) * (j + 1) + m)];
			paramArrayOfByte[((j - n) * (j + 1) + m)] = i;
		}
	}
}

int class_s__function_a_1byte_array_2byte__get_width_height_depth(byte paramArrayOfByte[], byte paramByte)
{
	int j = 0;
	int k = 0;
	if (paramArrayOfByte[3] == 1) {
		k = 1;
	}
	if (paramByte == 2) {
		return paramArrayOfByte[4];
	}
	int i;
	if ((i = paramArrayOfByte[5]) < 0) {
		i += 256;
	}
	if (k != 0) {
		j = 5 + i * 4;
	} else {
		j = 5 + i * 3;
	}
	int m;
	int n;
	if (paramByte == 0)
	{
		if ((m = (short)paramArrayOfByte[(++j)]) < 0) {
			m = (short)(m + 256);
		}
		if ((n = (short)paramArrayOfByte[(++j)]) < 0) {
			n = (short)(n + 256);
		}
		i = m << 8 | n;
	}
	if (paramByte == 1)
	{
		j += 2;
		if ((m = (short)paramArrayOfByte[(++j)]) < 0) {
			m = (short)(m + 256);
		}
		if ((n = (short)paramArrayOfByte[(++j)]) < 0) {
			n = (short)(n + 256);
		}
		i = m << 8 | n;
	}
	return i;
}

NLboolean nlSaveCompressImage_V3File(const char *from, const char *to, int img_type)
{
    NETLizard_Texture tex;
    NLboolean res = nlReadCompressTextureV3_File(from, &tex);
    if(!res)
        return NL_FALSE;
    res = nlSaveCompressImage_V3Memory(&tex, to, img_type);
    delete_NETLizard_Texture(&tex);
    return res;
}

NLboolean nlSaveCompressImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type)
{
    NLuchar *data = nlMakeOpenGLTextureDataRGBACompress(tex, NULL);
    int channel = tex->format != NL_RGB ? SOIL_LOAD_RGBA : SOIL_LOAD_RGBA;
    NLboolean res = SOIL_save_image(to, img_type, tex->width, tex->height, channel, data);
    free(data);
    return res;
}

