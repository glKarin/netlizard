#include "lua_variant.h"

#include <QMetaObject>
#include <QWidget>
#include <QDebug>

extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}

#include "lua_def.h"
#include "common/nlglobals.h"
#include "engine/nlscenecamera.h"
#include "engine/nlscene.h"
#include "engine/nlactor.h"
#include "engine/nlcomponent.h"
#include "engine/nlrigidbody.h"
#include "engine/nlrenderable.h"
#include "engine/nlforce.h"
#include "engine/nlscript.h"

namespace NL
{
QVariant load_to_qvariant(struct lua_State* L, int varindex)
{
    QVariant var;
    if(lua_isinteger(L, varindex))
    {
        int i = lua_tointeger(L, varindex);
        var.setValue<int>(i);
    }
    else
    {
        int type = lua_type(L, varindex);
        if(type == LUA_TNUMBER)
        {
            float f = lua_tonumber(L, varindex);
            var.setValue<float>(f);
        }
        else if(type == LUA_TBOOLEAN)
        {
            bool b = lua_toboolean(L, varindex);
            var.setValue<bool>(b ? true : false);
        }
        else if(type == LUA_TSTRING)
        {
            const char *s = lua_tostring(L, varindex);
            var.setValue<QString>(s);
        }
        else if(type == LUA_TLIGHTUSERDATA)
        {
            void *p = (void *)(lua_touserdata(L, varindex));
            var.setValue<void *>(p);
        }
        else if(type == LUA_TUSERDATA)
        {
            void **p = (void **)(lua_touserdata(L, varindex));
            QString tableType = NL::get_metatable_name(L, varindex);
#define LOAD_VAR(cname) \
            if(tableType == #cname) \
            { \
                var.setValue<cname *>((cname *)*p); \
            }
            LOAD_VAR(NLSceneCamera)
            else LOAD_VAR(NLScene)
            else LOAD_VAR(NLComponent)
            else LOAD_VAR(NLScript)
            else LOAD_VAR(NLActor)
            else LOAD_VAR(NLRenderable)
            else LOAD_VAR(NLRigidbody)
            else LOAD_VAR(NLRenderable)
            else LOAD_VAR(NLForce)
            else
            {
                QVariant uv = NLEngineGlobals::Instance()->convert_lua_variant(p, tableType);
                if(uv.isValid())
                    var = uv;
                else
                    var.setValue<void *>(*p);
            }
#undef LOAD_VAR
        }
    }
    return var;
}

QVariant load_to_qvarianti(struct lua_State* L, int varindex, int type)
{
    const char *vartype = lua_typename(L, type);
    return load_to_qvariants(L, varindex, vartype);
}

QVariant load_to_qvariants(struct lua_State* L, int varindex, int typeindex)
{
    const char *vartype = lua_tostring(L, typeindex);
    return load_to_qvariants(L, varindex, vartype);
}

QVariant load_to_qvariants(struct lua_State* L, int varindex, const QString &vartype)
{
    QByteArray ba = vartype.toLocal8Bit();
    QByteArray utype(QMetaObject::normalizedType(ba.constData()));
    QVariant var;
    if(lua_isinteger(L, varindex) || (utype == NLVARTYPE_C_INT || utype == NLVARTYPE_LUA_INT))
    {
        int i = lua_tointeger(L, varindex);
        var.setValue<int>(i);
    }
    else
    {
        int type = lua_type(L, varindex);
        if(type == LUA_TNUMBER || (utype == NLVARTYPE_C_FLOAT || utype == NLVARTYPE_LUA_FLOAT))
        {
            float f = lua_tonumber(L, varindex);
            var.setValue<float>(f);
        }
        else if(type == LUA_TBOOLEAN || (utype == NLVARTYPE_C_BOOL || utype == NLVARTYPE_LUA_BOOL))
        {
            bool b = lua_toboolean(L, varindex);
            var.setValue<bool>(b ? true : false);
        }
        else if(type == LUA_TSTRING || (utype == NLVARTYPE_C_QSTRING || utype == NLVARTYPE_LUA_STRING || utype == NLVARTYPE_C_STRING))
        {
            const char *s = lua_tostring(L, varindex);
            var.setValue<QString>(s);
        }
        else if(type == LUA_TLIGHTUSERDATA || (utype == NLVARTYPE_C_VOIDPTR || utype == NLVARTYPE_LUA_LIGHTUSERDATA))
        {
            void *p = (void *)(lua_touserdata(L, varindex));
            var.setValue<void *>(p);
        }
        else if(type == LUA_TUSERDATA)
        {
            void **p = (void **)(lua_touserdata(L, varindex));
            QString tableType = QString(utype).replace("*", "").trimmed();
#define LOAD_VAR(cname) \
            if(tableType == #cname) \
            { \
                var.setValue<cname *>((cname *)*p); \
            }
            LOAD_VAR(NLSceneCamera)
            else LOAD_VAR(NLScene)
            else LOAD_VAR(NLComponent)
            else LOAD_VAR(NLScript)
            else LOAD_VAR(NLActor)
            else LOAD_VAR(NLRenderable)
            else LOAD_VAR(NLRigidbody)
            else LOAD_VAR(NLRenderable)
            else LOAD_VAR(NLForce)
            else LOAD_VAR(QObject)
            else LOAD_VAR(QWidget)
            else LOAD_VAR(void)
            else
            {
                tableType = NL::get_metatable_name(L, varindex);
                LOAD_VAR(NLSceneCamera)
                else LOAD_VAR(NLScene)
                else LOAD_VAR(NLComponent)
                else LOAD_VAR(NLScript)
                else LOAD_VAR(NLActor)
                else LOAD_VAR(NLRenderable)
                else LOAD_VAR(NLRigidbody)
                else LOAD_VAR(NLRenderable)
                else LOAD_VAR(NLForce)
                else
                {
                    QVariant uv = NLEngineGlobals::Instance()->convert_lua_variant(p, tableType);
                    if(uv.isValid())
                        var = uv;
                    else
                        var.setValue<void *>(*p);
                }
            }
#undef LOAD_VAR
        }
    }
    return var;
}

int push_from_qvariant(struct lua_State* L, const QVariant &v)
{
    int type = v.type();
    if(type == QVariant::Int
            || type == QVariant::Char
            || type == QVariant::LongLong
            || type == QVariant::UInt
            || type == QVariant::ULongLong
            )
        lua_pushinteger(L, v.toInt());
    else if(type == QVariant::Double
            )
        lua_pushnumber(L, v.toFloat());
    else if(type == QVariant::Bool
            )
        lua_pushboolean(L, v.toBool() ? 1 : 0);
    else if(type == QVariant::String
            )
    {
        QByteArray b = v.toByteArray();
        lua_pushstring(L, b.constData());
    }
    else
    {
        QString varType = v.typeName();
#define PUSH_VAR(name) \
        if(varType == #name"*") \
        { \
             PUSH_NLOBJECT_TO_STACK(L, name, v.value<name *>()); \
        }
        PUSH_VAR(NLSceneCamera)
        else PUSH_VAR(NLScene)
        else PUSH_VAR(NLComponent)
        else PUSH_VAR(NLScript)
        else PUSH_VAR(NLActor)
        else PUSH_VAR(NLRenderable)
        else PUSH_VAR(NLForce)
        else PUSH_VAR(NLRigidbody)
        else
        {
            if(type == QMetaType::QObjectStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<QObject *>();
            else if(type == QMetaType::QWidgetStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<QWidget *>();
            else if(type == QMetaType::VoidStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<void *>();
            else
            {
                int rn = NLEngineGlobals::Instance()->push_variant_to_lua(v, L);
                if(rn > 0)
                    return rn;
                else
                    return 0;
            }
        }
#undef PUSH_VAR
    }
    return 1;
}

int push_from_qvarianti(struct lua_State* L, const QVariant &v, int type)
{
    const char *tname = lua_typename(L, type);
    return push_from_qvariants(L, v, tname);
}

int push_from_qvariants(struct lua_State* L, const QVariant &v, int typeindex)
{
    const char *vartype = lua_tostring(L, typeindex);
    return push_from_qvariants(L, v, vartype);
}

int push_from_qvariants(struct lua_State* L, const QVariant &v, const QString &vartype)
{
    QByteArray ba = vartype.toLocal8Bit();
    QByteArray utype(QMetaObject::normalizedType(ba.constData()));
    int type = v.type();
    if(type == QVariant::Int
            || type == QVariant::Char
            || type == QVariant::LongLong
            || type == QVariant::UInt
            || type == QVariant::ULongLong
            || (utype == NLVARTYPE_C_INT || utype == NLVARTYPE_LUA_INT)
            )
        lua_pushinteger(L, v.toInt());
    else if(type == QVariant::Double
            || (utype == NLVARTYPE_C_FLOAT || utype == NLVARTYPE_LUA_FLOAT)
            )
        lua_pushnumber(L, v.toFloat());
    else if(type == QVariant::Bool
            || (utype == NLVARTYPE_C_BOOL || utype == NLVARTYPE_LUA_BOOL)
            )
        lua_pushboolean(L, v.toBool() ? 1 : 0);
    else if(type == QVariant::String
            || (utype == NLVARTYPE_C_STRING || utype == NLVARTYPE_C_QSTRING || utype == NLVARTYPE_LUA_STRING)
            )
    {
        QByteArray b = v.toByteArray();
        lua_pushstring(L, b.constData());
    }
    else
    {
        QString varType = v.typeName();
#define PUSH_VAR(name) \
        if(utype == #name"*" || utype == #name || varType == #name"*") \
        { \
             PUSH_NLOBJECT_TO_STACK(L, name, v.value<name *>()); \
        }
        PUSH_VAR(NLSceneCamera)
        else PUSH_VAR(NLScene)
        else PUSH_VAR(NLComponent)
        else PUSH_VAR(NLScript)
        else PUSH_VAR(NLActor)
        else PUSH_VAR(NLRenderable)
        else PUSH_VAR(NLForce)
        else PUSH_VAR(NLRigidbody)
        else
        {
            if(type == QMetaType::QObjectStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<QObject *>();
            else if(type == QMetaType::QWidgetStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<QWidget *>();
            else if(type == QMetaType::VoidStar)
                *((void **)lua_newuserdata(L, sizeof(void *))) = v.value<void *>();
            else
            {
                int rn = NLEngineGlobals::Instance()->push_variant_to_lua(v, L, QString(utype));
                if(rn > 0)
                    return rn;
                else
                    return 0;
            }
        }
#undef PUSH_VAR
    }
    return 1;
}

NLProperties lua_table_to_properties(struct lua_State *L, int index)
{
    NLProperties props;
    int top = lua_gettop(L);
    if(top == 0)
        return props;
    if(lua_type(L, index) != LUA_TTABLE)
        return props;
    const bool notInTop = !(index == -1 || index == top);
    if(notInTop)
        lua_pushvalue(L, index);

    lua_pushnil(L); //先在栈顶压入一个空的，因为lua_next会先弹出一个key
    while (lua_next(L, -2) != 0)//表在栈底
    {
         //先push key,push value
        QString key(lua_tostring(L, -2));
        QVariant value = load_to_qvariant(L, -1);
        props.Insert(key, value);

        lua_pop(L, 1);//因为上面Lua_next会把key出栈，不会把value出栈所以要把value出栈
    }

    if(notInTop)
        lua_pop(L, 1);
    return props;
}

}
