#include "bound.h"

#include <math.h>

void bound_make(bound_t *bo, const vector3_t *a, const vector3_t *b)
{
    BOUNDV_MIN_X(bo) = VECTOR3V_X(a);
    BOUNDV_MIN_Y(bo) = VECTOR3V_Y(a);
    BOUNDV_MIN_Z(bo) = VECTOR3V_Z(a);
    BOUNDV_MAX_X(bo) = VECTOR3V_X(b);
    BOUNDV_MAX_Y(bo) = VECTOR3V_Y(b);
    BOUNDV_MAX_Z(bo) = VECTOR3V_Z(b);
}

void bound_make_with_vertors(bound_t *bo, const vector3_t arr[], int count)
{
    int i;
    float min_x = 0.0;
    float min_y = 0.0;
    float min_z = 0.0;
    float max_x = 0.0;
    float max_y = 0.0;
    float max_z = 0.0;
    int inited = 0;

    for(i = 0; i < count; i++)
    {
        const vector3_t *v = arr + i;
        if(inited)
        {
            if(VECTOR3V_X(v) < min_x)
                min_x = VECTOR3V_X(v);
            else if(VECTOR3V_X(v) > max_x)
                max_x = VECTOR3V_X(v);

            if(VECTOR3V_Y(v) < min_y)
                min_y = VECTOR3V_Y(v);
            else if(VECTOR3V_Y(v) > max_y)
                max_y = VECTOR3V_Y(v);

            if(VECTOR3V_Z(v) < min_z)
                min_z = VECTOR3V_Z(v);
            else if(VECTOR3V_Z(v) > max_z)
                max_z = VECTOR3V_Z(v);
        }
        else
        {
            min_x = VECTOR3V_X(v);
            min_y = VECTOR3V_Y(v);
            min_z = VECTOR3V_Z(v);
            max_x = VECTOR3V_X(v);
            max_y = VECTOR3V_Y(v);
            max_z = VECTOR3V_Z(v);
            inited = 1;
            continue;
        }
    }
    BOUNDV_MIN_X(bo) = min_x;
    BOUNDV_MIN_Y(bo) = min_y;
    BOUNDV_MIN_Z(bo) = min_z;
    BOUNDV_MAX_X(bo) = max_x;
    BOUNDV_MAX_Y(bo) = max_y;
    BOUNDV_MAX_Z(bo) = max_z;
}

int bound_point_in_box(const bound_t *b, const vector3_t *p)
{
    const vector3_t *v1 = &(BOUNDV_MIN(b));
    const vector3_t *v2 = &(BOUNDV_MAX(b));
    return(
            (VECTOR3V_X(p) >= VECTOR3V_X(v1) && VECTOR3V_X(p) <= VECTOR3V_X(v2))
            && (VECTOR3V_Y(p) >= VECTOR3V_Y(v1) && VECTOR3V_Y(p) <= VECTOR3V_Y(v2))
            && (VECTOR3V_Z(p) >= VECTOR3V_Z(v1) && VECTOR3V_Z(p) <= VECTOR3V_Z(v2))
            ? 1 : 0);
}

int bound_point_in_box2d(const bound_t *b, const vector3_t *p)
{
    const vector3_t *v1 = &(BOUNDV_MIN(b));
    const vector3_t *v2 = &(BOUNDV_MAX(b));
    return(
            (VECTOR3V_X(p) >= VECTOR3V_X(v1) && VECTOR3V_X(p) <= VECTOR3V_X(v2))
            && (VECTOR3V_Y(p) >= VECTOR3V_Y(v1) && VECTOR3V_Y(p) <= VECTOR3V_Y(v2))
            ? 1 : 0);
}

void bound_get_box_plane(const bound_t *ab, plane_t r[6])
{
#define COPY_NORMAL(n, xx, yy, zz) \
    { \
    VECTOR3_X(n) = xx; \
        VECTOR3_Y(n) = yy; \
        VECTOR3_Z(n) = zz; \
    }
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

int bound_in_frustum(const bound_t *b, float frustum[6][4])
{
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

int bound_in_frustum_with_matrix(const bound_t *b, const GLmatrix *proj_mat, const GLmatrix *view_mat)
{
    float frustum[6][4];
    matrix_cale_frustum(proj_mat, view_mat, frustum);
    return bound_in_frustum(b, frustum);
}

void bound_diff(const bound_t *a, vector3_t *r)
{
    VECTOR3V_X(r) = BOUNDV_MAX_X(a) - BOUNDV_MIN_X(a);
    VECTOR3V_Y(r) = BOUNDV_MAX_Y(a) - BOUNDV_MIN_Y(a);
    VECTOR3V_Z(r) = BOUNDV_MAX_Z(a) - BOUNDV_MIN_Z(a);
}

void bound_center(const bound_t *a, vector3_t *r)
{
    bound_diff(a, r);
    vector3_scalev(r, 0.5);
    vector3_addve(r, &BOUNDV_MIN(a));
}

float bound_sqrt(const bound_t *b)
{
    vector3_t v;
    bound_diff(b, &v);
    return vector3_length(&v);
}
