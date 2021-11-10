#include "netlizard.h"

#include "priv_local.h"

static char * append_char(char *str, NLsizei *len, char ch)
{
    char *tmp;

    tmp = NEW_II(char, *len + 1 + 1);
    if(len > 0)
        memcpy(tmp, str, *len);
    tmp[*len] = ch;
    (*len)++;
    //tmp[*len] = '\0';
    free(str);

    return tmp;
}

char * nlDecodeStringi(const NLint *arr, NLsizei length)
{
    NLsizei len = 0;
    char *str;
    int i2;

    str = NEW(char);
    ZERO(str, char);
    for(i2 = 0; i2 < length; i2++)
    {
        jlong l1; // long
        char ch;

        if ((l1 = arr[i2]) < 0L) {
            l1 += 4294967296LL;
        }
        ch = (char)(jint)((l1 & 0xFF000000) >> 24);
        str = append_char(str, &len, ch);

        int i1;
        if ((i1 = (char)(jint)((l1 & 0xFF0000) >> 16)) == 0) {
            break;
        }
        ch = (char)i1;
        str = append_char(str, &len, ch);

        if ((i1 = (char)(jint)((l1 & 0xFF00) >> 8)) == 0) {
            break;
        }
        ch = (char)i1;
        str = append_char(str, &len, ch);

        if ((i1 = (char)(jint)(l1 & 0xFF)) == 0) {
            break;
        }
        ch = (char)i1;
        str = append_char(str, &len, ch);

        //printf("%s\n", str);
    }
    return str;
}

char * nlDecodeStringCi(const NLint *arr, NLsizei length)
{
    NLsizei len = 0;
    char *str = NEW(char);
    ZERO(str, char);
    int i2;
    for(i2 = 0; i2 < length; i2++)
    {
        const int *iptr = arr + i2;
        int tmp = *iptr;
        long l1 = tmp;
        if (l1 < 0L) {
            tmp += 4294967296LL;
        }
        char *ptr = (char *)&tmp;
        int i;
        for(i = 0; i < 4; i++)
        {
            char ch = ptr[4 - i - 1];
            if((int)ch == 0)
                break;

            str = append_char(str, &len, ch);
        }
    }
    //printf("%s\n", str);
    return str;
}

char * nlDecodeString(const char *arr, NLsizei length)
{
    return nlDecodeStringi((const NLint *)arr, length / 4);
}

char * nlDecodeStringC(const char *arr, NLsizei length)
{
    return nlDecodeStringCi((const NLint *)arr, length / 4);
}

NLint * nlEncodeStringC(const char *str, NLint *rlen)
{
    const char *ptr = str;
    int len = 0;
    int *data = NULL;
    while(*ptr)
    {
        int j;
        char arr[4] = {0, 0, 0, 0};
        for(j = 0; j < 4; j++)
        {
            if(*ptr)
                arr[4 - j - 1] = *ptr;
            else
                break;
            ptr ++;
        }
        if(!data)
        {
            data = NEW(int32_t);
            ZERO(data, int32_t);
            int32_t *iptr = (int32_t *)arr;
            if(*iptr < 0)
                *iptr += 4294967296LL;
            *data = *iptr;
        }
        else
        {
            int *tmp = NEW_II(int32_t, len + 1);
            memcpy(tmp, data, sizeof(int32_t) * len);
            int32_t *iptr = (int32_t *)arr;
            if(*iptr < 0)
                *iptr += 4294967296LL;
            tmp[len] = *iptr;
            free(data);
            data = tmp;
        }
        len++;
    }
    *rlen = len;
    return data;
}
