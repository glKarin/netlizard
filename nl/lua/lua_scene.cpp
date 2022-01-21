#include "lua_scene.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "engine/nlscene.h"
#include "lua_def.h"
#include "lua_variant.h"

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
    bool b = false;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = scene->KeyState(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        if(str && strlen(str) > 0)
            b = scene->KeyState(str[0]);
    }
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_MouseState(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = false;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = scene->MouseState(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        if(str && strlen(str) > 0)
            b = scene->MouseState(str[0]);
    }
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_KeyPressed(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = false;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = scene->KeyPressed(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        if(str && strlen(str) > 0)
            b = scene->KeyPressed(str[0]);
    }
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int Scene_MousePressed(lua_State *L)
{
    CALLER_SCENE(L, scene);
    bool b = false;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = scene->MousePressed(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        if(str && strlen(str) > 0)
            b = scene->MousePressed(str[0]);
    }
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

static int Scene_SetCurrentCamera(lua_State *L)
{
    CALLER_SCENE(L, scene);
    GET_LUA_OBJECT(L, NLSceneCamera, c, 2);
    int b = 1;
    scene->SetCurrentCamera(c);
    lua_pushboolean(L, b);
    return 1;
}

static int Scene_AddActor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    GET_LUA_OBJECT(L, NLActor, c, 2);
    int b = 1;
    scene->AddActor(c);
    lua_pushboolean(L, b);
    return 1;
}

static int Scene_RemoveActor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    int b = 1;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        scene->RemoveActor(i);
    }
    else
    {
        int type = lua_type(L, 2);
        if(type == LUA_TUSERDATA)
        {
            GET_LUA_OBJECT(L, NLActor, c, 2);
            if(c)
                scene->RemoveActor(c);
            else
                b = 0;
        }
        else
        {
            const char *s = lua_tostring(L, 2);
            scene->RemoveActor(s);
        }
    }
    lua_pushboolean(L, b);
    return 1;
}

static int Scene_CreateActor(lua_State *L)
{
    CALLER_SCENE(L, scene);
    NLActor *a = scene->CreateActor(NL::lua_table_to_properties(L, 2));
    PUSH_NLOBJECT_TO_STACK(L, NLActor, a)
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
            SCENE_FUNC(KeyPressed),
            SCENE_FUNC(MousePressed),
            SCENE_FUNC(AddActor),
            SCENE_FUNC(RemoveActor),
            SCENE_FUNC(SetCurrentCamera),
            SCENE_FUNC(CreateActor),
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
