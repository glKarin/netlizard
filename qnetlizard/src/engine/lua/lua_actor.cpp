#include "lua_actor.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlactor.h"
#include "nlrigidbody.h"
#include "lua_def.h"

#define CALLER_ACTOR(L, name) GET_LUA_CALLER(L, NLActor, name)
#define CALLER_ACTOR_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLActor, name)
#define CALLER_RIGIDBODY(L, name) GET_LUA_CALLER(L, NLRigidbody, name)
#define CALLER_RIGIDBODY_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLRigidbody, name)

static int Actor_new(lua_State *L)
{
    PUSH_NLOBJECT_TO_STACK(L, NLActor, new NLActor)
    return 1;
}

static int Actor_delete(lua_State *L)
{
    CALLER_ACTOR_USERDATA(L, actor);
    delete *actor;
    *actor = 0;
    lua_pushboolean(L, 1);
    return 1;
}

static int Actor_SetPosition(lua_State *L)
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

static int Actor_Name(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    QByteArray ba = actor->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Actor_ClassName(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    QByteArray ba = actor->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Actor_SetEnabled(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int b = lua_toboolean(L, 2);
    actor->SetEnabled(b ? true : false);
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
        PUSH_NLOBJECT_TO_STACK(L, NLActor, a)
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
        PUSH_NLOBJECT_TO_STACK(L, NLActor, a)
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
        PUSH_NLOBJECT_TO_STACK(L, NLComponent, c)
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
        PUSH_NLOBJECT_TO_STACK(L, NLScene, s)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int Actor_IsEnabled(lua_State *L)
{
    CALLER_ACTOR(L, actor);
    int i = actor->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, i);
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
        PUSH_NLOBJECT_TO_STACK(L, NLRigidbody, r)
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
    GET_LUA_OBJECT(L, NLActor, c, 2);
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
            GET_LUA_OBJECT(L, NLActor, c, 2);
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



static int Rigidbody_new(lua_State *L)
{
    PUSH_NLOBJECT_TO_STACK(L, NLActor, new NLRigidbody)
    return 1;
}

static int Rigidbody_delete(lua_State *L)
{
    CALLER_RIGIDBODY_USERDATA(L, rb);
    delete *rb;
    *rb = 0;
    lua_pushboolean(L, 1);
    return 1;
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
    ACTOR_FUNC(Name), \
    ACTOR_FUNC(ClassName), \
    ACTOR_FUNC(SetEnabled), \
    ACTOR_FUNC(GetChild), \
    ACTOR_FUNC(ChildrenCount), \
    ACTOR_FUNC(ParentActor), \
    ACTOR_FUNC(GetComponent), \
    ACTOR_FUNC(ComponentCount), \
    ACTOR_FUNC(Scene), \
    ACTOR_FUNC(IsRigidbody), \
    ACTOR_FUNC(ToRigidbody), \
    ACTOR_FUNC(IsEnabled), \
    ACTOR_FUNC(CanRender), \
    ACTOR_FUNC(AddChild), \
    ACTOR_FUNC(RemoveChild)

bool actor_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLActor"))
        return true;

    SET_GLOBAL_CFUNC(L, "new_NLActor", Actor_new)
    SET_GLOBAL_CFUNC(L, "delete_NLActor", Actor_delete)

    if(luaL_newmetatable(L, "NLActor"))
    {
        const struct luaL_Reg Funcs[] = {
            ACTOR_FUNCS,
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "Actor_register";
        return true;
    }

    return false;
}

#define RIGIDBODY_FUNC(x) {#x, Rigidbody_##x}
bool rigidbody_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLRigidbody"))
        return true;
    SET_GLOBAL_CFUNC(L, "new_NLRigidbody", Rigidbody_new)
    SET_GLOBAL_CFUNC(L, "delete_NLRigidbody", Rigidbody_delete)

    if(luaL_newmetatable(L, "NLRigidbody"))
    {
        const struct luaL_Reg Funcs[] = {
            ACTOR_FUNCS,
            RIGIDBODY_FUNC(MoveDirection),
            NULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        qDebug() << "Actor_register";
        return true;
    }
    return false;
}
}
