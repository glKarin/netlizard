#ifndef _KARIN_LUA_GLOBALS_H
#define _KARIN_LUA_GLOBALS_H

struct lua_State;

namespace NL
{
bool register_lua_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_GLOBALS_H
