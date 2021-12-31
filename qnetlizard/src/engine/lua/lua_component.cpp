#include "lua_component.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlcomponent.h"

static int component_Name(lua_State *L)
{
    NLComponent *comp = (NLComponent *)(lua_touserdata(L, 1));
    QByteArray ba = comp->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int component_ClassName(lua_State *L)
{
    NLComponent *comp = (NLComponent *)(lua_touserdata(L, 1));
    QByteArray ba = comp->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int component_SetEnabled(lua_State *L)
{
    NLComponent *comp = (NLComponent *)(lua_touserdata(L, 1));
    int b = lua_toboolean(L, 2);
    comp->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int component_Actor(lua_State *L)
{
    NLComponent *comp = (NLComponent *)(lua_touserdata(L, 1));
    NLActor *a = comp->Actor();
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

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, component_##x); \
    lua_setfield(L, -2, #x);
bool component_registe_metatable(struct lua_State *L)
{
    if(luaL_newmetatable(L, "NLComponent"))
    {
        REG_FUNC(Name);
        REG_FUNC(ClassName);
        REG_FUNC(SetEnabled);
        REG_FUNC(Actor);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        qDebug() << "component_register";
        return true;
    }
    return false;
}

}
