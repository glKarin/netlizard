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
#define CALLER_COMPONENT_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLComponent, name)

static int Component_new(lua_State *L)
{
    PUSH_NLOBJECT_TO_STACK(L, NLComponent, new NLComponent(NL::lua_table_to_properties(L, 1)))
    return 1;
}

static int Component_delete(lua_State *L)
{
    CALLER_COMPONENT_USERDATA(L, comp);
    delete *comp;
    *comp = 0;
    return 0;
}

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

static int Component_IsEnabled(lua_State *L)
{
    CALLER_COMPONENT(L, comp);
    int i = comp->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, i);
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

    SET_GLOBAL_CFUNC(L, "new_NLComponent", Component_new)
    SET_GLOBAL_CFUNC(L, "delete_NLComponent", Component_delete)

    if(luaL_newmetatable(L, "NLComponent"))
    {
        const struct luaL_Reg Funcs[] = {
            COMPONENT_FUNC(Name),
            COMPONENT_FUNC(ClassName),
            COMPONENT_FUNC(SetEnabled),
            COMPONENT_FUNC(Actor),
            COMPONENT_FUNC(IsEnabled),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        return true;
    }
    return false;
}

}
