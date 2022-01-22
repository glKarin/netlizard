#include "lua_globals.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "lua_def.h"
#include "lua/lua_actor.h"
#include "lua/lua_component.h"
#include "lua/lua_scene.h"
#include "lua/lua_script.h"
#include "lua/lua_scenecamera.h"
#include "lua/lua_variant.h"
#include "lua/lua_vector3.h"
#include "lua_variant.h"

static int Globals_NullUserData(lua_State *L)
{
    int top = lua_gettop(L);
    void **ptr = (void **)lua_newuserdata(L, sizeof(void **));
    *ptr = 0;
    if(top >= 1)
    {
        const char *mt = lua_tostring(L, 1);
        if(luaL_getmetatable(L, mt) == LUA_TTABLE)
            lua_setmetatable(L, -2);
        else
            lua_pop(L, 1);
    }
    return 1;
}

static int Globals_UserDataIsNull(lua_State *L)
{
    void *p = 0;
    if(lua_isuserdata(L, 1))
    {
        void **ptr = (void **)(lua_touserdata(L, 1));
        p = *ptr;
    }
    else if(lua_islightuserdata(L, 1))
        p = lua_touserdata(L, 1);
    lua_pushboolean(L, p ? 0 : 1);
    return 1;
}

static int Globals_UserDataNull(lua_State *L)
{
    int res = 0;
    if(lua_isuserdata(L, 1))
    {
        void **ptr = (void **)(lua_touserdata(L, 1));
        *ptr = 0;
        res = 1;
    }
    lua_pushboolean(L, res);
    return 1;
}

static int Globals_UserDataPointer(lua_State *L)
{
    void *p = 0;
    if(lua_isuserdata(L, 1))
    {
        void **ptr = (void **)(lua_touserdata(L, 1));
        if(lua_gettop(L) >= 2)
        {
            *ptr = (void *)lua_tointeger(L, 2);
        }
        p = *ptr;
    }
    else if(lua_islightuserdata(L, 1))
        p = lua_touserdata(L, 1);
    lua_pushinteger(L, (uintptr_t)p);
    return 1;
}

namespace NL
{

static bool globals_register_metatable(struct lua_State *L)
{
    SET_GLOBAL_CFUNC(L, "NullUserData", Globals_NullUserData)
    SET_GLOBAL_CFUNC(L, "UserDataIsNull", Globals_UserDataIsNull)
    SET_GLOBAL_CFUNC(L, "UserDataNull", Globals_UserDataNull)
    SET_GLOBAL_CFUNC(L, "UserDataPointer", Globals_UserDataPointer)

    return true;
}

bool register_lua_metatable(lua_State *L)
{
    if(!L)
        return false;
    NL::globals_register_metatable(L);
    NL::actor_register_metatable(L);
    NL::component_register_metatable(L);
    NL::scene_register_metatable(L);
    NL::rigidbody_register_metatable(L);
    NL::scenecamera_register_metatable(L);
    NL::script_register_metatable(L);
    NL::vector3_register_metatable(L);
    return true;
}

}
