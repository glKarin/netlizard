#ifndef _KARIN_MATH_STD_H
#define _KARIN_MATH_STD_H

#ifdef __cplusplus
extern "C" {
#endif

#define FLOAT_EQUALS(a, b, p) (fabs((a) - (b)) < (p))
#define FLOAT_GREATER(a, b, p) (((a) - (b)) > (p))
#define FLOAT_LESS(a, b, p) (((b) - (a)) > (p))
#define FLOAT_GEQUALS(a, b, p) ((FLOAT_EQUALS(a, b, p)) || (FLOAT_GREATER(a, b, p)))
#define FLOAT_LEQUALS(a, b, p) ((FLOAT_EQUALS(a, b, p)) || (FLOAT_LESS(a, b, p)))

#define EPSLON 1e-6f

#define IS_ZERO(x) fabs(x) < 1e-6f

#ifdef __cplusplus
}
#endif

#endif // _KARIN_MATH_STD_H
