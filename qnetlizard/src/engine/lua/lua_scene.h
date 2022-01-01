#ifndef _KARIN_LUA_SCENE_H
#define _KARIN_LUA_SCENE_H

struct lua_State;

namespace NL
{
bool scene_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_SCENE_H
