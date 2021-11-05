#include "netlizard.h"

#include "priv_local.h"

typedef int (*NETLizard_Log_f)(int type, const char *str);

typedef struct _LogFunc_STD
{
    unsigned type;
    FILE *file;
} LogFunc_STD;

typedef struct _LogFunc_User
{
    unsigned type;
    NETLizard_Log_f callback;
} LogFunc_User;

typedef union _LogState
{
    unsigned type;
    LogFunc_STD std;
    LogFunc_User user;
} LogState;

static LogState _out;
static LogState _err;
static NLboolean _log = NL_FALSE;

int nlvasprintf(char **ptr, const char *fmt, va_list ap)
{
    int length = vsnprintf(NULL, 0, fmt, ap);
    *ptr = NULL;
    if(length < 0)
        return length;
    char *str = calloc(length + 1, sizeof(char));
    va_list ap_copy;
    va_copy(ap_copy, ap);
    length = vsnprintf(str, length + 1, fmt, ap_copy);
    va_end(ap_copy);
    if(length < 0)
        free(str);
    else
        *ptr = str;
    return length;
}

int nlasprintf(char **ptr, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int length = nlvasprintf(ptr, fmt, ap);
    va_end(ap);
    return length;
}

void nlLogFunc(NLenum type, NLenum way, void *f)
{
    LogState *state;

    if(way != NL_LOG_STD && way != NL_LOG_USER)
    {
        E_INVALID_ENUM;
        return;
    }
    switch(type)
    {
        case NL_LOG_OUT:
            state = &_out;
            break;
        case NL_LOG_ERR:
            state = &_err;
            break;
        default:
            E_INVALID_ENUM;
            return;
    }
    state->type = way;
    if(way == NL_LOG_USER)
        state->user.callback = f;
    else
        state->std.file = f;
}

unsigned enable_log(unsigned b)
{
    if(_log != b)
    {
        _log = b;
        return NL_TRUE;
    }
    return NL_FALSE;
}

unsigned log_enabled(void)
{
    return _log;
}

int nlflogfln(int type, const char *fmt, ...)
{
    if(!_log)
        return -1;
    int res = 0;
    const LogState *state = type == NL_LOG_ERR ? &_err : &_out;
    if(state->type == NL_LOG_USER)
    {
        if(state->user.callback)
        {
            va_list ap;
            va_start(ap, fmt);
            char *str = NULL;
            res = nlvasprintf(&str, fmt, ap);
            va_end(ap);
            if(res >= 0)
            {
                char *nstr = calloc(res + 1 + 1, sizeof(char));
                strncpy(nstr, str, res);
                nstr[res] = '\n';
                state->user.callback(type, nstr);
                free(str);
                free(nstr);
                res++;
            }
        }
    }
    else
    {
        if(state->std.file)
        {
            va_list ap;
            va_start(ap, fmt);
            res = vfprintf(state->std.file, fmt, ap);
            va_end(ap);
            fprintf(state->std.file, "\n");
            fflush(state->std.file);
            res++;
        }
    }
    return res;
}

int nlflogf(int type, const char *fmt, ...)
{
    if(!_log)
        return -1;
    int res = 0;
    const LogState *state = type == NL_LOG_ERR ? &_err : &_out;
    if(state->type == NL_LOG_USER)
    {
        if(state->user.callback)
        {
            va_list ap;
            va_start(ap, fmt);
            char *str = NULL;
            res = nlvasprintf(&str, fmt, ap);
            va_end(ap);
            if(res >= 0)
            {
                state->user.callback(type, str);
                free(str);
            }
        }
    }
    else
    {
        if(state->std.file)
        {
            va_list ap;
            va_start(ap, fmt);
            res = vfprintf(state->std.file, fmt, ap);
            va_end(ap);
            fflush(state->std.file);
        }
    }
    return res;
}

int nllogfln(const char *fmt, ...)
{
    if(!_log)
        return -1;
    int type = NL_LOG_OUT;
    int res = 0;
    const LogState *state = type == NL_LOG_ERR ? &_err : &_out;
    if(state->type == NL_LOG_USER)
    {
        if(state->user.callback)
        {
            va_list ap;
            va_start(ap, fmt);
            char *str = NULL;
            res = nlvasprintf(&str, fmt, ap);
            va_end(ap);
            if(res >= 0)
            {
                char *nstr = calloc(res + 1 + 1, sizeof(char));
                strncpy(nstr, str, res);
                nstr[res] = '\n';
                state->user.callback(type, nstr);
                free(str);
                free(nstr);
                res++;
            }
        }
    }
    else
    {
        if(state->std.file)
        {
            va_list ap;
            va_start(ap, fmt);
            res = vfprintf(state->std.file, fmt, ap);
            va_end(ap);
            fprintf(state->std.file, "\n");
            res++;
            fflush(state->std.file);
        }
    }
    return res;
}

int nllogf(const char *fmt, ...)
{
    if(!_log)
        return -1;
    int type = NL_LOG_OUT;
    int res = 0;
    const LogState *state = type == NL_LOG_ERR ? &_err : &_out;
    if(state->type == NL_LOG_USER)
    {
        if(state->user.callback)
        {
            va_list ap;
            va_start(ap, fmt);
            char *str = NULL;
            res = nlvasprintf(&str, fmt, ap);
            va_end(ap);
            if(res >= 0)
            {
                state->user.callback(type, str);
                free(str);
            }
        }
    }
    else
    {
        if(state->std.file)
        {
            va_list ap;
            va_start(ap, fmt);
            res = vfprintf(state->std.file, fmt, ap);
            va_end(ap);
            fflush(state->std.file);
        }
    }
    return res;
}


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
