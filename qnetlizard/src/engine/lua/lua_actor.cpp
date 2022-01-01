#include "lua_actor.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlactor.h"
#include "lua_def.h"

#define CALLER_ACTOR(L, name) GET_LUA_CALLER(L, NLActor, name)

static int actor_SetPosition(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->SetPosition(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_SetRotation(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->SetRotation(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_SetScale(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->SetScale(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_Position(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Position();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Rotation(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Rotation();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Scale(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Scale();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Direction(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Direction();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Up(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Up();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Right(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Right();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int actor_Move(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->Move(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_Turn(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->Turn(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_Zoom(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->Zoom(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_Name(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    QByteArray ba = actor->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int actor_ClassName(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    QByteArray ba = actor->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int actor_SetEnabled(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = lua_toboolean(L, 2);
    actor->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_ChildrenCount(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->ChildrenCount();
    lua_pushinteger(L, i);
    return 1;
}

static int actor_GetChild(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLActor *a = NULL;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        a = actor->GetChild(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        a = actor->GetChild(str);
    }
    if(a)
    {
        lua_pushlightuserdata(L, a);
        luaL_getmetatable(L, "NLActor");
        lua_setmetatable(L, -2);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int actor_ParentActor(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLActor *a = actor->ParentActor();
    if(a)
    {
        lua_pushlightuserdata(L, a);
        luaL_getmetatable(L, "NLActor");
        lua_setmetatable(L, -2);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int actor_ComponentCount(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->ComponentCount();
    lua_pushinteger(L, i);
    return 1;
}

static int actor_GetComponent(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLComponent *c = NULL;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        c = actor->GetComponent(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        c = actor->GetComponent(str);
    }
    if(c)
    {
        lua_pushlightuserdata(L, c);
        luaL_getmetatable(L, "NLComponent");
        lua_setmetatable(L, -2);
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
    lua_pushcfunction(L, actor_##x); \
    lua_setfield(L, -2, #x);
bool actor_registe_metatable(struct lua_State *L)
{
    if(luaL_newmetatable(L, "NLActor"))
    {
        REG_FUNC(SetPosition);
        REG_FUNC(SetRotation);
        REG_FUNC(SetScale);
        REG_FUNC(Position);
        REG_FUNC(Rotation);
        REG_FUNC(Scale);
        REG_FUNC(Direction);
        REG_FUNC(Up);
        REG_FUNC(Right);
        REG_FUNC(Move);
        REG_FUNC(Turn);
        REG_FUNC(Zoom);
        REG_FUNC(Name);
        REG_FUNC(ClassName);
        REG_FUNC(SetEnabled);
        REG_FUNC(GetChild);
        REG_FUNC(ChildrenCount);
        REG_FUNC(ParentActor);
        REG_FUNC(GetComponent);
        REG_FUNC(ComponentCount);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "actor_register";
        return true;
    }
    return false;
}

}
