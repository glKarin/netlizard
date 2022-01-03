#include "lua_script.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlscript.h"
#include "lua_def.h"

#define CALLER_SCRIPT(L, name) GET_LUA_CALLER(L, NLScript, name)
#define CALLER_SCRIPT_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLScript, name)

static int Script_new(lua_State *L)
{
    PUSH_NLOBJECT_TO_STACK(L, NLScript, new NLScript)
    return 1;
}

static int Script_delete(lua_State *L)
{
    CALLER_SCRIPT_USERDATA(L, script);
    delete *script;
    *script = 0;
    lua_pushboolean(L, 1);
    return 1;
}

static int Script_Name(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    QByteArray ba = script->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Script_ClassName(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    QByteArray ba = script->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Script_SetEnabled(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    int b = lua_toboolean(L, 2);
    script->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Script_Actor(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    NLActor *a = script->Actor();
    if(a)
    {
        PUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Script_IsEnabled(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    int i = script->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, i);
    return 1;
}

static int Script_SetScriptFile(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    const char *s = lua_tostring(L, 2);
    int b = script->SetScriptFile(s) ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int Script_SetScriptSource(lua_State *L)
{
    CALLER_SCRIPT(L, script);
    const char *s = lua_tostring(L, 2);
    script->SetScriptSource(s);
    lua_pushboolean(L, 1);
    return 1;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Script_##x); \
    lua_setfield(L, -2, #x);
#define COMPONENT_FUNC(x) {#x, Script_##x}
bool script_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLScript"))
        return true;

    SET_GLOBAL_CFUNC(L, "new_NLScript", Script_new)
    SET_GLOBAL_CFUNC(L, "delete_NLScript", Script_delete)

    if(luaL_newmetatable(L, "NLScript"))
    {
        const struct luaL_Reg Funcs[] = {
            COMPONENT_FUNC(Name),
            COMPONENT_FUNC(ClassName),
            COMPONENT_FUNC(SetEnabled),
            COMPONENT_FUNC(Actor),
            COMPONENT_FUNC(IsEnabled),
            COMPONENT_FUNC(SetScriptFile),
            COMPONENT_FUNC(SetScriptSource),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "Script_register";
        return true;
    }
    return false;
}

}
