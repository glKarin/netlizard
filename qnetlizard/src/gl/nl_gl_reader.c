#include "nl_gl.h"
#include "netlizard.h"
 
#include <stdlib.h>

GLboolean NETLizard_ReadGLCT3DMapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
	if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DModelFile(name, i, resource_path, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLCT3DItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DItemModelFile(name, i, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLCT3DEp2MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
	if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DEp2ModelFile(name, i, resource_path, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLCT3DEp2ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DEp2ItemModelFile(name, i, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLSpecnaz3DMapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadSpecnaz3DModelFile(name, i, resource_path, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLSpecnaz3DItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadEgypt3DRoleModelFile(name, i, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLEgypt3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadEgypt3DRoleModelFile(name, index, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLEgypt3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadEgypt3DItemModelFile(name, index, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLEgypt3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadEgypt3DModelFile(name, resource_path, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLClone3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadClone3DRoleModelFile(name, index, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLClone3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadClone3DItemModelFile(name, index, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLClone3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadClone3DModelFile(name, resource_path, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLCT3DEp3MapModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DEp3ModelFile(name, i, resource_path, &m))
        return GL_FALSE;

//    if(model)
//	{
//		int i;
//		for(i = 0; i < model->item_count; i++)
//		{
//			if(model->item_meshes[i].item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL)
//				model->item_meshes[i].item_type |= NL_3D_ITEM_TYPE_THIN;
//			// TODO: now no item event support for this game map's door.
//		}
//	}

    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLCT3DEp3ItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadCT3DEp3ItemModelFile(name, i, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    nlDeleteNETLizard3DModel(&m);
    return GL_TRUE;
}
