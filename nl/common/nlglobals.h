#ifndef _KARIN_NLGLOBALS_H
#define _KARIN_NLGLOBALS_H

#include <QtGlobal>

#include "common/nlinc.h"

struct NLLIB_EXPORT NLEngineRegisterObject
{
    virtual const char * Name() const = 0;
    virtual void RegisterMetaType() = 0;
    virtual void UnregisterMetaType() = 0;
};

namespace NL
{
NLLIB_EXPORT bool init_engine(NLEngineRegisterObject *obj = 0);
NLLIB_EXPORT void deinit_engine();
NLLIB_EXPORT void register_engine(NLEngineRegisterObject *obj);
NLLIB_EXPORT bool unregister_engine(const char *name);
NLLIB_EXPORT bool engine_is_register(const char *name);
NLLIB_EXPORT bool engine_is_inited();
}

#endif // _KARIN_NLGLOBALS_H
