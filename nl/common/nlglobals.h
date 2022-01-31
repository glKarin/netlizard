#ifndef _KARIN_NLGLOBALS_H
#define _KARIN_NLGLOBALS_H

#include <QtGlobal>

#include "common/nlinc.h"
#include "common/nldef.h"

class QTranslator;

struct NLLIB_EXPORT NLEngineRegisterObject
{
    typedef QHash<QString, NLVariantCompare_f> VariantCompareFuncMap;
    virtual const char * Name() const = 0;
    virtual void RegisterMetaType() = 0;
    virtual void UnregisterMetaType() = 0;
    virtual VariantCompareFuncMap RegisterVariantCompareFunc() = 0;
    virtual VariantCompareFuncMap UnregisterVariantCompareFunc() = 0;
};

class NLLIB_EXPORT NLEngineGlobals
{
public:
    ~NLEngineGlobals();
    bool init_engine(NLEngineRegisterObject *obj = 0);
    void deinit_engine();
    void register_engine(NLEngineRegisterObject *obj);
    bool unregister_engine(const char *name);
    bool engine_is_register(const char *name) const { return(_engine_register_names.contains(name)); }
    bool engine_is_inited() const { return _engine_inited; }
    static NLEngineGlobals * Instance();
    bool variant_compare(const QString &type, const QVariant &a, const QVariant &b) const;
    NLVariantCompare_f variant_compare_func(const QString &type) const;

private:
    bool load_translator();
    void unload_translator();

private:
    bool _engine_inited;
    QHash<QString, NLEngineRegisterObject *> _engine_register_names;
    NLEngineRegisterObject::VariantCompareFuncMap _engine_variant_compare_funcs;
    QTranslator *_engine_translator;

private:
    NLEngineGlobals();
    Q_DISABLE_COPY(NLEngineGlobals)
};

#endif // _KARIN_NLGLOBALS_H
