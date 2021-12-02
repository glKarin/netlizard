#ifndef _KARIN_NLGLALGODEBUG_H
#define _KARIN_NLGLALGODEBUG_H

#include "nl_gl.h"

struct vector3_s;

void NETLizard_DebugHighlightRenderGL3DModelPlane(const GL_NETLizard_3D_Model *model, GLuint scene, GLuint plane_index);
void NETLizard_DebugHighlightRenderGL3DItemModelEdge(const GL_NETLizard_3D_Model *model, GLuint scene, GLuint item_index, const struct vector3_s *pos, const struct vector3_s *dir);

#endif
