#ifndef _KARIN_FRUSTUM_H
#define _KARIN_FRUSTUM_H

#include "mesa_gl_math.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FRUSTUM_LEFT(b) ((b).left)
#define FRUSTUM_RIGHT(b) ((b).right)
#define FRUSTUM_BOTTOM(b) ((b).bottom)
#define FRUSTUM_TOP(b) ((b).top)
#define FRUSTUM_ZNEAR(b) ((b).z_near)
#define FRUSTUM_ZFAR(b) ((b).z_far)

#define FRUSTUMV_LEFT(b) ((b)->left)
#define FRUSTUMV_RIGHT(b) ((b)->right)
#define FRUSTUMV_BOTTOM(b) ((b)->bottom)
#define FRUSTUMV_TOP(b) ((b)->top)
#define FRUSTUMV_ZNEAR(b) ((b)->z_near)
#define FRUSTUMV_ZFAR(b) ((b)->z_far)

#define FRUSTUM(a, b, c, d, e, f) {a, b, c, d, e, f}
#define FRUSTUMV(a, b) {a[0], b[0], a[1], b[1], a[2], b[2]}

#define PERSPECTIVE_FOVY(b) ((b).fovy)
#define PERSPECTIVE_ASPECT(b) ((b).aspect)
#define PERSPECTIVE_ZNEAR(b) ((b).z_near)
#define PERSPECTIVE_ZFAR(b) ((b).z_far)

#define PERSPECTIVEV_FOVY(b) ((b)->fovy)
#define PERSPECTIVEV_ASPECT(b) ((b)->aspect)
#define PERSPECTIVEV_ZNEAR(b) ((b)->z_near)
#define PERSPECTIVEV_ZFAR(b) ((b)->z_far)

#define PERSPECTIVE(a, b, c, d) {a, b, c, d}
#define PERSPECTIVE_WH(a, b, c, d, e) {a, (b) / (c), d, e}

typedef struct frustum_s
{
    float left;
    float right;
    float bottom;
    float top;
    float z_near;
    float z_far;
} frustum_t;

typedef struct perspective_s
{
    float fovy;
    float aspect;
    float z_near;
    float z_far;
} perspective_t;

void frustum_make(frustum_t *bo, float left, float right, float bottom, float top, float z_near, float z_far);
void frustum_matrix(const frustum_t *bo, GLmatrix *mat);

void perspective_make(perspective_t *bo, float fovy, float aspect, float z_near, float z_far);
void perspective_make_with_wh(perspective_t *bo, float fovy, float width, float height, float z_near, float z_far);
void perspective_matrix(const perspective_t *bo, GLmatrix *mat);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_FRUSTUM_H
