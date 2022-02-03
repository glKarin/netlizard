#ifndef _KARIN_NLDEF_H
#define _KARIN_NLDEF_H

#include <QVariant>

typedef QPair<QString, QVariant> QVariantPair;
typedef bool (*NLVariantCompare_f)(const QVariant &a, const QVariant &b);

#define NLClamp(t, min, max) qMax(min, qMin(t, max))

#define NLinstanceof(obj, T) ((dynamic_cast<T *>(&obj)) != 0)
#define NLinstanceofv(obj, T) ((dynamic_cast<T *>(obj)) != 0)

#define NLcountof(arr) (sizeof(arr) / sizeof(arr[0]))

#define NLSINGLE_INSTANCE_DEF(C) static C * Instance();
#define NLSINGLE_INSTANCE_DECL(C) C * C::Instance() \
{\
    static C _instance; \
    return &_instance; \
}
#define NLSINGLE_INSTANCE_OBJ(C) C::Instance()

#define NLPOINTER_STR(p) QString().sprintf("%p", (p))

#define NLBITS(x) (1 << (x))
#define NLBITS_ALL (~0U)

#define NLSWAP(a, b, T) \
{ \
    T __x = (a); \
    a = (b) ; \
    b = __x; \
}

#define NLSWAPV(a, b, T) \
{ \
    T __x = *(a); \
    *(a) = *(b) ; \
    *(b) = __x; \
}

#define NLPTR_MOVE(dst, src) \
{ \
    (dst) = (src); \
    (src) = NULL; \
}

#define NLPTR_DELETE(T, x) \
if(x) { \
    T *_Tx = x; \
    x = 0; \
    delete _Tx; \
}

#endif // _KARIN_NLDEF_H
