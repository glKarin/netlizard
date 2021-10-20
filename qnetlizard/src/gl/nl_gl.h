#ifndef _KARIN_NL_GL_H
#define _KARIN_NL_GL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "netlizard.h"

#include "gl_texture.h"

typedef struct _GL_NETLizard_3D_Plane
{
	GLfloat position[3];
	GLfloat normal[3];
} GL_NETLizard_3D_Plane;

typedef struct _GL_NETLizard_BSP_Tree_Node
{
	GLuint direction;
	GLfloat plane[4][3];
	GLint prev_scene;
	GLint next_scene;
	GLfloat normal[3];
} GL_NETLizard_BSP_Tree_Node;

typedef struct _GL_NETLizard_3D_Material
{
	GLint tex_index;
	GLuint index_start;
	GLuint index_count;
    GLenum mode;
} GL_NETLizard_3D_Material;

typedef struct _GL_NETLizard_3D_Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texcoord[2];
} GL_NETLizard_3D_Vertex;

typedef struct _GL_NETLizard_3D_Vertex_Data
{
    GL_NETLizard_3D_Vertex *vertex;
    GLuint vertex_count;
	GLushort *index;
    GLuint index_count;
} GL_NETLizard_3D_Vertex_Data;

//typedef struct _GL_NETLizard_3D_Vertex_Array
//{
//	GLuint gl;
//    GL_NETLizard_3D_Vertex_Data vertex_data;
//} GL_NETLizard_3D_Vertex_Array;

typedef struct _GL_NETLizard_3D_Mesh
{
    GL_NETLizard_3D_Vertex_Data vertex_data;
	GL_NETLizard_3D_Material *materials;
    GLuint count;
    GLint *tex_index;
	GL_NETLizard_3D_Plane *plane;
    GLuint plane_count;
    struct {
        GLfloat min[3];
        GLfloat max[3];
    } box;
	GLuint item_index_range[2];
	GLint *bsp;
	GLuint bsp_count;

    GLfloat position[3];
    GLfloat rotation[2];
    GLint item_type;
} GL_NETLizard_3D_Mesh;
typedef GL_NETLizard_3D_Mesh GL_NETLizard_3D_Item_Mesh;

//typedef struct _GL_NETLizard_3D_Item_Mesh
//{
//	GL_NETLizard_3D_Mesh item_mesh;
//	GLfloat pos[3];
//	GLfloat angle[2];
//	GLint item_type;
//} GL_NETLizard_3D_Item_Mesh;

typedef struct _GL_NETLizard_3D_Model
{
    NETLizard_Game game;
	GL_NETLizard_3D_Mesh *meshes;
	GLuint count;
    GL_NETLizard_3D_Item_Mesh *item_meshes;
	GLuint item_count;
    texture_s **texes;
	GLuint tex_count;
    texture_s *bg_tex;
	GLfloat start_pos[3];
	GLfloat start_angle[2];
	GL_NETLizard_BSP_Tree_Node *bsp_data;
	GLuint bsp_count;
} GL_NETLizard_3D_Model;

typedef struct _GL_NETLizard_3D_Item_Model
{
	GL_NETLizard_3D_Mesh item_mesh;
    texture_s tex;
} GL_NETLizard_3D_Item_Model;

GLvoid NETLizard_RenderGL3DModel(const GL_NETLizard_3D_Model *model);
GLvoid NETLizard_RenderGL3DItemModel(const GL_NETLizard_3D_Item_Model *m);
GLvoid NETLizard_RenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scenes, GLuint count);
GLvoid NETLizard_RenderGL3DModelFrameAnimation(const GL_NETLizard_3D_Model *m, const NETLizard_3D_Frame_Animation *config, GLuint anim, GLuint frame);

GLvoid delete_GL_NETLizard_3D_Mesh(GL_NETLizard_3D_Mesh *mesh);
GLvoid delete_GL_NETLizard_3D_Model(GL_NETLizard_3D_Model *model);

GLboolean NETLizard_MakeGL3DModel(const NETLizard_3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel);
GLboolean NETLizard_MakeGLRE3DModel(const NETLizard_RE3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel);
GL_NETLizard_3D_Model * NETLizard_MakeGL3DItemModel(const NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DMapModelFile(const char *name, int i, const char *resouce_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DItemModelFile(const char *name, int i, const char *resouce_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DEp2MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DEp2ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLSpecnaz3DMapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLSpecnaz3DItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLEgypt3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLEgypt3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLEgypt3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DEp3MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DEp3ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLRE3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLRE3DCarModelFile(const char *car_file, const char *tex_file, const char *resource_path, GL_NETLizard_3D_Model *model);

#if 0
GL_NETLizard_3D_Model * NETLizard_ReadGLClone3DMapModelFile(const char *name);
GL_NETLizard_3D_Model * NETLizard_ReadGLClone3DItemModelFile(const char *name, int index);
GL_NETLizard_3D_Animation_Model * NETLizard_ReadGLClone3DRoleModelFile(const char *name, int index);
#endif

GLvoid NETLizard_RenderGL3DMesh(const GL_NETLizard_3D_Mesh *m, texture_s **const texes);
GLvoid NETLizard_RenderGL3DItemMesh(const GL_NETLizard_3D_Mesh *m, const texture_s *tex);

typedef struct _GL_NETLizard_Font_Char
{
    GL_NETLizard_3D_Vertex_Data vertex_data;
	GLfloat width;
	GLfloat height;
	GLfloat x_stride;
	GLfloat y_stride;
} GL_NETLizard_Font_Char;

typedef struct _GL_NETLizard_Font
{
	GL_NETLizard_Font_Char *char_map;
	GLuint char_map_count;
	GLbyte *offset;
	GLuint offset_count;
	GLuint char_count;

	GLboolean private_e;
    GLboolean private_l;
    GLboolean private_m;

    texture_s tex;
} GL_NETLizard_Font;

GLboolean NETLizard_ReadFont(GL_NETLizard_Font * fnt, const char *map_file, const char *tex_file);
GLvoid delete_GL_NETLizard_Font(GL_NETLizard_Font *fnt);
GLvoid NETLizard_RenderFontChar(const GL_NETLizard_Font *fnt, GLuint i);
GLint NETLizard_GetFontIndex(const GL_NETLizard_Font *fnt, int ch);

#if 0
typedef struct _GL_NETLizard_Spirit_Cell
{
	GLuint buffers[total_buffer_type];
	GLfloat width;
	GLfloat height;
	GLfloat private_4;
	GLfloat private_5;
} GL_NETLizard_Spirit_Cell;

typedef struct _GL_NETLizard_Spirit
{
	texture *tex;
	GL_NETLizard_Spirit_Cell *spirits;
	GLuint spirit_count;
} GL_NETLizard_Spirit;

GL_NETLizard_Spirit * NETLizard_ReadSpirit(const char *map_file, const char *tex_file);
GLvoid delete_GL_NETLizard_Spirit(GL_NETLizard_Spirit *s);
GLvoid NETLizard_RenderSpirit(const GL_NETLizard_Spirit *s, GLuint i);

GLvoid NETLizard_MoveItemModel(GL_NETLizard_3D_Item_Mesh *dst, GL_NETLizard_3D_Item_Mesh *src);

#endif

#ifdef __cplusplus
}
#endif

#endif
