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
}
