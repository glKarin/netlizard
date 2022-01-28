#include "nlglobals.h"

#include <QVariant>
#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QDebug>

#include "nldef.h"
#include "nlphysics.h"

class NLRenderable;
class NLComponent;
class NLActor;
class NLScript;
class NLScene;
class NLSceneCamera;
class NLForce;
class NLObject;
class NLRigidbody;

static bool _engine_inited = false;
static QHash<QString, NLEngineRegisterObject *> _engine_register_names;

namespace NL
{
static bool load_translator()
{
    const QString locale = QLocale::system().name();
    const QString qmFile("nl." + locale);
    static QTranslator translator;

    if(translator.load(qmFile, "i18n"))
    {
        qDebug() << "Load nl i18n -> " + qmFile + ".qm ......done!";
        qApp->installTranslator(&translator);
        return true;
    }
    else
    {
        qDebug() << "Load nl i18n -> " + qmFile + ".qm ......fail!";
        return false;
    }
}

bool init_engine(NLEngineRegisterObject *obj)
{
    if(_engine_inited)
        return true;

    qRegisterMetaType<NLVector3>("NLVector3");
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
    qRegisterMetaType<NLVariantGeneralPointer>("NLVariantGeneralPointer");

    load_translator();

    if(obj)
        register_engine(obj);

    _engine_inited = true;
    return _engine_inited;
}

void deinit_engine()
{
    if(!_engine_inited)
        return;
    Q_FOREACH(const QString &name, _engine_register_names.keys())
    {
        _engine_register_names[name]->UnregisterMetaType();
        delete _engine_register_names[name];
    }
    _engine_register_names.clear();
    _engine_inited = false;
}

bool engine_is_register(const char *name)
{
    return(_engine_register_names.contains(name));
}

bool unregister_engine(const char *name)
{
    if(!engine_is_register(name))
        return false;
    _engine_register_names[name]->UnregisterMetaType();
    _engine_register_names.remove(name);
    return true;
}

void register_engine(NLEngineRegisterObject *obj)
{
    const char *name = obj->Name();
    if(engine_is_register(name))
        return;
    obj->RegisterMetaType();
    _engine_register_names.insert(name, obj);
}

bool engine_is_inited()
{
    return _engine_inited;
}
}
