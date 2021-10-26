#ifndef _KARIN_SHADOW_H
#define _KARIN_SHADOW_H

#include "lib/vector3.h"
#include "nl_gl.h"

#define SHADOW_Z_PASS 1
#define SHADOW_Z_FAIL 2

vector3_s cale_light_direction(const vector3_s *v, const vector3_s *lightpos, int dirlight);
void NETLizard_MakeShadowVolumeMesh(GL_NETLizard_3D_Mesh *r, const vector3_s *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method);
void NETLizard_RenderShadowVolumeMesh(const GL_NETLizard_3D_Mesh *nl_mesh, const vector3_s *light_position, const int dirlight, int m);

void NETLizard_CaleMeshTransform(GL_NETLizard_3D_Mesh *r, const GL_NETLizard_3D_Mesh *nl_mesh, int invert);
void NETLizard_RenderMeshShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_s *light_position, int dirlight, int method);

#endif
