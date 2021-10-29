#ifndef _KARIN_NLGLDEBUG_H
#define _KARIN_NLGLDEBUG_H

#include "nl_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NETLIZARD_DEBUG_RENDER_NONE 0
#define NETLIZARD_DEBUG_RENDER_MAP_BOUND 1
#define NETLIZARD_DEBUG_RENDER_VERTEX_NORMAL (1 << 1)
#define NETLIZARD_DEBUG_RENDER_SCENE_BOUND (1 << 2)
#define NETLIZARD_DEBUG_RENDER_ITEM_BOUND (1 << 3)

#define NETLizard_DebugRenderGL3DModel_VertexNormal(m) NETLizard_DebugRenderGL3DModel(m, NETLizard_DebugRenderGL3DMeshVertexNormal)
#define NETLizard_DebugRenderGL3DMapModelScene_VertexNormal(m, s, c) NETLizard_DebugRenderGL3DMapModelScene(m, s, c, NETLizard_DebugRenderGL3DMeshVertexNormal)

#define NETLizard_DebugRenderGL3DModel_Bound(m) NETLizard_DebugRenderGL3DModel(m, NETLizard_DebugRenderGL3DMeshBound)
#define NETLizard_DebugRenderGL3DMapModelScene_Bound(m, s, c) NETLizard_DebugRenderGL3DMapModelScene(m, s, c, NETLizard_DebugRenderGL3DMeshBound)

typedef void (*GL_NETLizard_Debug_Render_Mesh_f)(const GL_NETLizard_3D_Mesh *mesh);

void NETLizard_DebugRenderGL3DModel(const GL_NETLizard_3D_Model *model, GL_NETLizard_Debug_Render_Mesh_f func);
void NETLizard_DebugRenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count, GL_NETLizard_Debug_Render_Mesh_f func);

GLvoid NETLizard_DebugRenderGL3DMeshVertexNormal(const GL_NETLizard_3D_Mesh *m);
GLvoid NETLizard_DebugRenderGL3DMeshBound(const GL_NETLizard_3D_Mesh *m);


void NETLizard_DebugRenderGL3DMapModelBound(const GL_NETLizard_3D_Model *model);

#ifdef __cplusplus
}
#endif

#endif
