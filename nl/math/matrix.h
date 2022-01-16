#ifndef _KARIN_MATRIX_H
#define _KARIN_MATRIX_H

#include "math_def.h"

#include "mesa_gl_math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef GLmatrix matrix4_t;
struct vector3_s;
struct perspective_s;
struct ortho_s;
struct frustum_s;

NLMATHLIB_EXPORT void matrix_cale_frustum(const matrix4_t *proj_mat, const matrix4_t *view_mat, float frustum[6][4]);

NLMATHLIB_EXPORT void matrix_translate(matrix4_t *mat, const struct vector3_s *v);
NLMATHLIB_EXPORT void matrix_rotate(matrix4_t *mat, float angle, const struct vector3_s *v);
NLMATHLIB_EXPORT void matrix_scale(matrix4_t *mat, const struct vector3_s *v);
NLMATHLIB_EXPORT void matrix_lookat(matrix4_t *mat, const struct vector3_s *eye, const struct vector3_s *center, const struct vector3_s *up);

NLMATHLIB_EXPORT void matrix_ortho(matrix4_t *mat, const struct ortho_s *v);
NLMATHLIB_EXPORT void matrix_ortho2d(matrix4_t *mat, const struct ortho_s *v);
NLMATHLIB_EXPORT void matrix_frustum(matrix4_t *mat, const struct frustum_s *v);
NLMATHLIB_EXPORT void matrix_perspective(matrix4_t *mat, const struct perspective_s *v);

NLMATHLIB_EXPORT void matrix_normal_matrix(const matrix4_t *mat, matrix4_t *v);
NLMATHLIB_EXPORT void matrix_identity(matrix4_t *mat);
NLMATHLIB_EXPORT void matrix_transpose(matrix4_t *mat);
NLMATHLIB_EXPORT void matrix_inverse(matrix4_t *mat);
NLMATHLIB_EXPORT void matrix_load(matrix4_t *mat, const matrix4_t *v);
NLMATHLIB_EXPORT void matrix_multi_matrix(matrix4_t *mat, const matrix4_t *v);

NLMATHLIB_EXPORT struct vector3_s matrix_transform_row(const matrix4_t *mat, const struct vector3_s *v);
NLMATHLIB_EXPORT void matrix_transformv_row(const matrix4_t *mat, const struct vector3_s *v, struct vector3_s *t);
NLMATHLIB_EXPORT void matrix_transformv_self_row(const matrix4_t *mat, struct vector3_s *v);
struct vector3_s matrix_transform(const matrix4_t *mat, const struct vector3_s *v);
NLMATHLIB_EXPORT void matrix_transformv(const matrix4_t *mat, const struct vector3_s *v, struct vector3_s *t);
NLMATHLIB_EXPORT void matrix_transformv_self(const matrix4_t *mat, struct vector3_s *v);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_MATRIX_H
