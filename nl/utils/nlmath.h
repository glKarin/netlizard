#ifndef _KARIN_NLMATH_H
#define _KARIN_NLMATH_H

#include <QtGlobal>

#include "common/nlinc.h"
#include "common/nlmatrix.h"

#define ator(a) ((double)(a) / 180.0 * M_PI)
#define rtoa(r) ((double)(r) / M_PI * 180.0)

namespace NL
{
NLLIB_EXPORT float clamp_angle(float angle);
NLLIB_EXPORT void cale_normal_matrix(NLMatrix4 &r, const NLMatrix4 &m);
}

#endif // _KARIN_NLMATH_H
