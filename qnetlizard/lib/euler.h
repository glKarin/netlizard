#ifndef _KARIN_EULER_H
#define _KARIN_EULER_H

#ifdef __cplusplus
extern "C" {
#endif

#define rtod(rad) ((rad) / M_PI * 180.0)
#define dtor(deg) ((deg) / 180.0 * M_PI)

float clamp_degree(float angle);
float rad2deg(float rad);
float deg2rad(float deg);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_EULER_H
