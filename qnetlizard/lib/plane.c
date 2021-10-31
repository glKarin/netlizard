#include "plane.h"

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
