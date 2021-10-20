#ifndef _KARIN_TRIANGLE_H
#define _KARIN_TRIANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector3.h"

#define TRIANGLE_A_X(t) VECTOR3_X((t).a)
#define TRIANGLE_A_Y(t) VECTOR3_Y((t).a)
#define TRIANGLE_A_Z(t) VECTOR3_Z((t).a)
#define TRIANGLE_B_X(t) VECTOR3_X((t).b)
#define TRIANGLE_B_Y(t) VECTOR3_Y((t).b)
#define TRIANGLE_B_Z(t) VECTOR3_Z((t).b)
#define TRIANGLE_C_X(t) VECTOR3_X((t).c)
#define TRIANGLE_C_Y(t) VECTOR3_Y((t).c)
#define TRIANGLE_C_Z(t) VECTOR3_Z((t).c)

#define TRIANGLEV_A_X(t) VECTOR3_X((t)->a)
#define TRIANGLEV_A_Y(t) VECTOR3_Y((t)->a)
#define TRIANGLEV_A_Z(t) VECTOR3_Z((t)->a)
#define TRIANGLEV_B_X(t) VECTOR3_X((t)->b)
#define TRIANGLEV_B_Y(t) VECTOR3_Y((t)->b)
#define TRIANGLEV_B_Z(t) VECTOR3_Z((t)->b)
#define TRIANGLEV_C_X(t) VECTOR3_X((t)->c)
#define TRIANGLEV_C_Y(t) VECTOR3_Y((t)->c)
#define TRIANGLEV_C_Z(t) VECTOR3_Z((t)->c)

#define TRIANGLE(a, b, c, d, e, f, g, h, i) {VECTOR3(a, b, c), VECTOR3(d, e, f), VECTOR3(g, h, i)}
#define TRIANGLEV(a, b, c) {VECTOR3V(a), VECTOR3V(b), VECTOR3V(c)}

#define TRIANGLE_A(t) (t).a
#define TRIANGLE_B(t) (t).b
#define TRIANGLE_C(t) (t).c
#define TRIANGLEV_A(t) (t)->a
#define TRIANGLEV_B(t) (t)->b
#define TRIANGLEV_C(t) (t)->c

typedef struct _triangle_s
{
    vector3_s a, b, c;
    vector3_s normal;
    int face;
} triangle_s;

void triangle_make(triangle_s *tri, const vector3_s *a, const vector3_s *b, const vector3_s *c);
void triangle_cale_normal(const triangle_s *tri, vector3_s *normal);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_TRIANGLE_H
