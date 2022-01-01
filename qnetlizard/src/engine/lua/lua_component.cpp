#include "lua_component.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlcomponent.h"
#include "lua_def.h"

#define CALLER_COMPONENT(L, name) GET_LUA_CALLER(L, NLComponent, name)

static int Component_Name(lua_State *L)
{
    CALLER_COMPONENT(L, comp);
    QByteArray ba = comp->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Component_ClassName(lua_State *L)
{
    CALLER_COMPONENT(L, comp);
    QByteArray ba = comp->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Component_SetEnabled(lua_State *L)
{
    CALLER_COMPONENT(L, comp);
    int b = lua_toboolean(L, 2);
    comp->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Component_Actor(lua_State *L)
{
    CALLER_COMPONENT(L, comp);
    NLActor *a = comp->Actor();
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

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Component_##x); \
    lua_setfield(L, -2, #x);
#define COMPONENT_FUNC(x) {#x, Component_##x}
bool component_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLComponent"))
        return true;
    if(luaL_newmetatable(L, "NLComponent"))
    {
        const struct luaL_Reg Funcs[] = {
            COMPONENT_FUNC(Name),
            COMPONENT_FUNC(ClassName),
            COMPONENT_FUNC(SetEnabled),
            COMPONENT_FUNC(Actor),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "Component_register";
        return true;
    }
    return false;
}

}
