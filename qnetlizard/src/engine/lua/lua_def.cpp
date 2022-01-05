#include "lua_def.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
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
        QString value(lua_tostring(L, -1));
        props.Insert(key, value);
        lua_pop(L, 1);//因为上面Lua_next会把key出栈，不会把value出栈所以要把value出栈
    }

    if(notInTop)
        lua_pop(L, 1);
    return props;
}
}
