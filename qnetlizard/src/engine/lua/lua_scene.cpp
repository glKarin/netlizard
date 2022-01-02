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

static int Scene_SetClearColor(lua_State *L)
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

static int Scene_GetActor(lua_State *L)
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
        PUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Scene_ActorCount(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = scene->ActorCount();
    lua_pushinteger(L, i);
    return 1;
}

static int Scene_CurrentFPS(lua_State *L)
{
    CALLER_SCENE(L, scene);
    float f = scene->CurrentFPS();
    lua_pushnumber(L, f);
    return 1;
}

static int Scene_MousePointerPosition(lua_State *L)
{
    CALLER_SCENE(L, scene);
    QPoint p = scene->MousePointerPosition();
    lua_pushinteger(L, p.x());
    lua_pushinteger(L, p.y());
    return 2;
}

static int Scene_MoveCursorToCenter(lua_State *L)
{
    CALLER_SCENE(L, scene);
    scene->MoveCursorToCenter();
    lua_pushboolean(L, 1);
    return 1;
}

static int Scene_SetCursorVisible(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int b = lua_toboolean(L, 2);
    scene->SetCursorVisible(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Scene_GrabMouseCursor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int b = lua_toboolean(L, 2);
    scene->GrabMouseCursor(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Scene_IsCursorVisible(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = scene->IsCursorVisible();
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_IsGrabMouseCursor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = scene->IsGrabMouseCursor();
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_CurrendDelta(lua_State *L)
{
    CALLER_SCENE(L, scene);
    float f = scene->CurrendDelta();
    lua_pushnumber(L, f);
    return 1;
}

static int Scene_KeyState(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = lua_tointeger(L, 2);
    bool b = scene->KeyState(i);
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_MouseState(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int i = lua_tointeger(L, 2);
    bool b = scene->MouseState(i);
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_CurrentCamera(lua_State *L)
{
    CALLER_SCENE(L, scene);
    NLSceneCamera *c = scene->CurrentCamera();
    if(c)
    {
        PUSH_NLOBJECT_TO_STACK(L, NLSceneCamera, c)
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
    lua_pushcfunction(L, Scene_##x); \
    lua_setfield(L, -2, #x);
#define SCENE_FUNC(x) {#x, Scene_##x}
bool scene_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLScene"))
        return true;
    if(luaL_newmetatable(L, "NLScene"))
    {
        const struct luaL_Reg Funcs[] = {
            SCENE_FUNC(SetClearColor),
            SCENE_FUNC(GetActor),
            SCENE_FUNC(ActorCount),
            SCENE_FUNC(CurrentFPS),
            SCENE_FUNC(MousePointerPosition),
            SCENE_FUNC(MoveCursorToCenter),
            SCENE_FUNC(SetCursorVisible),
            SCENE_FUNC(GrabMouseCursor),
            SCENE_FUNC(IsCursorVisible),
            SCENE_FUNC(IsGrabMouseCursor),
            SCENE_FUNC(CurrendDelta),
            SCENE_FUNC(KeyState),
            SCENE_FUNC(MouseState),
            SCENE_FUNC(CurrentCamera),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "Scene_register";
        return true;
    }
    return false;
}

}
