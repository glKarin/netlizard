#include "triangle.h"

void triangle_make(triangle_t *obj, const vector3_t *a, const vector3_t *b, const vector3_t *c)
{
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

void triangle_cale_normal(const triangle_t *tri, vector3_t *normal)
{
    vector3_t first, second;
    vector3_subtractv(&first, &(TRIANGLEV_B(tri)), &(TRIANGLEV_A(tri)));
    vector3_subtractv(&second, &(TRIANGLEV_C(tri)), &(TRIANGLEV_A(tri)));

    //vector3_crossv(normal, &second, &first); // org
    vector3_crossv(normal, &first, &second);
    vector3_normalizev(normal);
}

