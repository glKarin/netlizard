#include "nlmath.h"

#include <math.h>

namespace NL
{

float clamp_angle(float angle)
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

void cale_normal_matrix(NLMatrix4 &r, const NLMatrix4 &mat)
{
    float m[16] = {
        GL_MATRIX_M(mat)[0], GL_MATRIX_M(mat)[1], GL_MATRIX_M(mat)[2], 0.0,
        GL_MATRIX_M(mat)[4], GL_MATRIX_M(mat)[5], GL_MATRIX_M(mat)[6], 0.0,
        GL_MATRIX_M(mat)[8], GL_MATRIX_M(mat)[9], GL_MATRIX_M(mat)[10], 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    Mesa_glLoadIdentity(&r);
    Mesa_InverseTransposeMatrix(&r, m);
}

}
