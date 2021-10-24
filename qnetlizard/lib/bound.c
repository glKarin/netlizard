#include "bound.h"

void bound_make(bound_s *bo, const vector3_s *a, const vector3_s *b)
{
    if(!bo)
        return;
    BOUNDV_MIN_X(bo) = VECTOR3V_X(a);
    BOUNDV_MIN_Y(bo) = VECTOR3V_Y(a);
    BOUNDV_MIN_Z(bo) = VECTOR3V_Z(a);
    BOUNDV_MAX_X(bo) = VECTOR3V_X(b);
    BOUNDV_MAX_Y(bo) = VECTOR3V_Y(b);
    BOUNDV_MAX_Z(bo) = VECTOR3V_Z(b);
}

int bound_point_in_box(const bound_s *b, const vector3_s *p)
{
    if(!b || !p)
        return 0;
    const vector3_s *v1 = &(BOUNDV_MIN(b));
    const vector3_s *v2 = &(BOUNDV_MAX(b));
    return(
            (VECTOR3V_X(p) >= VECTOR3V_X(v1) && VECTOR3V_X(p) <= VECTOR3V_X(v2))
            && (VECTOR3V_Y(p) >= VECTOR3V_Y(v1) && VECTOR3V_Y(p) <= VECTOR3V_Y(v2))
            && (VECTOR3V_Z(p) >= VECTOR3V_Z(v1) && VECTOR3V_Z(p) <= VECTOR3V_Z(v2))
            ? 1 : 0);
}

void bound_get_box_plane(const bound_s *ab, plane_s r[])
{
#define COPY_NORMAL(n, xx, yy, zz) \
    { \
    VECTOR3_X(n) = xx; \
        VECTOR3_Y(n) = yy; \
        VECTOR3_Z(n) = zz; \
    }
    if(!ab || !r)
        return;
    // bottom
    PLANE_POSITION(r[0]) = BOUNDV_MIN(ab);
    COPY_NORMAL(PLANE_NORMAL(r[0]), 0.0, 0.0, 1.0)
    // left
    PLANE_POSITION(r[1]) = BOUNDV_MIN(ab);
    COPY_NORMAL(PLANE_NORMAL(r[1]), 1.0, 0.0, 0.0)
    // front
    PLANE_POSITION(r[2]) = BOUNDV_MIN(ab);
    COPY_NORMAL(PLANE_NORMAL(r[2]), 0.0, 1.0, 0.0)
    // top
    PLANE_POSITION(r[3]) = BOUNDV_MAX(ab);
    COPY_NORMAL(PLANE_NORMAL(r[3]), 0.0, 0.0, -1.0)
    // right
    PLANE_POSITION(r[4]) = BOUNDV_MAX(ab);
    COPY_NORMAL(PLANE_NORMAL(r[4]), -1.0, 0.0, 0.0)
    // back
    PLANE_POSITION(r[5]) = BOUNDV_MAX(ab);
    COPY_NORMAL(PLANE_NORMAL(r[5]), 0.0, -1.0, 0.0)
#undef COPY_NORMAL
}

int bound_in_frustum(const bound_s *b, float frustum[][4])
{
    if(!b || !frustum)
        return 0;

    //printf("in %d: %f %f %f  | %f %f %f\n", 1, BOUNDV_MIN_X(b), BOUNDV_MIN_Y(b), BOUNDV_MIN_Z(b), BOUNDV_MAX_X(b), BOUNDV_MAX_Y(b), BOUNDV_MAX_Z(b));
    float x = BOUNDV_MIN_X(b);
    float y = BOUNDV_MIN_Y(b);
    float z = BOUNDV_MIN_Z(b);
    float x2 = BOUNDV_MAX_X(b);
    float y2 = BOUNDV_MAX_Y(b);
    float z2 = BOUNDV_MAX_Z(b);
    int p;
    for( p = 0; p < 6; p++ )
    {
        if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2]    * z + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x2 + frustum[p][1] * y + frustum[p][2]    * z + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x + frustum[p][1] * y2 + frustum[p][2]    * z + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x2 + frustum[p][1] * y2 + frustum[p][2]    * z + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2]    * z2 + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x2 + frustum[p][1] * y + frustum[p][2]    * z2 + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x + frustum[p][1] * y2 + frustum[p][2]    * z2 + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * x2 + frustum[p][1] * y2 + frustum[p][2]    * z2 + frustum[p][3] > 0 )
            continue;
        return 0;
    }
    return 1;
}

int bound_in_frustum_with_matrix(const bound_s *b, const GLmatrix *proj_mat, const GLmatrix *view_mat)
{
    if(!b || !proj_mat || !view_mat)
        return 0;

    float frustum[6][4];
    matrix_cale_frustum(proj_mat, view_mat, frustum);
    return bound_in_frustum(b, frustum);
}
