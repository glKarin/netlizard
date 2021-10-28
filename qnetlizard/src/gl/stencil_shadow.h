#ifndef _KARIN_SHADOW_H
#define _KARIN_SHADOW_H

#include "lib/vector3.h"
#include "nl_gl.h"

#define SHADOW_Z_PASS 1
#define SHADOW_Z_FAIL 2

#define SHADOW_INVERT_ALL 1
#define SHADOW_INVERT_EXCLUDE_CEIL_AND_FLOOR 2

void NETLizard_RenderMeshShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_s *light_position, int dirlight, int method, int invert);

#endif
