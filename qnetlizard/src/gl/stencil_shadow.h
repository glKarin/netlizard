#ifndef _KARIN_SHADOW_H
#define _KARIN_SHADOW_H

#ifdef __cplusplus
extern "C" {
#endif

struct _GL_NETLizard_3D_Mesh;
struct vector3_s;

#define SHADOW_Z_PASS 1
#define SHADOW_Z_FAIL 2

#define SHADOW_INVERT_ALL 1
#define SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR 2

void NETLizard_RenderMeshShadow(const struct _GL_NETLizard_3D_Mesh *mesh, const struct vector3_s *light_position, int dirlight, int method, int invert);

#ifdef __cplusplus
}
#endif

#endif
