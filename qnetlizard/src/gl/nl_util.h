#ifndef _KARIN_NL_UTIL_H
#define _KARIN_NL_UTIL_H

#include "nl_gl.h"
#include "bound.h"

#ifdef __cplusplus
extern "C" {
#endif

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int c[], float frustum[][4]);
void NETLizard_GetNETLizard3DMapBound(const GL_NETLizard_3D_Model *model, int *scenes, unsigned int count, bound_s *box);
void NETLizard_GetNETLizard3DMeshBound(const GL_NETLizard_3D_Mesh *mesh, unsigned int count, bound_s *box);

#ifdef __cplusplus
}
#endif
#endif
