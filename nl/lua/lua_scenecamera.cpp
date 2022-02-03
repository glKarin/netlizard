#include "lua_scenecamera.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "engine/nlscenecamera.h"
#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "lua_def.h"
#include "lua_variant.h"

#define CALLER_SCENECAMERA(L, name) NLNLGET_LUA_CALLER(L, NLSceneCamera, name)
#define CALLER_SCENECAMERA_USERDATA(L, name) NLNLGET_LUA_CALLER_USERDATA(L, NLSceneCamera, name)

static int SceneCamera_new(lua_State *L)
{
    int top = lua_gettop(L);
    if(top == 0)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLSceneCamera, new NLSceneCamera)
    }
    else if(top == 1)
    {
        NLGET_LUA_OBJECT(L, NLScene, scene, 1);
        NLPUSH_NLOBJECT_TO_STACK(L, NLSceneCamera, new NLSceneCamera(scene))
    }
    else
    {
        NLGET_LUA_OBJECT(L, NLScene, scene, 1);
        int type = lua_tointeger(L, 2);
        NLPUSH_NLOBJECT_TO_STACK(L, NLSceneCamera, new NLSceneCamera(static_cast<NLSceneCamera::SceneCamera_Type>(type), scene))
    }
    return 1;
}

static int SceneCamera_delete(lua_State *L)
{
    CALLER_SCENECAMERA_USERDATA(L, camera);
    delete *camera;
    *camera = 0;
    return 0;
}

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
        NLPUSH_NLOBJECT_TO_STACK(L, NLScene, s)
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

static int SceneCamera_UpdateCamera(lua_State *L)
{
    CALLER_SCENECAMERA(L, camera);
    NLGET_LUA_OBJECT(L, NLActor, actor, 2);
    NLScene *scene = camera->Scene();
    camera->Update(scene->width(), scene->height());
    if(actor)
    {
        NLActor *pa = actor->ParentActor();
        if(pa)
            camera->SetGlobalMatrix(pa->GlobalMatrix());
        camera->SetPosition(actor->Position());
        camera->SetRotation(actor->Rotation());
    }
    lua_pushboolean(L, 1);
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

    lua_register(L, "new_NLSceneCamera", SceneCamera_new);
    lua_register(L, "delete_NLSceneCamera", SceneCamera_delete);
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
            SCENECAMERA_FUNC(UpdateCamera),
            NLNULL_luaL_Reg
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
