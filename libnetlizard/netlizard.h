#ifndef _KARIN_NETLIZARD_H
#define _KARIN_NETLIZARD_H

#include <stdint.h>
#include <stddef.h>

#if defined(__WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__) || defined(_WIN32)
#    define NL_EXPORT __declspec(dllexport)
#    define NL_IMPORT __declspec(dllimport)
#    define NL_HIDDEN
#else
#  define NL_EXPORT     __attribute__((visibility("default")))
#  define NL_IMPORT     __attribute__((visibility("default")))
#  define NL_HIDDEN     __attribute__((visibility("hidden")))
#endif

#if defined(_NL_STATIC)
#define NLAPI
#elif defined(_NL_DLL)
#  define NLAPI NL_EXPORT
#else
#  define NLAPI NL_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

////////// internal macros //////////
#define NL_ARG_MIN_LEN(len) // array/string param min length, if string include '\0'
#define NL_ARG_DEF_VAL(val) // param default value if set null or 0
#define NL_ARG_VAL_UNUSED() // param is unused
#define NL_ARG_READ_WRITE(mask) // param pointer is read and write
#define NL_ARG_NOT_NULL(b) // param pointer is not null, function do not check pointer is null
#define NL_ARG_PTR_ALLOC(T) // param pointer is alloc memory
#define NL_ARG_ENUM(enum_name) // integer param is a enum
#define NL_ARG_PTR_STR() // pointer param is a string
#define NL_ARG_PTR_ARR() // pointer param is a array
#define NL_ARG_LEN(arg_name) // integer param is a param's size/length

#define NL_RET_PTR_ALLOC(T) // return pointer is alloc memory
#define NL_RET_PTR_CONST(T) // return pointer is constant, do not modify
#define NL_RET_PTR_REF_ARG(T, arg_name) // return pointer is a in-param

////////// macros //////////
/* NETLizard 3D game v2 texture header. 3D CT - 3D CT2 */
#define NL_TEXTURE_V2_HEADER_MAGIC "&&&"
/* NETLizard 3D game v3 texture header. 3D Egypt - 3D Clone */
#define NL_TEXTURE_V3_HEADER_MAGIC "BIP"
/* NETLizard Racing evolution 3D mesh header.*/
#define NL_RE3D_MESH_HEADER_MAGIC "NL_MESH"
/* NETLizard game png encode/decode factor. exam: a_byte = a_byte ^ 100 */
#define NL_ENCODE_DECODE_FACTOR 100
#define NL_RESOURCE_FILE_FORMAT_LENGTH 12

#define NL_VERSION_1_0 0x01000101
#define NL_VERSION_LASTEST NL_VERSION_1_0

#define NL_TRUE 1
#define NL_FALSE 0

#define NL_LOG 0x1

#define NL_LOG_OUT 0x1
#define NL_LOG_ERR 0x2

#define NL_LOG_STD 0x1
#define NL_LOG_USER 0x2

#define NL_NO_ERROR                             0x0
#define NL_INVALID_VALUE                        0x0501
#define NL_INVALID_ENUM                         0x0500
#define NL_INVALID_OPERATION                    0x0502
#define NL_STACK_OVERFLOW                       0x0503
#define NL_STACK_UNDERFLOW                      0x0504
#define NL_OUT_OF_MEMORY                        0x0505

#define NL_VENDOR                               0x1F00
#define NL_VERSION                              0x1F02
#define NL_ERROR                              0x1F10

////////// typedef //////////
/* Basic type define */
typedef void NLvoid;
typedef uint8_t NLubyte;
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
typedef float NLclampf;
typedef double NLclampd;
typedef uint8_t NLboolean;
typedef int32_t NLsizei; // uint32_t
typedef uint32_t NLenum;
typedef uint32_t NLbitfield;

typedef float NLclamp;
typedef uint32_t NLsize;
typedef void * NLdata;
typedef void * NLfunc;
typedef NLboolean NLbool;

//typedef char byte; // unsigned

////////// enum //////////
/* NETLizard game define */
enum NETLizard_Game_e
{
    NL_RACING_EVOLUTION_3D = 0, // Racing Evolution 3D
    NL_CONTR_TERRORISM_3D, // Contr Terrorism 3D
    NL_ARMY_RANGER_3D, // 3D Army Ranger : Operation Arctic
    NL_CONTR_TERRORISM_3D_EPISODE_2, // 3D Contr Terrorism : Episode-2
    NL_SHADOW_OF_EGYPT_3D, // 3D Shadows of Egypt
    NL_CLONE_3D, // Clone 3D
    NL_CONTR_TERRORISM_3D_EPISODE_3, // 3D Contr Terrorism : Episode-3
    NL_TOTAL_GAME, // Total 3D game
    NL_2D_GAME, // In 2D game and old 3D game, `png` file need decode.
    NL_OLD_GAME // In old game, `png` file not need decode.
};
typedef enum NETLizard_Game_e NETLizard_Game;

/* Texture pixel format */
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

/* NETLizard 3D game item model category */
typedef enum NETLizard_3D_Item_Type_e
{
    NL_3D_ITEM_TYPE_GENERAL = 0, // exam general item
    NL_3D_ITEM_TYPE_THIN = 1, // maybe not cull face when rendering. exam net
    NL_3D_ITEM_TYPE_2D = 1 << 1, // maybe no shadow when rendering. exam Stalin photo in CT 3D
    NL_3D_ITEM_TYPE_FAN_Z_AXIS = 1 << 2, // can rotate on x/y-axis. exam fan on ceil, tail fan in helicopter
    NL_3D_ITEM_TYPE_FAN_Y_AXIS = 1 << 3, // can rotate on z-axis. exam fan on wall, top fan in helicopter
    NL_3D_ITEM_TYPE_ROLE = 1 << 4, // has frame animation. exam character in Egypt 3D and Clone 3D
    NL_3D_ITEM_TYPE_SKYBOX = 1 << 5, // it is a skybox. exam skybox in Egypt 3D level 0, 8, 9, 10, 12
    NL_3D_ITEM_TYPE_DOOR_VERTICAL = 1 << 6, // can translate on z-axis. exam door that move vertical
    NL_3D_ITEM_TYPE_DOOR_HORIZONTAL = 1 << 7, // can translate on x/y-axis. exam door that move horizontal
    NL_3D_ITEM_TYPE_ELEVATOR = 1 << 8, // can translate on z-axis. exam elevator in Clone 3D
    NL_3D_ITEM_TYPE_LADDER = 1 << 9, // it is a way that can move on z-axis. exam ladder in CT-EP2 3D and CT-EP3 3D
    NL_3D_ITEM_TYPE_WEAPON = 1 << 10, // a weapon. exam guns or knifes
    NL_3D_ITEM_TYPE_SKYLINE = 1 << 11, // has floor plane. exam float bridge in Egypt 3D level 5, 8, 9, 10, 12
    NL_3D_ITEM_TYPE_PORTAL = 1 << 12, // can teleport. exam teleporter in Egypt 3D
    NL_3D_ITEM_TYPE_SWITCH = 1 << 13, // it is a switch controller. exam elevator switch in Clone 3D
    NL_3D_ITEM_TYPE_TRANSPARENT = 1 << 14, // maybe no shadow when rendering. exam net
    NL_3D_ITEM_TYPE_EMPTY = 1 << 15, // maybe ignore when collision testing. exam tower in Egypt 3D level 0, 10
    NL_3D_ITEM_TYPE_PARTICLE = 1 << 16, // it is a particle item. exam debris of bodys and box, bloods,
    NL_3D_ITEM_TYPE_SCENE = 1 << 17, // exam scene. exam elevator in Specnaz 3D level 4
    NL_3D_ITEM_TYPE_FAN_X_AXIS = 1 << 18 // can rotate on z-axis. exam fan on wall, top fan in helicopter
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

////////// structure //////////
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
    NLint start_position[3]; // player start position
    NLint start_rotation[2]; // player start direction
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
typedef struct NETLizard_3D_Frame_Animation_s
{
    NETLizard_3D_Animation_Type type;
    NLint begin_frame;
    NLint end_frame;
    NLuint count;
} NETLizard_3D_Frame_Animation;

/* NETLizard sprite cell */
typedef struct NETLizard_Sprite_Cell_s
{
    NLshort index;
    NLshort x;
    NLshort y;
    NLshort width;
    NLshort height;
    NLshort private_4;
    NLshort private_5;
} NETLizard_Sprite_Cell;

/* NETLizard sprite */
typedef struct NETLizard_Sprite_s
{
    NETLizard_Sprite_Cell *data;
    NLuint count;
} NETLizard_Sprite;

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
      NLuint count;
  } char_raster_map; // char table
  struct {
      NLchar *data;
      NLuint count;
  } offset;
  NLuint char_count; // char count

  NLuchar private_e; // byte unknown
  NLuchar private_l; // byte unknown
  NLuchar private_m; // byte unknown

  char fnt_file[8]; // file name, aways 'fnt.png'
} NETLizard_Font;

/* NETLizard 3D model data config */
typedef struct NETLizard_3D_Model_Config_s
{
    NETLizard_Game game; // game
    NLint tex_count; // tex file count
    NETLizard_Texture_Type texture_type; // tex file type
    NLuint index_factory; // scene vertex index factory
    NLuint item_index_factory; // item vertex index factory
    NLboolean invert_texcoord_y; // invert scene texcoord'y: 1.0 - x
    NLboolean item_invert_texcoord_y; // invert item texcoord'y: 1.0 - x
    char sky_file[10];
    char tex_path_format[12];
    char obj_path_format[14];
    NLuint level_count;
} NETLizard_3D_Model_Config;

/* NETLizard 3D map teleport config, Egypt 3D */
typedef struct NETLizard_Level_Teleport_s
{
    NLenum game;
    NLint level;
    NLint item;
    NLbitfield mask; // 1: x translate, 2: y translate, 4: z translate, 8: x rotate, 16: y rotate
    NLint position[3];
    NLint rotation[2];
} NETLizard_Level_Teleport;

/* NETLizard 3D map elevator config, Clone 3D */
typedef struct NETLizard_Level_Elevator_s
{
    NLenum game;
    NLint level;
    NLint switch_item;
    NLint elevator_item;
    NLbitfield mask; // 1: move up, 2: move down
    NLint min;
    NLint max;
    NLboolean invert;
} NETLizard_Level_Elevator;

/* NETLizard 3D map door config */
typedef struct NETLizard_Level_Door_s
{
    NLenum game;
    NLint level;
    struct {
        NLint item;
        NLint start;
        NLint end;
    } item[2];
    NLbitfield mask; // 1: left/bottom door can move to left/bottom, 2: right/top door can move to right/up
    NLenum orientation; // 1: up-down, 2: left-right
    struct {
        NLint min[3];
        NLint max[3];
    } box;
} NETLizard_Level_Door;

////////// function //////////
/* PNG util */
NLAPI NLboolean nlIsPNGFile(const char *name); // check file is normal png
NLAPI NLboolean nlIsPNG(const char *data, NLsizei len); // check data is normal png
NLAPI NLboolean nlIsNLPNGFile(const char *name); // check file is NETLizard png
NLAPI NLboolean nlIsNLPNG(const char *data, NLsizei len); // check data is NETLizard png
NLAPI NL_RET_PTR_REF_ARG(char *, data) char * nlEncodeDecodeData(const char *arr, char *data, NLsizei length); // encode/decode data to special data
NLAPI NL_RET_PTR_ALLOC(char *) char * nlEncodeDecodeFile(const char *file, NLint *rlen); // encode/decode file to new adta
NLAPI NL_RET_PTR_REF_ARG(char *, arr) char * nlEncodeDecodeDatav(char *arr, NLsizei length); // encode/decode data self
NLAPI NL_RET_PTR_ALLOC(char *) char * nlEncodeDecodeDataa(const char *arr, NLsizei length); // encode/decode data to new adta

/* Texture util */
NLAPI NLboolean nlIsNL3DTextureV2File(const char *name); // check file is NETLizard 3D texture v2
NLAPI NLboolean nlIsNL3DTextureV2(const char *data, NLsizei length); // check data is NETLizard 3D texture v2
NLAPI NLboolean nlIsNL3DTextureV3File(const char *name); // check file is NETLizard 3D texture v3
NLAPI NLboolean nlIsNL3DTextureV3(const char *data, NLsizei length); // check data is NETLizard 3D texture v3

/* String util: In java source, some string using integer array */
NLAPI NL_RET_PTR_ALLOC(char *) char * nlDecodeStringi(const NLint *arr, NLsizei length); // decode integer array to string
NLAPI NL_RET_PTR_ALLOC(char *) char * nlDecodeStringCi(const NLint *arr, NLsizei length); // decode integer array to string(C)
NLAPI NL_RET_PTR_ALLOC(char *) char * nlDecodeString(const char *arr, NLsizei length); // decode byte array to string
NLAPI NL_RET_PTR_ALLOC(char *) char * nlDecodeStringC(const char *arr, NLsizei length); // decode byte array to string(C)
NLAPI NL_RET_PTR_ALLOC(char *) NLint * nlEncodeStringC(const char *str, NLint *rlen); // encode string to integer array

/* Text util: exam some text in `about`, `help` menu */
NLAPI NL_RET_PTR_ALLOC(char *) char * nlReadAndHandleTextFile(const char *name, NLint *len); // decode text file to data
NLAPI NL_RET_PTR_ALLOC(char *) char * nlLoadAndHandleTextData(const char *data, NLsizei len, NLint *rlen); // decode text data to data
NLAPI NLboolean nlConvertAndHandleTextFile(const char *from, const char *to); // decode text file to file
NLAPI NLboolean nlSaveAndHandleTextData(const char *data, NLsizei len, const char *to); // decode text data to file

/* Media util */
NLAPI NL_RET_PTR_REF_ARG(char *, level) char * nlGet3DGameLevelMusicFileName(NETLizard_Game game, NLint level, char *file NL_ARG_MIN_LEN(8)); // get 3D game music file of level
NLAPI NL_RET_PTR_REF_ARG(char *, file) char * nlGet3DGameMenuMusicFileName(NETLizard_Game game, char *file NL_ARG_MIN_LEN(8)); // get 3D game main menu music file

/* 3D sprite: exam broken bodies in `3D Clone` */
NLAPI NLint nlGetSpiritFileCount(const char *file);
NLAPI NLint nlGetSpiritDataCount(const char *data, NLsizei length);
NLAPI NLboolean nlReadSpiritMapFile(const char *file, NETLizard_Sprite *ret);
NLAPI NLboolean nlLoadSpiritMapData(const char *data, NLsizei len, NETLizard_Sprite *ret);
NLAPI void nlDeleteNETLizardSprite(NETLizard_Sprite *sprite); // free sprite

/* Font: exam `fnt.png` */
NLAPI NLboolean nlLoadNETLizardFontData(const char *data, NLsizei size, NETLizard_Font *fnt); // load font data
NLAPI NLboolean nlReadNETLizardFontFile(const char *map_file, NETLizard_Font *fnt); // load font file
NLAPI void nlDeleteNETLizardFont(NETLizard_Font *fnt); // free font

// 3D
NLAPI void nlDeleteNETLizard3DMesh(NETLizard_3D_Mesh *mesh); // free 3D mesh
NLAPI void nlDeleteNETLizard3DItemMesh(NETLizard_3D_Item_Mesh *mesh); // free 3D item mesh
NLAPI void nlDeleteNETLizard3DModel(NETLizard_3D_Model *model); // free 3D model

// util
NLAPI NL_RET_PTR_CONST(char *) const char * nlGet3DModelFrameAnimationName(NETLizard_3D_Animation_Type anim); // get 3D Egypt/3D Clone player character animation name
NLAPI NLboolean nlCheck3DGameLevelIsAvailable(NETLizard_Game game, int level); // check 3D game level is availabel
NLAPI int nlGetItemType(NETLizard_Game game, int index); // get 3D game item type
NLAPI const NETLizard_3D_Frame_Animation * nlGet3DModelFrameAnimationConfig(NETLizard_Game game, NLuint index); // get 3D Egypt/3D Clone player character animation index start and end
NLAPI NL_RET_PTR_CONST(char *) const char * nlGet3DGameLevelName(NETLizard_Game game, NLuint level);
NLAPI NLboolean nlGet3DGameLevelRange(NETLizard_Game game, NLint *start, NLint *count);
NLAPI NL_RET_PTR_CONST(char *) const char * nlGet3DGameName(NETLizard_Game game);
NLAPI const NETLizard_3D_Model_Config * nlGet3DGameModelConfig(NETLizard_Game game);
NLAPI const NETLizard_Level_Teleport * nlGet3DGameTeleport(NLenum game, NLint level, NLint item_id, NLint *length);
NLAPI const NETLizard_Level_Elevator * nlGet3DGameElevator(NLenum game, NLint level, NLint item_id, NLint *length);
NLAPI const NETLizard_Level_Door * nlGet3DGameDoor(NLenum game, NLint level, NLint item_id, NLint *length);

// Contr Terrisiem 3D
NLAPI NLboolean nlReadCT3DModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadCT3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Army Ranger 3D: Operation Artcle
NLAPI NLboolean nlReadSpecnaz3DModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadSpecnaz3DModelData(const char* data, NLsizei size, int level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadSpecnaz3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadSpecnaz3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Contr Terrisiem 3D: Episode-2
NLAPI NLboolean nlReadCT3DEp2ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DEp2ModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadCT3DEp2ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DEp2ItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// 3D Shadows of Egypt
NLAPI NLboolean nlReadEgypt3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadEgypt3DModelData(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadEgypt3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadEgypt3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadEgypt3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadEgypt3DRoleModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Clone 3D
NLAPI NLboolean nlReadClone3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadClone3DModelData(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadClone3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadClone3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadClone3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadClone3DRoleModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Contr Terrisiem 3D: Episode-3
NLAPI NLboolean nlReadCT3DEp3ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DEp3ModelData(const char* data, NLsizei size, NLint level, const char *resc_path, NETLizard_3D_Model *model);
NLAPI NLboolean nlReadCT3DEp3ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model);
NLAPI NLboolean nlLoadCT3DEp3ItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model);

// Racing Evolution 3D
NLAPI NLboolean nlIsRE3DMeshFile(const char *name);
NLAPI NLboolean nlIsRE3DMesh(const char *data, NLsizei len);
NLAPI void nlDeleteNETLizardRE3DMesh(NETLizard_RE3D_Mesh *mesh);
NLAPI void nlDeleteNETLizardRE3DModel(NETLizard_RE3D_Model *model);
NLAPI NLboolean nlReadRE3DMeshFile(const char *name, NETLizard_RE3D_Model *model);
NLAPI NLboolean nlLoadRE3DMeshData(const char *data,  NLsizei size,  NETLizard_RE3D_Model *model);

/* Texture util */
NLAPI NETLizard_Texture_Type nlGetPNGType(const char *data, NLsizei length); // check png image/texture file type
NLAPI NETLizard_Texture_Type nlGetPNGFileType(const char *file); // check png image/texture data type
NLAPI void nlDeleteNETLizardTexture(NETLizard_Texture *tex); // free texture

/* PNG */
NLAPI NL_RET_PTR_ALLOC(char *) char * nlReadAndHandlePNGFile(const char *name, NLint *len); // encode/decode png file to data
NLAPI NL_RET_PTR_ALLOC(char *) char * nlLoadAndHandlePNGData(const char *data, NLint len); // encode/decode png data to data
NLAPI NLboolean nlConvertAndHandlePNGFile(const char *from, const char *to); // encode/decode png file to file
NLAPI NLboolean nlSaveAndHandlePNGData(const char *data, NLint len, const char *to); // encode/decode png data to file

/* Texture v2 */
NLAPI NLboolean nlReadTextureV2File(const char *name, NETLizard_Texture *tex); // load 3D texture v2 file
NLAPI NLboolean nlLoadTextureV2Data(const char *data, NLsizei length, NETLizard_Texture *tex); // load 3D texture v2 data
NLAPI NLboolean nlConvertTextureV2FileToImageFile(const char *from, const char *to, int img_type); // save 3D texture v2 file to normal png/jpg file
NLAPI NLboolean nlSaveTextureV2DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v2 data to normal png/jpg file
NLAPI NLboolean nlSavePixelDataToTextureV2File(const NLuchar *data,  int width, int height, NETLizard_Texture_format format, const char *to); // save raw pixel data to 3D texture v2 file
NLAPI NLboolean nlConvertImageFileToTextureV2File(const char *from, const char *to); // save normal png/jpg file to 3D texture v2 file

/* Texture v3 */
NLAPI NLboolean nlReadTextureV3File(const char *name, NLint i1, NETLizard_Texture *tex); // load 3D texture v3 file
NLAPI NLboolean nlLoadTextureV3Data(const char *data, NLsizei length, NLint i1, NETLizard_Texture *tex); // load 3D texture v3 data
NLAPI NLboolean nlConvertTextureV3FileToImageFile(const char *from, NLint i, const char *to, int img_type); // save 3D texture v3 file to normal png/jpg file
NLAPI NLboolean nlSaveTextureV3DataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v3 data to normal png/jpg file
NLAPI NLboolean nlSavePixelDataToTextureV3File(const NLuchar *data, NLint _i, int width, int height, NETLizard_Texture_format format, const char *to); // save raw pixel data to 3D texture v3 file

/* Texture v3 compress */
NLAPI NLboolean nlReadCompressTextureV3File(const char *name, NETLizard_Texture *tex); // load 3D texture v3 file
NLAPI NLboolean nlLoadCompressTextureV3Data(const char *data, NLsizei length, NETLizard_Texture *tex); // load 3D texture v3 data
NLAPI NLboolean nlConvertTextureV3CompressFileToImageFile(const char *from, const char *to, int img_type); // save 3D texture v3 file to normal png/jpg file
NLAPI NLboolean nlSaveTextureV3CompressDataToImageFile(const NETLizard_Texture *tex, const char *to, int img_type); // save 3D texture v3 data to normal png/jpg file

/* OpenGL util */
NLAPI NL_RET_PTR_ALLOC(NLuchar *) NLuchar * nlMakePixelDataRGBACompress(const NETLizard_Texture *tex, NLint *rlen);
NLAPI NL_RET_PTR_ALLOC(NLuchar *) NLuchar * nlMakePixelDataRGB(const NETLizard_Texture *tex, NLint *rlen);
NLAPI NL_RET_PTR_ALLOC(NLuchar *) NLuchar * nlMakePixelDataRGBA(const NETLizard_Texture *tex, NLint *rlen);
NLAPI NL_RET_PTR_ALLOC(NLuchar *) NLuchar * nlMakePixelData(const NETLizard_Texture *tex, NLint *rlen);

/* misc */
NLAPI void nlEnable(NLenum e);
NLAPI void nlDisable(NLenum e);
NLAPI NLboolean nlIsEnabled(NLenum e);
NLAPI NLenum nlGetError(void);
NLAPI NL_RET_PTR_CONST(char *) const char * nlErrorString(NLenum error);
NLAPI NL_RET_PTR_CONST(char *) const char * nlGetString(NLenum name);
NLAPI void nlGetIntegerv(NLenum name, NLint *ret);
NLAPI void nlGetFloatv(NLenum name, NLfloat *ret);
NLAPI void nlGetBooleanv(NLenum name, NLboolean *ret);
NLAPI void nlGetPointerv(NLenum name, NLvoid **ret);

NLAPI void nlLogFunc(NLenum type, NLenum way, void *f);

#ifdef __cplusplus
}
#endif

#endif
