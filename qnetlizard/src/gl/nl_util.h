#ifndef _KARIN_NL_UTIL_H
#define _KARIN_NL_UTIL_H

#include "nl_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int c[], float frustum[][4]);

#ifdef __cplusplus
}
#endif
#endif
