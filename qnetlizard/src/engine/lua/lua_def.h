#ifndef _KARIN_LUA_DEF_H
#define _KARIN_LUA_DEF_H

#define GET_LUA_CALLER(L, T, name) \
    T *name = (T *)(lua_touserdata(L, 1))

#endif // _KARIN_LUA_DEF_H
