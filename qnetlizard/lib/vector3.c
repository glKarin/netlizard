#include "vector3.h"

#include <math.h>

#define K_ZERO 0.00001
#define IS_ZERO(n) ((n) < K_ZERO && (n) > -K_ZERO)

#define IF_NULL_RETURN(a) if(!a) return;
#define IF_NULL_RETURN2(a, b) if(!a || !b) return;
#define IF_NULL_RETURN3(a, b, c) if(!a || !b || !c) return;
#define IF_NULL_RETURNV(a, v) if(!a) return v;
#define IF_NULL_RETURNV2(a, b, v) if(!a || !b) return v;

static const vector3_s Identity_Vector3 = { { 0, 0, 0 } };

vector3_s vector3_direction(const vector3_s *a, const vector3_s *b)
{
    vector3_s r = Identity_Vector3;
    vector3_directionv(&r, a, b);
    return r;
}

vector3_s vector3_normalize(const vector3_s *a)
{
	float m;
	IF_NULL_RETURNV(a, Identity_Vector3)
	
	m = vector3_length(a);
	if(m != 0.0)
	{
		m = 1.0 / m;
		return vector3_scale(a, m);
	}

	return Identity_Vector3;
}

float vector3_dot(const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURNV2(a, b, 0)
	
	return VECTOR3V_X(a) * VECTOR3V_X(b) + VECTOR3V_Y(a) * VECTOR3V_Y(b) + VECTOR3V_Z(a) * VECTOR3V_Z(b);
}

vector3_s vector3_cross(const vector3_s *a, const vector3_s *b)
{
	vector3_s r = Identity_Vector3;
    vector3_crossv(&r, a, b);
	return r;
}

float vector3_length(const vector3_s *a)
{
#define POW2(n) ((n) * (n))
	IF_NULL_RETURNV(a, 0)
    return sqrt(POW2(VECTOR3V_X(a)) + POW2(VECTOR3V_Y(a)) + POW2(VECTOR3V_Z(a)));
#undef POW2
}

vector3_s vector3_add(const vector3_s *a, const vector3_s *b)
{
	vector3_s r = Identity_Vector3;
	vector3_addv(&r, a, b);
	return r;
}

vector3_s vector3_subtract(const vector3_s *a, const vector3_s *b)
{
	vector3_s r = Identity_Vector3;
	vector3_subtractv(&r, a, b);
	return r;
}

vector3_s vector3_multiply(const vector3_s *a, const vector3_s *b)
{
	vector3_s r = Identity_Vector3;
	vector3_multiplyv(&r, a, b);
	return r;
}

vector3_s vector3_divide(const vector3_s *a, const vector3_s *b)
{
	vector3_s r = Identity_Vector3;
	vector3_dividev(&r, a, b);
	return r;
}

vector3_s vector3_scale(const vector3_s *a, float n)
{
    vector3_s r = Identity_Vector3;

	IF_NULL_RETURNV(a, Identity_Vector3)

	VECTOR3_X(r) = VECTOR3V_X(a) * n;
	VECTOR3_Y(r) = VECTOR3V_Y(a) * n;
	VECTOR3_Z(r) = VECTOR3V_Z(a) * n;
	return r;
}

int vector3_valid(const vector3_s *a)
{
	IF_NULL_RETURNV(a, -1)
	return (VECTOR3V_X(a) != 0 || VECTOR3V_Y(a) != 0 || VECTOR3V_Z(a) != 0) ? 0 : -1;
}

int vector3_equals(const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURNV(a, -1)
	
	return (VECTOR3V_X(a) == VECTOR3V_X(b) && VECTOR3V_Y(a) == VECTOR3V_Y(b) && VECTOR3V_Z(a) == VECTOR3V_Z(b)) ? 0 : -1;
}

vector3_s vector3_invert(const vector3_s *a)
{
    vector3_s r = Identity_Vector3;
	IF_NULL_RETURNV(a, Identity_Vector3)

	VECTOR3_X(r) = -VECTOR3V_X(a);
	VECTOR3_Y(r) = -VECTOR3V_Y(a);
	VECTOR3_Z(r) = -VECTOR3V_Z(a);
	return r;
}


void vector3_normalizev(vector3_s *a)
{
	float m;
	IF_NULL_RETURN(a)
	
	m = vector3_length(a);
	if(m != 0.0)
	{
		m = 1.0 / m;
        vector3_scalev(a, m);
	}
}

void vector3_scalev(vector3_s *a, float n)
{
	IF_NULL_RETURN(a)
	
	VECTOR3V_X(a) *= n;
	VECTOR3V_Y(a) *= n;
	VECTOR3V_Z(a) *= n;
}

void vector3_invertv(vector3_s *a)
{
	IF_NULL_RETURN(a)
	
	VECTOR3V_X(a) = -VECTOR3V_X(a);
	VECTOR3V_Y(a) = -VECTOR3V_Y(a);
	VECTOR3V_Z(a) = -VECTOR3V_Z(a);
}

void vector3_addv(vector3_s *r, const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURN3(a, b, r)
	
	VECTOR3V_X(r) = VECTOR3V_X(a) + VECTOR3V_X(b);
	VECTOR3V_Y(r) = VECTOR3V_Y(a) + VECTOR3V_Y(b);
	VECTOR3V_Z(r) = VECTOR3V_Z(a) + VECTOR3V_Z(b);
}

void vector3_subtractv(vector3_s *r, const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURN3(a, b, r)
	
	VECTOR3V_X(r) = VECTOR3V_X(a) - VECTOR3V_X(b);
	VECTOR3V_Y(r) = VECTOR3V_Y(a) - VECTOR3V_Y(b);
	VECTOR3V_Z(r) = VECTOR3V_Z(a) - VECTOR3V_Z(b);
}

void vector3_multiplyv(vector3_s *r, const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURN3(a, b, r)
	
	VECTOR3V_X(r) = VECTOR3V_X(a) * VECTOR3V_X(b);
	VECTOR3V_Y(r) = VECTOR3V_Y(a) * VECTOR3V_Y(b);
	VECTOR3V_Z(r) = VECTOR3V_Z(a) * VECTOR3V_Z(b);
}

void vector3_dividev(vector3_s *r, const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURN3(a, b, r)
	
	if(IS_ZERO(VECTOR3V_X(b)) || IS_ZERO(VECTOR3V_Y(b)) || IS_ZERO(VECTOR3V_Z(b)))
		return;
	
	VECTOR3V_X(r) = VECTOR3V_X(a) / VECTOR3V_X(b);
	VECTOR3V_Y(r) = VECTOR3V_Y(a) / VECTOR3V_Y(b);
	VECTOR3V_Z(r) = VECTOR3V_Z(a) / VECTOR3V_Z(b);
}

void vector3_crossv(vector3_s *r, const vector3_s *a, const vector3_s *b)
{
	IF_NULL_RETURN3(a, b, r)
	
	VECTOR3V_X(r) = VECTOR3V_Y(a) * VECTOR3V_Z(b) - VECTOR3V_Z(a) * VECTOR3V_Y(b);
	VECTOR3V_Y(r) = VECTOR3V_Z(a) * VECTOR3V_X(b) - VECTOR3V_X(a) * VECTOR3V_Z(b);
	VECTOR3V_Z(r) = VECTOR3V_X(a) * VECTOR3V_Y(b) - VECTOR3V_Y(a) * VECTOR3V_X(b);
}

void vector3_directionv(vector3_s *r,const vector3_s *a, const vector3_s *b)
{
    IF_NULL_RETURN3(a, b, r)

    vector3_subtractv(r, b, a);
    vector3_normalizev(r);
}
