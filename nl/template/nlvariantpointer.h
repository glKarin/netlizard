#ifndef _KARIN_NLVARIANTPOINTER_H
#define _KARIN_NLVARIANTPOINTER_H

#include "engine/nldef.h"

template <class T>
struct NLLIB_EXPORT NLVariantPointer
{
    T *ptr;
    const char *name;
    explicit NLVariantPointer(T *ptr = 0, const char *name = "void")
        : ptr(ptr),
          name(name)
    {
        if(!name)
            name = "void";
    }
    operator bool() const { return ptr != 0; }
    operator T*() { return ptr; }
    operator const T*() const { return ptr; }
    T operator*() { return *ptr; }
    T * operator->() { return ptr; }
    T operator[](int i) { return ptr[i]; }
    bool operator==(const NLVariantPointer &a) const { return qstrcmp(name, a.name) == 0 && ptr == a.ptr; }
    bool operator!=(const NLVariantPointer &a) const { return qstrcmp(name, a.name) != 0 || ptr != a.ptr; }
    operator QString() const { return QString().sprintf("(%s *)(%p)", name, ptr); }
};

typedef NLVariantPointer<void> NLVariantGeneralPointer;
typedef NLVariantGeneralPointer NLVariantVoidPointer;
#define NLMAKE_VARIANT_POINTER(X, p) (NL::make_variant_pointer<X>(p, #X))
#define NLMAKE_VARIANT_NULL_POINTER(X) (NL::make_variant_pointer<X>(0, #X))
#define NLMAKE_VARIANT_VOID_POINTER(X, p) (NL::make_variant_pointer<void>(p, #X))

namespace NL
{
template <class T>
struct NLVariantPointer<T> make_variant_pointer(T *ptr, const char *name)
{
    Q_ASSERT(name != 0);
    return NLVariantPointer<T>(ptr, name);
}
}
Q_DECLARE_METATYPE(NLVariantGeneralPointer)

#endif // _KARIN_NLVARIANTPOINTER_H
