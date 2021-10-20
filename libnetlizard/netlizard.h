#ifndef _KARIN_NETLIZARD_H
#define _KARIN_NETLIZARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define ARR_MIN_LEN(x)

/* NETLizard 3D game v2 texture header. 3D CT - 3D CT2 */
#define NL_TEXTURE_V2_HEADER_MAGIC "&&&"
/* NETLizard 3D game v3 texture header. 3D Egypt - 3D Clone */
#define NL_TEXTURE_V3_HEADER_MAGIC "BIP"
/* NETLizard Racing evolution 3D mesh header.*/
#define NL_RE3D_MESH_HEADER_MAGIC "NL_MESH"
/* NETLizard game png encode/decode factor. exam: a_byte = a_byte ^ 100 */
#define NL_ENCODE_DECODE_FACTOR 100

#define NL_TRUE 1
#define NL_FALSE 0

/* Basic type define */
typedef void NLvoid;
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
typedef void * NLdata;

//typedef char byte; // unsigned

/* NETLizard game define */
enum NETLizard_Game_e
{
	NL_RACING_EVOLUTION_3D = 0,
	NL_CONTR_TERRORISM_3D,
	NL_ARMY_RANGER_3D,
	NL_CONTR_TERRORISM_3D_EPISODE_2,
	NL_SHADOW_OF_EGYPT_3D,
	NL_CLONE_3D,
    NL_CONTR_TERRORISM_3D_EPISODE_3,
    NL_TOTAL_GAME, // Total 3D game
    NL_2D_GAME, // In 2D game and old 3D game, `png` file need decode.
    NL_OLD_GAME // In old game, `png` file not need decode.
};
typedef enum NETLizard_Game_e NETLizard_Game;

/* Texture format */
typedef enum NETLizard_Texture_format_e
{
    NL_RGB = 0,
    NL_RGBA
} NETLizard_Texture_format;

/* NETLizard image/texture type */
typedef enum NETLizard_Texture_Type_e
{
    NL_TEXTURE_NORMAL_PNG = 0, // only a png file
    NL_TEXTURE_ENCODE_PNG, // png file that need decode
    NL_TEXTURE_3D_ENGINE_V2, // 3D texture v2: 3D CT, 3D CT2, 3D Spacnaz
    NL_TEXTURE_3D_ENGINE_V3, // 3D texture v3: 3D Egypt, 3D Clone
    NL_TEXTURE_3D_ENGINE_V3_COMPRESS, // 3D texture v3 compressed: first-person weapon in 3D Egypt, 3D Clone
    NL_TEXTURE_UNKNOWN
} NETLizard_Texture_Type;

/* Texture structure */
typedef struct NETLizard_Texture_s
{
    NETLizard_Texture_Type type; // type
    NETLizard_Texture_format format; // format
    NLint width; // width
    NLint height; // height
    NLint depth; // depth
    struct {
        NLint *data;
        NLsizei count;
    } color_map; // color table
    struct {
        NLuchar *data;
        NLsizei count;
    } color_index; // color index table, every element is a pixel and a index of `color table`
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

/* NETLizard 3D game item category */
typedef enum NETLizard_3D_Item_Type_e
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

/* NETLizard 3D game character animation category */
typedef enum NETLizard_3D_Animation_Type_e
{
    NL_FRAME_ANIMATION_IDLE = 0,
    NL_FRAME_ANIMATION_WALK = 1,
    NL_FRAME_ANIMATION_RUN = 2,
    NL_FRAME_ANIMATION_FIGHTING_1 = 3,
    NL_FRAME_ANIMATION_FIGHTING_2 = 4,
    NL_FRAME_ANIMATION_ATTACK_1 = 5,
    NL_FRAME_ANIMATION_ATTACK_2 = 6,
    NL_FRAME_ANIMATION_DEAD_1 = 7,
    NL_FRAME_ANIMATION_DEAD_2 = 8,
    NL_FRAME_ANIMATION_TOTAL = 9,
    NL_FRAME_ANIMATION_UNKNOWN = -1
} NETLizard_3D_Animation_Type;

/* NETLizard 3D model type */
typedef enum NETLizard_3D_Model_Type_e
{
    NL_CT_3D_MAP_MODEL = 0,
    NL_CT_3D_ITEM_MODEL,
    NL_CT_3D_EP2_MAP_MODEL,
    NL_CT_3D_EP2_ITEM_MODEL,
    NL_ARMY_RANGER_3D_MAP_MODEL,
    NL_ARMY_RANGER_3D_ITEM_MODEL,
    NL_EGYPT_3D_MAP_MODEL,
    NL_EGYPT_3D_ITEM_MODEL,
    NL_EGYPT_3D_ROLE_MODEL,
    NL_CLONE_3D_MAP_MODEL,
    NL_CLONE_3D_ITEM_MODEL,
    NL_CLONE_3D_ROLE_MODEL,
    NL_CT_3D_EP3_MAP_MODEL,
    NL_CT_3D_EP3_ITEM_MODEL,
    NL_RE_3D_MAP_MODEL,
    NL_RE_3D_CAR_MODEL
} NETLizard_3D_Model_Type;

/* NETLizard 3D map BSP??? node */
typedef struct NETLizard_BSP_Tree_Node_s
{
    NLint direction;
    NLint plane[4][3];
    NLint prev_scene;
    NLint next_scene;
    NLint normal[3];
} NETLizard_BSP_Tree_Node;

/* NETLizard 3D map plane */
typedef struct NETLizard_3D_Plane_s
{
    NLint position[3];
    NLint normal[3];
} NETLizard_3D_Plane;

/* NETLizard 3D model primitive, triangle */
typedef struct NETLizard_3D_Primitive_s
{
    NLuint index[3]; // index of `vertex table`
    NLuint texcoord[6];
    NLint tex_index; // texture index
    NETLizard_3D_Plane plane;
} NETLizard_3D_Primitive;

/* NETLizard 3D model mesh */
typedef struct NETLizard_3D_Mesh_s
{
    struct {
        NLint min[3];
        NLint max[3];
    } box; // AABB box with min-max
    struct {
        NLint *data;
        NLuint count;
    } vertex; // vertex table
    struct {
        NETLizard_3D_Plane *data;
        NLuint count;
    } plane; // planes
    struct {
        NETLizard_3D_Primitive *data;
        NLuint count;
    } primitive; // primitive
    NLuint item_index_range[2]; // If model is 3D map, it means items index's start and end
    struct {
        NETLizard_BSP_Tree_Node *data;
        NLuint count;
    } bsp; // BSP???
} NETLizard_3D_Mesh;

/* NETLizard 3D item model mesh */
typedef struct NETLizard_3D_Item_Mesh_s
{
    NLint position[3];
    NLint rotation[2];
    NLuint obj_index; // item's index
    NETLizard_3D_Mesh item_mesh;
} NETLizard_3D_Item_Mesh;

/* NETLizard 3D model mesh */
typedef struct NETLizard_3D_Model_s
{
    NETLizard_Game game;
    NETLizard_3D_Model_Type type; // 3D model type
    NLint start_pos[3]; // player start position
    NLint start_angle[2]; // player start direction
    struct {
        NETLizard_3D_Mesh *data;
        NLuint count;
    } data; // If type is 3D map, it means map's mesh; if type is 3D item model, it is null
    struct {
        NETLizard_3D_Item_Mesh *data;
        NLuint count;
    } item_data; // if type is 3D item, length is 1
    NLboolean has_sky; // If type is 3D map, it means map sky texture
    struct {
        NETLizard_BSP_Tree_Node *data;
        NLuint count;
    } bsp_data; // If type is 3D map, it means BSP??? data
} NETLizard_3D_Model;

/* NETLizard Racing Evolution 3D model mesh */
typedef struct NETLizard_RE3D_Mesh_s
{
    NLuint vertex_count;
    struct {
        NLfloat *data;
        NLuint count;
    } vertex; // vertex
    struct {
        NLfloat *data;
        NLuint count;
    } texcoord; // texcoord
    /* triangle strip */
    struct {
        NLuint *data;
        NLuint count;
    } index; // index
    struct {
        NLuint *data;
        NLuint count;
    } primitive; // triangle strip's count
    NLfloat translation[3]; // position
    NLint tex_index;
} NETLizard_RE3D_Mesh;

/* NETLizard Racing Evolution 3D model */
typedef struct NETLizard_RE3D_Model_s
{
    NETLizard_Game game;
    NETLizard_3D_Model_Type type;
    struct {
        NETLizard_RE3D_Mesh *data;
        NLuint count;
    } meshes;
    struct {
        char **data; // file name string
        NLuint count;
    } texes;
} NETLizard_RE3D_Model;

/* NETLizard 3D model frame animation */
typedef struct _NETLizard_3D_Frame_Animation
{
    NETLizard_3D_Animation_Type type;
    NLint begin_frame;
    NLint end_frame;
    NLuint count;
} NETLizard_3D_Frame_Animation;

/* NETLizard 3D spirit */
typedef struct NETLizard_Spirit_s
{
    NLshort index;
    NLshort x;
    NLshort y;
    NLshort width;
    NLshort height;
    NLshort private_4;
    NLshort private_5;
} NETLizard_Spirit;

/* NETLizard 3D font char */
typedef struct NETLizard_Font_Char_Raster_s
{
    NLshort x;
    NLshort y;
    NLshort width;
    NLshort height;
    NLshort x_stride;
    NLshort y_stride;
} NETLizard_Font_Char_Raster;

/* NETLizard 3D font */
typedef struct NETLizard_Font_s
{
  struct {
      NETLizard_Font_Char_Raster *data;
      NLsizei count;
  } char_raster_map; // char table
  struct {
      NLchar *data;
      NLsizei count;
  } offset;
  NLsizei char_count; // char count

  NLuchar private_e; // byte unknow
  NLuchar private_l; // byte unknow
  NLuchar private_m; // byte unknow

  char fnt_file[8]; // file name, aways 'fnt.png'
} NETLizard_Font;

/* NETLizard 3D model data config */
#define NL_SKYFILE_LENGTH 10
#define NL_SUBFIX_LENGTH 12
#define NL_OBJ_SUBFIX_LENGTH 14
typedef struct NETLizard_3D_Model_Config_s
{
    NETLizard_Game game; // game
    NLint tex_count; // tex file count
    NETLizard_Texture_Type texture_type; // tex file type
    NLuint index_factory; // scene vertex index factory
    NLuint item_index_factory; // item vertex index factory
    NLboolean invert_texcoord_y; // invert scene texcoord'y: 1.0 - x
    NLboolean item_invert_texcoord_y; // invert item texcoord'y: 1.0 - x
    char sky_file[NL_SKYFILE_LENGTH];
    char tex_path_format[NL_SUBFIX_LENGTH];
    char obj_path_format[NL_OBJ_SUBFIX_LENGTH];
} NETLizard_3D_Model_Config;
#undef NL_SKYFILE_LENGTH
#undef NL_SUBFIX_LENGTH
#undef NL_OBJ_SUBFIX_LENGTH

/* PNG util */
NLboolean nlIsPNGFile(const char *name); // check file is normal png
NLboolean nlIsPNG(const char *data, NLsizei len); // check data is normal png
NLboolean nlIsNLPNGFile(const char *name); // check file is NETLizard png
NLboolean nlIsNLPNG(const char *data, NLsizei len); // check data is NETLizard png
char * nlEncodeDecodeData(const char *arr, char *data, NLsizei length); // encode/decode data to new data
char * nlEncodeDecodeFile(const char *file, NLint *rlen); // encode/decode file to new adta
char * nlEncodeDecodeDatav(char *arr, NLsizei length); // encode/decode data self
char * nlEncodeDecodeDatan(const char *arr, NLsizei length); // encode/decode data to new adta

/* Texture util */
NLboolean nlIsNL3DTextureV2File(const char *name); // check file is NETLizard 3D texture v2
NLboolean nlIsNL3DTextureV2(const char *data, NLsizei length); // check data is NETLizard 3D texture v2
NLboolean nlIsNL3DTextureV3File(const char *name); // check file is NETLizard 3D texture v3
NLboolean nlIsNL3DTextureV3(const char *data, NLsizei length); // check data is NETLizard 3D texture v3

/* String util: In java source, some string using integer array */
char * nlDecodeStringi(const NLint *arr, NLsizei length); // decode integer array to string
char * nlDecodeStringCi(const NLint *arr, NLsizei length); // decode integer array to string(C)
char * nlDecodeString(const char *arr, NLsizei length); // decode byte array to string
char * nlDecodeStringC(const char *arr, NLsizei length); // decode byte array to string(C)
NLint * nlEncodeStringC(const char *str, NLint *rlen); // encode string to integer array

/* Text util: exam some text in `about`, `help` menu */
char * nlReadAndHandleTextFile(const char *name, NLint *len); // decode text file to data
char * nlLoadAndHandleTextData(const char *data, NLsizei len, NLint *rlen); // decode text data to data
NLboolean nlConvertAndHandleTextFile(const char *from, const char *to); // decode text file to file
NLboolean nlSaveAndHandleTextData(const char *data, NLsizei len, const char *to); // decode text data to file

/* Media util */
char * nlGet3DGameLevelMusicFileName(NETLizard_Game game, NLint level, char *file ARR_MIN_LEN(8)); // get 3D game music file of level
char * nlGet3DGameMenuMusicFileName(NETLizard_Game game, char *file ARR_MIN_LEN(8)); // get 3D game main menu music file

/* 3D spirit: exam broken bodies in `3D Clone` */
int nlReadSpiritMapFile(const char *file, NETLizard_Spirit **ret);
int nlLoadSpiritMapData(const char *data, NLsizei len, NETLizard_Spirit **ret);

/* Font: exam `fnt.png` */
NLboolean nlLoadNETLizardFontData(const char *data, NLsizei size, NETLizard_Font *fnt); // load font data
NLboolean nlReadNETLizardFontFile(const char *map_file, NETLizard_Font *fnt); // load font file
void delete_NETLizard_Font(NETLizard_Font *fnt); // free font

// 3D
void delete_NETLizard_3D_Mesh(NETLizard_3D_Mesh *mesh); // free 3D mesh
void delete_NETLizard_3D_Item_Mesh(NETLizard_3D_Item_Mesh *mesh); // free 3D item mesh
void delete_NETLizard_3D_Model(NETLizard_3D_Model *model); // free 3D model

// util
const char * nlGet3DModelAnimationName(NETLizard_3D_Animation_Type anim); // get 3D Egypt/3D Clone player character animation name
NLboolean nlCheck3DGameLevelIsAvailable(NETLizard_Game game, int level); // check 3D game level is availabel
int nlGetItemType(NETLizard_Game game, int index); // get 3D game item type
const NETLizard_3D_Frame_Animation * nlGet3DModelFrameAnimationConfig(NETLizard_Game game, NLuint index); // get 3D Egypt/3D Clone player character animation index start and end
const char * nlGet3DGameLevelName(NETLizard_Game game, NLuint level);
NLboolean nlGet3DGameLevelRange(NETLizard_Game game, NLint *start, NLint *count);
const char * nlGet3DGameName(NETLizard_Game game);
const NETLizard_3D_Model_Config * nlGet3DGameModelConfig(NETLizard_Game game);

// Racing Evolution 3D

// Contr Terrisiem 3D
NLboolean nlReadCT3DModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadCT3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Army Ranger 3D: Operation Artcle
NLboolean nlReadSpecnaz3DModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadSpecnaz3DModelData(const char* data, NLsizei size, int level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadSpecnaz3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadSpecnaz3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Contr Terrisiem 3D: Episode-2
NLboolean nlReadCT3DEp2ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DEp2ModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadCT3DEp2ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DEp2ItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// 3D Shadows of Egypt
NLboolean nlReadEgypt3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadEgypt3DModelData(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadEgypt3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadEgypt3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);
NLboolean nlReadEgypt3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadEgypt3DRoleModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Clone 3D
NLboolean nlReadClone3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadClone3DModel(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadClone3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlReadClone3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model);

// Contr Terrisiem 3D: Episode-3
NLboolean nlReadCT3DEp3ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DEp3ModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLboolean nlReadCT3DEp3ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLboolean nlLoadCT3DEp3ItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Racing Evolution 3D
NLboolean nlIsRE3DMeshFile(const char *name);
NLboolean nlIsRE3DMesh(const char *data, NLsizei len);
void delete_NETLizard_RE3D_Mesh(NETLizard_RE3D_Mesh *mesh);
void delete_NETLizard_RE3D_Model(NETLizard_RE3D_Model *model);
NLboolean nlReadRE3DMeshFile(const char *name, NETLizard_RE3D_Model *model);
NLboolean nlLoadRE3DMeshData(const char *data,  NLsizei size,  NETLizard_RE3D_Model *model);

/* Texture util */
NETLizard_Texture_Type nlGetPNGType(const char *data, NLsizei length); // check png image/texture file type
NETLizard_Texture_Type nlGetPNGFileType(const char *file); // check png image/texture data type
void delete_NETLizard_Texture(NETLizard_Texture *tex); // free texture

/* PNG */
char * nlReadAndHandlePNGFile(const char *name, NLint *len); // encode/decode png file to data
char * nlLoadAndHandlePNGData(const char *data, NLint len); // encode/decode png data to data
NLboolean nlConvertAndHandlePNGFile(const char *from, const char *to); // encode/decode png file to file
NLboolean nlSaveAndHandlePNGData(const char *data, NLint len, const char *to); // encode/decode png data to file

/* Texture v2 */
NLboolean nlReadTextureV2File(const char *name, NETLizard_Texture *tex); // load 3D texture v2 file
NLboolean nlLoadTextureV2Data(const char *data, NLsizei length, NETLizard_Texture *tex); // load 3D texture v2 data
NLboolean nlConvertTextureV2FileToImageFile(const char *from, const char *to, int img_type); // save 3D texture v2 file to normal png/jpg file
NLboolean nlSaveTextureV2DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v2 data to normal png/jpg file
NLboolean nlSavePixelDataToTextureV2File(const NLuchar *data,  int width, int height, NETLizard_Texture_format format, const char *to); // save raw pixel data to 3D texture v2 file
NLboolean nlConvertImageFileToTextureV2File(const char *from, const char *to); // save normal png/jpg file to 3D texture v2 file

/* Texture v3 */
NLboolean nlReadTextureV3File(const char *name, NLint i1, NETLizard_Texture *tex); // load 3D texture v3 file
NLboolean nlLoadTextureV3Data(const char *data, NLsizei length, NLint i1, NETLizard_Texture *tex); // load 3D texture v3 data
NLboolean nlConvertTextureV3FileToImageFile(const char *from, NLint i, const char *to, int img_type); // save 3D texture v3 file to normal png/jpg file
NLboolean nlSaveTextureV3DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v3 data to normal png/jpg file
NLboolean nlSavePixelDataToTextureV3File(const NLuchar *data, NLint _i, int width, int height, NETLizard_Texture_format format, const char *to); // save raw pixel data to 3D texture v3 file

/* Texture v3 compress */
NLboolean nlReadCompressTextureV3File(const char *name, NETLizard_Texture *tex); // load 3D texture v3 file
NLboolean nlLoadCompressTextureV3Data(const char *data, NLsizei length, NETLizard_Texture *tex); // load 3D texture v3 data
NLboolean nlConvertTextureV3CompressFileToImageFile(const char *from, const char *to, int img_type); // save 3D texture v3 file to normal png/jpg file
NLboolean nlSaveTextureV3CompressDataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v3 data to normal png/jpg file

/* OpenGL util */
NLuchar * nlMakePixelDataRGBACompress(const NETLizard_Texture *tex, NLint *rlen);
NLuchar * nlMakePixelDataRGB(const NETLizard_Texture *tex, NLint *rlen);
NLuchar * nlMakePixelDataRGBA(const NETLizard_Texture *tex, NLint *rlen);
NLuchar * nlMakePixelData(const NETLizard_Texture *tex, NLint *rlen);

#ifdef __cplusplus
}
#endif

#endif
