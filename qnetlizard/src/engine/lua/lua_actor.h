#ifndef _KARIN_LUA_ACTOR_H
#define _KARIN_LUA_ACTOR_H

struct lua_State;
class NLActor;

namespace NL
{
bool actor_registe_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_ACTOR_H
