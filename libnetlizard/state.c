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
        ES_INVALID_ENUM("%s(NLenum e=<0x%x>)", b ? "nlEnable" : "nlDisable", e);
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
        ES_INVALID_ENUM("nlIsEnabled(NLenum e=<0x%x>)", e);
        break;
    }
    return res;
}
