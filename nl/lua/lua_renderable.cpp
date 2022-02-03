#include "lua_renderable.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "engine/nlrenderable.h"
#include "lua_def.h"
#include "lua_object.h"
#include "lua_variant.h"

#define CALLER_RENDERABLE(L, name) NLNLGET_LUA_CALLER(L, NLRenderable, name)
#define CALLER_RENDERABLE_USERDATA(L, name) NLNLGET_LUA_CALLER_USERDATA(L, NLRenderable, name)

static int Renderable_delete(lua_State *L)
{
    CALLER_RENDERABLE_USERDATA(L, rend);
    delete *rend;
    *rend = 0;
    return 0;
}

static int Renderable_Actor(lua_State *L)
{
    CALLER_RENDERABLE(L, rend);
    NLActor *a = rend->Actor();
    if(a)
    {
        NLPUSH_NLOBJECT_TO_STACK(L, NLActor, a)
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Renderable_##x); \
    lua_setfield(L, -2, #x);
#define RENDERABLE_FUNC(x) {#x, Renderable_##x}
bool renderable_register_metatable(struct lua_State *L)
{
    if(metatable_is_register(L, "NLRenderable"))
        return true;

    lua_register(L, "delete_NLRenderable", Renderable_delete);

    if(luaL_newmetatable(L, "NLRenderable"))
    {
        const struct luaL_Reg Funcs[] = {
            RENDERABLE_FUNC(Actor),
            NLNULL_luaL_Reg
        };
        luaL_setfuncs(L, Funcs, 0);
        register_object_metatable_function(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
        return true;
    }
    return false;
}

}
