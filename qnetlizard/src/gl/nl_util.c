#include "nl_util.h"

#include "bound.h"

int NETLizard_GetMapRenderScenes(const GL_NETLizard_3D_Model *model, int scenes[], float frustum[][4])
{
    if(!model || !scenes || !frustum)
        return -1;

    unsigned int i;
    unsigned int count = 0;
    for(i = 0; i < model->count; i++)
    {
        const GL_NETLizard_3D_Mesh *mesh = model->meshes + i;
        bound_s bound = BOUNDV(mesh->box.min, mesh->box.max);
        vector3_s pos = VECTOR3V(mesh->position);
        vector3_addv_self(&BOUND_MIN(bound), &pos);
        vector3_addv_self(&BOUND_MAX(bound), &pos);
        int r = bound_in_frustum(&bound, frustum);
        if(r)
        {
            scenes[count] = i;
            count++;
        }
    }
    return count;
}

void NETLizard_GetNETLizard3DMapBound(const GL_NETLizard_3D_Model *model, int *scenes, unsigned int count, bound_s *box)
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

void NETLizard_GetNETLizard3DMeshBound(const GL_NETLizard_3D_Mesh *meshs, unsigned int count, bound_s *box)
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
