#include "netlizard.h"

#include "priv_local.h"

NLuchar * nlMakePixelDataRGBACompress(const NETLizard_Texture *tex, NLint *rlen)
{
    int32_t *tex_color = NEW_II(int32_t, tex->color_map.count);
    memcpy(tex_color, tex->color_map.data, sizeof(int32_t) * tex->color_map.count);
    const int8_t *tex_index = (const int8_t *)/*(byte *)*/(tex->color_index.data); // signed char
    const int length = tex->width * tex->height;
    int32_t *data_p = NEW_II(int32_t, length);
    int n = tex->color_map.count;

	int m = 255;
	int i1;
	for (i1 = 0; i1 < n; i1++)
	{
		int i2 = tex_color[i1];
		tex_color[i1] = ((i2 & 0xFF000000) + (((i2 & 0xFF0000) >> 16) * m >> 8 << 16) + (((i2 & 0xFF00) >> 8) * m >> 8 << 8) + ((i2 & 0xFF) * m >> 8));
	}
	int i = tex->width;
	int k = tex->width;
	int j = tex->height;
	int i3 = 0;
	int i7 = 0;
	int i8 = 0;
	int i9 = 0;
	for (i1 = 0; i1 < j; i1++)
	{
		for (n = 0; n < i; n++)
		{
			if (i8 == 0)
			{
				if ((i9 = tex_index[i7]) < 0)
				{
					i8 = -i9;
					i7++;
				}
				i9 = tex_color[tex_index[i7]];
				i7++;
				if ((i9 == 0) && (i8 > 0))
				{
					if (n + i8 < k)
					{
						i3 += i8 + 1;
						n += i8;
						i8 = 0;
						continue;
					}
					i8 = i8 - (k - n) + 1;
					i3 += k - n;
					break;
				}
			}
			else
			{
				if ((i9 == 0) && (i8 > 0) && (n + i8 <= k))
				{
					i3 += i8;
					n += i8 - 1;
					i8 = 0;
					continue;
				}
				i8--;
			}
			if (i9 != 0) {
				//printf(" %d ", i9);
				data_p[i3] = i9;
			}
			i3++;
		}
    //i3 += 0;//i6;
    }

    NLsizei len = tex->width * tex->height * 4;
    if(rlen)
        *rlen = len;
    NLuchar *data = NEW_II(NLuchar, len);
	int ii;
	for(ii = 0; ii < length; ii++)
	{
		int color = data_p[ii];
		data[ii * 4] = (byte)((color & 0x00ff0000) >> 16);
		data[ii * 4 + 1] = (byte)((color & 0x0000ff00) >> 8);
		data[ii * 4 + 2] = (byte)(color & 0x000000ff);
		if(tex->format != NL_RGB)
			data[ii * 4 + 3] = (byte)((color & 0xff000000) >> 24);
		else
			data[ii * 4 + 3] = (byte)(0xff);
	}
	free(data_p);
    free(tex_color);

	return data;
}

NLuchar * nlMakePixelDataRGB(const NETLizard_Texture *tex, NLint *rlen)
{
    int len = tex->width * tex->height * 3;
    NLuchar *data = NEW_II(NLuchar, len);
    const int length = tex->width * tex->height;

	int i;
	for(i = 0; i < length; i++)
	{
        byte index = ((byte *)(tex->color_index.data))[i];
        uint32_t color = ((uint32_t *)(tex->color_map.data))[(int)index];
		/*
		data[i * 3] = (byte)(color << 8 >> 24);
		data[i * 3 + 1] = (byte)(color << 16 >> 24);
		data[i * 3 + 2] = (byte)(color << 24 >> 24);
		*/
		data[i * 3] = (byte)((color & 0x00ff0000) >> 16);
		data[i * 3 + 1] = (byte)((color & 0x0000ff00) >> 8);
		data[i * 3 + 2] = (byte)(color & 0x000000ff);
		//printf("%d: (%d * %d = %d) %x->%d %d %d\n", i, tex->width, tex->height, tex->width * tex->height, color, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
	}

    if(rlen)
        *rlen = len;

	return data;
}

NLuchar * nlMakePixelDataRGBA(const NETLizard_Texture *tex, NLint *rlen)
{
#if 1
    return nlMakePixelDataRGBACompress(tex, rlen);
#else
    int len = tex->width * tex->height * 4;
    uint8_t *data = NEW_II(uint8_t, int); // unsigned char
    int length = tex->width * tex->height;
	int i;
	for(i = 0; i < length; i++)
	{
        byte index = ((byte *)(tex->color_index.data))[i];
        uint32_t color = ((uint32_t *)(tex->color_map.data))[(int)index];
		data[i * 4] = (byte)((color & 0x00ff0000) >> 16);
		data[i * 4 + 1] = (byte)((color & 0x0000ff00) >> 8);
		data[i * 4 + 2] = (byte)(color & 0x000000ff);
		/*
		data[i * 4] = (byte)(color << 8 >> 24);
		data[i * 4 + 1] = (byte)(color << 16 >> 24);
		data[i * 4 + 2] = (byte)(color << 24 >> 24);
		*/
		if(tex->format != NL_RGB)
			data[i * 4 + 3] = (byte)((color & 0xff000000) >> 24);
			//data[i * 4 + 3] = (byte)(color >> 24 );
		else
			data[i * 4 + 3] = (byte)(0xff);
    }
    if(rlen)
        *rlen = len;
	return data;
#endif
}

NLuchar * nlMakePixelData(const NETLizard_Texture *tex, NLint *rlen)
{
    return tex->format == NL_RGB ? nlMakePixelDataRGB(tex, rlen) : nlMakePixelDataRGBA(tex, rlen);
}

void nlDeleteNETLizardTexture(NETLizard_Texture *tex)
{
    if(tex->color_index.data)
    {
        free(tex->color_index.data);
	}
    tex->color_index.count = 0;
    if(tex->color_map.data)
    {
        free(tex->color_map.data);
	}
    tex->color_map.count = 0;
}

/*
int nlCompareColor(unsigned int a, unsigned int b)
{
	static const unsigned char pr = 255 / 7;
	static const unsigned char pg = 255 / 7;
	static const unsigned char pb = 255 / 3;
	unsigned char ar = (unsigned char)(a << 8 >> 24);
	unsigned char ag = (unsigned char)(a << 16 >> 24);
	unsigned char ab = (unsigned char)(a << 24 >> 24);
	unsigned char br = (unsigned char)(b << 8 >> 24);
	unsigned char bg = (unsigned char)(b << 16 >> 24);
	unsigned char bb = (unsigned char)(b << 24 >> 24);
	return ((ar / pr == br / pr) && (ag / pg == bg / pg) && (ab / pb == bb / pb)) ? 1 : 0;
}
*/

