#include "lua_def.h"

#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

namespace NL
{
bool metatable_is_register(struct lua_State *L, const char *name)
{
    int res = luaL_getmetatable(L, name);
    lua_pop(L, 1);
    return res == LUA_TTABLE ? true : false;
}

QString get_metatable_name(struct lua_State *L, int index)
{
    int res = lua_getmetatable(L, index);
    if(res == 0)
        return QString();

    QString ret;
    lua_pushstring(L, "__name");
    res = lua_rawget(L, -2);
    if(res == LUA_TSTRING)
    {
        const char *name = lua_tostring(L, -1);
        ret = name;
        lua_pop(L, 2);
    }
    else
        lua_pop(L, 2);
    return ret;
}

QString get_metatable_name(struct lua_State *L, const QString &tname)
{
    QByteArray ba = tname.toLocal8Bit();
    int res = luaL_getmetatable(L, ba.constData());
    if(res != LUA_TTABLE)
        return QString();

    QString ret;
    lua_pushstring(L, "__name");
    res = lua_rawget(L, -2);
    if(res == LUA_TSTRING)
    {
        const char *name = lua_tostring(L, -1);
        ret = name;
        lua_pop(L, 2);
    }
    else
        lua_pop(L, 2);
    return ret;
}

}
