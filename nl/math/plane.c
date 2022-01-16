#include "plane.h"

#include "math_std.h"

#include <stdio.h>
#include <math.h>
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
    return vector3_dot(point, &(PLANEV_NORMAL(plane))) + plane_d(plane);

    float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
    return vector3_dot(point, &(PLANEV_NORMAL(plane))) - distance;
}

// Ax + By + Cz + D = 0: D is left
float plane_d(const plane_t *plane)
{
    float distance = -vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
    return distance;
}

int plane_equals(const plane_t *plane, const plane_t *o)
{
    return vector3_equals(&PLANEV_NORMAL(plane), &PLANEV_NORMAL(o))
            && plane_d(plane) == plane_d(o);
}

int plane_ray_intersect(const plane_t *plane, const ray_t *line, float *lamda, vector3_t *point, float *scale)
{
    float dotProduct = vector3_dot(&(RAYV_DIRECTION(line)), &(PLANEV_NORMAL(plane)));
    float l2;

    if(IS_ZERO(dotProduct))
        return 0;

    vector3_t vec;
    vector3_subtractv(&vec, &(PLANEV_POSITION(plane)), &(RAYV_POSITION(line)));
    l2 = (vector3_dot(&(PLANEV_NORMAL(plane)), &vec)) / dotProduct;

    //l2 = -(vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_POSITION(line))) - plane_d(plane)) / dotProduct;

    if(lamda)
        *lamda = l2;

     // ray start point is allow on plane
    //if (l2 <= -COLLISION_ZERO) // not allow
    int res = l2 < 0 ? -1 : 1;
    // if (l2 < 0) return -1;

    float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
    float t = - (vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_POSITION(line))) - distance) / dotProduct;
    //fprintf(stderr, "plane_point_clip: %f = (%f, %f) / %f\n", t, vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_POSITION(line))), distance, dotProduct);fflush(stderr);
    if(scale)
        *scale = t;
    if(point)
        vector3_movev(point, &RAYV_POSITION(line), &RAYV_DIRECTION(line), t);
    return res;
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
    float a = plane_point_to_plane_distance(plane, v);

    //fprintf(stderr, "plane_point_clip: %f = %f %f %f | %f %f %f - %f %f %f\n", a, v->v[0], v->v[1], v->v[2], plane->normal.v[0], plane->normal.v[1], plane->normal.v[2], plane->position.v[0], plane->position.v[1], plane->position.v[2]);fflush(stderr);
    return a > 0 ? 1 : (a < 0 ? -1 : 0);
}

int plane_point_clip_precision(const plane_t *plane, const vector3_t *v, float precision)
{
    int f = plane_point_clip(plane, v);
    if(f == 0)
        return 0;
    if(f > 0)
    {
        vector3_t pos;
        vector3_movev(&pos, v, &PLANEV_NORMAL(plane), -precision);
        int a = plane_point_clip(plane, &pos);
        if(a == 0)
            return 0;
        else if(a < 0)
            return 0;
        else
            return 1;
    }
    else
    {
        vector3_t pos;
        vector3_movev(&pos, v, &PLANEV_NORMAL(plane), precision);
        int a = plane_point_clip(plane, &pos);
        if(a == 0)
            return 0;
        else if(a > 0)
            return 0;
        else
            return -1;
    }
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
#ifdef _DEV_TEST
#define _RET_SIGN(x, y) ((x) + (y))
#else
#define _RET_SIGN(x, y) (x)
#endif
int plane_line_intersect(const plane_t *plane, const line_t *line, float *lamda, vector3_t *point, int *dir, int *mask)
{
    vector3_t p;
    float l;
    ray_t a;

    int same = line_iszero(line); // 线段起点和终点是否相同
    int ac = plane_point_clip(plane, &LINEV_A(line)); // 点A到平南裁剪
    int bc = plane_point_clip(plane, &LINEV_B(line)); // 点B到平南裁剪
    if(same) // 如果线段起点和终点相同
    {
        if(ac == 0 && bc == 0) // 两点都在平面里
        {
            if(mask)
                *mask = 1 | 2;
            if(dir)
                *dir = 0;
            if(lamda)
                *lamda = 0;
            if(point)
                *point = LINEV_A(line);
            return _RET_SIGN(1, 100); // 0;
        }
        else
        {
            if(ac > 0) // 两点都在平面上方
            {
                if(mask)
                    *mask = 1 | 2;
                if(dir)
                    *dir = 0;
                if(lamda)
                    *lamda = 0;
                if(point)
                    *point = LINEV_A(line);
                return 0; // 110???
            }
            else
            {
                if(mask)
                    *mask = 0;
                if(dir)
                    *dir = 0;
                if(lamda)
                    *lamda = 0;
                return 0;
            }
        }
    }

    int m = 0;
    if(ac >= 0)
        m |= 1;
    if(bc >= 0)
        m |= 2;
    if(mask)
    {
        *mask = m;
    }

    if(m == 0) // 如果两点都在平面下方, 则不相交
        return _RET_SIGN(-1, -200);
    if(m == (1 | 2)) // 如果两点都在平面上方, 则不相交
        return _RET_SIGN(-1, -210);
    if(ac == 0 && bc < 0) // 点A在平面上
    {
        if(lamda)
            *lamda = 0;
        if(point)
            *point = LINEV_A(line);
        if(dir)
            *dir = 1;
        return _RET_SIGN(1, 200);
    }
    if(bc == 0 && ac < 0) // 点B在平面上
    {
        if(lamda)
            *lamda = 0;
        if(point)
            *point = LINEV_B(line);
        if(dir)
            *dir = -1;
        return _RET_SIGN(1, 210);
    }

    ray_line_to_ray(&a, line);
    const float length = line_length(line); // UNUSED: because `sqrt` may be has precision on float
    int res = plane_ray_intersect(plane, &a, &l, &p, NULL);
    if(res == 0)
    {
        return 0;
    }

    //fprintf(stderr, " 111111 } %d, %f %f\n", res, l, length);fflush(stderr);
    if(res > 0)
    {
        // cale $line.b point with $plane.normal and ray's collsion's distance, instead of line's $length
        plane_t bp;
        plane_make(&bp, &LINEV_B(line), &PLANEV_NORMAL(plane));
        float bl;
        plane_ray_intersect(&bp, &a, &bl, NULL, NULL);
        //fprintf(stderr, " a->b } %f, %f %f\n", l, bl, length);fflush(stderr);

        if(l > bl)
        {
            if(dir)
                *dir = 1;
            return _RET_SIGN(-1, -300);
        }
        else
        {
            if(lamda)
                *lamda = l;
            if(point)
                *point = p;
            if(dir)
                *dir = 1;
            return _RET_SIGN(1, 300);
        }
    }

    line_t inv = LINEV(VECTOR3_V(LINEV_B(line)), VECTOR3_V(LINEV_A(line)));
    ray_line_to_ray(&a, &inv);
    res = plane_ray_intersect(plane, &a, &l, &p, NULL);
    if(res <= 0)
        return _RET_SIGN(-1, -500);

    // cale $line.a point with $plane.normal and inverse ray's collsion's distance, instead of line's $length
    plane_t bp;
    plane_make(&bp, &LINEV_A(line), &PLANEV_NORMAL(plane));
    float al;
    plane_ray_intersect(&bp, &a, &al, NULL, NULL);
    //fprintf(stderr, " b->a } %f, %f %f\n", l, length, al);fflush(stderr);

    if(l > al)
    {
        if(dir)
            *dir = 2;
        return _RET_SIGN(-1, -510);
    }
    else
    {
        if(lamda)
            *lamda = al - l;
        if(point)
            *point = p;
        if(dir)
            *dir = -1;
        return _RET_SIGN(1, 500);
    }
}
#undef _RET_SIGN
