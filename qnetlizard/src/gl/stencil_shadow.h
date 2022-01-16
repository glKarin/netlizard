#ifndef _KARIN_SHADOW_H
#define _KARIN_SHADOW_H

#include "math/vector3.h"
#include "nl_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SHADOW_Z_PASS 1
#define SHADOW_Z_FAIL 2

#define SHADOW_INVERT_ALL 1
#define SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR 2

void NETLizard_RenderMeshShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_t *light_position, int dirlight, int method, int invert);

#ifdef __cplusplus
}
#endif

#endif
