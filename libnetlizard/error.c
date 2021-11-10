#include "netlizard.h"

#include "priv_local.h"

#define ERROR_STR_MAX_LENGTH 256

static int _nl_errno = 0;
static char _nl_error[ERROR_STR_MAX_LENGTH];

int set_errno(int err)
{
    memset(_nl_error, 0, sizeof(char) * ERROR_STR_MAX_LENGTH);
    if(_nl_errno != err)
    {
        int e = _nl_errno;
        _nl_errno = err;
        return e;
    }
    else
        return _nl_errno;
}

int clear_errno(void)
{
    return set_errno(0);
}

int current_errno(void)
{
    return _nl_errno;
}

const char * current_error(void)
{
    return _nl_error;
}

int set_errno_string(int err, const char *str, ...)
{
    int r = set_errno(err);
    if(str)
    {
        va_list ap;
        va_start(ap, str);
        vsnprintf(_nl_error, ERROR_STR_MAX_LENGTH - 1, str, ap);
        va_end(ap);
    }
    return r;
}

NLenum nlGetError(void)
{
    return clear_errno();
}

const char * nlErrorString(NLenum error)
{
    static const char *_Error_Strings[] = {
        "No error",
        "Invalid value",
        "Invalid enum",
        "Invalid operation",
        "stack overflow",
        "Stack underflow",
        "Out of memory",
    };
    if(error != NL_NO_ERROR && (error < NL_INVALID_VALUE || error > NL_OUT_OF_MEMORY))
    {
        ES_INVALID_ENUM("nlIsEnabled(nlErrorString e=<0x%x>)", error);
        return NULL;
    }
    const int index = error == NL_NO_ERROR ? error : error - 0x0500;
    return _Error_Strings[index];
}

