#ifndef _KARIN_NETLIZARD_H
#define _KARIN_NETLIZARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define ARR_MIN_LEN(x)

#define NL_TEXTURE_V2_MAGIC_HEADER "&&&"
#define NL_TEXTURE_V3_MAGIC_HEADER "BIP"
#define NL_ENCODE_DECODE_FACTOR 100

#define NL_TRUE 1
#define NL_FALSE 0

typedef int8_t NLbyte;
typedef int8_t NLchar;
typedef uint8_t NLuchar;
typedef int16_t NLshort;
typedef uint16_t NLushort;
typedef int32_t NLint;
typedef uint32_t NLuint;
typedef int64_t NLlong;
typedef uint64_t NLulong;
typedef float NLfloat;
typedef double NLdouble;
typedef uint8_t NLboolean;
typedef uint32_t NLsizei;
typedef NLsizei NLsize;

#define UNUSED(ptr) free(ptr); ptr = NULL;
#define UNPARSED 0

//typedef char byte; // unsigned

enum game_name_e
{
	NL_RACING_EVOLUTION_3D = 0,
	NL_CONTR_TERRORISM_3D,
	NL_ARMY_RANGER_3D,
	NL_CONTR_TERRORISM_3D_EPISODE_2,
	NL_SHADOW_OF_EGYPT_3D,
	NL_CLONE_3D,
    NL_CONTR_TERRORISM_3D_EPISODE_3,
    NL_TOTAL_GAME,
	NL_2D_GAME,
    NL_OLD_GAME
};

typedef enum game_name_e game_name;

// texture
typedef enum NETLizard_Texture_format_e
{
    NL_RGB = 0,
    NL_RGBA
} NETLizard_Texture_format;

typedef struct NETLizard_Texture_s
{
    NETLizard_Texture_format format;
    NLint width;
    NLint height;
    NLint depth;
    struct {
        NLint *data;
        NLsizei length;
    } color_map;
    struct {
        NLuchar *data;
        NLsizei length;
    } color_index;
} NETLizard_Texture;

// model
// 变量命名规则
/*
 * array 数组
 * int int型
 * short short型
 * byte byte型
 *
 * scene 场景
 * item 物品
 * role 角色动画
 *
 * vertex 顶点坐标
 * primitive 图元（顶点索引，纹理坐标索引，纹理索引，法线）
 * plane 平面（法线索引，顶点索引）
 * aabb AABB盒子（最大， 最小）
 * begin 场景中物品起始索引
 * end 场景中物品最后索引
 * translation 位置移动
 * rotation 角度旋转
 * obj 物品索引
 */

typedef enum _NETLizard_3D_Item_Type
{
    Item_Other_Type = 0,
    Item_Tiny_Type = 1,
    Item_2D_Type = 1 << 1,
    Item_FanV_Type = 1 << 2,
    Item_FanH_Type = 1 << 3,
    Item_Role_Type = 1 << 4,
    Item_Box_Type = 1 << 5,
    Item_DoorV_Type = 1 << 6,
    Item_DoorH_Type = 1 << 7,
    Item_Elevator_Type = 1 << 8,
    Item_Ladder_Type = 1 << 9,
    Item_Weapon_Type = 1 << 10,
    Item_Skyline_Type = 1 << 11,
    Item_Portal_Type = 1 << 12,
    Item_Switcher_Type = 1 << 13
} NETLizard_3D_Item_Type;

typedef enum _NETLizard_3D_Animation_Type
{
    Animation_Idle_Type = 0,
    Animation_Walk_Type = 1,
    Animation_Run_Type = 2,
    Animation_Fighting1_Type = 3,
    Animation_Fighting2_Type = 4,
    Animation_Attack1_Type = 5,
    Animation_Attack2_Type = 6,
    Animation_Dead1_Type = 7,
    Animation_Dead2_Type = 8,
    Animation_Total_Type = 9,
    Animation_Unknow_Type
} NETLizard_3D_Animation_Type;

typedef enum _NETLizard_Texture_Type
{
    Texture_NormalPNG_Type = 0,
    Texture_EncodePNG_Type,
    Texture_3DEngineV2_Type,
    Texture_3DEngineV3_Type,
    Texture_3DEngineV3Compress_Type,
    Texture_Unknow_Type
} NETLizard_Texture_Type;

typedef enum _NETLizard_3D_Model_Type
{
    CT_3D_Map = 0,
    CT_3D_Item,
    CT_3D_Ep2_Map,
    CT_3D_Ep2_Item,
    Army_Ranger_3D_Map,
    Army_Ranger_3D_Item,
    Egypt_3D_Map,
    Egypt_3D_Item,
    Egypt_3D_Role,
    Clone_3D_Map,
    Clone_3D_Item,
    Clone_3D_Role,
    CT_3D_Ep3_Map,
    CT_3D_Ep3_Item
} NETLizard_3D_Model_Type;

typedef struct _NETLizard_BSP_Tree_Node
{
    NLint direction;
    NLint plane[4][3];
    NLint prev_scene;
    NLint next_scene;
    NLint normal[3];
} NETLizard_BSP_Tree_Node;

typedef struct _NETLizard_3D_Plane
{
    NLint position[3];
    NLint normal[3];
} NETLizard_3D_Plane;

typedef struct _NETLizard_3D_Primitive
{
    NLuint index[3];
    NLuint texcoord[6];
    NLint tex_index;
    NETLizard_3D_Plane plane;
} NETLizard_3D_Primitive;

typedef struct _NETLizard_3D_Mesh
{
    NLint ortho[6];
    struct {
        NLint *data;
        NLuint length;
    } vertex;
    struct {
        NETLizard_3D_Plane *data;
        NLuint length;
    } plane;
    struct {
        NETLizard_3D_Primitive *data;
        NLuint length;
    } primitive;
    NLuint item_index_range[2];
    struct {
        NETLizard_BSP_Tree_Node *data;
        NLuint length;
    } bsp;
} NETLizard_3D_Mesh;

typedef struct _NETLizard_3D_Item_Mesh
{
    NLint pos[3];
    NLint angle[2];
    NLuint obj_index;
    NETLizard_3D_Mesh item_mesh;
} NETLizard_3D_Item_Mesh;

typedef struct _NETLizard_3D_Model
{
    NETLizard_3D_Model_Type game;
    NLint start_pos[3];
    NLint start_angle[2];
    struct {
        NETLizard_3D_Mesh *data;
        NLuint length;
    } data;
    struct {
        NETLizard_3D_Item_Mesh *data;
        NLuint length;
    } item_data;
    NLboolean has_sky;
    struct {
        NETLizard_BSP_Tree_Node *data;
        NLuint length;
    } bsp_data;
} NETLizard_3D_Model;

typedef struct _NETLizard_Spirit
{
    NLshort index;
    NLshort x;
    NLshort y;
    NLshort width;
    NLshort height;
    NLshort private_4;
    NLshort private_5;
} NETLizard_Spirit;

typedef struct _NETLizard_Font_Char_Raster
{
    NLshort x;
    NLshort y;
    NLshort width;
    NLshort height;
    NLshort x_strip;
    NLshort y_strip;
} NETLizard_Font_Char_Raster;

typedef struct _NETLizard_Font
{
  struct {
      NETLizard_Font_Char_Raster *data;
      NLsizei length;
  } char_raster_map;
  struct {
      NLuchar *data;
      NLsizei length;
  } offset;
  NLsizei char_count;

  NLuchar private_e; // byte
  NLuchar private_l; // byte
  NLuchar private_m; // byte

  char fnt_file[8];
} NETLizard_Font;

//extern int nl_debug_mode;

extern const int Game_Count;
extern const char *Game_Names[];
extern const int Game_Level[];

extern const char *CT3D_Level[];
extern const char *Specnaz3D_Level[];
extern const char *CT3DEp2_Level[];
extern const char *Egypt3D_Level[];
extern const char *Clone3D_Level[];
extern const char *RE3D_Level[];
extern const char *CT3DEp3_Level[];
extern const char **Game_Level_Name[];

extern char * game_resource_path[NL_TOTAL_GAME];

// util
// png
NLboolean nlIsPNGFile(const char *name);
NLboolean nlIsPNG(const char *data);
NLboolean nlIsNLPNGFile(const char *name);
NLboolean nlIsNLPNG(const char *data);
char * nlEncodeDecodeData(const char *arr, char *data, NLsizei length);
char * nlEncodeDecodeThisData(char *arr, NLsizei length);
// texture
NLboolean nlIsNL3DV2TextureFile(const char *name);
NLboolean nlIsNL3DV2Texture(const char *data);
NLboolean nlIsNL3DV3TextureFile(const char *name);
NLboolean nlIsNL3DV3Texture(const char *data);
// string
char * nlParseStringi(const NLint *arr, size_t length);
char * nlParseStringCi(const NLint *arr, size_t length);
char * nlParseString(const char *arr, size_t length);
char * nlParseStringC(const char *arr, size_t length);
//array * nlEncodeStringC(const char *str);
// text
char * nlHandleText_File2Memory(const char *name, NLint *len);
char * nlHandleText_Memory2Memory(const char *data, NLint len, NLint *rlen);
NLboolean nlHandleText_File2File(const char *from, const char *to);
NLboolean nlHandleText_Memory2File(const char *data, NLint len, const char *to);
// media
char * nlGetLevelMusicFileName(game_name game, int level, char *file ARR_MIN_LEN(8));
char * nlGetMenuMusicFileName(game_name game, char *file ARR_MIN_LEN(8));
// spirit
int nlReadSpiritMap(const char *file, NETLizard_Spirit **ret);
// font
NLboolean nlReadNETLizardFont(const char *map_file, NETLizard_Font *fnt);
void delete_NETLizard_Font(NETLizard_Font *fnt);

// 3D
void delete_NETLizard_3D_Mesh(NETLizard_3D_Mesh *mesh);
void delete_NETLizard_3D_Item_Mesh(NETLizard_3D_Item_Mesh *mesh);
void delete_NETLizard_3D_Model(NETLizard_3D_Model *model);
// util
const char * nlGetAnimationName(NETLizard_3D_Animation_Type anim);
void nlSetResourcePath(game_name name, const char *path);
int nlCheckLevelIsAvailable(game_name game, int level);
int nlGetItemType(game_name game, int index);
int nlGetAnimationBeginAndEnd(game_name game, int index, int animation[]);
// Racing Evolution 3D
// Contr Terrisiem 3D
NLboolean nlReadCT3DModelFile(const char* name, int level, NETLizard_3D_Model *model);
NLboolean nlReadCT3DItemModelFile(const char* name, int index, NETLizard_3D_Model *model);
// Army Ranger 3D: Operation Artcle
// Contr Terrisiem 3D: Episode-2
// 3D Shadows of Egypt
// Clone 3D
// Contr Terrisiem 3D: Episode-3

// texture
NETLizard_Texture_Type nlCheckPNGType(const char *data);
NETLizard_Texture_Type nlCheckPNGFileType(const char *file);
void delete_NETLizard_Texture(NETLizard_Texture *tex);
// png
char * nlHandlePNG_File2Memory(const char *name, NLint *len);
char * nlHandlePNG_Memory2Memory(const char *data, NLint len);
NLboolean nlHandlePNG_File2File(const char *from, const char *to);
NLboolean nlHandlePNG_Memory2File(const char *data, NLint len, const char *to);
// texture v2
NLboolean nlReadTextureV2_File(const char *name, NETLizard_Texture *tex);
NLboolean nlReadTextureV2_Memory(const char *data, NLsizei length, NETLizard_Texture *tex);
NLboolean nlSaveImage_V2File(const char *from, const char *to, int img_type);
NLboolean nlSaveImage_V2Memory(const NETLizard_Texture *tex, const char *to, int img_type);
NLboolean nlSaveTextureV2_Memory(const NLuchar *data,  int width, int height, NETLizard_Texture_format format, const char *to);
NLboolean nlSaveTextureV2_File(const char *from, const char *to);
// texture v3
NLboolean nlReadTextureV3_File(const char *name, int i1, NETLizard_Texture *tex);
NLboolean nlReadTextureV3_Memory(const char *data, NLsizei length, int i1, NETLizard_Texture *tex);
NLboolean nlSaveImage_V3File(const char *from, int i, const char *to, int img_type);
NLboolean nlSaveImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type);
NLboolean nlSaveTextureV3_Memory(const NLuchar *data, int _i, int width, int height, NETLizard_Texture_format format, const char *to);
NLboolean nlReadCompressTextureV3_Memory(const char *data, NLsizei length, NETLizard_Texture *tex);
NLboolean nlReadCompressTextureV3_File(const char *name, NETLizard_Texture *tex);
NLboolean nlSaveCompressImage_V3File(const char *from, const char *to, int img_type);
NLboolean nlSaveCompressImage_V3Memory(const NETLizard_Texture *tex, const char *to, int img_type);
// OpenGL
NLuchar * nlMakeOpenGLTextureDataRGBACompress(const NETLizard_Texture *tex, NLsizei *rlen);
NLuchar * nlMakeOpenGLTextureDataRGB(const NETLizard_Texture *tex, NLsizei *rlen);
NLuchar * nlMakeOpenGLTextureDataRGBA(const NETLizard_Texture *tex, NLsizei *rlen);

#ifdef __cplusplus
}
#endif

#endif
