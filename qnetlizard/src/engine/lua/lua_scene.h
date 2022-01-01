#ifndef _KARIN_LUA_SCENE_H
#define _KARIN_LUA_SCENE_H

struct lua_State;
class NLScene;

namespace NL
{
bool scene_registe_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_SCENE_H
