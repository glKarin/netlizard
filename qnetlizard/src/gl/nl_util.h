#ifndef _KARIN_NL_UTIL_H
#define _KARIN_NL_UTIL_H

#include "nl_gl.h"
#include "lib/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bound_s;
struct plane_s;

typedef vector3_t nl_vector3_t;

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int c[], float frustum[6][4]);
int NETLizard_GetMapRenderItems(const GL_NETLizard_3D_Model *model, int items[], float frustum[6][4]);
void NETLizard_GetNETLizard3DMapBound(const GL_NETLizard_3D_Model *model, int scenes[], unsigned int count, struct bound_s *box);
void NETLizard_GetNETLizard3DMeshBound(const GL_NETLizard_3D_Mesh *mesh, unsigned int count, struct bound_s *box);
void NETLizard_GetNETLizard3DMeshTransformBound(const GL_NETLizard_3D_Mesh *meshs, unsigned int count, struct bound_s *box);
int NETLizard_GetNETLizard3DMapNeighboringScenes(const GL_NETLizard_3D_Model *model, int scene, int scenes[]);
void NETLizard_GetMeshBound(const GL_NETLizard_3D_Mesh *scene, struct bound_s *bound);
void NETLizard_GetMeshPlane(const GL_NETLizard_3D_Mesh *scene, int j, struct plane_s *plane);
void NETLizard_GetSceneFullBound(const GL_NETLizard_3D_Model *model, GLuint s, struct bound_s *bound);
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
