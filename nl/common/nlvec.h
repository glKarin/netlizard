#ifndef _KARIN_NLVEC_H
#define _KARIN_NLVEC_H

#include <QMetaType>

#include "math/vector3.h"

typedef struct vector3_s NLVector3;

Q_DECLARE_METATYPE(NLVector3)

namespace NL
{

inline bool NLVector3_equals(const NLVector3 &a, const NLVector3 &b)
{
    return vector3_equals(&a, &b);
}

}

#endif // _KARIN_NLVEC_H
