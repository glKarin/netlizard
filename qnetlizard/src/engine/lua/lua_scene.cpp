#include "lua_scene.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlscene.h"
#include "lua_def.h"

#define CALLER_SCENE(L, name) GET_LUA_CALLER(L, NLScene, name)

static int scene_SetClearColor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    float r = lua_tonumber(L, 2);
    float g = lua_tonumber(L, 3);
    float b = lua_tonumber(L, 4);
    float a = lua_tonumber(L, 5);
    scene->SetClearColor(QColor::fromRgbF(r, g, b, a));
    lua_pushinteger(L, 1);
    return 1;
}

static int scene_GetActor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    NLActor *a = NULL;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        a = scene->GetActor(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        a = scene->GetActor(str);
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

static int scene_ActorCount(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = scene->ActorCount();
    lua_pushinteger(L, i);
    return 1;
}

static int scene_CurrentFPS(lua_State *L)
{
    CALLER_SCENE(L, scene);
    float f = scene->CurrentFPS();
    lua_pushnumber(L, f);
    return 1;
}

static int scene_MousePointerPosition(lua_State *L)
{
    CALLER_SCENE(L, scene);
    scene->MousePointerPosition();
    lua_pushboolean(L, 1);
    return 1;
}

static int scene_MoveCursorToCenter(lua_State *L)
{
    CALLER_SCENE(L, scene);
    scene->MoveCursorToCenter();
    lua_pushboolean(L, 1);
    return 1;
}

static int scene_SetCursorVisible(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int b = lua_toboolean(L, 2);
    scene->SetCursorVisible(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int scene_GrabMouseCursor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int b = lua_toboolean(L, 2);
    scene->GrabMouseCursor(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int scene_IsCursorVisible(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = scene->IsCursorVisible();
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int scene_IsGrabMouseCursor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = scene->IsGrabMouseCursor();
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int scene_CurrendDelta(lua_State *L)
{
    CALLER_SCENE(L, scene);
    float f = scene->CurrendDelta();
    lua_pushnumber(L, f);
    return 1;
}

static int scene_KeyState(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = lua_tointeger(L, 2);
    bool b = scene->KeyState(i);
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int scene_MouseState(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = lua_tointeger(L, 2);
    bool b = scene->MouseState(i);
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, scene_##x); \
    lua_setfield(L, -2, #x);
bool scene_registe_metatable(struct lua_State *L)
{
    if(luaL_newmetatable(L, "NLScene"))
    {
        REG_FUNC(SetClearColor);
        REG_FUNC(GetActor);
        REG_FUNC(ActorCount);
        REG_FUNC(CurrentFPS);
        REG_FUNC(MousePointerPosition);
        REG_FUNC(MoveCursorToCenter);
        REG_FUNC(SetCursorVisible);
        REG_FUNC(GrabMouseCursor);
        REG_FUNC(IsCursorVisible);
        REG_FUNC(IsGrabMouseCursor);
        REG_FUNC(CurrendDelta);
        REG_FUNC(KeyState);
        REG_FUNC(MouseState);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "scene_register";
        return true;
    }
    return false;
}

}
