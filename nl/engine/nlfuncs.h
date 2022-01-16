#ifndef _KARIN_NLFUNCS_H
#define _KARIN_NLFUNCS_H

#include "nlproperties.h"

class NLObject;
class NLScene;

namespace NL
{
template <class T> /*NLLIB_EXPORT*/ T clamp(T t, T min, T max);

NLLIB_EXPORT NLPropertyInfoList object_propertics(const NLObject *obj);
NLLIB_EXPORT NLPropertyInfoList scene_propertics(const NLScene *obj);
NLLIB_EXPORT bool property_equals(const QVariant &a, const QVariant &b);
}

template <class T> T NL::clamp(T t, T min, T max)
{
    return qMax(min, qMin(t, max));
}

#endif // _KARIN_NLFUNCS_H
