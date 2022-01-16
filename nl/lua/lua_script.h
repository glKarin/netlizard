#ifndef _KARIN_LUA_SCRIPT_H
#define _KARIN_LUA_SCRIPT_H

struct lua_State;

namespace NL
{
bool script_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_SCRIPT_H
