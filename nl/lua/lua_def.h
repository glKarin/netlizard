#ifndef _KARIN_LUA_DEF_H
#define _KARIN_LUA_DEF_H

class QString;

#define GET_LUA_OBJECT(L, T, name, index) \
    T *name = lua_isuserdata(L, index) ? *((T **)(lua_touserdata(L, index))) : 0

#define GET_LUA_OBJECT_USERDATA(L, T, name, index) \
    T **name = lua_isuserdata(L, index) ? (T **)(lua_touserdata(L, index)) : 0

#define GET_LUA_CALLER(L, T, name) GET_LUA_OBJECT(L, T, name, 1)

#define GET_LUA_CALLER_USERDATA(L, T, name) GET_LUA_OBJECT_USERDATA(L, T, name, 1)

#define PUSH_NLOBJECT_TO_STACK(L, T, data) {\
    T **ptr = (T **)lua_newuserdata(L, sizeof(T *)); \
    *ptr = data; \
    luaL_getmetatable(L, #T); \
    lua_setmetatable(L, -2); \
}

#define SET_GLOBAL_CFUNC(L, name, func) {\
    lua_pushcfunction(L, func); \
    lua_setglobal(L, name); \
}

#define NULL_luaL_Reg {NULL, NULL}

#define NLVARTYPE_C_INT "int"
#define NLVARTYPE_C_FLOAT "float"
#define NLVARTYPE_C_STRING "String"
#define NLVARTYPE_C_QSTRING "QString"
#define NLVARTYPE_C_BOOL "bool"
#define NLVARTYPE_C_VOIDPTR "void*"
#define NLVARTYPE_LUA_INT "integer"
#define NLVARTYPE_LUA_FLOAT "number"
#define NLVARTYPE_LUA_BOOL "bool"
#define NLVARTYPE_LUA_STRING "string"
#define NLVARTYPE_LUA_LIGHTUSERDATA "lightuserdata"

struct lua_State;

namespace NL
{
bool metatable_is_register(struct lua_State *L, const char *name);
QString get_metatable_name(struct lua_State *L, int index);
QString get_metatable_name(struct lua_State *L, const QString &name);
}

#endif // _KARIN_LUA_DEF_H
