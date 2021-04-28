#include "netlizard.h"

#include "texture.h"
#include "priv_local.h"

NLuchar * nlMakeOpenGLTextureDataRGBACompress(const NETLizard_Texture *tex, NLsizei *rlen)
{
    int *tex_color = (int *)(tex->color_map.data);
    signed char *tex_index = (signed char *)/*(byte *)*/(tex->color_index.data);
    int length = tex->width * tex->height;
	int *data_p = NEW_II(int, length);
    int n = tex->color_map.length;

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
	/*
	for (i1 = 0; i1 < tex->height; i1++)
	{
		printf("===%d===\n", i1);
		for (n = 0; n < tex->width; n++)
		{
			printf("%d-%d  ", n, data_p[i1 * tex->width + n]);
		}
		printf("\n");
	}
		printf("===%d===\n", tex->height);
		printf("===%d===\n", tex->width);
		*/
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

	return data;
}

NLuchar * nlMakeOpenGLTextureDataRGB(const NETLizard_Texture *tex, NLsizei *rlen)
{
    int len = tex->width * tex->height * 3;
    NLuchar *data = NEW_II(NLuchar, len);
    int length = tex->width * tex->height;

	int i;
	for(i = 0; i < length; i++)
	{
        byte index = ((byte *)(tex->color_index.data))[i];
        unsigned int color = ((unsigned int *)(tex->color_map.data))[(int)index];
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

NLuchar * nlMakeOpenGLTextureDataRGBA(const NETLizard_Texture *tex, NLsizei *rlen)
{
#if 1
    return nlMakeOpenGLTextureDataRGBACompress(tex, rlen);
#else
    int len = tex->width * tex->height * 4;
    unsigned char *data = NEW_II(char, int);
    int length = tex->width * tex->height;
	int i;
	for(i = 0; i < length; i++)
	{
        byte index = ((byte *)(tex->color_index.data))[i];
        unsigned int color = ((unsigned int *)(tex->color_map.data))[(int)index];
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

NLuchar * nlMakeOpenGLTextureData(const NETLizard_Texture *tex, NLsizei *rlen)
{
    return tex->format == NL_RGB ? nlMakeOpenGLTextureDataRGB(tex, rlen) : nlMakeOpenGLTextureDataRGBA(tex, rlen);
}

void delete_NETLizard_Texture(NETLizard_Texture *tex)
{
    if(tex->color_index.data)
    {
        free(tex->color_index.data);
	}
    tex->color_index.length = 0;
    if(tex->color_map.data)
    {
        free(tex->color_map.data);
	}
    tex->color_map.length = 0;
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

