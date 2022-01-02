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

#define m_L (m_lua.L)

bool NLScript::Script_Lua::Init()
{
    if(L)
        return false;
    L = luaL_newstate();
    luaL_openlibs(L);

    NL::actor_register_metatable(L);
    NL::component_register_metatable(L);
    NL::scene_register_metatable(L);
    NL::rigidbody_register_metatable(L);
    NL::scenecamera_register_metatable(L);

    return true;
}

bool NLScript::Script_Lua::Deinit()
{
    if(!L)
        return false;

    if(func & Script_Lua_Func_Destroy)
    {
        lua_getglobal(L, "Destroy");
        lua_pcall(L, 0, 0, 0);
        qDebug() << "lua script Destroy()......done";
    }
    else
        qDebug() << "lua script Destroy()......missing";

    lua_close(L);
    L = 0;
    func = -1;
    return true;
}

bool NLScript::Script_Lua::Exec(float delta)
{
    if(!L)
        return false;

    if(func < 0)
    {
        PUSH_NLOBJECT_TO_STACK(L, NLScene, script->Scene())
        lua_setglobal(L, "nl_Scene");

        PUSH_NLOBJECT_TO_STACK(L, NLActor, script->Actor())
        lua_setglobal(L, "nl_Actor");

        lua_pushnumber(L, delta);
        lua_setglobal(L, "nl_Delta");

        int err = luaL_dostring(L, script->m_data.constData());
        if(err)
        {
            qWarning() << "lua script error!" << err;
            Deinit();
            return false;
        }

        func = 0;
        if(lua_getglobal(L, "Init") == LUA_TFUNCTION)
        {
            qDebug() << "lua script Init()......find";
            lua_pcall(L, 0, 1, 0);
            int ret = lua_toboolean(L, -1);
            if(ret)
            {
                qDebug() << "lua script Init() -> true";
            }
            else
            {
                qWarning() << "lua script Init() -> false";
                Deinit();
            }
            lua_pop(L, 1); // pop return result
            if(!ret)
                return false;
            func |= Script_Lua_Func_Init;
        }
        else
            qWarning() << "lua script Init()......missing";

        if(lua_getglobal(L, "Update") == LUA_TFUNCTION)
        {
            func |= Script_Lua_Func_Update;
            lua_pushnumber(L, delta);
            lua_pcall(L, 1, 0, 0);
            qDebug() << "lua script Update(number)......find";
        }
        else
            qWarning() << "lua script Update(number)......missing";

        if(lua_getglobal(L, "Destroy") == LUA_TFUNCTION)
        {
            func |= Script_Lua_Func_Destroy;
            qDebug() << "lua script Destroy()......find";
        }
        else
            qWarning() << "lua script Destroy()......missing";
        lua_pop(L, 1); // pop function
    }
    else
    {
        lua_pushnumber(L, delta);
        lua_setglobal(L, "nl_Delta");
        if(func & Script_Lua_Func_Update)
        {
            lua_getglobal(L, "Update");
            lua_pushnumber(L, delta);
            lua_pcall(L, 1, 0, 0);
            //qDebug() << "lua script Update(number)" << delta;
        }
        else
        {
            luaL_dostring(L, script->m_data.constData());
            //lua_settop(L, 0);
        }
    }

    return true;
}

NLScript::NLScript(NLActor *parent) :
    NLObject(NLPROPERTIY_NAME(NLScript), parent),
    m_mounted(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLScript::NLScript(const NLProperties &prop, NLActor *parent) :
    NLObject(NLPROPERTIES_NAME(prop, NLScript), parent),
    m_mounted(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, NLActor *parent) :
    NLObject(scene, NLPROPERTIY_NAME(NLScript), parent),
    m_mounted(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(scene, NLPROPERTIES_NAME(prop, NLScript), parent),
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
    m_lua.script = this;
}

void NLScript::Update(float delta)
{
    if(!IsActived())
        return;
    NLObject::Update(delta);
    /*qDebug() << */ExecScript(delta);
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