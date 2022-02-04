#include "nlluascriptobject.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "common/nldbg.h"
#include "nlscript.h"
#include "template/nlsequencemap.h"
#include "lua/lua_def.h"
#include "lua/lua_globals.h"
#include "lua/lua_variant.h"

NLLuaScriptObject::NLLuaScriptObject(const QByteArray &data, NLScript *parent) :
    NLScriptObject(data, parent),
    L(0)
{
    CLASS_NAME(NLLuaScriptObject);
}

NLLuaScriptObject::~NLLuaScriptObject()
{
    Destroy(); // !! vitrual
}

bool NLLuaScriptObject::InitScript()
{
    if(L)
        return false;
    L = luaL_newstate();
    if(!L)
        return false;
    luaL_openlibs(L);

    NL::register_lua_metatable(L);

    qDebug() << "lua script engine initilized!";

    return true;
}

bool NLLuaScriptObject::DestroyScript()
{
    if(!L)
        return false;

    int func = Func();
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
    return true;
}

bool NLLuaScriptObject::ResetScript()
{
    if(!L)
        return false;

    int func = Func();
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

bool NLLuaScriptObject::UpdateScript(float delta)
{
    if(!L)
        return false;

    int &func = Func();
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
                    DestroyScript();
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
                int err = luaL_dostring(L, Data().constData());
                if(err)
                {
                    const char *errstr = lua_tostring(L, -1);
                    qWarning() << "lua script when Update! -> error: " << err << errstr;
                    lua_pop(L, 1);
                    DestroyScript();
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
        RegisterInitialGlobalVariant(delta);

        int err = luaL_dostring(L, Data().constData());
        if(err)
        {
            const char *errstr = lua_tostring(L, -1);
            qWarning() << "lua script initial execute -> error: " << err << errstr;
            lua_pop(L, 1);
            DestroyScript();
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
                DestroyScript();
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
                DestroyScript();
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
                DestroyScript();
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

void NLLuaScriptObject::SetGlobalVariant(const NLVariantSequenceHash &vars)
{
    if(!L)
        return;
    Q_FOREACH(const QString &key, vars.keys())
    {
        QVariant va = vars.value(key);
        QByteArray ba = key.toLocal8Bit();
        const char *keyName = ba.constData();
        int type = lua_getglobal(L, keyName);
        lua_pop(L, -1);
        int pushed = NL::push_from_qvarianti(L, va, type);

        if(pushed > 0) // TODO: only single return value
            lua_setglobal(L, keyName);
    }
}

NLVariantSequenceHash NLLuaScriptObject::GetGlobalVariant()
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

void NLLuaScriptObject::RegisterInitialGlobalVariant(float delta)
{
    if(!L)
        return;
    NLScript *script = Script();
    NLPUSH_NLOBJECT_TO_STACK(L, NLScene, script->Scene())
    lua_setglobal(L, "nl_Scene");

    NLPUSH_NLOBJECT_TO_STACK(L, NLActor, script->Actor())
    lua_setglobal(L, "nl_Actor");

    NLPUSH_NLOBJECT_TO_STACK(L, NLScript, script)
    lua_setglobal(L, "nl_Script");

    lua_pushnumber(L, delta);
    lua_setglobal(L, "nl_Delta");
}
