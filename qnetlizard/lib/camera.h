#ifndef _KARIN_CAMERA_H
#define _KARIN_CAMERA_H

#include "vector3.h"
#include "mesa_gl_math.h"

#ifdef __cplusplus
extern "C" {
#endif

// using left-right on X, up-down on Y, forward-backward on Z(far-side is less than near-side)
// same as OpenGL(right-hand)

#define ator(a) ((double)(a) / 180.0 * M_PI)
#define rtoa(r) ((double)(r) / M_PI * 180.0)

#define camidentity newcam

enum Direction_e
{
    Direction_Forward = 0,
    Direction_Backward,
    Direction_Left,
    Direction_Right,
    Direction_Up,
    Direction_Down,
    Direction_Total
};

enum Rotation_e
{
    Rotation_Left = 0,
    Rotation_Right,
    Rotation_Up,
    Rotation_Down,
    Rotation_Roll_Left,
    Rotation_Roll_Right,
    Rotation_Total
};

typedef struct _camera_s
{
    vector3_t position;
    vector3_t direction; // init is to Z(0, 0, -1)
    vector3_t rotation; // Z is always 0(not allow Z-rotation for rolling head)
    vector3_t up; // always up(0, 1, 0) for simple
    vector3_t scale;
} camera_s;

void cammove(camera_s *cam, const vector3_t *unit);
void camrot(camera_s *cam, const vector3_t *unit);
void newcam(camera_s *cam);
void initcam(camera_s *cam, const vector3_t *pos, const vector3_t *rot);
void angtodir(vector3_t *r, const vector3_t *a);
float clampangle(float angle);
void camscale(camera_s *cam, const vector3_t *factory);

void camtrans_gl(GLmatrix *r, const camera_s *cam);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_H
