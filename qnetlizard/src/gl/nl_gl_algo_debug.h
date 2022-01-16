#ifndef _KARIN_NLGLALGODEBUG_H
#define _KARIN_NLGLALGODEBUG_H

struct _GL_NETLizard_3D_Model;

#ifdef __cplusplus
extern "C" {
#endif

struct vector3_s;

void NETLizard_DebugHighlightRenderGL3DModelPlane(const struct _GL_NETLizard_3D_Model *model, unsigned int scene, unsigned int plane_index);
void NETLizard_DebugHighlightRenderGL3DItemModelEdge(const struct _GL_NETLizard_3D_Model *model, unsigned int scene, unsigned int item_index, const struct vector3_s *pos, const struct vector3_s *dir);

#ifdef __cplusplus
}
#endif

#endif
