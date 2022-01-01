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

    lua_pushlightuserdata(m_L, Actor());
    luaL_getmetatable(m_L, "NLActor");
    lua_setmetatable(m_L, -2);
    lua_setglobal(m_L, "nl_Actor");

    lua_pushlightuserdata(m_L, Scene());
    luaL_getmetatable(m_L, "NLScene");
    lua_setmetatable(m_L, -2);
    lua_setglobal(m_L, "nl_Scene");

    lua_pushnumber(m_L, delta);
    lua_setglobal(m_L, "nl_Delta");

    /*qDebug() << "lua" << */luaL_dostring(m_L, m_data.constData());
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
    NL::component_registe_metatable(m_L);
    NL::scene_registe_metatable(m_L);

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
