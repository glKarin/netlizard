#ifndef _KARIN_LUA_COMPONENT_H
#define _KARIN_LUA_COMPONENT_H

struct lua_State;

namespace NL
{
bool component_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_COMPONENT_H
