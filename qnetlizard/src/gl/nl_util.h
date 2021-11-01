#ifndef _KARIN_NL_UTIL_H
#define _KARIN_NL_UTIL_H

#include "nl_gl.h"
#include "lib/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bound_s;

typedef vector3_t nl_vector3_t;

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int c[], float frustum[6][4]);
void NETLizard_GetNETLizard3DMapBound(const GL_NETLizard_3D_Model *model, int scenes[], unsigned int count, struct bound_s *box);
void NETLizard_GetNETLizard3DMeshBound(const GL_NETLizard_3D_Mesh *mesh, unsigned int count, struct bound_s *box);
// (x, y, z) -> (x, z, -y)
void conv_gl_vector3(nl_vector3_t *v);
vector3_t to_gl_vector3(const vector3_t *v);
// (x, y, z) -> (x, -z, y)
void conv_nl_vector3(vector3_t *v);
nl_vector3_t to_nl_vector3(const vector3_t *v);

#ifdef __cplusplus
}
#endif
#endif
