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

bool init_engine()
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

    _engine_inited = true;
    return _engine_inited;
}

bool engine_is_inited()
{
    return _engine_inited;
}
}
