#ifndef _KARIN_MATRIX_H
#define _KARIN_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesa_gl_math.h"
#include "vector3.h"
#include "ortho.h"
#include "frustum.h"

typedef GLmatrix matrix4_s;

void matrix_cale_frustum(const matrix4_s *proj_mat, const matrix4_s *view_mat, float frustum[][4]);

void matrix_translate(matrix4_s *mat, const vector3_s *v);
void matrix_rotate(matrix4_s *mat, float angle, const vector3_s *v);
void matrix_scale(matrix4_s *mat, const vector3_s *v);
void matrix_lookat(matrix4_s *mat, const vector3_s *eye, const vector3_s *center, const vector3_s *up);

void matrix_ortho(matrix4_s *mat, const ortho_s *v);
void matrix_ortho2d(matrix4_s *mat, const ortho_s *v);
void matrix_frustum(matrix4_s *mat, const frustum_s *v);
void matrix_perspective(matrix4_s *mat, const perspective_s *v);

void matrix_normal_matrix(const matrix4_s *mat, matrix4_s *v);
void matrix_identity(matrix4_s *mat);
void matrix_transpose(matrix4_s *mat);
void matrix_inverse(matrix4_s *mat);
void matrix_load(matrix4_s *mat, const matrix4_s *v);
void matrix_multi_matrix(matrix4_s *mat, const matrix4_s *v);

vector3_s matrix_transform_row(const matrix4_s *mat, const vector3_s *v);
void matrix_transformv_row(const matrix4_s *mat, const vector3_s *v, vector3_s *t);
void matrix_transformv_self_row(const matrix4_s *mat, vector3_s *v);
vector3_s matrix_transform(const matrix4_s *mat, const vector3_s *v);
void matrix_transformv(const matrix4_s *mat, const vector3_s *v, vector3_s *t);
void matrix_transformv_self(const matrix4_s *mat, vector3_s *v);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_MATRIX_H
