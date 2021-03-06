#ifndef _KARIN_NL_GL_H
#define _KARIN_NL_GL_H

#include "netlizard.h"
#include "gl_texture.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _texture_s;
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
    GLushort *index;
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
//	GLushort *index;
//    GLuint index_count;
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
    GLuint plane_type; // 0 no plane, 1 has plane, 2 from vertex, 3 from bound
    struct {
        GLfloat min[3];
        GLfloat max[3];
    } box;
	GLuint item_index_range[2];
	GLint *bsp;
	GLuint bsp_count;

    GLfloat position[3];
    GLfloat rotation[3];
    GLint item_type;
    GLint obj_index;
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
    struct _texture_s **texes;
	GLuint tex_count;
    struct _texture_s *bg_tex;
	GLfloat start_position[3];
	GLfloat start_rotation[2];
	GL_NETLizard_BSP_Tree_Node *bsp_data;
	GLuint bsp_count;
} GL_NETLizard_3D_Model;

typedef struct _GL_NETLizard_3D_Item_Model
{
	GL_NETLizard_3D_Mesh item_mesh;
    struct _texture_s tex;
} GL_NETLizard_3D_Item_Model;

void NETLizard_RenderGL3DModel(const GL_NETLizard_3D_Model *model);
void NETLizard_RenderGL3DItemModel(const GL_NETLizard_3D_Item_Model *m);
void NETLizard_RenderGL3DMapModel(const GL_NETLizard_3D_Model *model, GLint *scenes, GLuint count);
void NETLizard_RenderGL3DModelFrameAnimation(const GL_NETLizard_3D_Model *m, const NETLizard_3D_Frame_Animation *config, GLuint anim, GLuint frame);
void NETLizard_RenderGL3DMapModelScene(const GL_NETLizard_3D_Model *model, GLint *scene, GLuint count);
void NETLizard_RenderGL3DMapModelItem(const GL_NETLizard_3D_Model *model, GLint *items, GLuint count);

void delete_GL_NETLizard_3D_Material(GL_NETLizard_3D_Material *mat);
void delete_GL_NETLizard_3D_Mesh(GL_NETLizard_3D_Mesh *mesh);
void delete_GL_NETLizard_3D_Model(GL_NETLizard_3D_Model *model);

GLboolean NETLizard_MakeGL3DModel(const NETLizard_3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel);
GLboolean NETLizard_MakeGLRE3DModel(const NETLizard_RE3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel);
GL_NETLizard_3D_Model * NETLizard_MakeGL3DItemModel(const NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DMapModelFile(const char *name, int i, const char *resouce_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DItemModelFile(const char *name, int i, const char *resouce_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DEp2MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DEp2ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLSpecnaz3DMapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLSpecnaz3DItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLEgypt3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLEgypt3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLEgypt3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLCT3DEp3MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLCT3DEp3ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLRE3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLRE3DCarModelFile(const char *car_file, const char *tex_file, const char *resource_path, GL_NETLizard_3D_Model *model);

GLboolean NETLizard_ReadGLClone3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLClone3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);
GLboolean NETLizard_ReadGLClone3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model);

void NETLizard_RenderGL3DMesh(const GL_NETLizard_3D_Mesh *m, struct _texture_s **const texes);
void NETLizard_RenderGL3DItemMesh(const GL_NETLizard_3D_Mesh *m, const struct _texture_s *tex);

typedef struct _GL_NETLizard_Font_Char
{
    GL_NETLizard_3D_Vertex_Data vertex_data;
    GL_NETLizard_3D_Material index_data;
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

    struct _texture_s tex;

    GLfloat width;
    GLfloat height;
} GL_NETLizard_Font;

GLboolean NETLizard_ReadFont(GL_NETLizard_Font * fnt, const char *map_file, const char *tex_file);
void delete_GL_NETLizard_Font(GL_NETLizard_Font *fnt);
void NETLizard_RenderFontChar(const GL_NETLizard_Font *fnt, GLuint i);
GLint NETLizard_GetFontIndex(const GL_NETLizard_Font *fnt, int ch);

void NETLizard_FontRenderString(const GL_NETLizard_Font *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char *str);
void NETLizard_FontRenderDigit(const GL_NETLizard_Font *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint num);
void NETLizard_FontRenderChar(const GL_NETLizard_Font *f, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a, char ch);

GLfloat NETLizard_FontGetStringWidth(const GL_NETLizard_Font *f, const char *str);
GLint NETLizard_FontGetCharCountOfWidth(const GL_NETLizard_Font *f, GLfloat width, const char *str);

int NETLizard_FontGetDigitCenterPosition(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, int num, GLfloat *rx, GLfloat *ry);
int NETLizard_FontGetStringCenterPosition(const GL_NETLizard_Font *fnt, GLfloat x, GLfloat y, GLfloat w, GLfloat h, const char *str, GLfloat *rx, GLfloat *ry);

typedef struct _GL_NETLizard_Sprite_Cell
{
    GL_NETLizard_3D_Vertex_Data vertex_data;
    GL_NETLizard_3D_Material index_data;
	GLfloat width;
	GLfloat height;
	GLfloat private_4;
	GLfloat private_5;
} GL_NETLizard_Sprite_Cell;

typedef struct _GL_NETLizard_Sprite
{
    GL_NETLizard_Sprite_Cell *sprites;
    GLuint sprite_count;
    struct _texture_s tex;
} GL_NETLizard_Sprite;

GLboolean NETLizard_ReadSpirit(GL_NETLizard_Sprite *sprite, const char *map_file, const char *tex_file);
void delete_GL_NETLizard_Sprite(GL_NETLizard_Sprite *s);
void NETLizard_RenderSpirit(const GL_NETLizard_Sprite *s, GLuint i);

void NETLizard_MoveItemModel(GL_NETLizard_3D_Item_Mesh *dst, GL_NETLizard_3D_Item_Mesh *src);

#ifdef __cplusplus
}
#endif

#endif
