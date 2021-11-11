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
        int index = mesh->plane_type == 0 ? 3 : mesh->plane_type - 1;
        if(scenes[index] == -1) // Only get first
            scenes[index] = i;
    }
    // 筛选场景: 可能有场景重叠, 优先选择有自带碰撞面的, 然后是通过顶点数据计算出碰撞面的, 然后是通过包围盒计算出碰撞面的
    // 1 > 2 > 3 > 0
    for(i = 0; i < 4; i++)
    {
        if(scenes[i] != -1)
            return scenes[i];
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

    const vector3_t *pos = &obj->position;
    const float height = obj->height;
    const float width = obj->radius;
    const GL_NETLizard_3D_Mesh *mesh = map->item_meshes + s;

    if(NETLizard_IgnoreCollisionTestingItem(mesh->item_type))
    {
        result.res = ICT_Ignore;
        return result;
    }

    bound_t bound = SCENE_BOUND(mesh);
    nl_vector3_t expand = VECTOR3(width, width, 0);
    bound_expand(&bound, &expand);
    if(!bound_point_in_box(&bound, new_pos))
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
    line_make(&line, &obj->position, new_pos);
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
            line_direction(&line, &line_dir);
            line_len = line_length(&line);
            line_zero = line_iszero(&line);
        }

        j++;
    }

    //fprintf(stderr,"------------------\n\n");fflush(stderr);
    if(result.res == ICT_Only_In_AABB)
        result.res = ICT_Pass;
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

    const vector3_t *pos = &obj->position;
    const float height = obj->height;
    const float width = obj->radius;
    nl_vector3_t npos = *new_pos;

    // 获取位置所在的场景
    int s = NETLizard_FindScenePointIn(map, new_pos);
    if(s == -1) // New位置不在任何场景范围内
    {
        result.res = SCT_Outside;
        s = NETLizard_FindScenePointIn(map, pos); // 尝试获取原位置的场景
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
    line_make(&line, &obj->position, &npos);
    nl_vector3_t line_dir;

    unsigned fail = 0;
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
            //VECTOR3_Z(cpoint) -= obj->height;
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
                        collision_object_t nco = {cpoint, width, height};
                        //fprintf(stderr," rrrrrrr %f %f %f - %f %f %f\n", cpoint.v[0], cpoint.v[1], cpoint.v[2], npoint.v[0], npoint.v[1], npoint.v[2]);fflush(stderr);
                        collision_result_t ct = NETLizard_SceneCollisionTesting_r(map, &nco, &npoint, include_item);
                        //fprintf(stderr," new scene %d - %d\n", ct.res, ct.scene);fflush(stderr);

                        if(ct.res == SCT_Pass || ct.res == SCT_Movement) // 测试通过则把测试线段的B点改为`延墙壁计算的New位置`继续进行碰撞测试
                        { // TODO: scene发生变更
                            unsigned scene_change = result.scene != ct.scene;
                            result.npos = ct.npos;
                            result.scene = ct.scene;
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
                    line_direction(&line, &line_dir);
                    line_len = line_length(&line);
                    line_zero = line_iszero(&line);
                }
            }
            else
            {
                result.res = SCT_Only_In_AABB;
                fail = 1;
                break;
            }
        }

        j++;
    }

    //fprintf(stderr,"------------------\n\n");fflush(stderr);
    if(result.res == SCT_Only_In_AABB && !fail)
        result.res = SCT_Pass;
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

#if 0
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
        if(im->item_type != NL_3D_ITEM_TYPE_ELEVATOR && new_position->z < im->item_mesh.ortho[2] + im->pos[2])
            continue;
        const GL_NETLizard_3D_Plane *planes = im->item_mesh.plane;
        if(im->item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL || im->item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL || im->item_type & NL_3D_ITEM_TYPE_WEAPON || im->item_type & NL_3D_ITEM_TYPE_THIN || im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
            continue;
        bound_t aabb = {
            {im->item_mesh.ortho[3] - width + im->pos[0], im->item_mesh.ortho[4] - width + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + width + im->pos[0], im->item_mesh.ortho[1] + width + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        // 检测新位置是否在地图中某一物品盒子内
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
        if(im->item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL || im->item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL || im->item_type & NL_3D_ITEM_TYPE_WEAPON || im->item_type & NL_3D_ITEM_TYPE_THIN || im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
            continue;
        bound_t aabb = {
            {im->item_mesh.ortho[3] + im->pos[0], im->item_mesh.ortho[4] + im->pos[1], im->item_mesh.ortho[5] + im->pos[2]},
            {im->item_mesh.ortho[0] + im->pos[0], im->item_mesh.ortho[1] + im->pos[1], im->item_mesh.ortho[2] + im->pos[2]}
        };
        nl_vector3_t *v1 = &(aabb.min_position);
        nl_vector3_t *v2 = &(aabb.max_position);
        // 检测新位置是否在地图中某一物品盒子内
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

    // 对于无地板的场景，如空中的场景，获取该场景下方的场景，直到获取有地板的场景，从而计算出地板坐标
    // 获取该场景盒子最小z坐标
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
            // 计算当前位置的地板坐标
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
 * 1 - 先对新位置进行与地图各场景的碰撞测试
 * 计算出新位置所在场景和是否与场景盒子发生碰撞
 * 1 - 2 如果计算出所在场景，便于场景中的物品进行碰撞测试
 * 如果产生碰撞，则新位置无效
 * 1 - 2 - 3 如果没有碰撞，则计算当前新位置与场景地板垂直相交点，计算着地坐标
 * 其间如果当前场景无地板平面，则找出该场景下方的场景，查找地板平面，如果还没有，则继续向下寻找，直到找到下方带有地板平面的场景，重新改变当前场景
 * 1 - 2 - 3 - 4 在新场景中的新位置与场景中物品进行碰撞测试
 * 1 - 2 - 3 - 4 - 5 无论是否产生碰撞，都要计算当前新位置是否在场景中某一物品上方，从而计算出新的地板坐标
 * 1 - 2 - 3 如果无法找到垂直方向上的所有场景的地板平面，则直接计算与场景中物品的上方平面是否有碰撞
 * 如果有，则物品顶面坐标作为地板坐标（针对Shadow of Egypt 3D游戏 level 10中浮桥，所在场景0中垂直方向没有地板平面，人物在浮桥物品的上方行走）
 *
 * 2 如果新位置没有在任何场景内（即使在场景盒子中，但没有与各碰撞面相交），直接计算新位置是否在场景物品上，从而计算出物品的顶面坐标作为地板坐标（Shadow of Egypt 3D中 level 8 9 12 skyline，场景无碰撞面，直接计算浮桥的顶面坐标）
 * 3 再其他情况下，如果新位置没有在任何场景内（即使在场景盒子中，但没有与各碰撞面相交），新位置直接使用，类似于穿越（由于当前视角相机是一个点，而不是面或体，在Shadow of Egypt 3D中 level 5 8 9 10 12走浮桥时，防止位于浮桥间隔空间处掉落的情况）
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
 * 返回1则可继续运动
 * 返回0则有碰撞，不可继续运动，碰撞点返回
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
            if(im->item_type & NL_3D_ITEM_TYPE_WEAPON || im->item_type & NL_3D_ITEM_TYPE_THIN || im->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
            {
                it = k;
                continue;
            }
            ir = 1;
            // 检测新位置是否在物品盒子中真实场景内
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
