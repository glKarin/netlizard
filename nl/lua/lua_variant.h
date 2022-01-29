#ifndef _KARIN_LUA_VARIANT_H
#define _KARIN_LUA_VARIANT_H

struct lua_State;
class NLProperties;
class QVariant;
class QString;

namespace NL
{
QVariant load_to_qvariant(struct lua_State* L, int varindex);
QVariant load_to_qvarianti(struct lua_State* L, int varindex, int type);
QVariant load_to_qvariants(struct lua_State* L, int varindex, int typeindex);
QVariant load_to_qvariants(struct lua_State* L, int varindex, const QString &type);
bool push_from_qvariant(struct lua_State* L, const QVariant &value);
bool push_from_qvarianti(struct lua_State* L, const QVariant &value, int type);
bool push_from_qvariants(struct lua_State* L, const QVariant &value, int typeindex);
bool push_from_qvariants(struct lua_State* L, const QVariant &value, const QString &type);
NLProperties lua_table_to_properties(struct lua_State *L, int index);
}

#endif // _KARIN_LUA_VARIANT_H
