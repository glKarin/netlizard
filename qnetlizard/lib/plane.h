#ifndef _KARIN_PLANE_H
#define _KARIN_PLANE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector3.h"

#define PLANE_POSITION_X(b) VECTOR3_X((b).position)
#define PLANE_POSITION_Y(b) VECTOR3_Y((b).position)
#define PLANE_POSITION_Z(b) VECTOR3_Z((b).position)
#define PLANE_NORMAL_X(b) VECTOR3_X((b).normal)
#define PLANE_NORMAL_Y(b) VECTOR3_Y((b).normal)
#define PLANE_NORMAL_Z(b) VECTOR3_Z((b).normal)

#define PLANEV_POSITION_X(b) VECTOR3_X((b)->position)
#define PLANEV_POSITION_Y(b) VECTOR3_Y((b)->position)
#define PLANEV_POSITION_Z(b) VECTOR3_Z((b)->position)
#define PLANEV_NORMAL_X(b) VECTOR3_X((b)->normal)
#define PLANEV_NORMAL_Y(b) VECTOR3_Y((b)->normal)
#define PLANEV_NORMAL_Z(b) VECTOR3_Z((b)->normal)

#define PLANE(a, b, c, d, e, f) {VECTOR3(a, b, c), VECTOR3(d, e, f)}
#define PLANEV(a, b) {VECTOR3V(a), VECTOR3V(b)}

typedef struct _plane_s
{
    vector3_s position;
    vector3_s normal;
} plane_s;

#define PLANE_POSITION(b) (b).position
#define PLANE_NORMAL(b) (b).normal
#define PLANEV_POSITION(b) (b)->position
#define PLANEV_NORMAL(b) (b)->normal

void plane_make(plane_s *plane, const vector3_s *pos, const vector3_s *nor);
float plane_point_to_plane_distance(const plane_s *plane, const vector3_s *point);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_PLANE_H
