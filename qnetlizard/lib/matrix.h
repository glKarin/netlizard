#ifndef _KARIN_MATRIX_H
#define _KARIN_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesa_gl_math.h"

typedef GLmatrix matrix4_s;

void matrix_cale_frustum(const matrix4_s *proj_mat, const matrix4_s *view_mat, float frustum[][4]);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_MATRIX_H
