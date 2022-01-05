#include "lua_scenecamera.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlscenecamera.h"
#include "lua_def.h"

#define CALLER_SCENECAMERA(L, name) GET_LUA_CALLER(L, NLSceneCamera, name)

static int SceneCamera_Type(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int i = camera->Type();
    lua_pushinteger(L, i);
    return 1;
}

static int SceneCamera_IsEnabled(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int b = camera->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int SceneCamera_SetType(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int i = lua_tointeger(L, 2);
    camera->SetType(static_cast<NLSceneCamera::SceneCamera_Type>(i));
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_SetEnabled(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int b = lua_toboolean(L, 2);
    camera->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_Scene(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    NLScene *s = camera->Scene();
    if(s)
    {
        PUSH_NLOBJECT_TO_STACK(L, NLScene, s)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int SceneCamera_SetPosition(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->SetPosition(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_SetRotation(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->SetRotation(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_SetScale(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->SetScale(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_Position(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Position();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Rotation(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Rotation();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Scale(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Scale();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Direction(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Direction();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Up(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Up();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Right_XPositive(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    const NLVector3 pos = camera->Right_XPositive();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int SceneCamera_Move(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->Move(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_Turn(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->Turn(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_Zoom(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    camera->Zoom(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_SetZIsUp(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int b = lua_toboolean(L, 2);
    camera->SetZIsUp(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int SceneCamera_ZIsUp(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    int b = camera->ZIsUp() ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, SceneCamera_##x); \
    lua_setfield(L, -2, #x);
#define SCENECAMERA_FUNC(x) {#x, SceneCamera_##x}
bool scenecamera_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLSceneCamera"))
        return true;
    if(luaL_newmetatable(L, "NLSceneCamera"))
    {
        const struct luaL_Reg Funcs[] = {
            SCENECAMERA_FUNC(Type),
            SCENECAMERA_FUNC(IsEnabled),
            SCENECAMERA_FUNC(SetType),
            SCENECAMERA_FUNC(SetEnabled),
            SCENECAMERA_FUNC(Scene),
            SCENECAMERA_FUNC(SetPosition),
            SCENECAMERA_FUNC(SetRotation),
            SCENECAMERA_FUNC(SetScale),
            SCENECAMERA_FUNC(Position),
            SCENECAMERA_FUNC(Rotation),
            SCENECAMERA_FUNC(Scale),
            SCENECAMERA_FUNC(Direction),
            SCENECAMERA_FUNC(Up),
            SCENECAMERA_FUNC(Right_XPositive),
            SCENECAMERA_FUNC(Move),
            SCENECAMERA_FUNC(Turn),
            SCENECAMERA_FUNC(Zoom),
            SCENECAMERA_FUNC(SetZIsUp),
            SCENECAMERA_FUNC(ZIsUp),
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
