#include "lua_def.h"

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

namespace NL
{
bool metatable_is_register(struct lua_State *L, const char *name)
{
    int res = luaL_getmetatable(L, name);
    lua_pop(L, 1);
    return res == LUA_TTABLE ? true : false;
}

NLProperties lua_table_to_properties(lua_State *L, int index)
{
    NLProperties props;
    int top = lua_gettop(L);
    if(top == 0)
        return props;
    if(lua_type(L, index) != LUA_TTABLE)
        return props;
    const bool notInTop = !(index == -1 || index == top);
    if(notInTop)
        lua_pushvalue(L, index);

    lua_pushnil(L); //先在栈顶压入一个空的，因为lua_next会先弹出一个key
    while (lua_next(L, -2) != 0)//表在栈底
    {
         //先push key,push value
        QString key(lua_tostring(L, -2));

        if(lua_isinteger(L, -1))
        {
            int i = lua_tointeger(L, -1);
            props.Insert(key, i);
        }
        else
        {
            int type = lua_type(L, -1);
            if(type == LUA_TNUMBER)
            {
                float f = lua_tonumber(L, -1);
                props.Insert(key, f);
            }
            else if(type == LUA_TBOOLEAN)
            {
                bool b = lua_toboolean(L, -1) ? true : false;
                props.Insert(key, b);
            }
            else if(type == LUA_TSTRING)
            {
                const char *s = lua_tostring(L, -1);
                props.Insert(key, QString(s));
            }
            else if(type == LUA_TLIGHTUSERDATA)
            {
                void *p = (void *)(lua_touserdata(L, -1));
                props.Insert(key, QVariant::fromValue<void *>(p));
            }
            else if(type == LUA_TUSERDATA)
            {
                void **p = (void **)(lua_touserdata(L, -1));
                if(*p)
                {
                    QObject *o = reinterpret_cast<QObject *>(*p);
                    props.Insert(key, QVariant::fromValue<QObject *>(o));
                }
                else
                    props.Insert(key, QVariant::fromValue<QObject *>(0));
            }
            else
            {
            }
        }

        lua_pop(L, 1);//因为上面Lua_next会把key出栈，不会把value出栈所以要把value出栈
    }

    if(notInTop)
        lua_pop(L, 1);
    return props;
}
}
