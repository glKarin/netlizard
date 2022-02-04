#include "nlglobals.h"

#include <QVariant>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QDebug>

#include "common/nlvec.h"
#include "common/nlmatrix.h"
#include "template/nlvariantpointer.h"
#include "engine/nlphysics.h"

class NLRenderable;
class NLComponent;
class NLActor;
class NLScript;
class NLScene;
class NLSceneCamera;
class NLForce;
class NLObject;
class NLRigidbody;
class NLScriptObject;

static bool vector3_variant_compare_func(const QVariant &a, const QVariant &b)
{
    NLVector3 av = a.value<NLVector3>();
    NLVector3 bv = b.value<NLVector3>();
    return NL::NLVector3_equals(av, bv);
}

static bool general_pointer_variant_compare_func(const QVariant &a, const QVariant &b)
{
    NLVariantGeneralPointer av = a.value<NLVariantGeneralPointer>();
    NLVariantGeneralPointer bv = b.value<NLVariantGeneralPointer>();
    return av == bv;
}

NLEngineGlobals::NLEngineGlobals()
    : _engine_inited(false),
      _engine_translator(0)
{

}

NLEngineGlobals::~NLEngineGlobals()
{
    deinit_engine();
}

NLEngineGlobals * NLEngineGlobals::Instance()
{
    static NLEngineGlobals _instance;
    return &_instance;
}

bool NLEngineGlobals::init_engine(NLEngineRegisterObject *obj)
{
    if(_engine_inited)
        return true;

    qRegisterMetaType<NLVector3>("NLVector3");
    qRegisterMetaType<NLMatrix4>("NLMatrix4");
    qRegisterMetaType<NL::Physics::m>("NLPhysics_m");
    qRegisterMetaType<NL::Physics::a>("NLPhysics_a");
    qRegisterMetaType<NL::Physics::g>("NLPhysics_g");
    qRegisterMetaType<NL::Physics::G>("NLPhysics_G");
    qRegisterMetaType<NL::Physics::v>("NLPhysics_v");
    qRegisterMetaType<NL::Physics::F>("NLPhysics_F");
    qRegisterMetaType<NL::Physics::t>("NLPhysics_t");
    qRegisterMetaType<NL::Physics::d>("NLPhysics_d");
    qRegisterMetaType<NLRenderable *>("NLRenderable *");
    qRegisterMetaType<NLComponent *>("NLComponent *");
    qRegisterMetaType<NLActor *>("NLActor *");
    qRegisterMetaType<NLScript *>("NLScript *");
    qRegisterMetaType<NLScene *>("NLScene *");
    qRegisterMetaType<NLSceneCamera *>("NLSceneCamera *");
    qRegisterMetaType<NLForce *>("NLForce *");
    qRegisterMetaType<NLObject *>("NLObject *");
    qRegisterMetaType<NLRigidbody *>("NLRigidbody *");
    qRegisterMetaType<NLScriptObject *>("NLScriptObject *");
    qRegisterMetaType<NLVariantGeneralPointer>("NLVariantGeneralPointer");

    load_translator();

    if(obj)
        register_engine(obj);

    _engine_variant_compare_funcs.insert("NLVector3", vector3_variant_compare_func);
    _engine_variant_compare_funcs.insert("vector3_t", vector3_variant_compare_func);
    _engine_variant_compare_funcs.insert("vector3_s", vector3_variant_compare_func);
    _engine_variant_compare_funcs.insert("NLVariantGeneralPointer", general_pointer_variant_compare_func);

    _engine_inited = true;
    return _engine_inited;
}

void NLEngineGlobals::deinit_engine()
{
    if(!_engine_inited)
        return;
    Q_FOREACH(const QString &name, _engine_register_names.keys())
    {
        QByteArray ba = name.toLocal8Bit();
        unregister_engine(ba.constData());
        delete _engine_register_names[name];
    }
    _engine_register_names.clear();
    _engine_variant_compare_funcs.clear();
    unload_translator();
    _engine_inited = false;
}

bool NLEngineGlobals::unregister_engine(const char *name)
{
    if(!engine_is_register(name))
        return false;
    NLEngineRegisterObject *obj = _engine_register_names[name];
    obj->UnregisterMetaType();
    NLEngineRegisterObject::VariantCompareFuncMap compareFuncs = obj->UnregisterVariantCompareFunc();
    Q_FOREACH(const QString &n, compareFuncs.keys())
    {
        _engine_variant_compare_funcs.remove(n);
    }
    _engine_register_names.remove(name);
    return true;
}

void NLEngineGlobals::register_engine(NLEngineRegisterObject *obj)
{
    if(!_engine_inited)
        return;
    const char *name = obj->Name();
    if(engine_is_register(name))
        return;
    obj->RegisterMetaType();
    NLEngineRegisterObject::VariantCompareFuncMap compareFuncs = obj->RegisterVariantCompareFunc();
    Q_FOREACH(const QString &name, compareFuncs.keys())
    {
        _engine_variant_compare_funcs[name] = compareFuncs.value(name);
    }

    _engine_register_names.insert(name, obj);
}

bool NLEngineGlobals::load_translator()
{
    if(_engine_translator)
        return true;
    QCoreApplication *qapp = QCoreApplication::instance();
    if(!qapp)
        return false;
    const QString locale = QLocale::system().name();
    const QString qmFile("nl." + locale);
    _engine_translator = new QTranslator(qapp);

    if(_engine_translator->load(qmFile, "i18n"))
    {
        qDebug() << "Load nl i18n -> " + qmFile + ".qm ......done!";
        qapp->installTranslator(_engine_translator);
        return true;
    }
    else
    {
        qDebug() << "Load nl i18n -> " + qmFile + ".qm ......fail!";
        return false;
    }
}

void NLEngineGlobals::unload_translator()
{
    if(_engine_translator)
    {
        delete _engine_translator;
        _engine_translator = 0;
    }
}

bool NLEngineGlobals::variant_compare(const QString &type, const QVariant &a, const QVariant &b) const
{
    if(!_engine_inited)
        return false;
    NLVariantCompare_f func = variant_compare_func(type);
    if(func)
        return func(a, b);
    return false;
}

NLVariantCompare_f NLEngineGlobals::variant_compare_func(const QString &type) const
{
    return _engine_variant_compare_funcs.value(type);
}

void NLEngineGlobals::register_lua_func(struct lua_State *L)
{
    if(!_engine_inited)
        return;
    Q_FOREACH(const QString &name, _engine_register_names.keys())
    {
        _engine_register_names[name]->RegisterLuaFunc(L);
    }
}

QVariant NLEngineGlobals::convert_lua_variant(void **ptr, const QString &metatableName)
{
    QVariant v;
    if(!_engine_inited)
        return v;
    Q_FOREACH(const QString &name, _engine_register_names.keys())
    {
        v = _engine_register_names[name]->LuaVariantToQVariant(ptr, metatableName);
        if(v.isValid())
            break;
    }
    return v;
}

int NLEngineGlobals::push_variant_to_lua(const QVariant &v, struct lua_State *L, const QString &specialType)
{
    int res = 0;
    if(!_engine_inited)
        return res;
    Q_FOREACH(const QString &name, _engine_register_names.keys())
    {
        res = _engine_register_names[name]->PushQVariantToLua(v, L, specialType);
        if(res > 0)
            break;
    }
    return res;
}
