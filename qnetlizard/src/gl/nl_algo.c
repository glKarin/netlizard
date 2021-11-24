#include "nl_algo.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "lib/bound.h"
#include "lib/line.h"
#include "lib/euler.h"

#define SCT_Invalid -1
#define SCT_Outside NETLizard_Collision_Testing_Scene_Outside
#define SCT_Only_In_AABB NETLizard_Collision_Testing_Scene_Only_In_AABB
#define SCT_Pass NETLizard_Collision_Testing_Scene_Pass
#define SCT_Missing_Plane NETLizard_Collision_Testing_Scene_Missing_Plane
#define SCT_Movement NETLizard_Collision_Testing_Scene_Movement
#define SCT_Item NETLizard_Collision_Testing_Scene_Item

#define ICT_Invalid -1
#define ICT_Inside NETLizard_Collision_Testing_Item_Inside
#define ICT_Only_In_AABB NETLizard_Collision_Testing_Item_Only_In_AABB
#define ICT_Pass NETLizard_Collision_Testing_Item_Pass
#define ICT_Missing_Plane NETLizard_Collision_Testing_Item_Missing_Plane
#define ICT_Movement NETLizard_Collision_Testing_Item_Movement
#define ICT_Ignore NETLizard_Collision_Testing_Item_Ignore

//#define SCENE_BOUND(mesh) BOUNDV(mesh->box.min, mesh->box.max)
#define SCENE_BOUND(mesh) NETLizard_GetSceneBound(mesh)
//#define SCENE_PLANE(plane) PLANEV(mesh->plane[i])
#define SCENE_PLANE(mesh, i) NETLizard_GetScenePlane(mesh, i)

#define UP_NORMAL_LIMIT 0.866025
#define _MAX(a, b) ((a) > (b) ? (a) : (b))

#define IS_FLOOR(normal) ((normal)[2] > UP_NORMAL_LIMIT)
#define IS_CEIL(normal) ((normal)[2] < -UP_NORMAL_LIMIT)

#define PUSH_BACK(T, arr, count, t) \
{ \
    T *_narr = (T *)calloc(count + 1, sizeof(T)); \
    if(arr) { \
        memcpy(_narr, arr, count * sizeof(T)); \
        free(arr); \
    } \
    _narr[count] = t; \
    count++; \
    arr = _narr; \
}

static void * push_back(int size, void **arr, int *count, const void *t)
{
    void *_narr = calloc(*count + 1, size);
    if(*arr)
    {
        memcpy(_narr, *arr, *count * size);
        free(*arr);
    }
    memcpy((char *)_narr + *count * size, t, size);
    *count += 1;
    *arr = _narr;
    return _narr;
}

static bound_t NETLizard_GetSceneBound(const GL_NETLizard_3D_Mesh *scene)
{
    nl_vector3_t min = VECTOR3V(scene->box.min);
    nl_vector3_t max = VECTOR3V(scene->box.max);
    GLmatrix mat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_glTranslate(&mat, scene->position[0], scene->position[1], scene->position[2]);
    matrix_transformv_self(&mat, &min);
    matrix_transformv_self(&mat, &max);
    Mesa_FreeGLMatrix(&mat);
    bound_t bound;
    bound_make(&bound, &min, &max);
    return bound;
}

static plane_t NETLizard_GetScenePlane(const GL_NETLizard_3D_Mesh *scene, int j)
{
    const GL_NETLizard_3D_Plane *plane = scene->plane + j;
    nl_vector3_t normal = VECTOR3V(plane->normal);
    nl_vector3_t position = VECTOR3V(plane->position);
    GLmatrix mat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_glTranslate(&mat, scene->position[0], scene->position[1], scene->position[2]);
    matrix_transformv_self(&mat, &position);
    Mesa_InverseTransposeMatrix(&mat);
    matrix_transformv_self_row(&mat, &normal);
    Mesa_FreeGLMatrix(&mat);
    plane_t p;
    plane_make(&p, &position, &normal);
    return p;
}

static int NETLizard_IgnoreCollisionTestingItem(int item_type)
{
    if((item_type & NL_3D_ITEM_TYPE_WEAPON)
            || (item_type & NL_3D_ITEM_TYPE_THIN)
            || (item_type & NL_3D_ITEM_TYPE_SKY_BOX)
            || (item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL)
            || (item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL)
            )
        return 1;
    return 0;
}

/*
  点a是否在全图范围内
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
  点position是否在场景scene内
  */
int NETLizard_PointInScene(const GL_NETLizard_3D_Mesh *scene, const nl_vector3_t *position)
{
    if(!scene || !position)
        return 0;
    bound_t aabb = SCENE_BOUND(scene);
    return bound_point_in_box(&aabb, position);
}

static int NETLizard_PointInScenePlane(const GL_NETLizard_3D_Mesh *scene, const nl_vector3_t *position)
{
    if(!scene || !position)
        return 0;

    unsigned int j = 0;
    for(j = 0; j < scene->plane_count; j++)
    {
        plane_t plane = SCENE_PLANE(scene, j);
        int c = plane_point_clip(&plane, position);
        if(c < 0)
            return 0;
    }
    return 1;
}

/*
  获取点p所在的场景索引
  */
int NETLizard_FindScenePointIn(const GL_NETLizard_3D_Model *map_model, const nl_vector3_t *p)
{
    if(!map_model || !p)
        return -1;
    int scenes[4] = {-1, -1, -1, -1}; // 1 2 3 0
    unsigned int i;
    for(i = 0; i < map_model->count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = map_model->meshes + i;
        bound_t aabb = SCENE_BOUND(mesh);
        if(!bound_point_in_box(&aabb, p))
            continue;
        fprintf(stderr, "%d (%f %f %f) %f %f %f, %f %f %f | %d, %d\n",i, p->v[0], p->v[1], p->v[2], aabb.min.v[0], aabb.min.v[1], aabb.min.v[2], aabb.max.v[0], aabb.max.v[1], aabb.max.v[2],mesh->plane_type, mesh->plane_count);
        int index = mesh->plane_type == 0 ? 3 : mesh->plane_type - 1;
        if(scenes[index] == -1) // Only get first
        {
            scenes[index] = i;
            continue;
        }
        if(mesh->plane_type == 1 || mesh->plane_type == 2)
        {
            int ac = NETLizard_PointInScenePlane(map_model->meshes + scenes[index], p);
            int bc = NETLizard_PointInScenePlane(mesh, p);
            if(bc && !ac)
                scenes[index] = i;
        }
    }
    // 筛选场景: 可能有场景重叠, 优先选择有自带碰撞面的, 然后是通过顶点数据计算出碰撞面的, 然后是通过包围盒计算出碰撞面的
    // 1 > 2 > 3 > 0
    for(i = 0; i < 4; i++)
    {
        fprintf(stderr, "%d ", scenes[i]);
    }
    fprintf(stderr, "\n");
    for(i = 0; i < 4; i++)
    {
        if(scenes[i] != -1)
        {
            fprintf(stderr, "final %d\n", scenes[i]);
            return scenes[i];
        }
    }

    return -1;
}

/*
  获取new_pos下的场景索引scene, 返回场景盒子的最高值rglz. floor为真则该场景必须有地板
  */
static int NETLizard_GetTopSceneUnderPoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const nl_vector3_t *new_pos, unsigned floor, int *scene, float *rglz)
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

static int NETLizard_GetMeshFloorZCoordInScenePoint(const GL_NETLizard_3D_Mesh *mesh,  const nl_vector3_t *new_pos, unsigned under, float *rglz)
{
    if(!mesh || !new_pos)
        return 0;
    unsigned int j;
    for(j = 0; j < mesh->plane_count; j++)
    {
        // 平面是否向上
        if(!IS_FLOOR(mesh->plane[j].normal))
            continue;
        plane_t pla = SCENE_PLANE(mesh, j);

        // 平面点和New位置的Z值比较, 高于New位置的Z则丢弃
        if(under)
        {
            if(VECTOR3V_Z(new_pos) < PLANE_POSITION_Z(pla))
                continue;
        }

        nl_vector3_t point = VECTOR3(0, 0, 0);

#if(GET_FLOOR_INVERT_NORMAL == 1) // 1: only invert z coord
        nl_vector3_t dir = PLANE_NORMAL(pla);
        VECTOR3_Z(dir) = -VECTOR3_Z(dir);
#elif(GET_FLOOR_INVERT_NORMAL == 2) // 2: invert plane normal
        nl_vector3_t dir = PLANE_NORMAL(pla);
        vector3_invertv(&dir);
#else // 3: using fixed down normal
        nl_vector3_t dir = VECTOR3(0, 0, -1);
#endif

        ray_t l = {*new_pos, dir};
        if(plane_ray_intersect(&pla, &l, NULL, &point))
        {
            if(rglz)
                *rglz = VECTOR3_Z(point);
            return 1;
        }
    }
    return 0;
}

/*
  获取点new_pos下的场景索引scene的地板Z坐标rglz
  */
#define GET_FLOOR_INVERT_NORMAL 3 // 1 2 3
int NETLizard_GetSceneFloorZCoordInScenePoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const collision_object_t *obj, int scene, unsigned include_item, float *rglz)
{
    if(!netlizard_3d_model || !obj)
        return 0;
    const GL_NETLizard_3D_Mesh *mesh = netlizard_3d_model->meshes + scene;
    const nl_vector3_t *new_pos = &obj->position;

    unsigned has = 0;
    float zcoord;
    if(include_item)
    {
        unsigned int j;
        for(j = mesh->item_index_range[0]; j < mesh->item_index_range[1]; j++)
        {
            const GL_NETLizard_3D_Mesh *im = netlizard_3d_model->item_meshes + j;
            if(NETLizard_IgnoreCollisionTestingItem(im->item_type))
                continue;
            bound_t aabb = SCENE_BOUND(im);
            nl_vector3_t expand = VECTOR3(obj->radius, obj->radius, 0);
            bound_expand(&aabb, &expand);
            if(!bound_point_in_box2d(&aabb, new_pos))
                continue;
            float f;
            int r = NETLizard_GetMeshFloorZCoordInScenePoint(netlizard_3d_model->item_meshes + j, new_pos, 1, &f);
            if(r)
            {
                zcoord = (has++) ? _MAX(zcoord, f) : f;
            }
        }
    }

    float f;
    int r = NETLizard_GetMeshFloorZCoordInScenePoint(mesh, new_pos, 0, &f);
    if(r)
    {
        zcoord = (has++) ? _MAX(zcoord, f) : f;
    }
    if(has)
    {
        if(rglz)
            *rglz = zcoord;
    }
    return has ? 1 : 0;
}

/*
  获取点new_pos下的所有场景的地板Z坐标rglz, 返回场景索引scene
  */
int NETLizard_GetSceneFloorZCoordUnderPoint(const GL_NETLizard_3D_Model *netlizard_3d_model, const collision_object_t *obj, unsigned include_item, int *scene, float *rglz)
{
    if(!netlizard_3d_model || !obj)
        return 0;
    const nl_vector3_t *new_pos = &obj->position;
    int gress_scene;
    float gress_z;
    int res = NETLizard_GetTopSceneUnderPoint(netlizard_3d_model, new_pos, 1, &gress_scene, &gress_z);
    if(!res)
        return 0;
    //nl_vector3_t pos = VECTOR3(VECTOR3V_X(new_pos), VECTOR3V_Y(new_pos), gress_z);
    collision_object_t nco = {VECTOR3(VECTOR3V_X(new_pos), VECTOR3V_Y(new_pos), gress_z), obj->radius, obj->height};
    res = NETLizard_GetSceneFloorZCoordInScenePoint(netlizard_3d_model, &nco, gress_scene, include_item, &gress_z);
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
  获取点new_pos下的场景索引scene的地板Z坐标rglz, 如果没有地板, 则继续向下寻找, 如果一直没有地板, 则获取整个场景盒子的最小Z, 返回新的场景索引rscene
  */
int NETLizard_GetScenePointZCoord(const GL_NETLizard_3D_Model *netlizard_3d_model, const collision_object_t *obj, int scene, unsigned include_item, int *rscene, float *rglz)
{
    if(!netlizard_3d_model || !obj)
        return 0;
    int res = 0;

    if(scene >= 0)
    {
        res = NETLizard_GetSceneFloorZCoordInScenePoint(netlizard_3d_model, obj, scene, include_item, rglz);
        if(res)
        {
            if(rscene)
                *rscene = scene;
            return 1;
        }
    }
    res = NETLizard_GetSceneFloorZCoordUnderPoint(netlizard_3d_model, obj, include_item, rscene, rglz);
    if(res)
        return 1;
    bound_t bound;
    NETLizard_GetNETLizard3DMapBound(netlizard_3d_model, 0, 0, &bound);
    const nl_vector3_t *new_pos = &obj->position;
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

typedef struct collision_result_s
{
    int res;
    int scene;
    nl_vector3_t npos;
    int item;
} collision_result_t;

typedef struct collision_plane_s
{
    plane_t plane;
    nl_vector3_t point;
    float lamda;
} collision_plane_t;

static collision_result_t NETLizard_ItemCollisionTesting(const GL_NETLizard_3D_Model *map, int s, const collision_object_t *obj, const nl_vector3_t *new_pos)
{
    collision_result_t result = {-1, -1, *new_pos, -1};

    nl_vector3_t pos = obj->position;
    nl_vector3_t npos = *new_pos;
    const float height = obj->height;
    const float width = obj->radius;
    const float leg_height = obj->leg_height;
    const float head_height = obj->head_height;
    const float body_length = height - leg_height;
    VECTOR3_Z(pos) -= body_length;
    VECTOR3_Z(npos) -= body_length;
    const GL_NETLizard_3D_Mesh *mesh = map->item_meshes + s;

    if(NETLizard_IgnoreCollisionTestingItem(mesh->item_type))
    {
        result.res = ICT_Ignore;
        return result;
    }

    bound_t bound = SCENE_BOUND(mesh);
    nl_vector3_t expand = VECTOR3(width, width, 0);
    bound_expand(&bound, &expand);
    if(!bound_point_in_box(&bound, &npos))
    {
        result.res = ICT_Pass;
        return result;
    }

    // 检查场景是否有碰撞面数据
    if(!mesh->plane) // 无碰撞面数据
    {
        result.res = ICT_Missing_Plane;
        return result;
    }

    result.res = ICT_Only_In_AABB;
    line_t line;
    line_make(&line, &pos, &npos);
    nl_vector3_t line_dir;

    float line_len = line_length(&line);
    int line_zero = line_iszero(&line);
    line_direction(&line, &line_dir);
    //LINE_A_Z(line) += obj->height;
    //LINE_B_Z(line) += obj->height;

    unsigned int j = 0;
    while(j < mesh->plane_count)
    {
        unsigned is_floor = IS_FLOOR(mesh->plane[j].normal);
        unsigned is_ceil = IS_CEIL(mesh->plane[j].normal);
        plane_t plane = SCENE_PLANE(mesh, j);
        float limit = 0;
        if(is_floor)
            limit = 0;
        else if(is_ceil)
            limit = 0; //height;
        else
            limit = width;
        if(limit)
        {
            vector3_moveve(&PLANE_POSITION(plane), &PLANE_NORMAL(plane), limit);
        }

        //ray_t l = {*new_pos, VECTOR3(-mesh->plane[j].normal[0], -mesh->plane[j].normal[1], -mesh->plane[j].normal[2])};
        vector3_t cpoint = VECTOR3(0, 0, 0);
        float lamda = 0.0;
        int dir = 0;
        int mask = 0;
        //fprintf(stderr,">>>>> %f, %f\n", LINE_A_Y(line), LINE_B_Y(line));fflush(stderr);
        int r = plane_line_intersect(&plane, &line, &lamda, &cpoint, &dir, &mask);
        //fprintf(stderr,"\talgo %d - %d: %f | %f, %f, %f ======= %f %f %f  === %f\n", r, mask,lamda, mesh->plane[j].normal[0], mesh->plane[j].normal[1], mesh->plane[j].normal[2],cpoint.v[0],cpoint.v[1],cpoint.v[2], limit);fflush(stderr);
        if(r == 0) // 平行
        {
            if(mask == 0) // 两点都在背面
            {
                result.res = ICT_Inside;
                break;
            }
            j++;
            continue; // 两点都在正面
        }
        if(r < 0) // 不相交
        {
            if(mask == 0) // 两点都在背面
            {
                result.res = ICT_Inside;
                break;
            }
            j++;
            continue; // 两点都在正面
        }

        // 相交, 且Old->New穿过, 且Old != New
        if(mask == 1 && dir > 0)
        {
            //VECTOR3_Z(cpoint) -= obj->height;
            result.res = ICT_Movement;

            result.npos = cpoint;
            LINE_B(line) = cpoint;
            VECTOR3_Z(result.npos) += body_length;
            line_direction(&line, &line_dir);
            line_len = line_length(&line);
            line_zero = line_iszero(&line);
        }

        j++;
    }

    //fprintf(stderr,"------------------\n\n");fflush(stderr);
    if(result.res == ICT_Only_In_AABB)
    {
        result.res = ICT_Pass;
        result.npos = *new_pos;
    }
    return result;
}

static collision_result_t NETLizard_SceneItemCollisionTesting(const GL_NETLizard_3D_Model *map, int s, const collision_object_t *obj, const nl_vector3_t *new_pos)
{
    collision_result_t result = {-1, -1, *new_pos, -1};
    result.res = ICT_Pass;
    result.scene = s;
    collision_object_t nobj = *obj;
    const GL_NETLizard_3D_Mesh *mesh = map->meshes + s;
    unsigned int k;
    for(k = mesh->item_index_range[0]; k < mesh->item_index_range[1]; k++)
    {
        collision_result_t r = NETLizard_ItemCollisionTesting(map, k, &nobj, new_pos);
        //fprintf(stderr,"  NETLizard_SceneItemCollisionTesting : %d : %d| \n\n", k, r.res);fflush(stderr);
        if(r.res == ICT_Pass) // 没有与物体碰撞则继续检测下个物体
            continue;
        if(r.res == ICT_Only_In_AABB) // 实际没有与物体碰撞则继续检测下个物体
            continue;
        if(r.res == ICT_Ignore)
        {
            result.res = r.res;
            result.item = k;
            continue;
        }
        if(r.res == ICT_Missing_Plane)
        {
            result.res = r.res;
            result.item = k;
            continue;
        }
        if(r.res == ICT_Movement)
        {
            result.res = r.res;
            result.item = k;
            result.npos = r.npos;
            nobj.position = r.npos;
            continue;
        }
        if(r.res == ICT_Inside)
        {
            result.res = r.res;
            result.item = k;
            break;
        }
    }
    return result;
}

static collision_result_t NETLizard_SceneCollisionTesting_r(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, const nl_vector3_t *new_pos, unsigned include_item)
{
    collision_result_t result = {-1, -1, *new_pos, -1};

    nl_vector3_t pos = obj->position;
    nl_vector3_t npos = *new_pos;
    const float height = obj->height;
    const float width = obj->radius;
    const float leg_height = obj->leg_height;
    const float head_height = obj->head_height;
    const float body_length = height - leg_height;
    VECTOR3_Z(pos) -= body_length;
    VECTOR3_Z(npos) -= body_length;

    // 获取位置所在的场景
    int s = NETLizard_FindScenePointIn(map, &npos);
    if(s == -1) // New位置不在任何场景范围内
    {
        result.res = SCT_Outside;
        s = NETLizard_FindScenePointIn(map, &pos); // 尝试获取原位置的场景
        if(s == -1)
            return result;
    }

    result.scene = s;
    if(include_item) // 首先检测物体碰撞
    {
        collision_result_t ires = NETLizard_SceneItemCollisionTesting(map, s, obj, new_pos);
        if(ires.res == ICT_Inside)
        {
            result.item = ires.item;
            result.res = SCT_Item;
            return result;
        }
        if(ires.res == ICT_Ignore || ires.res == ICT_Missing_Plane)
        {
            result.item = ires.item;
        }
        if(ires.res == ICT_Movement)
        {
            result.item = ires.item;
            npos = ires.npos;
        }
    }

    // 检查场景是否有碰撞面数据
    const GL_NETLizard_3D_Mesh *mesh = map->meshes + s;
    if(!mesh->plane) // 无碰撞面数据
    {
        result.res = SCT_Missing_Plane;
        return result;
    }

    result.res = SCT_Only_In_AABB;
    line_t line;
    line_make(&line, &pos, &npos);
    nl_vector3_t line_dir;

    unsigned fail = 0;
    float line_len = line_length(&line);
    int line_zero = line_iszero(&line);
    line_direction(&line, &line_dir);

    unsigned int j = 0;
    while(j < mesh->plane_count)
    {
        unsigned is_floor = IS_FLOOR(mesh->plane[j].normal);
        unsigned is_ceil = IS_CEIL(mesh->plane[j].normal);
        plane_t plane = SCENE_PLANE(mesh, j);
        float limit = 0;
        if(is_floor)
            limit = 0;
        else if(is_ceil)
            limit = head_height + body_length; //height;
        else
            limit = width;
        if(limit)
        {
            vector3_moveve(&PLANE_POSITION(plane), &PLANE_NORMAL(plane), limit);
        }

        //ray_t l = {*new_pos, VECTOR3(-mesh->plane[j].normal[0], -mesh->plane[j].normal[1], -mesh->plane[j].normal[2])};
        vector3_t cpoint = VECTOR3(0, 0, 0);
        float lamda = 0.0;
        int dir = 0;
        int mask = 0;
        //fprintf(stderr,">>>>> %f, %f\n", LINE_A_Y(line), LINE_B_Y(line));fflush(stderr);
        int r = plane_line_intersect(&plane, &line, &lamda, &cpoint, &dir, &mask);
        //fprintf(stderr,"\talgo %d - %d: %f | %f, %f, %f ======= %f %f %f  === %f\n", r, mask,lamda, mesh->plane[j].normal[0], mesh->plane[j].normal[1], mesh->plane[j].normal[2],cpoint.v[0],cpoint.v[1],cpoint.v[2], limit);fflush(stderr);
        if(r == 0) // 平行
        {
            if(mask == 0) // 两点都在背面
            {
                result.res = SCT_Only_In_AABB;
                fail = 1;
                break;
            }
            j++;
            continue; // 两点都在正面
        }
        if(r < 0) // 不相交
        {
            if(mask == 0) // 两点都在背面
            {
                result.res = SCT_Only_In_AABB;
                fail = 1;
                break;
            }
            j++;
            continue; // 两点都在正面
        }

        // 相交, 且Old->New穿过, 且Old != New
        if(mask == 1 && dir > 0 && !line_zero)
        {
            result.res = SCT_Movement;

            if(!is_floor && !is_ceil) // if is a wall // 如果是墙壁, 则计算碰撞点延墙壁方向前进的点(模拟触墙滑动)
            {
                float l = line_len - lamda;
                //fprintf(stderr," res == 4 - %f\n", l);fflush(stderr);
                if(l < 0) l = 0;
                if(l > 0) // 如果New位置在平面背面, 则进行延墙滑动
                {
                    // 计算延墙壁滑动的目的点
                    nl_vector3_t nml = PLANE_NORMAL(plane);
                    vector3_invertv(&nml);
                    float dot = vector3_dot(&line_dir, &nml);
                    float rad = acos(dot);

                    nl_vector3_t up = VECTOR3(0.0, 0.0, 1.0);
                    nl_vector3_t pla;
                    vector3_crossv(&pla, &nml, &up);
                    vector3_normalizev(&pla);
                    float dot2 = vector3_dot(&line_dir, &pla);
                    float t = sin(rad) * l * (dot2 >= 0 ? 1 : -1);
                    vector3_scalev(&pla, t);

                    nl_vector3_t npoint = cpoint;
                    VECTOR3_X(npoint) += VECTOR3_X(pla);
                    VECTOR3_Y(npoint) += VECTOR3_Y(pla);

                    // 保证新的点在平面上方(内方, 不被裁剪掉)
                    while(plane_point_clip(&plane, &npoint) < 0)
                        vector3_addve(&npoint, &PLANE_NORMAL(plane)); // forbid back to plane

                    // 比较新的 线段(碰撞点 - 延墙壁New位置) 于 测试的线段$line进行比较, 如果不同, 则递归测试
                    line_t lb;
                    line_make(&lb, &cpoint, &npoint);
                    if(!line_equals(&line, &lb)) // forbid recursion // 使用碰撞点和新的延墙壁New位置进行递归的碰撞测试
                    {
                        collision_object_t nco = {VECTOR3(VECTOR3_X(cpoint), VECTOR3_Y(cpoint), VECTOR3_Z(cpoint) + body_length), width, height, leg_height, head_height};
                        //fprintf(stderr," rrrrrrr %f %f %f - %f %f %f\n", cpoint.v[0], cpoint.v[1], cpoint.v[2], npoint.v[0], npoint.v[1], npoint.v[2]);fflush(stderr);
                        nl_vector3_t nnpoint = VECTOR3(VECTOR3_X(npoint), VECTOR3_Y(npoint), VECTOR3_Z(npoint) + body_length);
                        collision_result_t ct = NETLizard_SceneCollisionTesting_r(map, &nco, &nnpoint, include_item);
                        //fprintf(stderr," new scene %d - %d\n", ct.res, ct.scene);fflush(stderr);

                        if(ct.res == SCT_Pass || ct.res == SCT_Movement) // 测试通过则把测试线段的B点改为`延墙壁计算的New位置`继续进行碰撞测试
                        { // TODO: scene发生变更
                            unsigned scene_change = result.scene != ct.scene;
                            result.npos = ct.npos;
                            result.scene = ct.scene;
                            VECTOR3_Z(ct.npos) -= body_length;
                            LINE_B(line) = ct.npos;
                            line_direction(&line, &line_dir);
                            line_len = line_length(&line);
                            line_zero = line_iszero(&line);
#if 0
                            if(scene_change)
                            {
                                fprintf(stderr," new scene %d - %d\n", s, ct.scene);fflush(stderr);
                                mesh = map->meshes + ct.scene;
                                j = 0;
                            }
#endif
                        }
                        else // 测试失败则丢弃延墙壁计算的New位置: 测试线段的B点改为`碰撞点`继续进行碰撞测试
                        {
                            result.npos = cpoint;
                            LINE_B(line) = cpoint;
                            VECTOR3_Z(result.npos) += body_length;
                            line_direction(&line, &line_dir);
                            line_len = line_length(&line);
                            line_zero = line_iszero(&line);
                        }
                    }
                    else // 如果两条线段相同则对该平面重新进行一次碰撞测试
                    {
                        //fprintf(stderr," same\n");fflush(stderr);
                        LINE_B(line) = npoint;
                        line_direction(&line, &line_dir);
                        line_len = line_length(&line);
                        line_zero = line_iszero(&line);
                        continue; // 不继续, 再测试一次当前平面
                    }
                }
                else // 如果New位置刚好是碰撞点 则不进行延墙滑动: 设置碰撞点为New位置, 继续进行碰撞测试
                {
                    result.npos = cpoint;
                    LINE_B(line) = cpoint;
                    VECTOR3_Z(result.npos) += body_length;
                    line_direction(&line, &line_dir);
                    line_len = line_length(&line);
                    line_zero = line_iszero(&line);
                }
            }
            else
            {
                if(is_floor)
                {
                    result.npos = cpoint;
                    LINE_B(line) = cpoint;
                    VECTOR3_Z(result.npos) += body_length;
                    line_direction(&line, &line_dir);
                    line_len = line_length(&line);
                    line_zero = line_iszero(&line);
                }
                else
                {
                    result.res = SCT_Only_In_AABB;
                    fail = 1;
                    break;
                }
            }
        }

        j++;
    }

    //fprintf(stderr,"------------------\n\n");fflush(stderr);
    if(result.res == SCT_Only_In_AABB && !fail)
    {
        result.res = SCT_Pass;
        result.npos = *new_pos;
    }
    return result;
}


// 0 - 在场景外 返回 -1
// 1 - 仅在场景AABB内
// 2 - 在场景AABB内，且在场景所有碰撞面内
// 3 - 仅在场景AABB内，但是该场景无碰撞面数据
// 4 - 在场景AABB内，且在场景所有碰撞面内，但是与某一碰撞面的距离小于测试值
int NETLizard_MapCollisionTesting(const GL_NETLizard_3D_Model *map, const collision_object_t *obj, nl_vector3_t *new_pos, int *scene, unsigned include_item, int *collision_item)
{
    if(!map || !obj)
        return -1;

    collision_result_t result = NETLizard_SceneCollisionTesting_r(map, obj, new_pos, include_item);
    if(result.res <= 0)
        return result.res;
    if(scene)
        *scene = result.scene;
    if(result.res == 4)
    {
        *new_pos = result.npos;
    }
    if(collision_item)
        *collision_item = result.item;

    return result.res;
}
