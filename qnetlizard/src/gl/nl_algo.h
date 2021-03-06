#ifndef _KARIN_NLALGO_H
#define _KARIN_NLALGO_H

#include "math/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _GL_NETLizard_3D_Model;

typedef enum NETLizard_Collision_Testing_Scene_Result_e
{
    NETLizard_Collision_Testing_Scene_Outside = 0, // 在所有场景外
    NETLizard_Collision_Testing_Scene_Only_In_AABB = 1, // 仅在场景AABB内, 但是在所有平面外
    NETLizard_Collision_Testing_Scene_Pass = 2, // 测试通过, 且位置没有重新设置
    NETLizard_Collision_Testing_Scene_Missing_Plane = 3, // 仅在场景AABB内，但是该场景无碰撞面数据
    NETLizard_Collision_Testing_Scene_Movement = 4, // 在场景AABB内, 且在场景所有碰撞面内, 但是位置被重新设置
    NETLizard_Collision_Testing_Scene_Item = 5, // 与物体碰撞
    NETLizard_Collision_Testing_Scene_Only_In_Planes = 6 // 仅在所有平面内, 但距离过小
} NETLizard_Collision_Testing_Scene_Result;

typedef enum NETLizard_Collision_Testing_Item_Result_e
{
    NETLizard_Collision_Testing_Item_Inside = 0, // 在AABB内，并在所有碰撞面内
    NETLizard_Collision_Testing_Item_Only_In_AABB = 1, // 仅在AABB内，不在所有碰撞面内
    NETLizard_Collision_Testing_Item_Pass = 2, // 测试通过, 不在AABB内，也不在所有碰撞面内
    NETLizard_Collision_Testing_Item_Missing_Plane = 3, // 仅在AABB内，但是无碰撞面数据
    NETLizard_Collision_Testing_Item_Movement = 4, // 不在AABB内，也不在所有碰撞面内, 但是距离太近
    NETLizard_Collision_Testing_Item_Ignore = 5 // 忽略的 不进行碰撞测试
} NETLizard_Collision_Testing_Item_Result;

typedef struct collision_object_s
{
    vector3_t position;
    float radius;
    float height; // foot - eye
    float leg_height; // leg
    float head_height; // sys - head
} collision_object_t;

int NETLizard_FindScenePointIn(const struct _GL_NETLizard_3D_Model *map, const vector3_t *p);
int NETLizard_GetScenePointZCoord(const struct _GL_NETLizard_3D_Model *map, const collision_object_t *obj, int scene, unsigned include_item, int *rscene, float *rglz);
int NETLizard_MapCollisionTesting(const struct _GL_NETLizard_3D_Model *map, const collision_object_t *obj, vector3_t *new_pos, int *scene, unsigned include_item, int *collision_item);
int NETLizard_RayIntersect(const struct _GL_NETLizard_3D_Model *map, const vector3_t *obj, const vector3_t *direction, unsigned include_item, int *scene, int *collision_id, int *collision_type, vector3_t *cpoint, float *dis);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_NLALGO_H
