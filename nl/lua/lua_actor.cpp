#include "lua_actor.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "engine/nlactor.h"
#include "engine/nlrigidbody.h"
#include "lua_def.h"
#include "lua_object.h"
#include "lua_variant.h"

#define CALLER_ACTOR(L, name) NLNLGET_LUA_CALLER(L, NLActor, name)
#define CALLER_ACTOR_USERDATA(L, name) NLNLGET_LUA_CALLER_USERDATA(L, NLActor, name)
#define CALLER_RIGIDBODY(L, name) NLNLGET_LUA_CALLER(L, NLRigidbody, name)
#define CALLER_RIGIDBODY_USERDATA(L, name) NLNLGET_LUA_CALLER_USERDATA(L, NLRigidbody, name)

static int Actor_new(lua_State *L)
{
    NLPUSH_NLOBJECT_TO_STACK(L, NLActor, new NLActor(NL::lua_table_to_properties(L, 1)))
    return 1;
}

static int Actor_delete(lua_State *L)
{
    CALLER_ACTOR_USERDATA(L, actor);
    delete *actor;
    *actor = 0;
    return 0;
}

static int Actor_SetPosition(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    float x = lua_tonumber(L, -1);
    float y = lua_tonumber(L, -2);
    float z = lua_tonumber(L, -3);
    NLVector3 pos = VECTOR3(x, y, z);
    actor->SetPosition(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int Actor_SetRotation(lua_State *L)
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

static int Actor_SetScale(lua_State *L)
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

static int Actor_Position(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Position();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Rotation(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Rotation();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Scale(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Scale();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Direction(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Direction();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Up(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Up();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Right(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    const NLVector3 pos = actor->Right();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

static int Actor_Move(lua_State *L)
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

static int Actor_Turn(lua_State *L)
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

static int Actor_Zoom(lua_State *L)
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

static int Actor_ChildrenCount(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->ChildrenCount();
    lua_pushinteger(L, i);
    return 1;
}

static int Actor_GetChild(lua_State *L)
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
        NLPUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_ParentActor(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLActor *a = actor->ParentActor();
    if(a)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_ComponentCount(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->ComponentCount();
    lua_pushinteger(L, i);
    return 1;
}

static int Actor_GetComponent(lua_State *L)
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
        NLPUSH_NLOBJECT_TO_STACK(L, NLComponent, c)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_Scene(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLScene *s = actor->Scene();
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

static int Actor_CanRender(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->CanRender() ? 1 : 0;
    lua_pushboolean(L, i);
    return 1;
}

static int Actor_IsRigidbody(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = dynamic_cast<NLRigidbody *>(actor) ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_ToRigidbody(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLRigidbody *r = dynamic_cast<NLRigidbody *>(actor);
    if(r)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLRigidbody, r)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_AddChild(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLGET_LUA_OBJECT(L, NLActor, c, 2);
    int b = 0;
    if(c)
        b = actor->AddChild(c) ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_RemoveChild(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = 0;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = actor->RemoveChild(i) ? 1 : 0;
    }
    else
    {
        int type = lua_type(L, 2);
        if(type == LUA_TUSERDATA)
        {
            NLGET_LUA_OBJECT(L, NLActor, c, 2);
            b = c && actor->RemoveChild(c) ? 1 : 0;
        }
        else
        {
            const char *s = lua_tostring(L, 2);
            b = actor->RemoveChild(s) ? 1 : 0;
        }
    }
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_AddComponent(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLGET_LUA_OBJECT(L, NLComponent, c, 2);
    int b = 0;
    if(c)
        b = actor->AddComponent(c) ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_RemoveComponent(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = 0;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = actor->RemoveComponent(i) ? 1 : 0;
    }
    else
    {
        int type = lua_type(L, 2);
        if(type == LUA_TUSERDATA)
        {
            NLGET_LUA_OBJECT(L, NLComponent, c, 2);
            b = c && actor->RemoveComponent(c) ? 1 : 0;
        }
        else
        {
            const char *s = lua_tostring(L, 2);
            b = actor->RemoveComponent(s) ? 1 : 0;
        }
    }
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_ScriptCount(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->ScriptCount();
    lua_pushinteger(L, i);
    return 1;
}

static int Actor_GetScript(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLScript *c = NULL;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        c = actor->GetScript(i);
    }
    else
    {
        const char *str = lua_tostring(L, 2);
        c = actor->GetScript(str);
    }
    if(c)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLScript, c)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_AddScript(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLGET_LUA_OBJECT(L, NLScript, c, 2);
    int b = 0;
    if(c)
        b = actor->AddScript(c) ? 1 : 0;
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_RemoveScript(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = 0;
    if(lua_isinteger(L, 2))
    {
        int i = lua_tointeger(L, 2);
        b = actor->RemoveScript(i) ? 1 : 0;
    }
    else
    {
        int type = lua_type(L, 2);
        if(type == LUA_TUSERDATA)
        {
            NLGET_LUA_OBJECT(L, NLScript, c, 2);
            b = c && actor->RemoveScript(c) ? 1 : 0;
        }
        else
        {
            const char *s = lua_tostring(L, 2);
            b = actor->RemoveScript(s) ? 1 : 0;
        }
    }
    lua_pushboolean(L, b);
    return 1;
}

static int Actor_CreateChild(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLActor *a = actor->CreateChild(NL::lua_table_to_properties(L, 2));
    NLPUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    return 1;
}

static int Actor_CreateComponent(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLComponent *c = actor->CreateComponent(NL::lua_table_to_properties(L, 2));
    NLPUSH_NLOBJECT_TO_STACK(L, NLComponent, c)
    return 1;
}

static int Actor_CreateScript(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLScript *s = actor->CreateScript(NL::lua_table_to_properties(L, 2));
    NLPUSH_NLOBJECT_TO_STACK(L, NLScript, s)
    return 1;
}

static int Actor_Renderable(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLRenderable *r = actor->Renderable();
    if(r)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLRenderable, r)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_SetRenderable(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    NLGET_LUA_OBJECT(L, NLRenderable, r, 2);
    actor->SetRenderable(r);
    lua_pushboolean(L, 1);
    return 1;
}



static int Rigidbody_new(lua_State *L)
{
    NLPUSH_NLOBJECT_TO_STACK(L, NLActor, new NLRigidbody(NL::lua_table_to_properties(L, 1)))
    return 1;
}

static int Rigidbody_delete(lua_State *L)
{
    CALLER_RIGIDBODY_USERDATA(L, rb);
    delete *rb;
    *rb = 0;
    return 0;
}

static int Rigidbody_MoveDirection(lua_State *L)
{
    CALLER_RIGIDBODY(L, rb);
    const NLVector3 pos = rb->MoveDirection();
    lua_pushnumber(L, VECTOR3_X(pos));
    lua_pushnumber(L, VECTOR3_Y(pos));
    lua_pushnumber(L, VECTOR3_Z(pos));
    return 3;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Actor_##x); \
    lua_setfield(L, -2, #x);
#define ACTOR_FUNC(x) {#x, Actor_##x}
#define ACTOR_FUNCS \
    ACTOR_FUNC(SetPosition), \
    ACTOR_FUNC(SetRotation), \
    ACTOR_FUNC(SetScale), \
    ACTOR_FUNC(Position), \
    ACTOR_FUNC(Rotation), \
    ACTOR_FUNC(Scale), \
    ACTOR_FUNC(Direction), \
    ACTOR_FUNC(Up), \
    ACTOR_FUNC(Right), \
    ACTOR_FUNC(Move), \
    ACTOR_FUNC(Turn), \
    ACTOR_FUNC(Zoom), \
    ACTOR_FUNC(GetChild), \
    ACTOR_FUNC(ChildrenCount), \
    ACTOR_FUNC(ParentActor), \
    ACTOR_FUNC(GetComponent), \
    ACTOR_FUNC(ComponentCount), \
    ACTOR_FUNC(Scene), \
    ACTOR_FUNC(IsRigidbody), \
    ACTOR_FUNC(ToRigidbody), \
    ACTOR_FUNC(CanRender), \
    ACTOR_FUNC(AddChild), \
    ACTOR_FUNC(RemoveChild), \
    ACTOR_FUNC(AddComponent), \
    ACTOR_FUNC(RemoveComponent), \
    ACTOR_FUNC(AddScript), \
    ACTOR_FUNC(RemoveScript), \
    ACTOR_FUNC(GetScript), \
    ACTOR_FUNC(ScriptCount), \
    ACTOR_FUNC(CreateChild), \
    ACTOR_FUNC(CreateComponent), \
    ACTOR_FUNC(CreateScript), \
    ACTOR_FUNC(Renderable), \
    ACTOR_FUNC(SetRenderable)

bool actor_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLActor"))
        return true;

    lua_register(L, "new_NLActor", Actor_new);
    lua_register(L, "delete_NLActor", Actor_delete);

    if(luaL_newmetatable(L, "NLActor"))
    {
        const struct luaL_Reg Funcs[] = {
            ACTOR_FUNCS,
            NLNULL_luaL_Reg
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

#define RIGIDBODY_FUNC(x) {#x, Rigidbody_##x}
bool rigidbody_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLRigidbody"))
        return true;
    lua_register(L, "new_NLRigidbody", Rigidbody_new);
    lua_register(L, "delete_NLRigidbody", Rigidbody_delete);

    if(luaL_newmetatable(L, "NLRigidbody"))
    {
        const struct luaL_Reg Funcs[] = {
            ACTOR_FUNCS,
            RIGIDBODY_FUNC(MoveDirection),
            NLNULL_luaL_Reg
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
