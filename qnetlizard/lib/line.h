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

struct plane_s;

typedef struct line_s
{
    vector3_t a, b;
} line_t;

typedef struct ray_s
{
    vector3_t position;
    vector3_t direction;
} ray_t;

void line_make(line_t *line, const vector3_t *a, const vector3_t *b);
void line_direction(const line_t *line, vector3_t *dir);
int line_equals(const line_t *a, const line_t *b);
int line_equals_ignore_seq(const line_t *a, const line_t *b);
void line_ray_to_line(line_t *r, const ray_t *a, float length);
float line_length(const line_t *line);
void line_invertv(line_t *line);
int line_iszero(const line_t *line);

void ray_make(ray_t *line, const vector3_t *pos, const vector3_t *dir);
void ray_line_to_ray(ray_t *r, const line_t *l);
int ray_to_plane_intersect(const ray_t *line, const struct plane_s *plane, vector3_t *point);
int ray_to_plane_collision(const ray_t *line, const struct plane_s *plane, float *lamda, vector3_t *normal);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_LINE_H
