#include "plane.h"

void plane_make(plane_s *bo, const vector3_s *a, const vector3_s *b)
{
    if(!bo)
        return;
    PLANEV_POSITION_X(bo) = VECTOR3V_X(a);
    PLANEV_POSITION_Y(bo) = VECTOR3V_Y(a);
    PLANEV_POSITION_Z(bo) = VECTOR3V_Z(a);
    PLANEV_NORMAL_X(bo) = VECTOR3V_X(b);
    PLANEV_NORMAL_Y(bo) = VECTOR3V_Y(b);
    PLANEV_NORMAL_Z(bo) = VECTOR3V_Z(b);
}

float plane_point_to_plane_distance(const plane_s *plane, const vector3_s *point)
{
    float distance = vector3_dot(&(PLANEV_POSITION(plane)), &(PLANEV_POSITION(plane)));
    return vector3_dot(point, &(PLANEV_POSITION(plane))) - distance;
}
