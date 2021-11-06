#include "euler.h"

float clamp_degree(float angle)
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
        r = 360 - fabs(i % 360 + f);
    }
    else
        r = angle;
    if(r == 360.0)
        r = 0.0;
    return r;
}

float rad2deg(float rad)
{
    return rtod(rad);
}

float deg2rad(float deg)
{
    return dtor(deg);
}
