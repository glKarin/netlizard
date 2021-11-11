#ifndef _KARIN_NLALGO_H
#define _KARIN_NLALGO_H

#include "nl_gl.h"
#include "lib/vector3.h"
#include "nl_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum NETLizard_Collision_Testing_Scene_Result_e
{
    NETLizard_Collision_Testing_Scene_Outside = 0, // �����г�����
    NETLizard_Collision_Testing_Scene_Only_In_AABB = 1, // ���ڳ���AABB��, ����������ƽ����
    NETLizard_Collision_Testing_Scene_Pass = 2, // ����ͨ��, ��λ��û����������
    NETLizard_Collision_Testing_Scene_Missing_Plane = 3, // ���ڳ���AABB�ڣ����Ǹó�������ײ������
    NETLizard_Collision_Testing_Scene_Movement = 4, // �ڳ���AABB��, ���ڳ���������ײ����, ����λ�ñ���������
    NETLizard_Collision_Testing_Scene_Item = 5 // ��������ײ
} NETLizard_Collision_Testing_Scene_Result;

typedef enum NETLizard_Collision_Testing_Item_Result_e
{
    NETLizard_Collision_Testing_Item_Inside = 0, // ��AABB�ڣ�����������ײ����
    NETLizard_Collision_Testing_Item_Only_In_AABB = 1, // ����AABB�ڣ�����������ײ����
    NETLizard_Collision_Testing_Item_Pass = 2, // ����ͨ��, ����AABB�ڣ�Ҳ����������ײ����
    NETLizard_Collision_Testing_Item_Missing_Plane = 3, // ����AABB�ڣ���������ײ������
    NETLizard_Collision_Testing_Item_Movement = 4, // ����AABB�ڣ�Ҳ����������ײ����, ���Ǿ���̫��
    NETLizard_Collision_Testing_Item_Ignore = 5 // ���Ե� ��������ײ����
} NETLizard_Collision_Testing_Item_Result;

typedef struct collision_object_s
{
    vector3_t position;
    float radius;
    float height;
} collision_object_t;

int NETLizard_FindScenePointIn(const GL_NETLizard_3D_Model *map, const nl_vector3_t *p);
int NETLizard_GetScenePointZCoord(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, int scene, unsigned include_item, int *rscene, float *rglz);
int NETLizard_MapCollisionTesting(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, nl_vector3_t *new_pos, int *scene, unsigned include_item, int *collision_item);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_NLALGO_H
