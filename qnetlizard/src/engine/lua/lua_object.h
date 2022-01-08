#ifndef _KARIN_LUA_OBJECT_H
#define _KARIN_LUA_OBJECT_H

struct lua_State;

namespace NL
{
bool register_object_metatable_function(struct lua_State *L);
}

#endif // _KARIN_LUA_OBJECT_H
