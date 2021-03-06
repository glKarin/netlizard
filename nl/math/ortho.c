#include "ortho.h"

#include "vector3.h"

void ortho_make(ortho_t *bo, float left, float right, float bottom, float top, float z_near, float z_far)
{
    ORTHOV_LEFT(bo) = left;
    ORTHOV_RIGHT(bo) = right;
    ORTHOV_BOTTOM(bo) = bottom;
    ORTHOV_TOP(bo) = top;
    ORTHOV_ZNEAR(bo) = z_near;
    ORTHOV_ZFAR(bo) = z_far;
}

void ortho_make2d(ortho_t *bo, float left, float right, float bottom, float top)
{
    ortho_make(bo, left, right, bottom, top, -1, 1);
}

void ortho_matrix(const ortho_t *bo, GLmatrix *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_glOrtho(mat, ORTHOV_LEFT(bo), ORTHOV_RIGHT(bo), ORTHOV_BOTTOM(bo), ORTHOV_TOP(bo), ORTHOV_ZNEAR(bo), ORTHOV_ZFAR(bo));
}

void ortho_matrix2d(const ortho_t *bo, GLmatrix *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_gluOrtho2D(mat, ORTHOV_LEFT(bo), ORTHOV_RIGHT(bo), ORTHOV_BOTTOM(bo), ORTHOV_TOP(bo));
}

void ortho_min(const ortho_t *bo, vector3_t *m)
{
    VECTOR3V_X(m) = ORTHOV_LEFT(bo);
    VECTOR3V_Y(m) = ORTHOV_BOTTOM(bo);
    VECTOR3V_Z(m) = ORTHOV_ZNEAR(bo);
}

void ortho_max(const ortho_t *bo, vector3_t *m)
{
    VECTOR3V_X(m) = ORTHOV_RIGHT(bo);
    VECTOR3V_Y(m) = ORTHOV_TOP(bo);
    VECTOR3V_Z(m) = ORTHOV_ZFAR(bo);
}
