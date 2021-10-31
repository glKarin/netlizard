#ifndef _KARIN_MATRIX_H
#define _KARIN_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesa_gl_math.h"
#include "vector3.h"
#include "ortho.h"
#include "frustum.h"

typedef GLmatrix matrix4_t;

void matrix_cale_frustum(const matrix4_t *proj_mat, const matrix4_t *view_mat, float frustum[6][4]);

void matrix_translate(matrix4_t *mat, const vector3_t *v);
void matrix_rotate(matrix4_t *mat, float angle, const vector3_t *v);
void matrix_scale(matrix4_t *mat, const vector3_t *v);
void matrix_lookat(matrix4_t *mat, const vector3_t *eye, const vector3_t *center, const vector3_t *up);

void matrix_ortho(matrix4_t *mat, const ortho_t *v);
void matrix_ortho2d(matrix4_t *mat, const ortho_t *v);
void matrix_frustum(matrix4_t *mat, const frustum_t *v);
void matrix_perspective(matrix4_t *mat, const perspective_t *v);

void matrix_normal_matrix(const matrix4_t *mat, matrix4_t *v);
void matrix_identity(matrix4_t *mat);
void matrix_transpose(matrix4_t *mat);
void matrix_inverse(matrix4_t *mat);
void matrix_load(matrix4_t *mat, const matrix4_t *v);
void matrix_multi_matrix(matrix4_t *mat, const matrix4_t *v);

vector3_t matrix_transform_row(const matrix4_t *mat, const vector3_t *v);
void matrix_transformv_row(const matrix4_t *mat, const vector3_t *v, vector3_t *t);
void matrix_transformv_self_row(const matrix4_t *mat, vector3_t *v);
vector3_t matrix_transform(const matrix4_t *mat, const vector3_t *v);
void matrix_transformv(const matrix4_t *mat, const vector3_t *v, vector3_t *t);
void matrix_transformv_self(const matrix4_t *mat, vector3_t *v);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_MATRIX_H
