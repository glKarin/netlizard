#include "lua_object.h"

#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlobject.h"
#include "lua_def.h"

#define CALLER_OBJECT(L, name) GET_LUA_CALLER(L, NLObject, name)
#define CALLER_OBJECT_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLObject, name)

static int Object_Name(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    QByteArray ba = obj->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Object_ClassName(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    QByteArray ba = obj->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Object_SetEnabled(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    int b = lua_toboolean(L, 2);
    obj->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Object_IsEnabled(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    int i = obj->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, i);
    return 1;
}

static int Object_GetProperty(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    const char *name = lua_tostring(L, 2);
    QVariant v = obj->GetProperty(name);
    if(v.canConvert<QByteArray>())
    {
        QByteArray ba = v.toByteArray();
        lua_pushstring(L, ba.constData());
    }
    else
        lua_pushnil(L);
    return 1;
}

static int Object_SetProperty(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    const char *name = lua_tostring(L, 2);
    int res = 1;
    if(lua_isinteger(L, 3))
    {
        int i = lua_tointeger(L, 3);
        obj->SetProperty(name, i);
    }
    else
    {
        int type = lua_type(L, 3);
        if(type == LUA_TNUMBER)
        {
            float f = lua_tonumber(L, 3);
            obj->SetProperty(name, f);
        }
        else if(type == LUA_TBOOLEAN)
        {
            bool b = lua_toboolean(L, 3);
            obj->SetProperty(name, b ? true : false);
        }
        else if(type == LUA_TSTRING)
        {
            const char *s = lua_tostring(L, 3);
            obj->SetProperty(name, s);
        }
        else if(type == LUA_TLIGHTUSERDATA)
        {
            void *p = (void *)(lua_touserdata(L, 3));
            if(p)
            {
                QObject *o = reinterpret_cast<QObject *>(p);
                if(o)
                    obj->SetProperty(name, QVariant::fromValue<QObject *>(o));
                else
                    res = 0;
            }
            else
                obj->SetProperty(name, QVariant::fromValue<QObject *>(0));
        }
        else if(type == LUA_TUSERDATA)
        {
            void **p = (void **)(lua_touserdata(L, 3));
            if(*p)
            {
                QObject *o = reinterpret_cast<QObject *>(*p);
                if(o)
                    obj->SetProperty(name, QVariant::fromValue<QObject *>(o));
                else
                    res = 0;
            }
            else
                obj->SetProperty(name, QVariant::fromValue<QObject *>(0));
        }
        else
        {
            obj->SetProperty(name, QVariant());
        }
    }
    lua_pushboolean(L, res);
    return 1;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Object_##x); \
    lua_setfield(L, -2, #x);
#define OBJECT_FUNC(x) {#x, Object_##x}
bool register_object_metatable_function(struct lua_State *L)
{
    const struct luaL_Reg Funcs[] = {
        OBJECT_FUNC(Name),
        OBJECT_FUNC(ClassName),
        OBJECT_FUNC(SetEnabled),
        OBJECT_FUNC(IsEnabled),
        OBJECT_FUNC(GetProperty),
        OBJECT_FUNC(SetProperty),
        NULL_luaL_Reg
    };
    luaL_setfuncs(L, Funcs, 0);
    return true;
}

}
