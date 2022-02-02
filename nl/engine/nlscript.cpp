#include "nlscript.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "common/nldbg.h"
#include "nlactor.h"
#include "template/nlsequencemap.h"
#include "nlscriptcontainer.h"
#include "lua/lua_def.h"
#include "lua/lua_globals.h"
#include "lua/lua_variant.h"

#define m_L (m_lua.L)

bool NLScript::Script_Lua::Init()
{
    if(L)
        return false;
    L = luaL_newstate();
    if(!L)
        return false;
    luaL_openlibs(L);

    NL::register_lua_metatable(L);

    qDebug() << "lua script engine initilized!";

    script->AfterLuaInit(L);

    return true;
}

bool NLScript::Script_Lua::Deinit()
{
    if(!L)
        return false;

    script->BeforeLuaDeinit(L);
    if(func & Script_Lua_Func_Destroy)
    {
        lua_getglobal(L, "Destroy");
        int err = lua_pcall(L, 0, 0, 0);
        if(err)
        {
            const char *errstr = lua_tostring(L, -1);
            qWarning() << "lua script Destroy() -> error: " << err << errstr;
            lua_pop(L, 1);
        }
        else
            qDebug() << "lua script Destroy()......done";
    }
    else
        qWarning() << "lua script Destroy()......missing";

    lua_close(L);
    qDebug() << "lua script engine destroyed!";
    L = 0;
    func = -1;
    UnregisterGlobalVariant();
    return true;
}

bool NLScript::Script_Lua::Reset()
{
    if(!L)
        return false;

    if(func & Script_Lua_Func_Reset)
    {
        lua_getglobal(L, "Reset");
        int err = lua_pcall(L, 0, 0, 0);
        if(err)
        {
            const char *errstr = lua_tostring(L, -1);
            qWarning() << "lua script Reset() -> error: " << err << errstr;
            lua_pop(L, 1);
            return false;
        }
        else
        {
            qDebug() << "lua script Reset()......done";
            return true;
        }
    }
    else
    {
        qDebug() << "lua script Reset()......missing";
        return false;
    }
}

bool NLScript::Script_Lua::Exec(float delta)
{
    if(!L)
        return false;

    if(func >= 0)
    {
        lua_pushnumber(L, delta);
        lua_setglobal(L, "nl_Delta");
        if(func & Script_Lua_Func_Update)
        {
            RestoreGlobalVariant();
            {
                lua_getglobal(L, "Update");
                lua_pushnumber(L, delta);
                int err = lua_pcall(L, 1, 0, 0);
                if(err)
                {
                    const char *errstr = lua_tostring(L, -1);
                    qWarning() << "lua script Update(number) -> error: " << err << errstr;
                    lua_pop(L, 1);
                    Deinit();
                    return false;
                }
            }
            DumpGlobalVariant();
            //qDebug() << "lua script Update(number)" << delta;
        }
        else
        {
            RestoreGlobalVariant();
            {
                int err = luaL_dostring(L, script->m_data.constData());
                if(err)
                {
                    const char *errstr = lua_tostring(L, -1);
                    qWarning() << "lua script when Update! -> error: " << err << errstr;
                    lua_pop(L, 1);
                    Deinit();
                    return false;
                }
            }
            DumpGlobalVariant();
            //lua_settop(L, 0);
        }
    }
    else
    {
        func = 0;
        PUSH_NLOBJECT_TO_STACK(L, NLScene, script->Scene())
        lua_setglobal(L, "nl_Scene");

        PUSH_NLOBJECT_TO_STACK(L, NLActor, script->Actor())
        lua_setglobal(L, "nl_Actor");

        PUSH_NLOBJECT_TO_STACK(L, NLScript, script)
        lua_setglobal(L, "nl_Script");

        lua_pushnumber(L, delta);
        lua_setglobal(L, "nl_Delta");

        int err = luaL_dostring(L, script->m_data.constData());
        if(err)
        {
            const char *errstr = lua_tostring(L, -1);
            qWarning() << "lua script initial execute -> error: " << err << errstr;
            lua_pop(L, 1);
            Deinit();
            return false;
        }

        if(lua_getglobal(L, "Init") == LUA_TFUNCTION)
        {
            qDebug() << "lua script Init()......find";
            err = lua_pcall(L, 0, 1, 0);
            if(err)
            {
                const char *errstr = lua_tostring(L, -1);
                qWarning() << "lua script Init() -> error: " << err << errstr;
                lua_pop(L, 1);
                Deinit();
                return false;
            }
            int ret = lua_toboolean(L, -1);
            lua_pop(L, 1); // pop return result
            if(ret)
            {
                qDebug() << "lua script Init() -> true";
                RegisterGlobalVariant();
            }
            else
            {
                qWarning() << "lua script Init() -> false";
                Deinit();
                return false;
            }
            func |= Script_Lua_Func_Init;
        }
        else
        {
            qWarning() << "lua script Init()......missing";
            lua_pop(L, 1); // pop function
            RegisterGlobalVariant();
        }

        if(lua_getglobal(L, "Update") == LUA_TFUNCTION)
        {
            func |= Script_Lua_Func_Update;
            lua_pushnumber(L, delta);
            err = lua_pcall(L, 1, 0, 0);
            if(err)
            {
                const char *errstr = lua_tostring(L, -1);
                qWarning() << "lua script initial Update(number) -> error: " << err << errstr;
                lua_pop(L, 1);
                Deinit();
                return false;
            }
            qDebug() << "lua script Update(number)......find";
        }
        else
        {
            qWarning() << "lua script Update(number)......missing";
            lua_pop(L, 1); // pop function
        }

        if(lua_getglobal(L, "Destroy") == LUA_TFUNCTION)
        {
            func |= Script_Lua_Func_Destroy;
            qDebug() << "lua script Destroy()......find";
        }
        else
            qWarning() << "lua script Destroy()......missing";
        lua_pop(L, 1); // pop function

        if(lua_getglobal(L, "Reset") == LUA_TFUNCTION)
        {
            func |= Script_Lua_Func_Reset;
            qDebug() << "lua script Reset()......find";
        }
        else
            qWarning() << "lua script Reset()......missing";
        lua_pop(L, 1); // pop function
    }

    return true;
}

void NLScript::Script_Lua::RestoreGlobalVariant()
{
    if(!L || !script->IsGlobalDataDirty())
        return;
    Q_FOREACH(const QString &key, script->m_globalVaraint.SequenceKeys())
    {
        QVariant va = script->m_globalVaraint.value(key);
        QByteArray ba = key.toLocal8Bit();
        const char *keyName = ba.constData();
        int type = lua_getglobal(L, keyName);
        lua_pop(L, -1);
        int pushed = NL::push_from_qvarianti(L, va, type);

        if(pushed > 0) // TODO: only single return value
            lua_setglobal(L, keyName);
    }
}

void NLScript::Script_Lua::RegisterGlobalVariant()
{
    if(!L)
        return;
    NLVariantSequenceHash props = GetGlobalVariant();
    script->connect(script, SIGNAL(propertyChanged(const QString &, const QVariant &, int)), script, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
    script->SetGlobalVariant(props);
}

void NLScript::Script_Lua::UnregisterGlobalVariant()
{
    script->ClearGlobalVariant();
    script->disconnect(script, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
}

void NLScript::Script_Lua::DumpGlobalVariant()
{
    if(!L)
        return;
    NLVariantSequenceHash props = GetGlobalVariant();
    script->SetGlobalVariant(props);
}

NLVariantSequenceHash NLScript::Script_Lua::GetGlobalVariant()
{
    NLVariantSequenceHash props;

    if(!L)
        return props;
    lua_pushglobaltable(L);

    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        QString key(lua_tostring(L, -2));
        //qDebug() << key;
        // lua internal globals
        if(key != "_G" && key != "_VERSION"
                // internal globals
                && key != "nl_Actor"
                && key != "nl_Delta"
                && key != "nl_Script"
                && key != "nl_Scene"
                // internal
                && key != "scriptSource"
                && key != "scriptFile"
                && key != "enabled"
                )
        {
            QVariant v = NL::load_to_qvariant(L, -1);
            if(v.isValid())
            {
                props.insert(key, v);
            }
        }

        lua_pop(L, 1);
    }

    lua_pop(L, 1);
    return props;
}



NLScript::NLScript(NLActor *parent) :
    NLObject(NLObject::Type_Script, 0, NLProperties(), parent),
    m_mounted(false),
    m_globalDataDirty(false),
    m_globalDataUpdateLock(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLScript::NLScript(const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Script, 0, prop, parent),
    m_mounted(false),
    m_globalDataDirty(false),
    m_globalDataUpdateLock(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, NLActor *parent) :
    NLObject(NLObject::Type_Script, scene, NLProperties(), parent),
    m_mounted(false),
    m_globalDataDirty(false),
    m_globalDataUpdateLock(false)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Script, scene, prop, parent),
    m_mounted(false),
    m_globalDataDirty(false),
    m_globalDataUpdateLock(false)
{
    Construct();
}

NLScript::~NLScript()
{
    Destroy(); // !! vitrual
}

void NLScript::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLScript);
    if(objectName().isEmpty() || !GetInitProperty("name").isValid())
        setObjectName("NLScript");
    SetType(NLObject::Type_Script);

    NLProperties props = PropertyConfig();

    props.Insert("scriptFile", NLProperties("file", true));
    props.Insert("scriptSource", NLProperties("multiline", true)
                 ("direct", false)
                 ("syntax", "lua")
                 );

    SetPropertyConfig(props);
    m_lua.script = this;
}

void NLScript::Update(float delta)
{
    if(!IsActived())
        return;
    //NLObject::Update(delta);
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

const NLActor * NLScript::Actor() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLActor *>(p);
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
    InitLua(); // If has script source, try to load.
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> MOUNTED";
#endif
    emit mounted();
}

void NLScript::Unmount()
{
    if(!IsMounted())
        return;
    NLActor *actor = Actor();
    if(actor)
    {
        actor->TellScriptRemoved();
        SetActor(0);
    }
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

bool NLScript::InitLua()
{
    if(!IsMounted() || !HasScriptSource())
        return false;
    return m_lua.Init();
}

bool NLScript::DeinitLua()
{
    if(!IsMounted())
        return false;
    return m_lua.Deinit();
}

void NLScript::InitProperty()
{
    NLObject::InitProperty();
    QString str(GetInitProperty_T<QString>("scriptSource"));
    if(!str.isEmpty())
        SetScriptSource(str);
    else
    {
        str = GetInitProperty_T<QString>("scriptFile");
        if(!str.isEmpty())
            SetScriptFile(str);
    }
}

void NLScript::Reset()
{
    NLObject::Reset();
    m_lua.Reset();
    UnlockGlobalDataUpdate();
    SetGlobalDataDirty(false);
}

void NLScript::OnPropertyChanged(const QString &name, const QVariant &value, int type)
{
    if(IsLockGlobalDataUpdate())
        return;
    if(name == "objectName"
            || name == "enabled"
            || name == "scriptFile"
            || name == "scriptSource"
            )
        return;
    qDebug() << name << value << type;
    if(type == -1)
        m_globalVaraint.remove(name);
    else
        m_globalVaraint[name] = value;
    SetGlobalDataDirty(true);
}

void NLScript::SetGlobalVariant(const NLVariantSequenceHash &list)
{
    LockGlobalDataUpdate();
    const QList<QString> keys = list.SequenceKeys();
    Q_FOREACH(const QString &key, keys)
    {
        //qDebug() << "Set -> " << key << GetProperty(key) << list.value(key) << GetProperty(key) == list.value(key);
        SetProperty(key, list.value(key));
    }

    Q_FOREACH(const QString &key, m_globalVaraint.SequenceKeys())
    {
        if(!keys.contains(key))
        {
            RemoveProperty(key);
            //qDebug() << "Remove unused -> " << key << GetProperty(key);
        }
    }
    m_globalVaraint = list;
    UnlockGlobalDataUpdate();
    SetGlobalDataDirty(false);
}

void NLScript::ClearGlobalVariant()
{
    LockGlobalDataUpdate();
    Q_FOREACH(const QString &key, m_globalVaraint.SequenceKeys())
    {
        //qDebug() << "REMOVE -> " << key << GetProperty(key);
        RemoveProperty(key);
    }
    m_globalVaraint.clear();
    UnlockGlobalDataUpdate();
    SetGlobalDataDirty(false);
}
