#include "triangle.h"

void triangle_make(triangle_s *obj, const vector3_s *a, const vector3_s *b, const vector3_s *c)
{
    if(!obj)
        return;
    TRIANGLEV_A_X(obj) = VECTOR3V_X(a);
    TRIANGLEV_A_Y(obj) = VECTOR3V_Y(a);
    TRIANGLEV_A_Z(obj) = VECTOR3V_Z(a);
    TRIANGLEV_B_X(obj) = VECTOR3V_X(b);
    TRIANGLEV_B_Y(obj) = VECTOR3V_Y(b);
    TRIANGLEV_B_Z(obj) = VECTOR3V_Z(b);
    TRIANGLEV_C_X(obj) = VECTOR3V_X(c);
    TRIANGLEV_C_Y(obj) = VECTOR3V_Y(c);
    TRIANGLEV_C_Z(obj) = VECTOR3V_Z(c);
}

void triangle_cale_normal(const triangle_s *tri, vector3_s *normal)
{
    if(!tri || !normal)
        return;

    vector3_s first, second;
    vector3_subtractv(&first, &(TRIANGLEV_B(tri)), &(TRIANGLEV_A(tri)));
    vector3_subtractv(&second, &(TRIANGLEV_C(tri)), &(TRIANGLEV_A(tri)));

    //vector3_crossv(normal, &second, &first); // org
    vector3_crossv(normal, &first, &second);
    vector3_normalizev(normal);
}
