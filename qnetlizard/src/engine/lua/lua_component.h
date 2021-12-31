#ifndef _KARIN_LUA_COMPONENT_H
#define _KARIN_LUA_COMPONENT_H

#include <QString>

struct lua_State;
class NLComponent;

namespace NL
{
bool component_registe_metatable(struct lua_State *L);
}

#endif // _KARIN_LUA_COMPONENT_H
