#include "log.h"

#include <stdio.h>
#include <stdarg.h>

#define LOG_FILE "E:\\qobject\\log.txt"

static FILE *file;

int log_append(const char *str, ...)
{
    int res;
    int is_open;

    is_open = file != NULL;
    res = 0;
    if(!(!is_open && log_begin(1)))
        return -1;
    va_list ap;
    va_start(ap, str);
    res = vfprintf(file, str, ap);
    va_end(ap);
    fprintf(file, "\n");
    res++;
    if(is_open)
        log_flush();
    else
        log_end();
    return res;
}

int log_wappend(const wchar_t *str, ...)
{
    int res;
    int is_open;

    is_open = file != NULL;
    res = 0;
    if(!(!is_open && log_begin(1)))
        return -1;
    va_list ap;
    va_start(ap, str);
    res = vfwprintf(file, str, ap);
    va_end(ap);
    fwprintf(file, L"\n");
    res++;
    if(is_open)
        log_flush();
    else
        log_end();
    return res;
}

int log_begin(int append)
{
    if(!file)
    {
        file = fopen(LOG_FILE, append ? "a+" : "w");
    }
    return file != NULL;
}

int log_write(const char *str, ...)
{
    va_list ap;
    va_start(ap, str);
    int res = vfprintf(file, str, ap);
    va_end(ap);
    return res;
}

void log_flush(int append)
{
    if(!file)
        return;
    fflush(file);
}

void log_end()
{
    if(!file)
        return;
    FILE *f = file;
    file = NULL;
    fflush(f);
    fclose(f);
}
