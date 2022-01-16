#ifndef _KARIN_LUA_SCENECAMERA_H
#define _KARIN_LUA_SCENECAMERA_H

struct lua_State;

namespace NL
{
bool scenecamera_register_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_SCENECAMERA_H
