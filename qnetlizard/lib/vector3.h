#ifndef _KARIN_VECTOR3_H
#define _KARIN_VECTOR3_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vector3_s
{
	float v[3];
} vector3_t;

#define VECTOR3_X(vec) (vec).v[0]
#define VECTOR3_Y(vec) (vec).v[1]
#define VECTOR3_Z(vec) (vec).v[2]
#define VECTOR3_V(vec) (vec).v

#define VECTOR3V_X(vec) (vec)->v[0]
#define VECTOR3V_Y(vec) (vec)->v[1]
#define VECTOR3V_Z(vec) (vec)->v[2]
#define VECTOR3V_V(vec) (vec)->v

#define COLOR_R(vec) (vec).v[0]
#define COLOR_G(vec) (vec).v[1]
#define COLOR_B(vec) (vec).v[2]

#define TEXCOORD_S(vec) (vec).v[0]
#define TEXCOORD_T(vec) (vec).v[1]
#define TEXCOORD_R(vec) (vec).v[2]

#define EULER_PITCH(vec) (vec).v[0]
#define EULER_YAW(vec) (vec).v[1]
#define EULER_ROLL(vec) (vec).v[2]

#define VECTOR3V(arr) {{arr[0], arr[1], arr[2]}}
#define VECTOR3(x, y, z) {{x, y, z}}

void vector3_normalizev(vector3_t *a);
void vector3_scalev(vector3_t *a, float n);
void vector3_invertv(vector3_t *a);
void vector3_addv(vector3_t *r, const vector3_t *a, const vector3_t *b);
void vector3_subtractv(vector3_t *r, const vector3_t *a, const vector3_t *b);
void vector3_multiplyv(vector3_t *r, const vector3_t *a, const vector3_t *b);
void vector3_dividev(vector3_t *r, const vector3_t *a, const vector3_t *b);
void vector3_crossv(vector3_t *r, const vector3_t *a, const vector3_t *b);
void vector3_directionv(vector3_t *r, const vector3_t *a, const vector3_t *b);

vector3_t vector3_normalize(const vector3_t *vec);
vector3_t vector3_cross(const vector3_t *a, const vector3_t *b);
vector3_t vector3_add(const vector3_t *a, const vector3_t *b);
vector3_t vector3_subtract(const vector3_t *a, const vector3_t *b);
vector3_t vector3_multiply(const vector3_t *a, const vector3_t *b);
vector3_t vector3_divide(const vector3_t *a, const vector3_t *b);
vector3_t vector3_scale(const vector3_t *a, float n);
vector3_t vector3_invert(const vector3_t *a);
vector3_t vector3_direction(const vector3_t *a, const vector3_t *b);

void vector3_addve(vector3_t *r, const vector3_t *a);
void vector3_subtractve(vector3_t *r, const vector3_t *a);
void vector3_multiplyve(vector3_t *r, const vector3_t *a);
void vector3_divideve(vector3_t *r, const vector3_t *a);

float vector3_dot(const vector3_t *a, const vector3_t *b);
float vector3_length(const vector3_t *a);
int vector3_valid(const vector3_t *a);
int vector3_equals(const vector3_t *a, const vector3_t *b);
int vector3_iszero(const vector3_t *a);

vector3_t vector3_identity(void);
void vector3_identityv(vector3_t *vec);
void vector3_assignv(vector3_t *r, const vector3_t *a);

void vector3_makev(vector3_t *vec, float x, float y, float z);
vector3_t vector3_make(float x, float y, float z);
void vector3_makeptrv(vector3_t *vec, const float arr[3]);
vector3_t vector3_makeptr(const float arr[3]);

void vector3_moveve(vector3_t *r, const vector3_t *dir, float t);
void vector3_movev(vector3_t *r, const vector3_t *v, const vector3_t *dir, float t);
vector3_t vector3_move(const vector3_t *v, const vector3_t *dir, float t);

#ifdef __cplusplus
}
#endif

#endif
