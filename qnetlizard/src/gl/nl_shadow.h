#ifndef _KARIN_NL_SHADOW_H
#define _KARIN_NL_SHADOW_H

#include "nl_gl.h"
#include "shadow.h"

void NETLizard_RenderNETLizardModelSceneShadow(const GL_NETLizard_3D_Model *map_model, const int *scenes, unsigned int count, const vector3_s *light_position, int dirlight, int method);
void NETLizard_RenderNETLizardModelShadow(const GL_NETLizard_3D_Model *map_model,  const vector3_s *light_position, int dirlight, int method);
#if 0
void Shadow_RenderNETLizard3DAnimationModel(const GL_NETLizard_3D_Animation_Model *m, int a, int f, const float pos[3], float xangle, float yangle, const Light_Source_s *light, int method);
#endif
void NETLizard_RenderNETLizard3DMeshShadow(const GL_NETLizard_3D_Mesh *m, const vector3_s *light_position, int dirlight, int method);

#endif
