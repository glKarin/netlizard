#include "error.h"

#include "priv_local.h"

static int _nl_errno = 0;

int set_errno(int err)
{
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
