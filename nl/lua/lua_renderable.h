#ifndef _KARIN_LUA_RENDERABLE_H
#define _KARIN_LUA_RENDERABLE_H

struct lua_State;

namespace NL
{
bool renderable_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_RENDERABLE_H
