#ifndef _KARIN_LUA_DEF_H
#define _KARIN_LUA_DEF_H

#include "nlproperties.h"

#define GET_LUA_OBJECT(L, T, name, index) \
    T *name = lua_isuserdata(L, index) ? *((T **)(lua_touserdata(L, index))) : 0

#define GET_LUA_OBJECT_USERDATA(L, T, name, index) \
    T **name = lua_isuserdata(L, index) ? (T **)(lua_touserdata(L, index)) : 0

#define GET_LUA_CALLER(L, T, name) GET_LUA_OBJECT(L, T, name, 1)

#define GET_LUA_CALLER_USERDATA(L, T, name) GET_LUA_OBJECT_USERDATA(L, T, name, 1)

#define PUSH_NLOBJECT_TO_STACK(L, T, data) {\
    T **ptr = (T **)lua_newuserdata(L, sizeof(T **)); \
    *ptr = data; \
    luaL_getmetatable(L, #T); \
    lua_setmetatable(L, -2); \
}

#define SET_GLOBAL_CFUNC(L, name, func) {\
    lua_pushcfunction(L, func); \
    lua_setglobal(L, name); \
}

#define NULL_luaL_Reg {NULL, NULL}

struct lua_State;

namespace NL
{
bool metatable_is_register(struct lua_State *L, const char *name);
NLProperties lua_table_to_properties(lua_State *L, int index);
}

#endif // _KARIN_LUA_DEF_H
