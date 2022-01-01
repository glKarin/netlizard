#ifndef _KARIN_LUA_ACTOR_H
#define _KARIN_LUA_ACTOR_H

struct lua_State;

namespace NL
{
bool actor_register_metatable(struct lua_State *L);
bool rigidbody_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_ACTOR_H
