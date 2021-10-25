#ifndef _KARIN_NL_SHADOW_H
#define _KARIN_NL_SHADOW_H

#include "nl_gl.h"
#include "shadow.h"

void Shadow_RenderNETLizardModelScene(const GL_NETLizard_3D_Model *map_model, const int *scenes, unsigned int count, const vector3_s *light_position, int dirlight, int method);
void Shadow_RenderNETLizardModel(const GL_NETLizard_3D_Model *map_model,  const vector3_s *light_position, int dirlight, int method);
void Shadow_RenderMask(void);
#if 0
void Shadow_RenderNETLizard3DAnimationModel(const GL_NETLizard_3D_Animation_Model *m, int a, int f, const float pos[3], float xangle, float yangle, const Light_Source_s *light, int method);
#endif
void Shadow_RenderNETLizard3DMesh(const GL_NETLizard_3D_Mesh *m, const vector3_s *light_position, int dirlight, int method);

#endif
