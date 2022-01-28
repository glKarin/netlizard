#ifndef _KARIN_ENGINEREGISTEROBJECT_H
#define _KARIN_ENGINEREGISTEROBJECT_H

#include "engine/nlglobals.h"

struct EngineRegisterObject : public NLEngineRegisterObject
{
    virtual const char * Name() const { return "NETLizard"; }
    virtual void RegisterMetaType();
    virtual void UnregisterMetaType() {}
};

#endif // _KARIN_ENGINEREGISTEROBJECT_H
