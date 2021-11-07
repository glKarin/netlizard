#ifndef _KARIN_PLANE_H
#define _KARIN_PLANE_H

#include "vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#define PLANE_D(b) (plane_d(&b))
#define PLANEV_D(b) (plane_d(b))

#define PLANE(a, b, c, d, e, f) {VECTOR3(a, b, c), VECTOR3(d, e, f)}
#define PLANEV(a, b) {VECTOR3V(a), VECTOR3V(b)}

struct ray_s;
struct line_s;
struct triangle_s;

typedef struct plane_s
{
    vector3_t position;
    vector3_t normal;
} plane_t;

#define PLANE_POSITION(b) (b).position
#define PLANE_NORMAL(b) (b).normal
#define PLANEV_POSITION(b) (b)->position
#define PLANEV_NORMAL(b) (b)->normal

void plane_make(plane_t *plane, const vector3_t *pos, const vector3_t *nor);
float plane_point_to_plane_distance(const plane_t *plane, const vector3_t *point);
float plane_d(const plane_t *plane);

int plane_ray_intersect(const plane_t *plane, const struct ray_s *line, float *lamda, vector3_t *point);
void plane_triangle_plane(plane_t *plane, const struct triangle_s *tri);
int plane_point_clip(const plane_t *plane, const vector3_t *v);

int plane_line_intersect(const plane_t *plane, const struct line_s *line, float *lamda, vector3_t *point, int *dir, int *mask);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_PLANE_H
