#include "netlizard.h"

#include "priv_local.h"

#define GET_BY_TYPE(type) (type == NL_LOG_ERR ? &_log.err : &_log.out)
#define LOG_ENABLED() (nlIsEnabled(NL_LOG))

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

typedef struct _LogMachine
{
    LogState out;
    LogState err;
} LogMachine;

static LogMachine _log;

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
            state = &_log.out;
            break;
        case NL_LOG_ERR:
            state = &_log.err;
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

int nlflogfln(int type, const char *fmt, ...)
{
    if(!LOG_ENABLED())
        return -1;
    int res = 0;
    const LogState *state = GET_BY_TYPE(type);
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
    if(!LOG_ENABLED())
        return -1;
    int res = 0;
    const LogState *state = GET_BY_TYPE(type);
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
    if(!LOG_ENABLED())
        return -1;
    int type = NL_LOG_OUT;
    int res = 0;
    const LogState *state = GET_BY_TYPE(type);
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
    if(!LOG_ENABLED())
        return -1;
    int type = NL_LOG_OUT;
    int res = 0;
    const LogState *state = GET_BY_TYPE(type);
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
