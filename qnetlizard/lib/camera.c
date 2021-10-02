#include "camera.h"

#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

void newcam(camera_s *cam)
{
    if(!cam)
        return;

    // not use memcpy
    VECTOR3_X(cam->position) = VECTOR3_Y(cam->position) = VECTOR3_Z(cam->position) = 0;
    VECTOR3_X(cam->rotation) = VECTOR3_Y(cam->rotation) = VECTOR3_Z(cam->rotation) = 0;
    VECTOR3_X(cam->up) = VECTOR3_Z(cam->up) = 0;
    VECTOR3_Y(cam->up) = 1;
    VECTOR3_X(cam->direction) = VECTOR3_Y(cam->direction) = 0;
    VECTOR3_Z(cam->direction) = -1;
    VECTOR3_X(cam->scale) = VECTOR3_Y(cam->scale) = VECTOR3_Z(cam->scale) = 1;
}

void initcam(camera_s *cam, const vector3_s *pos, const vector3_s *rot)
{
    if(!cam)
        return;

    if(pos)
    {
        VECTOR3_X(cam->position) = VECTOR3_X(*pos);
        VECTOR3_Y(cam->position) = VECTOR3_Y(*pos);
        VECTOR3_Z(cam->position) = VECTOR3_Z(*pos);
    }

    if(rot)
    {
        VECTOR3_X(cam->rotation) = clampangle(VECTOR3_X(*rot));
        VECTOR3_Y(cam->rotation) = clampangle(VECTOR3_Y(*rot));
        //VECTOR3_Z(cam->rotation) = clampangle(VECTOR3_Z(*rot));
        angtodir(&cam->direction, &cam->rotation);
    }
    VECTOR3_X(cam->up) = VECTOR3_Z(cam->up) = 0;
    VECTOR3_Y(cam->up) = 1;
}

void cammove(camera_s *cam, const vector3_s *unit)
{
    vector3_s v;
    vector3_s left;

    if(!unit)
        return;

    vector3_crossv(&left, &cam->direction, &cam->up);

    v = vector3_scale(&left, VECTOR3_X(*unit));
    cam->position = vector3_add(&cam->position, &v);

    v = vector3_scale(&cam->up, VECTOR3_Y(*unit));
    cam->position = vector3_add(&cam->position, &v);

    v = vector3_scale(&cam->direction, VECTOR3_Z(*unit));
    cam->position = vector3_add(&cam->position, &v);
}

void camrot(camera_s *cam, const vector3_s *unit)
{
    if(!unit)
        return;

    VECTOR3_X(cam->rotation) = clampangle(VECTOR3_X(cam->rotation) + VECTOR3_X(*unit));
    VECTOR3_Y(cam->rotation) = clampangle(VECTOR3_Y(cam->rotation) + VECTOR3_Y(*unit));
    //VECTOR3_Z(cam->rotation) = clampangle(VECTOR3_Z(cam->rotation) + VECTOR3_Z(unit));
    angtodir(&cam->direction, &cam->rotation);
}

void angtodir(vector3_s *r, const vector3_s *a)
{
    float xrad, yrad;
    float x, y, z;
    float xz;

    if(!r || !a)
        return;

    xrad = ator(VECTOR3_X(*a));
    yrad = ator(VECTOR3_Y(*a));
    y = sin(xrad);
    xz = cos(xrad);
    x = -sin(yrad) * xz;
    z = cos(yrad) * xz;
    VECTOR3_X(*r) = -x;
    VECTOR3_Y(*r) = -y;
    VECTOR3_Z(*r) = -z;
    vector3_normalizev(r);
}

void camscale(camera_s *cam, const vector3_s *factory)
{
    vector3_multiply(&cam->scale, factory);
}

float clampangle(float angle)
{
    int i = (int)angle;
    float f = angle - i;
    float r = 0.0;
    if(angle > 360)
    {
        r = i % 360 + f;
    }
    else if(angle < 0)
    {
        r = 360 - abs(i % 360 + f);
    }
    else
        r = angle;
    if(r == 360.0)
        r = 0.0;
    return r;
}

void camtrans_gl(GLmatrix *r, const camera_s *cam)
{
    if(!r || !cam)
        return;
#if 0
    vector3_s forward = vector3_add(&cam->position, &cam->direction);
    Mesa_gluLookAt(r, 
                VECTOR3_X(cam->position),
                VECTOR3_Y(cam->position),
                VECTOR3_Z(cam->position),
                VECTOR3_X(forward),
                VECTOR3_Y(forward),
                VECTOR3_Z(forward),
                VECTOR3_X(cam->up),
                VECTOR3_Y(cam->up),
                VECTOR3_Z(cam->up)
                );
#else
    Mesa_glRotate(r, VECTOR3_X(cam->rotation), 1, 0, 0);
    Mesa_glRotate(r, VECTOR3_Y(cam->rotation), 0, 1, 0);
    Mesa_glTranslate(r, 
                -VECTOR3_X(cam->position),
                -VECTOR3_Y(cam->position),
                -VECTOR3_Z(cam->position)
                );
#endif
}
