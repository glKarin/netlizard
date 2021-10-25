#ifndef _KARIN_SHADOW_H
#define _KARIN_SHADOW_H

#include "lib/vector3.h"
#include "nl_gl.h"

#define SHADOW_VOLUME_LENGTH 10000
#define SHADOW_VOLUME_FAR_W 1
#define DIR_LIGHTING 0
#define ZFAIL_SHADOW 0 // using Z-Fail
#define SHADOW_MASK 1 // 
#define SHADOW_MASK_LIGHT 0 // 

#define _HARMATTAN_OPENGLES;

#define SHADOW_Z_PASS 0
#define SHADOW_Z_FAIL 1

vector3_s cale_light_direction(const vector3_s *v, const vector3_s *lightpos, int dirlight);
void Shadow_MakeVolume(GL_NETLizard_3D_Mesh *r, const vector3_s *light_position, const int dirlight, const GL_NETLizard_3D_Mesh *mat, int method);
void Shadow_RenderVolume(const GL_NETLizard_3D_Mesh *nl_mesh, const vector3_s *light_position, const int dirlight, int m);

void Shadow_CaleTrans(GL_NETLizard_3D_Mesh *r, const GL_NETLizard_3D_Mesh *nl_mesh, int invert);
void Shadow_RenderShadow(const GL_NETLizard_3D_Mesh *mesh, const vector3_s *light_position, int dirlight, int method);

#endif
