#ifndef _KARIN_NL_SHADOW_H
#define _KARIN_NL_SHADOW_H

#include "nl_gl.h"
#include "math/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NETLIZARD_SHADOW_RENDER_NONE 0
#define NETLIZARD_SHADOW_RENDER_ITEM 1
#define NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR (1 << 1)
#define NETLIZARD_SHADOW_RENDER_SCENE_WALL (1 << 2)
#define NETLIZARD_SHADOW_RENDER_SCENE (NETLIZARD_SHADOW_RENDER_SCENE_CEIL_AND_FLOOR | NETLIZARD_SHADOW_RENDER_SCENE_WALL)
#define NETLIZARD_SHADOW_RENDER_ALL 0xFF

void NETLizard_RenderNETLizardModelSceneShadow(const GL_NETLizard_3D_Model *map_model, const int scenes[], unsigned int count, const vector3_t *light_position, int dirlight, int method, int render_mask);
void NETLizard_RenderNETLizardModelShadow(const GL_NETLizard_3D_Model *map_model,  const vector3_t *light_position, int dirlight, int method, int render_mask);
#if 0
void Shadow_RenderNETLizard3DAnimationModel(const GL_NETLizard_3D_Animation_Model *m, int a, int f, const float pos[3], float xangle, float yangle, const Light_Source_s *light, int method);
#endif
void NETLizard_RenderNETLizard3DMeshShadow(const GL_NETLizard_3D_Mesh *m, const vector3_t *light_position, int dirlight, int method);

#ifdef __cplusplus
}
#endif

#endif
