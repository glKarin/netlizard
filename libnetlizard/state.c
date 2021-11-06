#include "netlizard.h"

#include "priv_local.h"

#define SET_IF_DIFF(target, param) (((target) != (param)) ? (((target) = (param)) || 1) : 0)

static NETLizard_State _state = {0};

static int set_enable(NLenum e, NLboolean b)
{
    int res = -1;
    switch(e)
    {
    case NL_LOG:
        res = SET_IF_DIFF(_state.log_enabled, b);
        break;
    default:
        E_INVALID_ENUM;
        break;
    }
    return res;
}

void nlEnable(NLenum e)
{
    set_enable(e, NL_TRUE);
}

void nlDisable(NLenum e)
{
    set_enable(e, NL_FALSE);
}

const NETLizard_State * current_state(void)
{
    return &_state;
}

NLboolean nlIsEnabled(NLenum e)
{
    NLboolean res = NL_FALSE;
    switch(e)
    {
    case NL_LOG:
        res = _state.log_enabled;
        break;
    default:
        E_INVALID_ENUM;
        break;
    }
    return res;
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
        E_INVALID_ENUM;
        return NULL;
    }
    const int index = error == NL_NO_ERROR ? error : error - 0x0500;
    return _Error_Strings[index];
}
