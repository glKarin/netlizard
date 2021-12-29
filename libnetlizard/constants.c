#include "netlizard.h"

#include "priv_local.h"

#define VERSION "1.0.0.1"
#define VENDOR "Karin"

static const char _Version[] = VERSION;
static const char _Vendor[] = VENDOR;

const char * nlGetString(NLenum name)
{
    const char *res = NULL;
    switch(name)
    {
    case NL_VERSION:
        res = _Version;
        break;
    case NL_VENDOR:
        res = _Vendor;
        break;

    case NL_ERROR:
        res = current_error();

    default:
        ES_INVALID_ENUM("nlGetString(NLenum name=<0x%x>)", name);
        break;
    }
    return res;
}
