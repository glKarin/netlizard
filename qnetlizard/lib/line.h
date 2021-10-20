#ifndef _KARIN_LINE_H
#define _KARIN_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector3.h"

#define RAY_POSITION_X(obj) VECTOR3_X((obj).position)
#define RAY_POSITION_Y(obj) VECTOR3_Y((obj).position)
#define RAY_POSITION_Z(obj) VECTOR3_Z((obj).position)
#define RAY_DIRECTION_X(obj) VECTOR3_X((obj).direction)
#define RAY_DIRECTION_Y(obj) VECTOR3_Y((obj).direction)
#define RAY_DIRECTION_Z(obj) VECTOR3_Z((obj).direction)

#define RAYV_POSITION_X(obj) VECTOR3_X((obj)->position)
#define RAYV_POSITION_Y(obj) VECTOR3_Y((obj)->position)
#define RAYV_POSITION_Z(obj) VECTOR3_Z((obj)->position)
#define RAYV_DIRECTION_X(obj) VECTOR3_X((obj)->direction)
#define RAYV_DIRECTION_Y(obj) VECTOR3_Y((obj)->direction)
#define RAYV_DIRECTION_Z(obj) VECTOR3_Z((obj)->direction)

#define RAY(a, b, c, d, e, f) {VECTOR3(a, b, c), VECTOR3(d, e, f)}
#define RAYV(a, b) {VECTOR3V(a), VECTOR3V(b)}
#define LINE(a, b, c, d, e, f) {VECTOR3(a, b, c), VECTOR3(d, e, f)}
#define LINEV(a, b) {VECTOR3V(a), VECTOR3V(b)}

#define RAY_POSITION(obj) (obj).position
#define RAY_DIRECTION(obj) (obj).direction
#define RAYV_POSITION(obj) (obj)->position
#define RAYV_DIRECTION(obj) (obj)->direction

#define LINE_A_X(obj) VECTOR3_X((obj).a)
#define LINE_A_Y(obj) VECTOR3_Y((obj).a)
#define LINE_A_Z(obj) VECTOR3_Z((obj).a)
#define LINE_B_X(obj) VECTOR3_X((obj).b)
#define LINE_B_Y(obj) VECTOR3_Y((obj).b)
#define LINE_B_Z(obj) VECTOR3_Z((obj).b)

#define LINEV_A_X(obj) VECTOR3_X((obj)->a)
#define LINEV_A_Y(obj) VECTOR3_Y((obj)->a)
#define LINEV_A_Z(obj) VECTOR3_Z((obj)->a)
#define LINEV_B_X(obj) VECTOR3_X((obj)->b)
#define LINEV_B_Y(obj) VECTOR3_Y((obj)->b)
#define LINEV_B_Z(obj) VECTOR3_Z((obj)->b)

#define LINE_A(obj) (obj).a
#define LINE_B(obj) (obj).b
#define LINEV_A(obj) (obj)->a
#define LINEV_B(obj) (obj)->b

struct _plane_s;

typedef struct _line_s
{
    vector3_s a, b;
} line_s;

typedef struct _ray_s
{
    vector3_s position;
    vector3_s direction;
} ray_s;

void line_make(line_s *line, const vector3_s *a, const vector3_s *b);
void line_direction(const line_s *line, vector3_s *dir);

void ray_make(ray_s *line, const vector3_s *pos, const vector3_s *dir);
void ray_line_to_ray(ray_s *r, const line_s *l);
int ray_line_to_plane_intersect(const ray_s *line, const struct _plane_s *plane, vector3_s *point);
int ray_line_to_plane_collision(const ray_s *line, const struct _plane_s *plane, float *lamda, vector3_s *normal);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_LINE_H
