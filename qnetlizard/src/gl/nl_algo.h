#ifndef _KARIN_NLALGO_H
#define _KARIN_NLALGO_H

#include "nl_gl.h"
#include "lib/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct collision_object_s
{
    vector3_t position;
    float radius;
    float height;
} collision_object_t;

typedef vector3_t nl_vector3_t;

int NETLizard_FindScenePointIn(const GL_NETLizard_3D_Model *map_model, const nl_vector3_t *p);
int NETLizard_GetScenePointZCoord(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, int scene, int *rscene, float *rglz);
int NETLizard_MapCollisionTesting(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, const vector3_t *new_pos, int *scene);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_NLALGO_H
