#ifndef _KARIN_AABB_H
#define _KARIN_AABB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector3.h"

#include "plane.h"

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

typedef struct _bound_s
{
    vector3_s min;
    vector3_s max;
} bound_s;

void bound_make(bound_s *bo, const vector3_s *a, const vector3_s *b);
int bound_point_in_box(const bound_s *b, const vector3_s *p);
void bound_get_box_plane(const bound_s *ab, plane_s r[]);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_AABB_H
