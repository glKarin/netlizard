#include "engineregisterobject.h"

#include <QDebug>

#include "mapeventhandlercomponent.h"
#include "simplecameracomponent.h"
#include "simplecontrol2dcomponent.h"
#include "simplecontrolcomponent.h"
#include "simpleimagecontrolcomponent.h"
#include "simplelightsourcecomponent.h"

#include "netlizardanimationmodelrenderer.h"
#include "netlizardfontrenderer.h"
#include "netlizarditemmodelrenderer.h"
#include "netlizardmapmodeldebugrenderer.h"
#include "netlizardmapmodelrenderer.h"
#include "netlizardshadowmodelrenderer.h"
#include "netlizardspriterenderer.h"
#include "netlizardtexturerenderer.h"

#include "simplecameraactor.h"
#include "simplelightsourceactor.h"

#define REG_METATYPE(T) qRegisterMetaType<T *>(#T " *")

void EngineRegisterObject::RegisterMetaType()
{
#if 0
    REG_METATYPE(MapEventHandlerComponent);
    REG_METATYPE(SimpleCameraComponent);
    REG_METATYPE(SimpleControl2DComponent);
    REG_METATYPE(SimpleControlComponent);
    REG_METATYPE(SimpleImageControlComponent);
    REG_METATYPE(SimpleLightSourceComponent);
    REG_METATYPE(NETLizardAnimationModelRenderer);
    REG_METATYPE(NETLizardFontRenderer);
    REG_METATYPE(NETLizardItemModelRenderer);
    REG_METATYPE(NETLizardMapModelDebugRenderer);
    REG_METATYPE(NETLizardMapModelRenderer);
    REG_METATYPE(NETLizardShadowModelRenderer);
    REG_METATYPE(NETLizardSpriteRenderer);
    REG_METATYPE(NETLizardSpriteRenderer);
    REG_METATYPE(SimpleCameraActor);
    REG_METATYPE(SimpleLightSourceActor);
#endif

#ifdef _DEV_TEST
    REG_METATYPE(EngineRegisterObject);
#endif

    qDebug() << "NETLizard register";
}

#ifdef _DEV_TEST
extern "C" {
#include "extern/lua/include/lua.h"
#include "extern/lua/include/lualib.h"
#include "extern/lua/include/lauxlib.h"
}
#include "netlizard.h"
#include "lua/lua_def.h"

#define REG_METATABLE_NAME "EngineRegisterObject"

static int NETLizard_Version(lua_State *L)
{
    const char *ver = nlGetString(NL_VERSION);
    lua_pushstring(L, ver);
    return 1;
}

static int NETLizardReg_new(lua_State *L)
{
    EngineRegisterObject **ptr = (EngineRegisterObject **)lua_newuserdata(L, sizeof(EngineRegisterObject *));
    *ptr = new EngineRegisterObject;
    qDebug() << "lua testing new -> " << (*ptr)->Name();
    luaL_getmetatable(L, REG_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

static int NETLizardReg_delete(lua_State *L)
{
    EngineRegisterObject **reg = (EngineRegisterObject **)(lua_touserdata(L, 1));
    qDebug() << "lua testing free -> " << (*reg)->Name();
    delete *reg;
    *reg = 0;
    return 0;
}

static int NETLizardReg_Name(lua_State *L)
{
    EngineRegisterObject *reg = *(EngineRegisterObject **)(lua_touserdata(L, 1));
    lua_pushstring(L, reg->Name());
    return 1;
}

void EngineRegisterObject::RegisterLuaFunc(struct lua_State *L)
{
    int res = luaL_getmetatable(L, REG_METATABLE_NAME);
    lua_pop(L, 1);
    if(res == LUA_TTABLE)
        return;

    lua_pushcfunction(L, NETLizard_Version);
    lua_setglobal(L, "NETLizard_Version");
    lua_pushcfunction(L, NETLizardReg_new);
    lua_setglobal(L, "NETLizardReg_new");

    if(luaL_newmetatable(L, REG_METATABLE_NAME))
    {
        const struct luaL_Reg Funcs[] = {
            {"Name", NETLizardReg_Name},
            {NULL, NULL}
        };
        luaL_setfuncs(L, Funcs, 0);
        lua_pushcfunction(L, NETLizardReg_delete); \
        lua_setfield(L, -2, "__gc");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
    }
}

QVariant EngineRegisterObject::LuaVariantToQVariant(void **ptr, const QString &metatableName)
{
    if(metatableName == REG_METATABLE_NAME)
    {
        //qDebug() << "lua testing LuaVariantToQVariant -> " << ((EngineRegisterObject *)*ptr)->Name();
        return QVariant::fromValue<EngineRegisterObject *>((EngineRegisterObject *)*ptr);
    }
    return QVariant();
}

int EngineRegisterObject::PushQVariantToLua(const QVariant &v, struct lua_State *L, const QString &specialType)
{
    if(specialType == REG_METATABLE_NAME"*" || specialType == REG_METATABLE_NAME)
    {
        EngineRegisterObject* obj = v.value<EngineRegisterObject *>();
        qDebug() << "lua testing PushQVariantToLua -> " << obj->Name();
        void **ptr = (void **)lua_newuserdata(L, sizeof(EngineRegisterObject **));
        *ptr = obj;
        return 1;
    }
    return 0;
}

/*
ccc = NETLizardReg_new();
ddd = NETLizardReg_new();
print(ccc:Name());
local a = NETLizardReg_new();
a = nil;
  */
#endif
