#include "nl_algo.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "lib/bound.h"
#include "lib/line.h"
#include "nl_util.h"

#define SCENE_BOUND(mesh) BOUNDV(mesh->box.min, mesh->box.max)
#define SCENE_PLANE(plane) PLANEV(plane.position, plane.normal)

#define UP_NORMAL_LIMIT 0.866025

#define IS_FLOOR(normal) ((normal)[2] > UP_NORMAL_LIMIT)
#define IS_CEIL(normal) ((normal)[2] < -UP_NORMAL_LIMIT)

/*
  ��a�Ƿ���ȫͼ��Χ��
  */
int NETLizard_PointInMapBound(const GL_NETLizard_3D_Model *map, const nl_vector3_t *a)
{
    if(!map || !a)
        return -1;
    bound_t bound;
    NETLizard_GetNETLizard3DMapBound(map, NULL, 0, &bound);
    return bound_point_in_box(&bound, a);
}

/*
  ��position�Ƿ��ڳ���scene��
  */
int NETLizard_PointInScene(const GL_NETLizard_3D_Mesh *scene, const nl_vector3_t *position)
{
    if(!scene || !position)
        return 0;
    bound_t aabb = SCENE_BOUND(scene);
    return bound_point_in_box(&aabb, position);
}

/*
  ��ȡ��p���ڵĳ�������
  */
int NETLizard_FindScenePointIn(const GL_NETLizard_3D_Model *map_model, const nl_vector3_t *p)
{
    if(!map_model || !p)
        return -1;
    unsigned int i;
    for(i = 0; i < map_model->count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = map_model->meshes + i;
        bound_t aabb = SCENE_BOUND(mesh);
        if(bound_point_in_box(&aabb, p))
            return i;
    }
    return -1;
}

/*
  ��ȡnew_pos�µĳ�������scene, ���س������ӵ����ֵrglz. floorΪ����ó��������еذ�
  */
int NETLizard_GetTopSceneUnderPoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, int floor, int *scene, float *rglz)
{
    if(!netlizard_3d_model || !new_pos)
        return 0;
    unsigned int i;
    int res = 0;
    int s = -1;
    float z = 0.0;
    for(i = 0; i < netlizard_3d_model->count; i++)
    {
        const GL_NETLizard_3D_Mesh *next_mesh = netlizard_3d_model->meshes + i;
        bound_t aabb = SCENE_BOUND(next_mesh);
        if(!bound_point_in_box2d(&aabb, new_pos))
            continue;
        if(floor)
        {
            if(!next_mesh->plane)
                continue;
            int j;
            int has_floor = 0;
            for(j = 0; j < next_mesh->plane_count; j++)
            {
                if(IS_FLOOR(next_mesh->plane[j].normal))
                {
                    has_floor = 1;
                    break;
                }
            }
            if(!has_floor)
                continue;
        }
        if(res == 0)
        {
            if(BOUND_MAX_Z(aabb) < VECTOR3V_Z(new_pos))
            {
                s = i;
                z = BOUND_MAX_Z(aabb);
                res = 1;
            }
        }
        else
        {
            if(BOUND_MAX_Z(aabb) > z && BOUND_MAX_Z(aabb) < VECTOR3V_Z(new_pos))
            {
                z = BOUND_MAX_Z(aabb);
                s = i;
            }
        }
    }
    if(res)
    {
        if(scene)
            *scene = s;
        if(rglz)
            *rglz = z;
    }

    return res;
}

/*
  ��ȡ��new_pos�µĳ�������scene�ĵذ�Z����rglz
  */
int NETLizard_GetSceneFloorZCoordInScenePoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, int scene, float *rglz)
{
    if(!netlizard_3d_model || !new_pos)
        return 0;
    const GL_NETLizard_3D_Mesh *mesh = netlizard_3d_model->meshes + scene;
    unsigned int j;
    for(j = 0; j < mesh->plane_count; j++)
    {
        // ���㵱ǰλ�õĵذ�����
        if(mesh->plane[j].normal[2] > UP_NORMAL_LIMIT)
        {
            plane_t pla = SCENE_PLANE(mesh->plane[j]);
            nl_vector3_t point = VECTOR3(0, 0, 0);
            nl_vector3_t dir = PLANE_NORMAL(pla);
            VECTOR3_Z(dir) = -VECTOR3_Z(dir);
            ray_t l = {*new_pos, dir};
            if(ray_line_to_plane_intersect(&l, &pla, &point))
            {
                if(rglz)
                    *rglz = VECTOR3_Z(point);
                return 1;
            }
        }
    }
    return 0;
}

/*
  ��ȡ��new_pos�µ����г����ĵذ�Z����rglz, ���س�������scene
  */
int NETLizard_GetSceneFloorZCoordUnderPoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, int *scene, float *rglz)
{
    if(!netlizard_3d_model || !new_pos)
        return 0;
    int gress_scene;
    float gress_z;
    int res = NETLizard_GetTopSceneUnderPoint(netlizard_3d_model, new_pos, 1, &gress_scene, &gress_z);
    if(!res)
        return 0;
    nl_vector3_t pos = VECTOR3(VECTOR3V_X(new_pos), VECTOR3V_Y(new_pos), gress_z);
    res = NETLizard_GetSceneFloorZCoordInScenePoint(netlizard_3d_model, &pos, gress_scene, &gress_z);
    if(res)
    {
        if(scene)
            *scene = gress_scene;
        if(rglz)
            *rglz = gress_z;
    }
    return res;
}

/*
  ��ȡ��new_pos�µĳ�������scene�ĵذ�Z����rglz, ���û�еذ�, ���������Ѱ��, ���һֱû�еذ�, ���ȡ�����������ӵ���СZ, �����µĳ�������rscene
  */
int NETLizard_GetScenePointZCoord(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, int scene, int *rscene, float *rglz)
{
    if(!netlizard_3d_model || !new_pos)
        return 0;
    int res = 0;

    if(scene >= 0)
    {
        NETLizard_GetSceneFloorZCoordInScenePoint(netlizard_3d_model, new_pos, scene, rglz);
        if(res)
        {
            if(rscene)
                *rscene = scene;
            return 1;
        }
    }
    res = NETLizard_GetSceneFloorZCoordUnderPoint(netlizard_3d_model, new_pos, rscene, rglz);
    if(res)
        return 1;
    bound_t bound;
    NETLizard_GetNETLizard3DMapBound(netlizard_3d_model, 0, 0, &bound);
    vector3_t pos = *new_pos;
    VECTOR3_Z(pos) = BOUND_MIN_Z(bound);
    res = NETLizard_FindScenePointIn(netlizard_3d_model, &pos);
    if(res >= 0)
    {
        if(rscene)
            *rscene = res;
        if(rglz)
            *rglz = BOUND_MIN_Z(bound);
        return 1;
    }
    if(rscene)
        *rscene = -1;
    if(rglz)
        *rglz = BOUND_MIN_Z(bound);
    return 0;
}

// 0 - �ڳ����� ���� -1
// 1 - ���ڳ���AABB��
// 2 - �ڳ���AABB�ڣ����ڳ���������ײ����
// 3 - ���ڳ���AABB�ڣ����Ǹó�������ײ������
// 4 - �ڳ���AABB�ڣ����ڳ���������ײ���ڣ�������ĳһ��ײ��ľ���С�ڲ���ֵ
int NETLizard_MapCollisionTesting(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, const vector3_t *new_pos, int *scene)
{
    if(!map || !obj)
        return -1;
    int res = 0;

    const vector3_t *pos = &obj->position;
    const float height = obj->height;
    const float width = obj->radius;
    float lamda = 0.0;
    int s = NETLizard_FindScenePointIn(map, new_pos);
    if(s == -1)
        return 0; // �����κγ�����Χ��
    if(scene)
        *scene = s;
    const GL_NETLizard_3D_Mesh *mesh = map->meshes + s;
    if(!mesh->plane)
        return 3;

    res = 1;
    unsigned int j;
    for(j = 0; j < mesh->plane_count; j++)
    {
        plane_t plane = SCENE_PLANE(mesh->plane[j]);
        ray_t l = {*new_pos, VECTOR3(-mesh->plane[j].normal[0], -mesh->plane[j].normal[1], -mesh->plane[j].normal[2])};
        vector3_t cpoint;
        int r = ray_line_intersect_plane(&l, &plane, &lamda, &cpoint);
        if(r < 0)
        {
            res = 1; // ���ڳ�����ĳ��ƽ����
            return res;
        }

        float limit = 0;
        int po = 0;
        if(IS_FLOOR(mesh->plane[j].normal))
            limit = 0;
        else if(IS_CEIL(mesh->plane[j].normal))
            limit = height;
        else
        {
            limit = width;
            po = 1;
        }
        if(lamda < limit && po)
            res = 4;
    }
    if(res == 1)
        res = 2;
    return res;
}

#if 0
// 0 ��AABB�ڣ�����������ײ����
// 1 ����AABB�ڣ�����������ײ����
// 2 ����AABB�ڣ�Ҳ����������ײ����
// 3 ��AABB�ڣ�����û����ײ������
// 4 ����AABB�ڣ�Ҳ����������ײ����, ���Ǿ���̫��
// 5 ������Ʒ����������ײ����
item_collision_testing_result_type Algo_NETLizard3DItemCollisionTesting(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *pos, const nl_vector3_t *new_pos, float width, float height, int scene, nl_vector3_t *c_normal, int *index)
{
    if(!netlizard_3d_model || !pos || !new_pos)
        return item_in_all_planes_and_in_aabb_type;

    int res = item_only_in_aabb_type;
    const GL_NETLizard_3D_Mesh *mesh = netlizard_3d_model->meshes + scene;
    //float lamda = 0.0;
    nl_vector3_t position = {pos->x, pos->y, pos->z - height};
    nl_vector3_t new_position = {new_pos->x, new_pos->y, new_pos->z - height};
    nl_vector3_t direction = Vector3_SubtractVector3(&new_position, &position);
    Vector3_Normalize(&direction);
    ray_t line = {new_position, direction};
    unsigned int k;
    for(k = mesh->item_index_range[0]; k < mesh->item_index_range[1]; k++)
    {
        const GL_NETLizard_3D_Item_Mesh *im = netlizard_3d_model->item_meshes + k;
        bound_t aabb = {
            {im->item_mesh.ortho[3] - width + im->pos[0], im->item_mesh.ortho[4] - width + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + width + im->pos[0], im->item_mesh.ortho[1] + width + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        // �����λ���Ƿ��ڵ�ͼ��ĳһ��Ʒ������
        if(Math3D_PointInAABB(&new_position, &aabb))
        {
            res = item_only_in_aabb_type;
            //printfi(im->item_type);
            if(index)
                *index = k;
            if(im->item_type & Item_Weapon_Type || im->item_type & Item_Tiny_Type || im->item_type & Item_Box_Type)
            {
                if(index)
                    *index = k;
                return item_not_need_collision_testing_type;
            }
            // �����λ���Ƿ�����Ʒ��������ʵ������
            if(im->item_mesh.plane_count > 0)
            {
                const GL_NETLizard_3D_Plane *planes = im->item_mesh.plane;
                unsigned int j;
                for(j = 0; j < im->item_mesh.plane_count; j++)
                {
                    plane_t plane = {
                        {planes[j].position[0] + im->pos[0], planes[j].position[1] + im->pos[1], planes[j].position[2] + im->pos[2]},
                        {-planes[j].normal[0], -planes[j].normal[1], -planes[j].normal[2]}
                    };
                    nl_vector3_t point = {0.0, 0.0, 0.0};
                    float lamda = 0.0;
                    int r;
                    r = Math3D_LineToPlaneIntersect(&line, &plane, &point);
                    if(r && Math3D_PointInAABB(&point, &aabb))
                    {
                        nl_vector3_t normal;
                        r = Math3D_LineToPlaneCollision(&line, &plane, &lamda, &normal);
                        //if(res > 0 && lamda <= width)
                        if(c_normal)
                        {
                            c_normal->x = -normal.x;
                            c_normal->y = -normal.y;
                            c_normal->z = -normal.z;
                        }
                        if(index)
                            *index = k;
                        res = item_in_all_planes_and_in_aabb_type;
                        return res;
                    }
                }

                if(index)
                    *index = k;
                res = item_only_in_aabb_type;
                return res;
            }
            else
            {
                res = item_in_aabb_and_item_no_planes_type;
                return res;
            }
        }
    }

    if(index)
        *index = -1;
    res = item_out_aabb_type;
    return res;
}

int Algo_GetItemTopCoord(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_position, int scene, float width, float *rglz, int *index, float *height)
{
    if(!netlizard_3d_model || !new_position)
        return 0;
    const GL_NETLizard_3D_Mesh *mesh = netlizard_3d_model->meshes + scene;
    float final_z = 0.0;
    float lamda = 0.0;
    int available = 0;
    float h = 0.0;
    unsigned int a;
    for(a = mesh->item_index_range[0]; a < mesh->item_index_range[1]; a++)
    {
        const GL_NETLizard_3D_Item_Mesh *im = netlizard_3d_model->item_meshes + a;
        if(im->item_type != Item_Elevator_Type && new_position->z < im->item_mesh.ortho[2] + im->pos[2])
            continue;
        const GL_NETLizard_3D_Plane *planes = im->item_mesh.plane;
        if(im->item_type & Item_DoorV_Type || im->item_type & Item_DoorH_Type || im->item_type & Item_Weapon_Type || im->item_type & Item_Tiny_Type || im->item_type & Item_Box_Type)
            continue;
        bound_t aabb = {
            {im->item_mesh.ortho[3] - width + im->pos[0], im->item_mesh.ortho[4] - width + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + width + im->pos[0], im->item_mesh.ortho[1] + width + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        // �����λ���Ƿ��ڵ�ͼ��ĳһ��Ʒ������
        if(Math3D_PointInAABB2D(new_position, &aabb))
        {
            unsigned int j;
            for(j = 0; j < im->item_mesh.plane_count; j++)
            {
                if(planes[j].normal[2] > UP_NORMAL_LIMIT)
                {
                    plane_t plane = {
                        {planes[j].position[0] + im->pos[0], planes[j].position[1] + im->pos[1], planes[j].position[2] + im->pos[2]},
                        {planes[j].normal[0], planes[j].normal[1], planes[j].normal[2]}
                    };
                    nl_vector3_t point = {0.0, 0.0, 0.0};
                    nl_vector3_t dir = plane.normal;
                    dir.z = - dir.z;
                    ray_t l = {*new_position, dir};
                    int res = Math3D_LineToPlaneIntersect(&l, &plane, &point);
                    if(res && Math3D_PointInAABB(&point, &aabb))// && point.z <= new_position->z)
                    {
                        float d = 0.0;
                        nl_vector3_t n = {0.0, 0.0, 0.0};
                        res = Math3D_LineToPlaneCollision(&l, &plane, &d, &n);
                        if(available == 0)
                        {
                            final_z = point.z;
                            lamda = d;
                            if(*index)
                                *index = a;
                            h = aabb.max_position.z - aabb.min_position.z;
                            available = 1;
                        }
                        else
                        {
                            if(lamda > d)
                            {
                                final_z = point.z;
                                lamda = d;
                                if(*index)
                                    *index = a;
                                h = aabb.max_position.z - aabb.min_position.z;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    if(available)
    {
        if(rglz)
            *rglz = final_z + 1;
        if(height)
            *height = h;
        return 1;
    }
    else
        return 0;
}

int Algo_GetItemTopCoordForAll(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_position, float *rglz, int *index)
{
    if(!netlizard_3d_model || !new_position)
        return 0;
    float final_z = 0.0;
    float height = 0.0;
    float lamda = 0.0;
    int available = 0;
    int ii = -1;
    unsigned int a;
    for(a = 0; a < netlizard_3d_model->item_count; a++)
    {
        const GL_NETLizard_3D_Item_Mesh *im = netlizard_3d_model->item_meshes + a;
        const GL_NETLizard_3D_Plane *planes = im->item_mesh.plane;
        if(im->item_type & Item_DoorV_Type || im->item_type & Item_DoorH_Type || im->item_type & Item_Weapon_Type || im->item_type & Item_Tiny_Type || im->item_type & Item_Box_Type)
            continue;
        bound_t aabb = {
            {im->item_mesh.ortho[3] + im->pos[0], im->item_mesh.ortho[4] + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + im->pos[0], im->item_mesh.ortho[1] + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        nl_vector3_t *v1 = &(aabb.min_position);
        nl_vector3_t *v2 = &(aabb.max_position);
        // �����λ���Ƿ��ڵ�ͼ��ĳһ��Ʒ������
        if(Math3D_PointInAABB2D(new_position, &aabb))
        {
            unsigned int j;
            for(j = 0; j < im->item_mesh.plane_count; j++)
            {
                if(planes[j].normal[2] > UP_NORMAL_LIMIT)
                {
                    plane_t plane = {
                        {planes[j].position[0] + im->pos[0], planes[j].position[1] + im->pos[1], planes[j].position[2] + im->pos[2]},
                        {planes[j].normal[0], planes[j].normal[1], planes[j].normal[2]}
                    };
                    nl_vector3_t point = {0.0, 0.0, 0.0};
                    nl_vector3_t dir = plane.normal;
                    dir.z = - dir.z;
                    ray_t l = {*new_position, dir};
                    int res = Math3D_LineToPlaneIntersect(&l, &plane, &point);
                    if(res && Math3D_PointInAABB(&point, &aabb))
                    {
                        float d = 0.0;
                        nl_vector3_t n = {0.0, 0.0, 0.0};
                        res = Math3D_LineToPlaneCollision(&l, &plane, &d, &n);
                        if(available == 0)
                        {
                            final_z = point.z;
                            lamda = d;
                            height = v2->z - v1->z;
                            ii = a;
                        }
                        if(lamda > d)
                        {
                            final_z = point.z;
                            lamda = d;
                            height = v2->z - v1->z;
                            ii = a;
                        }
                    }
                }
            }
            break;
        }
    }
    if(index)
        *index = ii;
    if(available)
    {
        if(rglz)
            *rglz = final_z + 1;// + 1;
        return 1;
    }
    else
        return 0;
}

int Algo_ComputeTopFloorCoord(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *n_xyz, int *scene, float *rglz)
{
    if(!netlizard_3d_model || !scene || !n_xyz)
        return 0;
    float nglx = n_xyz->x;
    float ngly = n_xyz->y;
    float nglz = n_xyz->z;
    const GL_NETLizard_3D_Mesh *mesh = netlizard_3d_model->meshes + *scene;

    // �����޵ذ�ĳ���������еĳ�������ȡ�ó����·��ĳ�����ֱ����ȡ�еذ�ĳ������Ӷ�������ذ�����
    // ��ȡ�ó���������Сz����
    float nrglz = nglz;
    const float y = nglz;
    float delta = - FLT_MAX;
    nglz = mesh->ortho[2];
    int res = 0;
    int s = *scene;
    int i;
    for(i = 0; i < (int)netlizard_3d_model->count; i++)
    {
        if(i == *scene)
            continue;
        const GL_NETLizard_3D_Mesh *next_mesh = netlizard_3d_model->meshes + i;
        nl_vector3_t new_position = {nglx, ngly, nglz};
        bound_t aabb = {
            {next_mesh->ortho[3], next_mesh->ortho[4], next_mesh->ortho[5]},
            {next_mesh->ortho[0], next_mesh->ortho[1], next_mesh->ortho[2]}
        };
        if(!Math3D_PointInAABB2D(&new_position, &aabb))
            continue;

        unsigned int k;
        for(k = 0; k < next_mesh->plane_count; k++)
        {
            // ���㵱ǰλ�õĵذ�����
            if(next_mesh->plane[k].normal[2] > UP_NORMAL_LIMIT)
            {
                plane_t pla = {
                    {next_mesh->plane[k].position[0], next_mesh->plane[k].position[1], next_mesh->plane[k].position[2]},
                    {next_mesh->plane[k].normal[0], next_mesh->plane[k].normal[1], next_mesh->plane[k].normal[2]}
                };
                nl_vector3_t point = {0.0, 0.0, 0.0};
                nl_vector3_t new_pos = {nglx, ngly, nglz};
                nl_vector3_t dir = pla.normal;
                dir.z = - dir.z;
                ray_t l = {new_pos, dir};
                Math3D_LineToPlaneIntersect(&l, &pla, &point);
                {
                    float d = point.z - y;
                    if((d < 0 && d > delta))
                    {
                        nrglz = point.z;
                        s = i;
                        delta = d;
                        res = 1;
                    }
                }
            }
        }
        ngly = next_mesh->ortho[2];
    }
    if(nrglz)
    {
        if(scene)
            *scene = s;
        if(rglz)
            *rglz = nrglz;
    }

    return res;
}

/*
 * 1 - �ȶ���λ�ý������ͼ����������ײ����
 * �������λ�����ڳ������Ƿ��볡�����ӷ�����ײ
 * 1 - 2 �����������ڳ��������ڳ����е���Ʒ������ײ����
 * ���������ײ������λ����Ч
 * 1 - 2 - 3 ���û����ײ������㵱ǰ��λ���볡���ذ崹ֱ�ཻ�㣬�����ŵ�����
 * ��������ǰ�����޵ذ�ƽ�棬���ҳ��ó����·��ĳ��������ҵذ�ƽ�棬�����û�У����������Ѱ�ң�ֱ���ҵ��·����еذ�ƽ��ĳ��������¸ı䵱ǰ����
 * 1 - 2 - 3 - 4 ���³����е���λ���볡������Ʒ������ײ����
 * 1 - 2 - 3 - 4 - 5 �����Ƿ������ײ����Ҫ���㵱ǰ��λ���Ƿ��ڳ�����ĳһ��Ʒ�Ϸ����Ӷ�������µĵذ�����
 * 1 - 2 - 3 ����޷��ҵ���ֱ�����ϵ����г����ĵذ�ƽ�棬��ֱ�Ӽ����볡������Ʒ���Ϸ�ƽ���Ƿ�����ײ
 * ����У�����Ʒ����������Ϊ�ذ����꣨���Shadow of Egypt 3D��Ϸ level 10�и��ţ����ڳ���0�д�ֱ����û�еذ�ƽ�棬�����ڸ�����Ʒ���Ϸ����ߣ�
 *
 * 2 �����λ��û�����κγ����ڣ���ʹ�ڳ��������У���û�������ײ���ཻ����ֱ�Ӽ�����λ���Ƿ��ڳ�����Ʒ�ϣ��Ӷ��������Ʒ�Ķ���������Ϊ�ذ����꣨Shadow of Egypt 3D�� level 8 9 12 skyline����������ײ�棬ֱ�Ӽ��㸡�ŵĶ������꣩
 * 3 ����������£������λ��û�����κγ����ڣ���ʹ�ڳ��������У���û�������ײ���ཻ������λ��ֱ��ʹ�ã������ڴ�Խ�����ڵ�ǰ�ӽ������һ���㣬����������壬��Shadow of Egypt 3D�� level 5 8 9 10 12�߸���ʱ����ֹλ�ڸ��ż���ռ䴦����������
 */
int Algo_ComputePositionInNETLizard3DMap(const GL_NETLizard_3D_Model *map_model, const nl_vector3_t *ori_v, const nl_vector3_t *new_v, float cwidth, float cheight, nl_vector3_t *return_v, int *cs, int *ci)
{
    if(!map_model || !ori_v || !new_v || !return_v)
        return 0;
    int scene = -1;
    float rglz = 0.0;
    float h = 0.0;
    int currentindex = -1;
    int currentscene = -1;
    if(cs)
        currentscene = *cs;
    nl_vector3_t v1 = *ori_v;
    nl_vector3_t v2 = *new_v;
    nl_vector3_t v = v1;

    map_collision_testing_result_type res = Algo_NETLizard3DMapCollisionTesting(map_model, &v2, cwidth, cheight, &scene);
    if(res == map_out_aabb_type && Algo_NETLizard3DMapCollisionTesting(map_model, &v1, cwidth, cheight, NULL) == map_out_aabb_type)
    {
        float nz = 0.0;
        int ns = -1;
        if(Algo_GetTopScene(map_model, &v2, &ns, &nz))
        {
            v2.z = nz;
            res = Algo_NETLizard3DMapCollisionTesting(map_model, &v2, cwidth, cheight, &scene);
        }
        else
            return 0;
    }

    if(res == map_in_all_planes_and_in_aabb_type)
    {
        currentscene = scene;
        nl_vector3_t normal = {0.0, 0.0, 0.0};
        if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &v2, cwidth, cheight - 1, currentscene, &normal, &currentindex))
        {
            if(Algo_ComputeFloorCoord(map_model, &v2, &scene, &rglz))
            {
                nl_vector3_t vt = v2;
                //vt.z = v1.z;//2017 06 27 commit // 2017 6 20 replace
                v1.z = rglz;
                if(/* 1 || */currentscene != scene)
                {
                    currentscene = scene;
                    currentindex = -1;
                    if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &v2, cwidth, -1, currentscene, &normal, &currentindex))
                    {
                        vt.z = map_model->meshes[currentscene].ortho[2];
                        // vt.y = v1.y; 1017 6 20 replace
                    }
                    else
                    {
                    }
                }
                if(Algo_GetItemTopCoord(map_model, &vt, scene, cwidth, &rglz, &currentindex, &h))
                {
                    v1.z = rglz;
                }
                v.x = v2.x;
                v.y = v2.y;
                v.z = v1.z + cheight;
            }
            else
            {
                if(Algo_GetItemTopCoord(map_model, &v2, currentscene, cwidth, &rglz, &currentindex, &h))
                {
                    v.x = v2.x;
                    v.y = v2.y;
                    v.z = rglz;
                    v.z = v.z + cheight;
                }
                else
                {
                    v.x = new_v->x; //xp
                    v.y = new_v->y; //yp
                    v.z = new_v->z; //zp
                }
            }
        }
        else
        {
            //else printf("%f %f %f\n", normal[0], normal[1], normal[2]);
        }
    }
    else if(res == map_in_aabb_and_scene_no_planes_type)
    {
        currentscene = scene;
        if(Algo_GetItemTopCoord(map_model, &v2, currentscene, cwidth, &rglz, &currentindex, &h))
        {
            v.x = v2.x;
            v.y = v2.y;
            v.z = rglz;
            v.z = v.z + cheight;
        }
        else
        {
            /*
                 v.x = new_v->x; //xp
                 v.y = new_v->y; //zp
                 const GL_NETLizard_3D_Mesh *mesh = map_model->meshes + currentscene;
                 v.z = mesh->ortho[5] + cheight;
                 */
        }
    }
    else if(res == map_only_in_aabb_type)
    {
        return 0;
    }
    else if(res == map_in_all_planes_and_in_aabb_but_less_than_collision_distance_type)
    {
        int s = -1;
        nl_vector3_t rv = {0.0, 0.0, 0.0};
        int nr = Algo_GetPositionCoordIfCollatedMapWall(map_model, cwidth, &v1, &v2, currentscene, &rv, &s);
        if(nr)
        {
            currentscene = s;
            nl_vector3_t normal = {0.0, 0.0, 0.0};
            if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &rv, cwidth, cheight - 1, currentscene, &normal, &currentindex))
            {
                if(Algo_ComputeFloorCoord(map_model, &rv, &scene, &rglz))
                {
                    nl_vector3_t vt = rv;
                    //vt.z = v1.z;//2017 06 27 commit // 2017 6 20 replace
                    v1.z = rglz;
                    if(/* 1 || */currentscene != scene)
                    {
                        currentscene = scene;
                        currentindex = -1;
                        if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &rv, cwidth, -1, currentscene, &normal, &currentindex))
                        {
                            vt.z = map_model->meshes[currentscene].ortho[2];
                            // vt.y = v1.y; 1017 6 20 replace
                        }
                    }
                    if(Algo_GetItemTopCoord(map_model, &vt, scene, cwidth, &rglz, &currentindex, &h))
                    {
                        v1.z = rglz;
                        /*
                             if(item_bottom_normal_invert)
                             v1.z += h;
                             */
                    }
                    v.x = rv.x;
                    v.y = rv.y;
                    v.z = v1.z + cheight;
                }
                else
                {
                    if(Algo_GetItemTopCoord(map_model, &rv, currentscene, cwidth, &rglz, &currentindex, &h))
                    {
                        v.x = rv.x;
                        v.y = rv.y;
                        v.z = rglz;
                        /*
                             if(item_bottom_normal_invert)
                             v.z += h;
                             */
                        v.z = v.z + cheight;
                    }
                    else
                    {
                        v.x = new_v->x; //xp
                        v.y = new_v->y; //yp
                        v.z = new_v->z; //zp
                    }
                }
            }
            else
            {
                //else printf("%f %f %f\n", normal[0], normal[1], normal[2]);
            }
        }
        else if(currentscene != scene)
        {
            currentscene = scene;
            v.x = new_v->x; //xp
            v.y = new_v->y; //zp
            v.z = ori_v->z; //yo
        }
        else
        {
            nl_vector3_t normal = {0.0, 0.0, 0.0};
            if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &v1, cwidth, cheight - 1, currentscene, &normal, &currentindex))
            {
                if(Algo_ComputeFloorCoord(map_model, &v1, &scene, &rglz))
                {
                    v1.z = rglz;
                    gl_vector3_t vt = v1;
                    if(/* 1 || */currentscene != scene)
                    {
                        currentscene = scene;
                        currentindex = -1;
                        if(Algo_NETLizard3DItemCollisionTesting(map_model, &v1, &v1, cwidth, -1, currentscene, &normal, &currentindex))
                        {
                            vt.z = v1.z;
                        }
                    }
                    if(Algo_GetItemTopCoord(map_model, &vt, scene, cwidth, &rglz, &currentindex, &h))
                    {
                        v1.z = rglz;
                    }
                    v.x = v1.x;
                    v.y = v1.y;
                    v.z = v1.z + cheight;
                }
                else
                {
                    if(Algo_GetItemTopCoord(map_model, &v2, currentscene, cwidth, &rglz, &currentindex, &h))
                    {
                        v.x = v1.x;
                        v.y = v1.y;
                        v.z = rglz;
                        v.z = v.z + cheight;
                    }
                    else
                    {
                        v.x = new_v->x; //xp
                        v.z = new_v->z; //zp
                        v.y = new_v->y; //yp
                    }
                }
            }
            else
            {
                //else printf("%f %f %f\n", normal[0], normal[1], normal[2]);
            }
        }
    }
    else
        return 0;
#if 0
    else // res = 0
    {
        if(currentscene == -1)
        {
            if(Algo_GetItemTopCoordForAll(map_model, &v2, &rgly, &currentindex))
            {
                v.x = new_v->x; //xp
                v.z = new_v->z; //zp
                v.y = rgly;
                v.y = v.y - cheight;
            }
            else
            {
                v.x = new_v->x; //xp
                v.z = new_v->z; //zp
                v.y = new_v->y; //yp
            }
        }
    }
#endif
    return_v->x = v.x;
    return_v->y = v.y;
    return_v->z = v.z;
    if(cs)
        *cs = currentscene;
    if(ci)
        *ci = currentindex;
    return 1;
}

/*
 * ����1��ɼ����˶�
 * ����0������ײ�����ɼ����˶�����ײ�㷵��
 */
int Algo_ComputePositionInNETLizard3DMapNotGravity(const GL_NETLizard_3D_Model *map_model, const nl_vector3_t *ori_v, const nl_vector3_t *new_v, float cwidth, float cheight, nl_vector3_t *return_v, int *cs, int *ci)
{
    if(!map_model || !ori_v || !new_v || !return_v)
        return 1;
    int scene = -1;
    int scene_o = -1;
    int currentindex = -1;
    int currentscene = -1;
    if(cs)
        currentscene = *cs;
    nl_vector3_t v1 = *ori_v;
    nl_vector3_t v2 = *new_v;

    map_collision_testing_result_type res = Algo_NETLizard3DMapCollisionTesting(map_model, &v2, cwidth, cheight, &scene);
    map_collision_testing_result_type res_o = Algo_NETLizard3DMapCollisionTesting(map_model, &v1, cwidth, cheight, &scene_o);
    int res2 = res == map_in_all_planes_and_in_aabb_but_less_than_collision_distance_type || res == map_in_all_planes_and_in_aabb_type ? 1 : 0;
    int res2_o = res_o == map_in_all_planes_and_in_aabb_but_less_than_collision_distance_type || res_o == map_in_all_planes_and_in_aabb_type ? 1 : 0;
    if(res2 == 0 && res2_o == 0)
        return 1;

    GLint scenes[2] = {-1, -1};
    GLint i = 0;
    if(res2)
    {
        scenes[0] = scene;
        i++;
    }
    if(res2_o)
    {
        scenes[i] = scene_o;
    }
    if(scenes[0] == scenes[1])
        scenes[1] = -1;

    nl_vector3_t v = v1;
    nl_vector3_t dir = Vector3_SubtractVector3(&v2, &v1);
    Vector3_Normalize(&dir);
    if(res2 && res2_o == 0)
    {
        v = v2;
        Vector3_Inverse(&dir);
    }
    ray_t line = {v, dir};
    bound_t laabb = {
        {KARIN_MIN(v1.x, v2.x), KARIN_MIN(v1.y, v2.y), KARIN_MIN(v1.z, v2.z)},
        {KARIN_MAX(v1.x, v2.x), KARIN_MAX(v1.y, v2.y), KARIN_MAX(v1.z, v2.z)}
    };

    float distance = 0.0;
    nl_vector3_t point = {0.0, 0.0, 0.0};
    int r = 0;
    if(res2 == 0 || res2_o == 0)
    {
        for(i = 0; i < 2; i++)
        {
            if(scenes[i] == -1)
                continue;
            const GL_NETLizard_3D_Mesh *mesh = map_model->meshes + scenes[i];
            if(mesh->plane_count == 0)
                continue;
            bound_t aabb = {
                {mesh->ortho[3], mesh->ortho[4], mesh->ortho[5]},
                {mesh->ortho[0], mesh->ortho[1], mesh->ortho[2]}
            };
            GLuint j;
            for(j = 0; j < mesh->plane_count; j++)
            {
                plane_t pla = {
                    {mesh->plane[j].position[0], mesh->plane[j].position[1], mesh->plane[j].position[2]},
                    {mesh->plane[j].normal[0], mesh->plane[j].normal[1], mesh->plane[j].normal[2]}
                };
                float dis = 0.0;
                if(Math3D_LineToPlaneCollision(&line, &pla, &dis, NULL))
                {
                    nl_vector3_t p = {0.0, 0.0, 0.0};
                    Math3D_LineToPlaneIntersect(&line, &pla, &p);
                    if(!Math3D_PointInAABB(&p, &laabb))
                        continue;
                    if(!Math3D_PointInAABB(&p, &aabb))
                        continue;
                    if(r == 0)
                    {
                        distance = dis;
                        point = p;
                        currentscene = scenes[i];
                        r = 1;
                    }
                    else
                    {
                        if(distance < dis)
                        {
                            distance = dis;
                            point = p;
                            currentscene = scenes[i];
                        }
                    }
                }
            }
        }
    }
    // items
    if(res2 && res2_o)
    {
        for(i = 0; i < 2; i++)
        {
            if(scenes[i] == -1)
                continue;
            const GL_NETLizard_3D_Mesh *mesh = map_model->meshes + scenes[i];
            unsigned int k;
            for(k = mesh->item_index_range[0]; k < mesh->item_index_range[1]; k++)
            {
                const GL_NETLizard_3D_Item_Mesh *im = map_model->item_meshes + k;
                if(im->item_mesh.plane_count == 0)
                    continue;
                bound_t aabb = {
                    {im->item_mesh.ortho[3] - cwidth + im->pos[0], im->item_mesh.ortho[4] - cwidth + im->pos[1], im->item_mesh.ortho[5] + im->pos[2] - cheight},
                    {im->item_mesh.ortho[0] + cwidth + im->pos[0], im->item_mesh.ortho[1] + cwidth + im->pos[1], im->item_mesh.ortho[2] + im->pos[2] + cheight}
                };
                GLuint j;
                for(j = 0; j < im->item_mesh.plane_count; j++)
                {
                    plane_t pla = {
                        {im->item_mesh.plane[j].position[0] + im->pos[0], im->item_mesh.plane[j].position[1] + im->pos[1], im->item_mesh.plane[j].position[2] + im->pos[2]},
                        {im->item_mesh.plane[j].normal[0], im->item_mesh.plane[j].normal[1], im->item_mesh.plane[j].normal[2]}
                    };
                    float dis = 0.0;
                    if(Math3D_LineToPlaneCollision(&line, &pla, &dis, NULL))
                    {
                        nl_vector3_t p = {0.0, 0.0, 0.0};
                        Math3D_LineToPlaneIntersect(&line, &pla, &p);
                        if(!Math3D_PointInAABB(&p, &laabb))
                            continue;
                        if(!Math3D_PointInAABB(&p, &aabb))
                            continue;
                        if(r == 0)
                        {
                            distance = dis;
                            point = p;
                            currentindex = k;
                            r = 1;
                        }
                        else
                        {
                            if(distance < dis)
                            {
                                distance = dis;
                                point = p;
                                currentindex = k;
                            }
                        }
                    }
                }
            }
        }
    }

    if(r == 0)
        return 1;
    if(return_v)
        *return_v = point;
    if(cs)
        *cs = currentscene;
    if(ci)
        *ci = currentindex;
    return 0;
}

int Algo_PointIsCollisionInScene(const GL_NETLizard_3D_Model *model, const nl_vector3_t *pos, float cwidth, float cheight, int *scene, int *item)
{
    if(!model || !pos)
        return 0;
    int s = -1;
    int it = -1;
    nl_vector3_t fpos = {pos->x, pos->y, pos->z};
    map_collision_testing_result_type res = Algo_NETLizard3DMapCollisionTesting(model, &fpos, cwidth, cheight, &s);
    if(res == map_out_aabb_type || res == map_only_in_aabb_type)
        return 0;

    int ir = 0;
    const GL_NETLizard_3D_Mesh *mesh = model->meshes + s;
    unsigned int k;
    for(k = mesh->item_index_range[0]; k < mesh->item_index_range[1]; k++)
    {
        const GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + k;
        bound_t aabb = {
            {im->item_mesh.ortho[3] - cwidth + im->pos[0], im->item_mesh.ortho[4] - cwidth + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + cwidth + im->pos[0], im->item_mesh.ortho[1] + cwidth + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        if(Math3D_PointInAABB(pos, &aabb))
        {
            res = item_only_in_aabb_type;
            //printfi(im->item_type);
            it = k;
            if(im->item_type & Item_Weapon_Type || im->item_type & Item_Tiny_Type || im->item_type & Item_Box_Type)
            {
                it = k;
                continue;
            }
            ir = 1;
            // �����λ���Ƿ�����Ʒ��������ʵ������
            if(im->item_mesh.plane_count > 0)
            {
                const GL_NETLizard_3D_Plane *planes = im->item_mesh.plane;
                unsigned int j;
                for(j = 0; j < im->item_mesh.plane_count; j++)
                {
                    plane_t plane = {
                        {planes[j].position[0] + im->pos[0], planes[j].position[1] + im->pos[1], planes[j].position[2] + im->pos[2]},
                        {-planes[j].normal[0], -planes[j].normal[1], -planes[j].normal[2]}
                    };
                    ray_t line = {*pos, {planes[j].normal[0], planes[j].normal[1], planes[j].normal[2]}};
                    float lamda = 0.0;
                    nl_vector3_t point = {0.0, 0.0, 0.0};
                    int r;
                    r = Math3D_LineToPlaneIntersect(&line, &plane, &point);
                    if(r && Math3D_PointInAABB(&point, &aabb))
                    {
                        //r = Math3D_LineToPlaneCollision(&line, &plane, &lamda, NULL);
                        it = k;
                        break;
                    }
                }
            }
        }
    }
    if(scene)
        *scene = s;
    if(ir)
    {
        if(item)
            *item = it;
        return 0;
    }
    return 1;
}
#endif