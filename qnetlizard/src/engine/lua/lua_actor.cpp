#include "lua_actor.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlactor.h"

static int actor_set_position(lua_State *L)
{
    qDebug() << "actor_set_position";
    NLActor *actor = (NLActor *)(lua_touserdata(L, 1));
    qDebug() << actor;
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    qDebug() << actor << x << y << z;
    actor->SetPosition(pos);
    lua_pushboolean(L, 1);
    return 1;
}

static int actor_set_rotation(lua_State *L)
{
    qDebug() << "actor_set_position";
    NLActor *actor = (NLActor *)(lua_touserdata(L, 1));
    qDebug() << actor;
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    NLVector3 pos = VECTOR3(x, y, z);
    qDebug() << actor << x << y << z;
    actor->SetRotation(pos);
    lua_pushboolean(L, 1);
    return 1;
}

namespace NL
{

bool actor_registe_global_object(struct lua_State *L, NLActor *actor, const QString &name)
{
    lua_pushlightuserdata(L, actor);
    qDebug() << "luaL_getmetatable" <<luaL_getmetatable(L, "NLActor");
    qDebug() << "lua_setmetatable"<<lua_setmetatable(L, -2);
    QByteArray ba = name.toLocal8Bit();
    lua_setglobal(L, ba.constData());
    return true;
}

bool actor_registe_metatable(struct lua_State *L)
{
    if(luaL_newmetatable(L, "NLActor"))
    {
        lua_pushcfunction(L, actor_set_position);
        lua_setfield(L, -2, "SetPosition");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        qDebug() << "actor_register";
        return true;
    }
    return false;
}

}
