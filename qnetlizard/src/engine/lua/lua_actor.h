#ifndef _KARIN_LUA_ACTOR_H
#define _KARIN_LUA_ACTOR_H

#include <QString>

struct lua_State;
class NLActor;

namespace NL
{
bool actor_registe_metatable(struct lua_State *L);
bool actor_registe_global_object(struct lua_State *L, NLActor *actor, const QString &name);
}

#endif // _KARIN_LUA_ACTOR_H
