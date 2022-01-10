#include "lua_object.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QRegExp>
#include <QDebug>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "nlobject.h"
#include "lua_def.h"

#define CALLER_OBJECT(L, name) GET_LUA_CALLER(L, NLObject, name)
#define CALLER_OBJECT_USERDATA(L, name) GET_LUA_CALLER_USERDATA(L, NLObject, name)

typedef struct GeneralData_s {
    union {
        bool b;
        int i;
        float f;
        void *p;
        const char *s;
    } basic;
    QString str;
} GeneralData;

static int Object_Name(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    QByteArray ba = obj->Name().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Object_ClassName(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    QByteArray ba = obj->ClassName().toLocal8Bit();
    lua_pushstring(L, ba.constData());
    return 1;
}

static int Object_SetEnabled(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    int b = lua_toboolean(L, 2);
    obj->SetEnabled(b ? true : false);
    lua_pushboolean(L, 1);
    return 1;
}

static int Object_IsEnabled(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    int i = obj->IsEnabled() ? 1 : 0;
    lua_pushboolean(L, i);
    return 1;
}

static int Object_GetProperty(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    const char *name = lua_tostring(L, 2);
    QVariant v = obj->GetProperty(name);
    if(v.canConvert<QByteArray>())
    {
        QByteArray ba = v.toByteArray();
        lua_pushstring(L, ba.constData());
    }
    else
        lua_pushnil(L);
    return 1;
}

static int Object_SetProperty(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    const char *name = lua_tostring(L, 2);
    int res = 1;
    if(lua_isinteger(L, 3))
    {
        int i = lua_tointeger(L, 3);
        obj->SetProperty(name, i);
    }
    else
    {
        int type = lua_type(L, 3);
        if(type == LUA_TNUMBER)
        {
            float f = lua_tonumber(L, 3);
            obj->SetProperty(name, f);
        }
        else if(type == LUA_TBOOLEAN)
        {
            bool b = lua_toboolean(L, 3);
            obj->SetProperty(name, b ? true : false);
        }
        else if(type == LUA_TSTRING)
        {
            const char *s = lua_tostring(L, 3);
            obj->SetProperty(name, s);
        }
        else if(type == LUA_TLIGHTUSERDATA)
        {
            void *p = (void *)(lua_touserdata(L, 3));
            if(p)
            {
                QObject *o = reinterpret_cast<QObject *>(p);
                if(o)
                    obj->SetProperty(name, QVariant::fromValue<QObject *>(o));
                else
                    res = 0;
            }
            else
                obj->SetProperty(name, QVariant::fromValue<QObject *>(0));
        }
        else if(type == LUA_TUSERDATA)
        {
            void **p = (void **)(lua_touserdata(L, 3));
            if(*p)
            {
                QObject *o = reinterpret_cast<QObject *>(*p);
                if(o)
                    obj->SetProperty(name, QVariant::fromValue<QObject *>(o));
                else
                    res = 0;
            }
            else
                obj->SetProperty(name, QVariant::fromValue<QObject *>(0));
        }
        else
        {
            obj->SetProperty(name, QVariant());
        }
    }
    lua_pushboolean(L, res);
    return 1;
}

static int Object_RemoveProperty(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    const char *name = lua_tostring(L, 2);
    obj->RemoveProperty(name);
    lua_pushboolean(L, 1);
    return 1;
}

static int Object_Invoke(lua_State *L)
{
    CALLER_OBJECT(L, obj);
    int res = 0;
    int resNum = 1;

    const QMetaObject *mo = obj->metaObject();
    const char *methodName = lua_tostring(L, 2);
    QByteArray methodBa = QMetaObject::normalizedSignature(methodName);
    int index = mo->indexOfMethod(methodBa.constData());

    if(index >= 0)
    {
        int top = lua_gettop(L);
        QMetaMethod method = mo->method(index);
        QByteArray returnType;
        if(top >= 3)
            returnType = QMetaObject::normalizedType(lua_tostring(L, 3));
        QGenericArgument args[10] = { QGenericArgument(0) };
        GeneralData datas[10];

        for(int i = 4, index = 0; i <= top && index < 10; i++, index++)
        {
            if(lua_isinteger(L, i))
            {
                datas[index].basic.i = lua_tointeger(L, i);
                args[index] = Q_ARG(int, datas[index].basic.i);
            }
            else
            {
                int type = lua_type(L, i);
                if(type == LUA_TNUMBER)
                {
                    datas[index].basic.f = lua_tonumber(L, i);
                    args[index] = Q_ARG(float, datas[index].basic.f);
                }
                else if(type == LUA_TBOOLEAN)
                {
                    datas[index].basic.b = lua_toboolean(L, i) ? true : false;
                    args[index] = Q_ARG(bool, datas[index].basic.b);
                }
                else if(type == LUA_TSTRING)
                {
                    datas[index].str = lua_tostring(L, i);
                    args[index] = Q_ARG(QString, datas[index].str);
                }
                else if(type == LUA_TUSERDATA)
                {
                    void **p = (void **)(lua_touserdata(L, i));
                    datas[index].basic.p = *p;
                    args[index] = Q_ARG(void *, datas[index].basic.p);
                }
                else if(type == LUA_TLIGHTUSERDATA)
                {
                    void *p = (void *)(lua_touserdata(L, i));
                    datas[index].basic.p = p;
                    args[index] = Q_ARG(void *, datas[index].basic.p);
                }
                else
                {
                    datas[index].basic.p = 0;
                    args[index] = Q_ARG(void *, 0);
                }
            }
        }
        bool noReturn = returnType.isEmpty() || returnType == "void";
        if(noReturn)
        {
            res = method.invoke(obj, Qt::DirectConnection, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
            lua_pushboolean(L, res);
        }
        else
        {
            GeneralData retData;
            QGenericReturnArgument ret(returnType, returnType == "QString" ? (void *)&retData.str : (void *)&retData.basic);
            res = method.invoke(obj, Qt::DirectConnection, ret, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
            lua_pushboolean(L, res);
            if(res)
            {
                resNum = 2;
                if(returnType == "float"
                        || returnType == "double"
                        || returnType == "long double"
                        )
                {
                    lua_pushnumber(L, retData.basic.f);
                }
                else if(returnType == "int"
                        || returnType == "char"
                        || returnType == "short"
                        || returnType == "long"
                        || returnType == "long long"
                        || returnType == "signed int"
                        || returnType == "signed char"
                        || returnType == "signed short"
                        || returnType == "signed long"
                        || returnType == "signed long long"
                        || returnType == "unsigned"
                        || returnType == "unsigned int"
                        || returnType == "unsigned char"
                        || returnType == "unsigned short"
                        || returnType == "unsigned long"
                        || returnType == "unsigned long long"
                        )
                {
                    lua_pushnumber(L, retData.basic.i);
                }
                else if(returnType == "bool")
                {
                    lua_pushboolean(L, retData.basic.b ? 1 : 0);
                }
                else
                {
                    if(returnType.indexOf("*") >= 0)
                    {
                        QString bas(returnType);
                        QString fba = bas.replace(QRegExp("^const\\s*"), "").replace("*", "").trimmed();
                        if(retData.basic.p)
                        {
                            void **p = (void **)lua_newuserdata(L, sizeof(void *));
                            *p = retData.basic.p;
                            if(fba != "void")
                            {
                                QByteArray fbab = fba.toLocal8Bit();
                                if(luaL_getmetatable(L, fbab.constData()) != LUA_TTABLE)
                                {
                                    lua_pop(L, 1);
                                }
                                else
                                {
                                    lua_setmetatable(L, -2);
                                }
                            }
                        }
                        else
                            lua_pushnil(L);
                    }
                    else
                    {
                        if(returnType == "QString")
                        {
                            QByteArray ba = retData.str.toLocal8Bit();
                            lua_pushstring(L, ba.constData());
                        }
                        else
                        {
                            resNum = 1;
                        }
                    }
                }
            }
        }
    }
    else
        lua_pushboolean(L, res);
    return resNum;
}

namespace NL
{

#define REG_FUNC(x) \
    lua_pushcfunction(L, Object_##x); \
    lua_setfield(L, -2, #x);
#define OBJECT_FUNC(x) {#x, Object_##x}
bool register_object_metatable_function(struct lua_State *L)
{
    const struct luaL_Reg Funcs[] = {
        OBJECT_FUNC(Name),
        OBJECT_FUNC(ClassName),
        OBJECT_FUNC(SetEnabled),
        OBJECT_FUNC(IsEnabled),
        OBJECT_FUNC(GetProperty),
        OBJECT_FUNC(SetProperty),
        OBJECT_FUNC(RemoveProperty),
        OBJECT_FUNC(Invoke),
        NULL_luaL_Reg
    };
    luaL_setfuncs(L, Funcs, 0);
    return true;
}

}
