#ifndef _KARIN_NL_UTIL_H
#define _KARIN_NL_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

struct _GL_NETLizard_3D_Model;
struct _GL_NETLizard_3D_Mesh;

struct bound_s;
struct plane_s;
struct vector3_s;

typedef struct vector3_s nl_vector3_t;

int NETLizard_GetMapRenderScenes(const struct _GL_NETLizard_3D_Model *model, int c[], float frustum[6][4]);
int NETLizard_GetMapRenderItems(const struct _GL_NETLizard_3D_Model *model, int items[], float frustum[6][4]);
void NETLizard_GetNETLizard3DMapBound(const struct _GL_NETLizard_3D_Model *model, int scenes[], unsigned int count, struct bound_s *box);
void NETLizard_GetNETLizard3DMeshBound(const struct _GL_NETLizard_3D_Mesh *mesh, unsigned int count, struct bound_s *box);
void NETLizard_GetNETLizard3DMeshTransformBound(const struct _GL_NETLizard_3D_Mesh *meshs, unsigned int count, struct bound_s *box);
int NETLizard_GetNETLizard3DMapNeighboringScenes(const struct _GL_NETLizard_3D_Model *model, int scene, int scenes[]);
void NETLizard_GetMeshBound(const struct _GL_NETLizard_3D_Mesh *scene, struct bound_s *bound);
void NETLizard_GetMeshPlane(const struct _GL_NETLizard_3D_Mesh *scene, int j, struct plane_s *plane);
void NETLizard_GetSceneFullBound(const struct _GL_NETLizard_3D_Model *model, unsigned int s, struct bound_s *bound);
// (x, y, z) -> (x, z, -y)
void conv_gl_vector3(nl_vector3_t *v);
struct vector3_s to_gl_vector3(const nl_vector3_t *v);
// (x, y, z) -> (x, -z, y)
void conv_nl_vector3(struct vector3_s *v);
nl_vector3_t to_nl_vector3(const struct vector3_s *v);

#ifdef __cplusplus
}
#endif
#endif
