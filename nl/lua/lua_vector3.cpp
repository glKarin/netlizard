#include "lua_vector3.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "common/nlvec.h"
#include "lua_def.h"
#include "lua_variant.h"

#define CALLER_VECTOR3(L, name) NLNLGET_LUA_CALLER(L, NLVector3, name)
#define CALLER_VECTOR3_USERDATA(L, name) NLNLGET_LUA_CALLER_USERDATA(L, NLVector3, name)

static int Vector3_new(lua_State *L)
{
    NLVector3 *v = (NLVector3 *)malloc(sizeof(NLVector3));
    int top = lua_gettop(L);
    if(top == 0)
        VECTOR3V_X(v) = VECTOR3V_Y(v) = VECTOR3V_Z(v) = 0;
    else if(top == 1)
        VECTOR3V_X(v) = VECTOR3V_Y(v) = VECTOR3V_Z(v) = lua_tonumber(L, 1);
    else if(top == 2)
    {
        VECTOR3V_X(v) = lua_tonumber(L, 1);
        VECTOR3V_Y(v) = lua_tonumber(L, 2);
        VECTOR3V_Z(v) = 0;
    }
    else
    {
        VECTOR3V_X(v) = lua_tonumber(L, 1);
        VECTOR3V_Y(v) = lua_tonumber(L, 2);
        VECTOR3V_Z(v) = lua_tonumber(L, 3);
    }
    NLPUSH_NLOBJECT_TO_STACK(L, NLVector3, v)
    return 1;
}

static int Vector3_delete(lua_State *L)
{
    CALLER_VECTOR3_USERDATA(L, v);
    qDebug() << "lua -> vec3 deleted";
    delete *v;
    *v = 0;
    return 0;
}

static int Vector3_X(lua_State *L)
{
    CALLER_VECTOR3(L, v);
    if(lua_gettop(L) >= 2)
        VECTOR3V_X(v) = lua_tonumber(L, 2);
    lua_pushnumber(L, VECTOR3V_X(v));
    return 1;
}

static int Vector3_Y(lua_State *L)
{
    CALLER_VECTOR3(L, v);
    if(lua_gettop(L) >= 2)
        VECTOR3V_Y(v) = lua_tonumber(L, 2);
    lua_pushnumber(L, VECTOR3V_Y(v));
    return 1;
}

static int Vector3_Z(lua_State *L)
{
    CALLER_VECTOR3(L, v);
    if(lua_gettop(L) >= 2)
        VECTOR3V_Z(v) = lua_tonumber(L, 2);
    lua_pushnumber(L, VECTOR3V_Z(v));
    return 1;
}

static int Vector3_XYZ(lua_State *L)
{
    CALLER_VECTOR3(L, v);
    int top = lua_gettop(L);
    if(top == 2)
        VECTOR3V_X(v) = lua_tonumber(L, 2);
    else if(top == 3)
    {
        VECTOR3V_X(v) = lua_tonumber(L, 2);
        VECTOR3V_Y(v) = lua_tonumber(L, 3);
    }
    else if(top >= 4)
    {
        VECTOR3V_X(v) = lua_tonumber(L, 2);
        VECTOR3V_Y(v) = lua_tonumber(L, 3);
        VECTOR3V_Z(v) = lua_tonumber(L, 4);
    }
    lua_pushnumber(L, VECTOR3V_X(v));
    lua_pushnumber(L, VECTOR3V_Y(v));
    lua_pushnumber(L, VECTOR3V_Z(v));
    return 3;
}


namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Vector3_##x); \
    lua_setfield(L, -2, #x);
#define VECTOR3_FUNC(x) {#x, Vector3_##x}
bool vector3_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLVector3"))
        return true;

    lua_register(L, "new_NLVector3", Vector3_new);
    //lua_register(L, "delete_NLVector3", Vector3_delete);

    if(luaL_newmetatable(L, "NLVector3"))
    {
        const struct luaL_Reg Funcs[] = {
            VECTOR3_FUNC(X),
            VECTOR3_FUNC(Y),
            VECTOR3_FUNC(Z),
            VECTOR3_FUNC(XYZ),
            NLNULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushcfunction(L, Vector3_delete); \
        lua_setfield(L, -2, "__gc");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        return true;
    }
    return false;
}

}
