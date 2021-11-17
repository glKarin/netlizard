#ifndef _KARIN_BOUND_H
#define _KARIN_BOUND_H

#include "vector3.h"
#include "matrix.h"
#include "plane.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOUND_MIN(b) (b).min
#define BOUND_MAX(b) (b).max
#define BOUNDV_MIN(b) (b)->min
#define BOUNDV_MAX(b) (b)->max

#define BOUND_MIN_X(b) VECTOR3_X((b).min)
#define BOUND_MIN_Y(b) VECTOR3_Y((b).min)
#define BOUND_MIN_Z(b) VECTOR3_Z((b).min)
#define BOUND_MAX_X(b) VECTOR3_X((b).max)
#define BOUND_MAX_Y(b) VECTOR3_Y((b).max)
#define BOUND_MAX_Z(b) VECTOR3_Z((b).max)

#define BOUNDV_MIN_X(b) VECTOR3_X((b)->min)
#define BOUNDV_MIN_Y(b) VECTOR3_Y((b)->min)
#define BOUNDV_MIN_Z(b) VECTOR3_Z((b)->min)
#define BOUNDV_MAX_X(b) VECTOR3_X((b)->max)
#define BOUNDV_MAX_Y(b) VECTOR3_Y((b)->max)
#define BOUNDV_MAX_Z(b) VECTOR3_Z((b)->max)

#define BOUND(a, b, c, d, e, f) {VECTOR3(a, b, c), VECTOR3(d, e, f)}
#define BOUNDV(a, b) {VECTOR3V(a), VECTOR3V(b)}

typedef struct bound_s
{
    vector3_t min;
    vector3_t max;
} bound_t;

void bound_make(bound_t *bo, const vector3_t *a, const vector3_t *b);
float bound_sqrt(const bound_t *b);
void bound_diff(const bound_t *a, vector3_t *r);
void bound_center(const bound_t *a, vector3_t *r);
int bound_point_in_box(const bound_t *b, const vector3_t *p);
int bound_point_in_box2d(const bound_t *b, const vector3_t *p);
void bound_get_box_plane(const bound_t *ab, plane_t r[6]);
int bound_in_frustum(const bound_t *b, float frustum[6][4]);
int bound_in_frustum_with_matrix(const bound_t *b, const GLmatrix *proj_mat, const GLmatrix *view_mat);
void bound_make_with_vertors(bound_t *bo, const vector3_t arr[], int count);
void bound_expand(bound_t *b, const vector3_t *a);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_BOUND_H
