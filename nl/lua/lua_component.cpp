#include "lua_component.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "engine/nlcomponent.h"
#include "lua_def.h"
#include "lua_object.h"

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

    SET_GLOBAL_CFUNC(L, "new_NLComponent", Component_new)
    SET_GLOBAL_CFUNC(L, "delete_NLComponent", Component_delete)

    if(luaL_newmetatable(L, "NLComponent"))
    {
        const struct luaL_Reg Funcs[] = {
            COMPONENT_FUNC(Actor),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        register_object_metatable_function(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        return true;
    }
    return false;
}

}
