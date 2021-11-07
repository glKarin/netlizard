#include "plane.h"

#include "math_std.h"

#include <stdio.h>
#include "line.h"
#include "triangle.h"

void plane_make(plane_t *bo, const vector3_t *a, const vector3_t *b)
{
    PLANEV_POSITION_X(bo) = VECTOR3V_X(a);
    PLANEV_POSITION_Y(bo) = VECTOR3V_Y(a);
    PLANEV_POSITION_Z(bo) = VECTOR3V_Z(a);
    PLANEV_NORMAL_X(bo) = VECTOR3V_X(b);
    PLANEV_NORMAL_Y(bo) = VECTOR3V_Y(b);
    PLANEV_NORMAL_Z(bo) = VECTOR3V_Z(b);
}

float plane_point_to_plane_distance(const plane_t *plane, const vector3_t *point)
{
    float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
    return vector3_dot(point, &(PLANEV_POSITION(plane))) - distance;
}

float plane_d(const plane_t *plane)
{
    float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
    return distance;
}

#define COLLISION_ZERO 0.0
int plane_ray_intersect(const plane_t *plane, const ray_t *line, float *lamda, vector3_t *point)
{
    float dotProduct = vector3_dot(&(RAYV_DIRECTION(line)), &(PLANEV_NORMAL(plane)));
    float l2;

    if((dotProduct <= COLLISION_ZERO) && (dotProduct >= -COLLISION_ZERO))
        return 0;

    if(dotProduct == 0)
        return 0;

    vector3_t vec;
    vector3_subtractv(&vec, &(PLANEV_POSITION(plane)), &(RAYV_POSITION(line)));
    l2 = (vector3_dot(&(PLANEV_NORMAL(plane)), &vec)) / dotProduct;

    if(lamda)
        *lamda = l2;

     // ray start point is allow on plane
    //if (l2 <= -COLLISION_ZERO) // not allow
    if (l2 < -COLLISION_ZERO) // allow
        return -1;

    if(point)
    {
        float d = dotProduct;
        float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
        float t = - (vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_POSITION(line))) - distance) / d;
        vector3_movev(point, &RAYV_POSITION(line), &RAYV_DIRECTION(line), t);
    }
    return 1;
}

void plane_triangle_plane(plane_t *plane, const triangle_t *tri)
{
    triangle_cale_normal(tri, &PLANEV_NORMAL(plane));
    PLANEV_POSITION_X(plane) = TRIANGLEV_A_X(tri);
    PLANEV_POSITION_Y(plane) = TRIANGLEV_A_Y(tri);
    PLANEV_POSITION_Z(plane) = TRIANGLEV_A_Z(tri);
}

int plane_point_clip(const plane_t *plane, const vector3_t *v)
{
    float a = vector3_dot(v, &(PLANEV_NORMAL(plane))) - plane_d(plane);

    //float a = PLANEV_NORMAL_X(plane) * VECTOR3V_X(v) + PLANEV_NORMAL_Y(plane) * VECTOR3V_Y(v) + PLANEV_NORMAL_Z(plane) * VECTOR3V_Z(v) + plane_d(plane);
    return a > 0 ? 1 : (a < 0 ? -1 : 0);
}

/*
  mask:
    0: A and B all under plane
    & 1: A above plane
    & 2: B above plane
  dir:
    0: not cross
    -1: B -> A cross plane
    1: A -> B cross plane
  return:
      -1: 不相交
      0: 平行
      1: 相交
  */
#define CMP_ZERO 0.0001
int plane_line_intersect(const plane_t *plane, const line_t *line, float *lamda, vector3_t *point, int *dir, int *mask)
{
    vector3_t p;
    float l;
    ray_t a;

    int same = line_iszero(line);
    int ac = plane_point_clip(plane, &LINEV_A(line));
    int bc = plane_point_clip(plane, &LINEV_B(line));
    if(same)
    {
        if(ac == 0 && bc == 0)
        {
            if(mask)
                *mask = 1 | 2;
            if(dir)
                *dir = 0;
            if(lamda)
                *lamda = 0;
            if(point)
                *point = LINEV_A(line);
            return 10;
        }
        else
        {
            if(mask)
            {
                if(ac > 0)
                    *mask = 1 | 2;
            }
            return -10;
        }
    }

    if(mask)
    {
        *mask = 0;
        if(ac >= 0)
            *mask |= 1;
        if(bc >= 0)
            *mask |= 2;
    }

    ray_line_to_ray(&a, line);
    const float length = line_length(line); // UNUSED: because `sqrt` may be has precision on float
    int res = plane_ray_intersect(plane, &a, &l, &p);
    if(res == 0)
    {
        return 0;
    }

    if(res > 0)
    {
        // cale $line.b point with $plane.normal and ray's collsion's distance, instead of line's $length
        plane_t bp;
        plane_make(&bp, &LINEV_B(line), &PLANEV_NORMAL(plane));
        float bl;
        plane_ray_intersect(&bp, &a, &bl, NULL);
        //fprintf(stderr, " a->b } %f, %f %f\n", l, bl, length);fflush(stderr);

        if(l > bl)
        //if(FLOAT_GREATER(l, length, CMP_ZERO))
        {
            if(dir)
                *dir = 1;
            return -11;
        }
        else
        {
            if(lamda)
                *lamda = l;
            if(point)
                *point = p;
            if(dir)
                *dir = 1;
            return 11;
        }
    }

    line_t inv = LINEV(VECTOR3_V(LINEV_B(line)), VECTOR3_V(LINEV_A(line)));
    ray_line_to_ray(&a, &inv);
    res = plane_ray_intersect(plane, &a, &l, &p);
    if(res <= 0)
        return -12;

    // cale $line.a point with $plane.normal and inverse ray's collsion's distance, instead of line's $length
    plane_t bp;
    plane_make(&bp, &LINEV_A(line), &PLANEV_NORMAL(plane));
    float al;
    plane_ray_intersect(&bp, &a, &al, NULL);
    //fprintf(stderr, " b->a } %f, %f %f\n", l, length, al);fflush(stderr);

    if(l > al)
    //if(FLOAT_GREATER(l, length, CMP_ZERO))
    {
        if(dir)
            *dir = 2;
        return -13;
    }
    else
    {
        if(lamda)
            *lamda = al - l;
        if(point)
            *point = p;
        if(dir)
            *dir = -1;
        return 12;
    }
}
