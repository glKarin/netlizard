#include "line.h"

#include "plane.h"

void line_make(line_s *obj, const vector3_s *a, const vector3_s *b)
{
    if(!obj)
        return;
    LINEV_A_X(obj) = VECTOR3V_X(a);
    LINEV_A_Y(obj) = VECTOR3V_Y(a);
    LINEV_A_Z(obj) = VECTOR3V_Z(a);
    LINEV_B_X(obj) = VECTOR3V_X(b);
    LINEV_B_Y(obj) = VECTOR3V_Y(b);
    LINEV_B_Z(obj) = VECTOR3V_Z(b);
}

int line_equals(const line_s *a, const line_s *b)
{
    if(!a || !b)
        return 0;
    return vector3_equals(&(LINEV_A(a)), &(LINEV_A(b)))
            && vector3_equals(&(LINEV_B(a)), &(LINEV_B(b)))
            ;
}

int line_equals_ignore_seq(const line_s *a, const line_s *b)
{
    if(!a || !b)
        return 0;
    return (vector3_equals(&(LINEV_A(a)), &(LINEV_A(b)))
            && vector3_equals(&(LINEV_B(a)), &(LINEV_B(b))))
            || (vector3_equals(&(LINEV_A(a)), &(LINEV_B(b)))
                && vector3_equals(&(LINEV_B(a)), &(LINEV_A(b))))
            ;
}

void ray_make(ray_s *obj, const vector3_s *a, const vector3_s *b)
{
    if(!obj)
        return;
    RAYV_POSITION_X(obj) = VECTOR3V_X(a);
    RAYV_POSITION_Y(obj) = VECTOR3V_Y(a);
    RAYV_POSITION_Z(obj) = VECTOR3V_Z(a);
    RAYV_DIRECTION_X(obj) = VECTOR3V_X(b);
    RAYV_DIRECTION_Y(obj) = VECTOR3V_Y(b);
    RAYV_DIRECTION_Z(obj) = VECTOR3V_Z(b);
}

#define COLLISION_ZERO 0.0
int ray_line_to_plane_intersect(const ray_s *line, const plane_s *plane, vector3_s *point)
{
    float d = vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_DIRECTION(line)));
    //if ( d == 0.0 ) return 0;
    if((d <= COLLISION_ZERO) && (d >= -COLLISION_ZERO))
        return 0;
    if(d == 0)
        return 0;
    if(point)
    {
        float distance = vector3_dot(&(PLANEV_NORMAL(plane)), &(PLANEV_POSITION(plane)));
        float t = - (vector3_dot(&(PLANEV_NORMAL(plane)), &(RAYV_POSITION(line))) - distance) / d;
        VECTOR3V_X(point) = RAYV_POSITION_X(line) + RAYV_DIRECTION_X(line) * t;
        VECTOR3V_Y(point) = RAYV_POSITION_Y(line) + RAYV_DIRECTION_Y(line) * t;
        VECTOR3V_Z(point) = RAYV_POSITION_Z(line) + RAYV_DIRECTION_Z(line) * t;
    }
    return 1;
}

int ray_line_to_plane_collision(const ray_s *line, const plane_s *plane, float *lamda, vector3_s *normal)
{
    float dotProduct = vector3_dot(&(RAYV_DIRECTION(line)), &(PLANEV_NORMAL(plane)));
    float l2;

    //判断是否平行于平面
    if((dotProduct <= COLLISION_ZERO) && (dotProduct >= -COLLISION_ZERO))
        return 0;

    if(dotProduct == 0)
        return 0;

    vector3_s vec;
    vector3_subtractv(&vec, &(PLANEV_POSITION(plane)), &(RAYV_POSITION(line)));
    l2 = (vector3_dot(&(PLANEV_NORMAL(plane)), &vec)) / dotProduct;

    if (l2 <= -COLLISION_ZERO)
        return 0;

    if(normal)
    {
        VECTOR3V_X(normal) = PLANEV_NORMAL_X(plane);
        VECTOR3V_Y(normal) = PLANEV_NORMAL_Y(plane);
        VECTOR3V_Z(normal) = PLANEV_NORMAL_Z(plane);
    }
    if(lamda)
        *lamda = l2;
    return 1;
}

void line_direction(const line_s *line, vector3_s *dir)
{
    if(!line || !dir)
        return;
    vector3_subtractv(dir, &(LINEV_B(line)), &(LINEV_A(line)));
    vector3_normalizev(dir);
}

void ray_line_to_ray(ray_s *r, const line_s *l)
{
    if(!r || !l)
        return;
    RAYV_POSITION_X(r) = LINEV_A_X(l);
    RAYV_POSITION_Y(r) = LINEV_A_Y(l);
    RAYV_POSITION_Z(r) = LINEV_A_Z(l);
    line_direction(l, &(RAYV_DIRECTION(r)));
}
