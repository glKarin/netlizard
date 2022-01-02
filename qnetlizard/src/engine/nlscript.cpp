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
#include "lua_component.h"
#include "lua_scene.h"
#include "lua_scenecamera.h"
#include "lua_def.h"

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

    NLProperties props;
    NLProperties m;

    m.insert("file", true);
    props.Insert("scriptFile", m);

    m.clear();
    m.insert("multiline", true);
    props.Insert("scriptSource", m);

    SetPropertyConfig(props);
}

void NLScript::Update(float delta)
{
    if(!IsActived())
        return;
    NLObject::Update(delta);
    /*qDebug() << */ExecScript(delta);
}

bool NLScript::ExecScript(float delta)
{
    if(!m_L)
        return false;
    PUSH_NLOBJECT_TO_STACK(m_L, NLScene, Scene())
    lua_setglobal(m_L, "nl_Scene");

    PUSH_NLOBJECT_TO_STACK(m_L, NLActor, Actor())
    lua_setglobal(m_L, "nl_Actor");

    lua_pushnumber(m_L, delta);
    lua_setglobal(m_L, "nl_Delta");

    bool res = luaL_dostring(m_L, m_data.constData());

    lua_settop(m_L, 0);
    return res;
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
    if(m_L)
        return false;
    m_L = luaL_newstate();
    luaL_openlibs(m_L);

    NL::actor_register_metatable(m_L);
    NL::component_register_metatable(m_L);
    NL::scene_register_metatable(m_L);
    NL::rigidbody_register_metatable(m_L);
    NL::scenecamera_register_metatable(m_L);

    return true;
}

bool NLScript::DeinitLua()
{
    if(!m_L)
        return false;
    lua_close(m_L);
    m_L = 0;
    return true;
}

bool NLScript::SetScriptFile(const QString &file)
{
    if(file != m_sourceFile)
    {
        m_sourceFile = file;
        emit propertyChanged("scriptFile", m_sourceFile);
    }
    if(!m_sourceFile.isEmpty())
    {
        QFile f(file);
        if(!f.exists())
        {
            SetScriptSource(QString());
            return false;
        }
        if(!f.open(QIODevice::ReadOnly))
        {
            SetScriptSource(QString());
            return false;
        }
        QTextStream is(&f);
        SetScriptSource(is.readAll());
        f.close();
        return true;
    }
    return false;
}

void NLScript::SetScriptSource(const QString &src)
{
    QByteArray ba;
    ba.append(src);
    if(m_data != ba)
    {
        m_data.clear();
        DeinitLua();
        if(!src.isEmpty())
        {
            m_data = ba;
            InitLua();
        }
        emit propertyChanged("scriptSource", QString(m_data));
    }
}
