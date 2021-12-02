#ifndef _KARIN_NLGLDEBUG_H
#define _KARIN_NLGLDEBUG_H

#include "nl_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NETLIZARD_DEBUG_TYPE_ITEM 1
#define NETLIZARD_DEBUG_TYPE_SCENE (1 << 1)
#define NETLIZARD_DEBUG_TYPE_ALL 0xFF

#define NETLIZARD_DEBUG_RENDER_NONE 0
#define NETLIZARD_DEBUG_RENDER_MAP_BOUND 1
#define NETLIZARD_DEBUG_RENDER_ITEM_VERTEX_NORMAL (1 << 1)
#define NETLIZARD_DEBUG_RENDER_SCENE_VERTEX_NORMAL (1 << 2)
#define NETLIZARD_DEBUG_RENDER_ITEM_BOUND (1 << 3)
#define NETLIZARD_DEBUG_RENDER_SCENE_BOUND (1 << 4)
#define NETLIZARD_DEBUG_RENDER_ITEM_PLANE (1 << 5)
#define NETLIZARD_DEBUG_RENDER_SCENE_PLANE (1 << 6)
#define NETLIZARD_DEBUG_RENDER_MAP_BSP (1 << 7)
#define NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_SCENE_PLANE (1 << 8)
#define NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_ITEM (1 << 9)

#define NETLizard_DebugRenderGL3DModel_VertexNormal(m, t) NETLizard_DebugRenderGL3DModel(m, t, NETLizard_DebugRenderGL3DMeshVertexNormal)
#define NETLizard_DebugRenderGL3DMapModelScene_VertexNormal(m, s, c, t) NETLizard_DebugRenderGL3DMapModelScene(m, s, c, t, NETLizard_DebugRenderGL3DMeshVertexNormal)

#define NETLizard_DebugRenderGL3DModel_Bound(m, t) NETLizard_DebugRenderGL3DModel(m, t, NETLizard_DebugRenderGL3DMeshBound)
#define NETLizard_DebugRenderGL3DMapModelScene_Bound(m, s, c, t) NETLizard_DebugRenderGL3DMapModelScene(m, s, c, t, NETLizard_DebugRenderGL3DMeshBound)

#define NETLizard_DebugRenderGL3DModel_Plane(m, t) NETLizard_DebugRenderGL3DModel(m, t, NETLizard_DebugRenderGL3DMeshPlane)
#define NETLizard_DebugRenderGL3DMapModelScene_Plane(m, s, c, t) NETLizard_DebugRenderGL3DMapModelScene(m, s, c, t, NETLizard_DebugRenderGL3DMeshPlane)

typedef void (*GL_NETLizard_Debug_Render_Mesh_f)(const GL_NETLizard_3D_Mesh *mesh);

void NETLizard_DebugRenderGL3DModel(const GL_NETLizard_3D_Model *model, GLuint type, GL_NETLizard_Debug_Render_Mesh_f func);
void NETLizard_DebugRenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count, GLuint type,  GL_NETLizard_Debug_Render_Mesh_f func);

GLvoid NETLizard_DebugRenderGL3DMeshVertexNormal(const GL_NETLizard_3D_Mesh *m);
GLvoid NETLizard_DebugRenderGL3DMeshBound(const GL_NETLizard_3D_Mesh *m);
GLvoid NETLizard_DebugRenderGL3DMeshPlane(const GL_NETLizard_3D_Mesh *m);

void NETLizard_DebugRenderGL3DMapModelBound(const GL_NETLizard_3D_Model *model);
void NETLizard_DebugRenderGL3DMapModelBSP(const GL_NETLizard_3D_Model *model);

#ifdef __cplusplus
}
#endif

#endif
