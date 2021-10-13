#ifndef _KARIN_NLFUNCS_H
#define _KARIN_NLFUNCS_H

#include "nldef.h"

#define ator(a) ((double)(a) / 180.0 * M_PI)
#define rtoa(r) ((double)(r) / M_PI * 180.0)

namespace NL
{
float clamp_angle(float angle);
void cale_normal_matrix(NLMatrix4 &r, const NLMatrix4 &m);
}

#endif // _KARIN_NLFUNCS_H
