#ifndef _KARIN_ORTHO_H
#define _KARIN_ORTHO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector3.h"
#include "matrix.h"

#define ORTHO_LEFT(b) ((b).left)
#define ORTHO_RIGHT(b) ((b).right)
#define ORTHO_BOTTOM(b) ((b).bottom)
#define ORTHO_TOP(b) ((b).top)
#define ORTHO_ZNEAR(b) ((b).z_near)
#define ORTHO_ZFAR(b) ((b).z_far)

#define ORTHOV_LEFT(b) ((b)->left)
#define ORTHOV_RIGHT(b) ((b)->right)
#define ORTHOV_BOTTOM(b) ((b)->bottom)
#define ORTHOV_TOP(b) ((b)->top)
#define ORTHOV_ZNEAR(b) ((b)->z_near)
#define ORTHOV_ZFAR(b) ((b)->z_far)

#define ORTHO(a, b, c, d, e, f) {a, b, c, d, e, f}
#define ORTHOV(a, b) {a[0], b[0], a[1], b[1], a[2], b[2]}
#define ORTHO2D(a, b, c, d) ORTHO(a, b, c, d, -1, 1)
#define ORTHO2DV(a, b) ORTHO2D(a[0], b[0], a[1], b[1])

typedef struct _ortho_s
{
    float left;
    float right;
    float bottom;
    float top;
    float z_near;
    float z_far;
} ortho_s;

void ortho_make(ortho_s *bo, float left, float right, float bottom, float top, float z_near, float z_far);
void ortho_make2d(ortho_s *bo, float left, float right, float bottom, float top);
void ortho_matrix(const ortho_s *bo, GLmatrix *mat);
void ortho_matrix2d(const ortho_s *bo, GLmatrix *mat);
void ortho_min(const ortho_s *bo, vector3_s *m);
void ortho_max(const ortho_s *bo, vector3_s *m);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_ORTHO_H
