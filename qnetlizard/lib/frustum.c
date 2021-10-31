#include "frustum.h"

void frustum_make(frustum_t *bo, float left, float right, float bottom, float top, float z_near, float z_far)
{
    FRUSTUMV_LEFT(bo) = left;
    FRUSTUMV_RIGHT(bo) = right;
    FRUSTUMV_BOTTOM(bo) = bottom;
    FRUSTUMV_TOP(bo) = top;
    FRUSTUMV_ZNEAR(bo) = z_near;
    FRUSTUMV_ZFAR(bo) = z_far;
}

void frustum_matrix(const frustum_t *bo, GLmatrix *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_glFrustum(mat, FRUSTUMV_LEFT(bo), FRUSTUMV_RIGHT(bo), FRUSTUMV_BOTTOM(bo), FRUSTUMV_TOP(bo), FRUSTUMV_ZNEAR(bo), FRUSTUMV_ZFAR(bo));
}

void perspective_make(perspective_t *bo, float fovy, float aspect, float z_near, float z_far)
{
    PERSPECTIVEV_FOVY(bo) = fovy;
    PERSPECTIVEV_ASPECT(bo) = aspect;
    PERSPECTIVEV_FOVY(bo) = z_near;
    PERSPECTIVEV_FOVY(bo) = z_far;
}

void perspective_make_with_wh(perspective_t *bo, float fovy, float width, float height, float z_near, float z_far)
{
    if(height == 0)
        return;
    perspective_make(bo, fovy, width / height, z_near, z_far);
}

void perspective_matrix(const perspective_t *bo, GLmatrix *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_gluPerspective(mat, PERSPECTIVEV_FOVY(bo), PERSPECTIVEV_ASPECT(bo), PERSPECTIVEV_ZNEAR(bo), PERSPECTIVEV_ZFAR(bo));
}
