#ifndef _KARIN_ENGINEREGISTEROBJECT_H
#define _KARIN_ENGINEREGISTEROBJECT_H

#include "common/nlglobals.h"

struct EngineRegisterObject : public NLEngineRegisterObject
{
    virtual const char * Name() const { return "NETLizard"; }
    virtual void RegisterMetaType();
    virtual void UnregisterMetaType() {}
    virtual VariantCompareFuncMap RegisterVariantCompareFunc() { return VariantCompareFuncMap(); }
    virtual VariantCompareFuncMap UnregisterVariantCompareFunc() { return VariantCompareFuncMap(); }
#ifdef _DEV_TEST
    virtual void RegisterLuaFunc(struct lua_State *L);
    virtual QVariant LuaVariantToQVariant(void **ptr, const QString &metatableName);
    virtual int PushQVariantToLua(const QVariant &v, struct lua_State *L, const QString &specialType = QString());
#else
    virtual void RegisterLuaFunc(struct lua_State *L) { Q_UNUSED(L); }
    virtual QVariant LuaVariantToQVariant(void **ptr, const QString &metatableName) { Q_UNUSED(ptr); Q_UNUSED(metatableName); return QVariant(); }
    virtual int PushQVariantToLua(const QVariant &v, struct lua_State *L, const QString &specialType = QString()) { Q_UNUSED(v); Q_UNUSED(L); Q_UNUSED(specialType); return 0; }
#endif
};

#ifdef _DEV_TEST
Q_DECLARE_METATYPE(EngineRegisterObject*)
#endif

#endif // _KARIN_ENGINEREGISTEROBJECT_H
