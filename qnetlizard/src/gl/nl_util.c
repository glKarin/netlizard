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
