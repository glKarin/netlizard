#ifndef _KARIN_LUA_DEF_H
#define _KARIN_LUA_DEF_H

#define GET_LUA_CALLER(L, T, name) \
    T *name = *((T **)(lua_touserdata(L, 1)))

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
}

#endif // _KARIN_LUA_DEF_H
