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

    if(scenes)
        c = count;
    else
        c = model->count;
    for(i = 0; i < c; i++)
    {
        int s;
        if(scenes)
            s = scenes[i];
        else
            s = i;
        const GL_NETLizard_3D_Mesh *mesh = model->meshes + s;
        if(i == 0)
        {
            min_x = mesh->box.min[0];
            min_y = mesh->box.min[1];
            min_z = mesh->box.min[2];
            max_x = mesh->box.max[0];
            max_y = mesh->box.max[1];
            max_z = mesh->box.max[2];
            continue;
        }
        if(mesh->box.min[0] < min_x)
            min_x = mesh->box.min[0];
        if(mesh->box.min[1] < min_y)
            min_y = mesh->box.min[1];
        if(mesh->box.min[2] < min_z)
            min_z = mesh->box.min[2];

        if(mesh->box.max[0] > max_x)
            max_x = mesh->box.max[0];
        if(mesh->box.max[1] > max_y)
            max_y = mesh->box.max[1];
        if(mesh->box.max[2] > max_z)
            max_z = mesh->box.max[2];
    }

    BOUNDV_MIN_X(box) = min_x;
    BOUNDV_MIN_Y(box) = min_y;
    BOUNDV_MIN_Z(box) = min_z;
    BOUNDV_MAX_X(box) = max_x;
    BOUNDV_MAX_Y(box) = max_y;
    BOUNDV_MAX_Z(box) = max_z;
}
