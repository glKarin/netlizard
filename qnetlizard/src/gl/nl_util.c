#include "nl_util.h"

#include "bound.h"

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int scenes[], float frustum[6][4])
{
    if(!model || !scenes || !frustum)
        return -1;

    unsigned int i;
    unsigned int count = 0;
    for(i = 0; i < model->count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = model->meshes + i;
        bound_t bound = BOUNDV(mesh->box.min, mesh->box.max);
        vector3_t pos = VECTOR3V(mesh->position);
        vector3_addve(&BOUND_MIN(bound), &pos);
        vector3_addve(&BOUND_MAX(bound), &pos);
        int r = bound_in_frustum(&bound, frustum);
        if(r)
        {
            scenes[count] = i;
            count++;
        }
    }
    return count;
}

void NETLizard_GetNETLizard3DMapBound(const GL_NETLizard_3D_Model *model, int scenes[], unsigned int count, bound_t *box)
{
    if(!model || !box)
        return;
    if(model->count == 0)
        return;

    float min_x = 0.0;
    float min_y = 0.0;
    float min_z = 0.0;
    float max_x = 0.0;
    float max_y = 0.0;
    float max_z = 0.0;
    unsigned int i;
    unsigned int c;
    GLboolean inited = GL_FALSE;

    c = scenes ? count : model->count;
    for(i = 0; i < c; i++)
    {
        int s = scenes ? scenes[i] : i;
        const GL_NETLizard_3D_Mesh *mesh = model->meshes + s;
        float box_min_x = mesh->box.min[0] + mesh->position[0];
        float box_min_y = mesh->box.min[1] + mesh->position[1];
        float box_min_z = mesh->box.min[2] + mesh->position[2];
        float box_max_x = mesh->box.max[0] + mesh->position[0];
        float box_max_y = mesh->box.max[1] + mesh->position[1];
        float box_max_z = mesh->box.max[2] + mesh->position[2];
        if(inited)
        {
            if(box_min_x < min_x)
                min_x = box_min_x;
            if(box_min_y < min_y)
                min_y = box_min_y;
            if(box_min_z < min_z)
                min_z = box_min_z;

            if(box_max_x > max_x)
                max_x = box_max_x;
            if(box_max_y > max_y)
                max_y = box_max_y;
            if(box_max_z > max_z)
                max_z = box_max_z;
        }
        else
        {
            min_x = mesh->box.min[0];
            min_y = mesh->box.min[1];
            min_z = mesh->box.min[2];
            max_x = mesh->box.max[0];
            max_y = mesh->box.max[1];
            max_z = mesh->box.max[2];
            inited = GL_TRUE;
            continue;
        }
    }

    BOUNDV_MIN_X(box) = min_x;
    BOUNDV_MIN_Y(box) = min_y;
    BOUNDV_MIN_Z(box) = min_z;
    BOUNDV_MAX_X(box) = max_x;
    BOUNDV_MAX_Y(box) = max_y;
    BOUNDV_MAX_Z(box) = max_z;
}

void NETLizard_GetNETLizard3DMeshBound(const GL_NETLizard_3D_Mesh *meshs, unsigned int count, bound_t *box)
{
    if(!meshs || !count)
        return;

    float min_x = 0.0;
    float min_y = 0.0;
    float min_z = 0.0;
    float max_x = 0.0;
    float max_y = 0.0;
    float max_z = 0.0;
    unsigned int i;
    unsigned int c;
    unsigned int k;
    GL_NETLizard_3D_Material *m;
    GL_NETLizard_3D_Vertex *v;
    GLushort index;
    GLboolean inited = GL_FALSE;

    for(i = 0; i < count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = meshs + i;
        for(c = 0; c < mesh->count; c++)
        {
            m = mesh->materials + c;
            for(k = 0; k < m->index_count; k++)
            {
                index = m->index[k];
                v = mesh->vertex_data.vertex + index;
                if(inited)
                {
                    if(v->position[0] < min_x)
                        min_x = v->position[0];
                    else if(v->position[0] > max_x)
                        max_x = v->position[0];

                    if(v->position[1] < min_y)
                        min_y = v->position[1];
                    else if(v->position[1] > max_y)
                        max_y = v->position[1];

                    if(v->position[2] < min_z)
                        min_z = v->position[2];
                    else if(v->position[2] > max_z)
                        max_z = v->position[2];
                }
                else
                {
                    min_x = v->position[0];
                    min_y = v->position[1];
                    min_z = v->position[2];
                    max_x = v->position[0];
                    max_y = v->position[1];
                    max_z = v->position[2];
                    inited = GL_TRUE;
                    continue;
                }
            }
        }
    }

    BOUNDV_MIN_X(box) = min_x;
    BOUNDV_MIN_Y(box) = min_y;
    BOUNDV_MIN_Z(box) = min_z;
    BOUNDV_MAX_X(box) = max_x;
    BOUNDV_MAX_Y(box) = max_y;
    BOUNDV_MAX_Z(box) = max_z;
}

void conv_nl_vector3(vector3_t *v)
{
    float z = VECTOR3V_Z(v);
    VECTOR3V_X(v) = VECTOR3V_X(v);
    VECTOR3V_Z(v) = VECTOR3V_Y(v);
    VECTOR3V_Y(v) = -z;
}

void conv_gl_vector3(nl_vector3_t *v)
{
    float y = VECTOR3V_Y(v);
    VECTOR3V_X(v) = VECTOR3V_X(v);
    VECTOR3V_Y(v) = VECTOR3V_Z(v)/* + 100*/;
    VECTOR3V_Z(v) = -y;
}

nl_vector3_t to_nl_vector3(const vector3_t *v)
{
    vector3_t t = *v;
    conv_nl_vector3(&t);
    return t;
}

vector3_t to_gl_vector3(const vector3_t *v)
{
    vector3_t t = *v;
    conv_gl_vector3(&t);
    return t;
}

void NETLizard_GetNETLizard3DMeshTransformBound(const GL_NETLizard_3D_Mesh *meshs, unsigned int count, bound_t *box)
{
    if(!meshs || !count)
        return;

    float min_x = 0.0;
    float min_y = 0.0;
    float min_z = 0.0;
    float max_x = 0.0;
    float max_y = 0.0;
    float max_z = 0.0;
    unsigned int i;
    unsigned int c;
    unsigned int k;
    GL_NETLizard_3D_Material *m;
    GL_NETLizard_3D_Vertex *v;
    GLushort index;
    GLboolean inited = GL_FALSE;

    for(i = 0; i < count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = meshs + i;
        GLmatrix mat;
        Mesa_AllocGLMatrix(&mat);
        Mesa_glTranslate(&mat, mesh->position[0], mesh->position[1], mesh->position[2]);
        Mesa_glRotate(&mat, mesh->rotation[0], 1.0f, 0.0f, 0.0f);
        Mesa_glRotate(&mat, mesh->rotation[1], 0.0f, 0.0f, 1.0f);
        for(c = 0; c < mesh->count; c++)
        {
            m = mesh->materials + c;
            for(k = 0; k < m->index_count; k++)
            {
                index = m->index[k];
                v = mesh->vertex_data.vertex + index;
                vector3_t pos = VECTOR3V(v->position);
                matrix_transformv_self(&mat, &pos);
                if(inited)
                {
                    if(VECTOR3_X(pos) < min_x)
                        min_x = VECTOR3_X(pos);
                    else if(VECTOR3_X(pos) > max_x)
                        max_x = VECTOR3_X(pos);

                    if(VECTOR3_Y(pos) < min_y)
                        min_y = VECTOR3_Y(pos);
                    else if(VECTOR3_Y(pos) > max_y)
                        max_y = VECTOR3_Y(pos);

                    if(VECTOR3_Z(pos) < min_z)
                        min_z = VECTOR3_Z(pos);
                    else if(VECTOR3_Z(pos) > max_z)
                        max_z = VECTOR3_Z(pos);
                }
                else
                {
                    min_x = VECTOR3_X(pos);
                    min_y = VECTOR3_Y(pos);
                    min_z = VECTOR3_Z(pos);
                    max_x = VECTOR3_X(pos);
                    max_y = VECTOR3_Y(pos);
                    max_z = VECTOR3_Z(pos);
                    inited = GL_TRUE;
                    continue;
                }
            }
        }
        Mesa_FreeGLMatrix(&mat);
    }

    BOUNDV_MIN_X(box) = min_x;
    BOUNDV_MIN_Y(box) = min_y;
    BOUNDV_MIN_Z(box) = min_z;
    BOUNDV_MAX_X(box) = max_x;
    BOUNDV_MAX_Y(box) = max_y;
    BOUNDV_MAX_Z(box) = max_z;
}

int NETLizard_GetNETLizard3DMapNeighboringScenes(const GL_NETLizard_3D_Model *model, int scene, int scenes[])
{
    unsigned int i;
    int index = 0;
    for(i = 0; i < model->bsp_count; i++)
    {
        const GL_NETLizard_BSP_Tree_Node *node = model->bsp_data + i;
        int s = -1;
        if(node->prev_scene == scene && node->next_scene >= 0)
            s = node->next_scene;
        if(node->next_scene == scene && node->prev_scene >= 0)
            s = node->prev_scene;
        if(s < 0 || s > model->count || s == scene)
            continue;
        int j;
        unsigned has = 0;
        for(j = 0; j < index; j++)
        {
            if(scenes[j] == s)
            {
                has = 1;
                break;
            }
        }
        if(has)
            continue;
        scenes[index++] = s;
    }
    return index;
}

void NETLizard_GetSceneBound(const GL_NETLizard_3D_Mesh *scene, bound_t *bound)
{
    if(!scene || !bound)
        return;
    nl_vector3_t min = VECTOR3V(scene->box.min);
    nl_vector3_t max = VECTOR3V(scene->box.max);
    if(scene->rotation[0] == 0 && scene->rotation[1] == 0) // no rotation
    {
        GLmatrix mat;
        Mesa_AllocGLMatrix(&mat);
        Mesa_glTranslate(&mat, scene->position[0], scene->position[1], scene->position[2]);
        Mesa_glRotate(&mat, scene->rotation[0], 1.0f, 0.0f, 0.0f);
        Mesa_glRotate(&mat, scene->rotation[1], 0.0f, 0.0f, 1.0f);
        matrix_transformv_self(&mat, &min);
        matrix_transformv_self(&mat, &max);
        Mesa_FreeGLMatrix(&mat);
        bound_make(bound, &min, &max);
    }
    else
    {
        NETLizard_GetNETLizard3DMeshTransformBound(scene, 1, bound);
    }
}

void NETLizard_GetSceneFullBound(const GL_NETLizard_3D_Model *model, GLuint s, bound_t *bound)
{
    if(!model || !bound)
        return;
    const GL_NETLizard_3D_Mesh *scene = model->meshes + s;
    NETLizard_GetSceneBound(scene, bound);
    unsigned int i;
    for(i = scene->item_index_range[0]; i < scene->item_index_range[1]; i++)
    {
        bound_t b;
        const GL_NETLizard_3D_Mesh *im = model->item_meshes + i;
        NETLizard_GetSceneBound(im, &b);
        bound_combine(bound, &b);
    }
}

void NETLizard_GetScenePlane(const GL_NETLizard_3D_Mesh *scene, int j, plane_t *p)
{
    const GL_NETLizard_3D_Plane *plane = scene->plane + j;
    nl_vector3_t normal = VECTOR3V(plane->normal);
    nl_vector3_t position = VECTOR3V(plane->position);
    GLmatrix mat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_glTranslate(&mat, scene->position[0], scene->position[1], scene->position[2]);
    Mesa_glRotate(&mat, scene->rotation[0], 1.0f, 0.0f, 0.0f);
    Mesa_glRotate(&mat, scene->rotation[1], 0.0f, 0.0f, 1.0f);
    matrix_transformv_self(&mat, &position);
    Mesa_NormalMatrix(&mat);
    //matrix_transformv_self_row(&mat, &normal); // !!! ???
    matrix_transformv_self(&mat, &normal);
    Mesa_FreeGLMatrix(&mat);
    plane_make(p, &position, &normal);
}
