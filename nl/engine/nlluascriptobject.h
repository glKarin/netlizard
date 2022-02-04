#ifndef _KARIN_NLLUASCRIPTTOBJECT_H
#define _KARIN_NLLUASCRIPTTOBJECT_H

#include "nlscriptobject.h"

struct lua_State;

class NLLIB_EXPORT NLLuaScriptObject : public NLScriptObject
{
    Q_OBJECT
public:
    explicit NLLuaScriptObject(const QByteArray &data, NLScript *parent);
    virtual ~NLLuaScriptObject();
    struct lua_State * LuaState() { return L; }

protected:
    virtual bool InitScript();
    virtual bool DestroyScript();
    virtual bool UpdateScript(float delta);
    virtual bool ResetScript();
    virtual NLVariantSequenceHash GetGlobalVariant();
    virtual void SetGlobalVariant(const NLVariantSequenceHash &vars);
    virtual void RegisterInitialGlobalVariant(float delta);

private:
    struct lua_State *L;
};

#endif // _KARIN_NLLUASCRIPTTOBJECT_H
