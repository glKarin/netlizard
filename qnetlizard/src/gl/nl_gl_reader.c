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
    delete_NETLizard_3D_Model(&m);
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
    delete_NETLizard_3D_Model(&m);
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
    delete_NETLizard_3D_Model(&m);
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
    delete_NETLizard_3D_Model(&m);
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
    delete_NETLizard_3D_Model(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLSpecnaz3DItemModelFile(const char *name, int i, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadSpecnaz3DItemModelFile(name, i, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    delete_NETLizard_3D_Model(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLEgypt3DRoleModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Animation_Model *model)
{
//    if(!name)
//        return GL_FALSE;
//    NETLizard_3D_Model m;
//    if(!nlReadEgypt3DRoleModelFile(name, index, &m))
//        return GL_FALSE;
//    NETLizard_MakeGL3DAnimationModel(&m, resource_path, model);
//    delete_NETLizard_3D_Model(&m);
//    return GL_TRUE;
}

GLboolean NETLizard_ReadGLEgypt3DItemModelFile(const char *name, int index, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_3D_Model m;
    if(!nlReadEgypt3DItemModelFile(name, index, &m))
        return GL_FALSE;
    NETLizard_MakeGL3DModel(&m, resource_path, model);
    delete_NETLizard_3D_Model(&m);
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
    delete_NETLizard_3D_Model(&m);
    return GL_TRUE;
}


#if 0
GL_NETLizard_3D_Animation_Model * NETLizard_ReadGLClone3DRoleModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadClone3DRoleModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Animation_Model *model = NETLizard_MakeGL3DAnimationModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLClone3DItemModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadClone3DItemModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLClone3DMapModelFile(const char *name)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadClone3DModelFile(name);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLCT3DItemModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadCT3DItemModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLCT3DEp2ItemModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadCT3DEp2ItemModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLSpecnaz3DMapModelFile(const char *name, int level)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadSpecnaz3DModelFile(name, level);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLSpecnaz3DItemModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadSpecnaz3DItemModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLCT3DEp3MapModelFile(const char *name, int i)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadCT3DEp3ModelFile(name, i);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	if(model)
	{
		int i;
		for(i = 0; i < model->item_count; i++)
		{
			if(model->item_meshes[i].item_type == Item_DoorV_Type)
				model->item_meshes[i].item_type = Item_Tiny_Type;
			// TODO: now no item event support for this game map's door.
		}
	}
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}

GL_NETLizard_3D_Model * NETLizard_ReadGLCT3DEp3ItemModelFile(const char *name, int index)
{
	if(!name)
		return NULL;
	NETLizard_3D_Model *m = nlReadCT3DEp3ItemModelFile(name, index);
	if(!m)
		return NULL;
	GL_NETLizard_3D_Model *model = NETLizard_MakeGL3DModel(m);
	delete_NETLizard_3D_Model(m);
	free(m);
	return model;
}
#endif
