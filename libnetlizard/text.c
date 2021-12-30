#include "netlizard.h"

#include "priv_local.h"
#include <locale.h>

// clone 3d
static wstrings class_p__function_a_1string_2char__text_w(const byte_array *arr, wchar_t paramChar); // wchar_t
static strings class_p__function_a_1string_2char__text(const array *arr, wchar_t paramChar); // char

char * nlReadAndHandleTextFile(const char *name, NLint *rlen)
{
    array arr;
    jlong len;
    char *data;

    len = file_get_contents(name, &arr);
    if(len <= 0)
		return NULL;

    data = nlLoadAndHandleTextData(arr.array, array_size(&arr), rlen);
    delete_array(&arr);

	return data;
}

char * nlLoadAndHandleTextData(const char *data, NLsizei len, NLint *rlen)
{
	//return nlEncodeDecodeData(arr);
    array arr;
    make_array(&arr, 1, len, (char *)data);

    strings res = class_p__function_a_1string_2char__text(&arr, L' ');
    *rlen = array_size(&res);

    return (char *)res.array;
}

NLboolean nlConvertAndHandleTextFile(const char *from, const char *to)
{
    char *data;
    NLint rlen;
    NLboolean res;

    data = nlReadAndHandleTextFile(from, &rlen);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, rlen, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);

    return res;
}

NLboolean nlSaveAndHandleTextData(const char *in_data, NLsizei len, const char *to)
{
    NLint rlen;
    char *data;
    NLboolean res;

    data = nlLoadAndHandleTextData(in_data, len, &rlen);
    if(!data)
        return NL_FALSE;

    array arr;
    make_array(&arr, 1, rlen, data);
    res = file_put_contents(to, &arr) > 0;

    free(data);
    return res;
}

// clone 3d
wstrings class_p__function_a_1string_2char__text_w(const byte_array *arr, wchar_t paramChar)
{
    jint length = array_size(arr);
    int8_t *o = (int8_t *)(arr->array); // signed char
    byte_array data;
    ARRAY_NULL(data);
    int8_t *paramString = NULL;
	if ((length > 1) && (o[0] == 59) && (o[1] == 67)) {
        new_array(&data, arr->size, arr->length);
        nlEncodeDecodeData(arr->array, data.array, length);
        length = array_size(&data);
        paramString = (int8_t *)(data.array);
	}
    else if ((length > 1) && (o[0] == -101) && (o[1] == -101)) {
        new_array(&data, arr->size, arr->length);
        nlEncodeDecodeData(arr->array, data.array, length);
        length = array_size(&data);
        paramString = (int8_t *)(data.array);
	}
	else
	{
        length = array_size(arr);
        paramString = (int8_t *)(arr->array);
	}
	wchar_t c1 = '\000';
	if ((length > 0) && (paramString[0] == 95)) {
		c1 = '\002';
	}
	/*
	if ((paramString.length >= 2) && (paramString[0] == -1) && (paramString[1] == -2)) {
		return a(paramString);
	}*/
	wchar_t c2 = c1;
    jint k;
	paramChar = length;
    jint j = (k = length - c1) + c1;
    wchar_t c3;
    for (c3 = c1; c3 < (unsigned int)j; c3++) {
		if (paramString[c3] == 0)
		{
			k = c3 - c1;
			paramChar = c3;
			break;
		}
    }
    wchar_t *arrayOfChar = NEW_II(wchar_t, k + 1);
    jint m;
	for (m = 0; m < k; m++)
	{
		if ((j = paramString[(m + c1)]) < 0) {
			j += 256;
        }
		arrayOfChar[m] = ((wchar_t)j);
    }
	while (c2 < paramChar)
	{
		if ((j = paramString[c2]) < 0) {
			j += 256;
		}
		if (j == 0) {
			break;
		}
		if (j == 32) {
			arrayOfChar[(c2 - c1)] = ' ';
		}
		if (j < 10) {
			arrayOfChar[(c2 - c1)] = ' ';
		}
		if ((j >= 192) && (j <= 255))
		{
			j += 848;
			arrayOfChar[(c2 - c1)] = ((wchar_t)j);
        }
		c2++;
    }
    arrayOfChar[k] = L'\0';
    //log_wappend(L"%ls", arrayOfChar);

    wstrings res;
    make_array(&res, sizeof(wchar_t), k + 1, arrayOfChar);

    delete_array(&data);

    return res;
}

strings class_p__function_a_1string_2char__text(const byte_array *arr, wchar_t paramChar)
{
    wstrings wstr = class_p__function_a_1string_2char__text_w(arr, paramChar);

    int k = wstr.length;
    wchar_t *arrayOfChar = (wchar_t *)wstr.array;
    size_t s = sizeof(wchar_t) / sizeof(char);
    const char *ori_locale = setlocale(LC_CTYPE, NULL);
    const char *locale = setlocale(LC_CTYPE, "ru");
    if(!locale)
    {
        nlflogfln(NL_LOG_ERR, "Not support ru lang.");
        locale = setlocale(LC_CTYPE, ""); // using localhost
    }
    char *str = NEW_II(char, k * s);

    int l = -1;
	char *p = str;
	int i = 0;
	int ii = 0;
    wchar_t *wp = arrayOfChar;
    while(i < k)
    {
        l = wctomb(p, *wp);
        if(l > 0)
        {
            p += l;
            ii += l;
        }
        wp++;
        i++;
    }

    array res;

    new_array_with_data(&res, 1, ii + 1, str, ii);
    ((char *)(res.array))[ii] = '\0';
    setlocale(LC_CTYPE, ori_locale);
    delete_array(&wstr);
    free(str);
    //nllogfln("%s", (char *)res.array);

	return res;
}

