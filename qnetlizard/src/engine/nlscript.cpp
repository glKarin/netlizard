#include "nlscript.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "qdef.h"
#include "nlactor.h"
#include "nlscriptcontainer.h"
#include "lua_actor.h"

NLScript::NLScript(NLActor *parent) :
    NLObject(NLPROPERTIY_NAME(NLScript), parent),
    m_L(0),
    m_mounted(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLScript::NLScript(const NLProperties &prop, NLActor *parent) :
    NLObject(NLPROPERTIES_NAME(prop, NLScript), parent),
    m_L(0),
    m_mounted(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, NLActor *parent) :
    NLObject(scene, NLPROPERTIY_NAME(NLScript), parent),
    m_L(0),
    m_mounted(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(scene, NLPROPERTIES_NAME(prop, NLScript), parent),
    m_L(0),
    m_mounted(false)
{
    Construct();
}

NLScript::~NLScript()
{
}

void NLScript::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLScript);
    setObjectName("NLScript");
    SetType(NLObject::Type_Script);
}

void NLScript::Update(float delta)
{
    if(!IsActived())
        return;
    NLObject::Update(delta);
    if(!m_data.isEmpty())
        qDebug() << "lua" << luaL_dostring(m_L, m_data.constData());
}

void NLScript::Destroy()
{
    if(!IsInited())
        return;
    if(IsMounted())
        Unmount();
    NLObject::Destroy();
}

NLActor * NLScript::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLScript::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLScript::Mount(NLActor *actor)
{
    if(!actor)
        return;
    if(IsMounted())
        return;
    if(!IsInited())
        Init();
    SetActor(actor);
    InitLua();
    m_mounted = true;
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> MOUNTED";
#endif
    emit mounted();
}

void NLScript::Unmount()
{
    if(!IsMounted())
        return;
    SetActor(0);
    DeinitLua();
    m_mounted = false;
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> UNMOUNTED";
#endif
    emit unmounted();
}

NLScriptContainer * NLScript::Container()
{
    return static_cast<NLScriptContainer *>(NLObject::Container());
}

void NLScript::SetContainer(NLScriptContainer *container)
{
    NLObject::SetContainer(container);
}

bool NLScript::InitLua()
{
    if(IsMounted())
        return false;
    m_L = luaL_newstate();
    luaL_openlibs(m_L);

    NL::actor_registe_metatable(m_L);

    NL::actor_registe_global_object(m_L, Actor(), "nl_Actor");

    return true;
}

bool NLScript::DeinitLua()
{
    if(!IsMounted())
        return false;
    lua_close(m_L);
    return true;
}

bool NLScript::SetScriptFile(const QString &file)
{
    if(file != m_sourceFile)
    {
        QFile f(file);
        if(!f.exists())
            return false;
        if(!f.open(QIODevice::ReadOnly))
            return false;
        QTextStream is(&f);
        is.setCodec("utf-8");
        SetScriptSource(is.readAll());
        f.close();
        m_sourceFile = file;
        emit propertyChanged("scriptFile", m_sourceFile);
        return true;
    }
    return false;
}

void NLScript::SetScriptSource(const QString &src)
{
    if(src != QString(m_data))
    {
        m_data.clear();
        m_data.append(src);
        emit propertyChanged("scriptSource", QString(m_data));
    }
}
