#include "nl_gl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "netlizard.h"
#include "netlizard_3d.h"
#include "lib/vector3.h"
#include "nl_texture.h"

GLboolean NETLizard_MakeGLRE3DModel(const NETLizard_RE3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel)
{
//	array *meshes = model->meshes;
//	if(!meshes || !meshes->array)
//        return GL_FALSE;
    new_netlizard_texture_from_file new_OpenGL_texture_2d;
    const char *sky_file;
    NETLizard_Game game = model->game;

    const NETLizard_3D_Model_Config *config = nlGet3DGameModelConfig(game);
    switch(config->texture_type)
    {
        case NL_TEXTURE_3D_ENGINE_V2:
            new_OpenGL_texture_2d = new_texture_from_nl_v2_3d_file;
            break;
        case NL_TEXTURE_3D_ENGINE_V3:
            new_OpenGL_texture_2d = new_texture_from_nl_v3_3d_file;
            break;
        case NL_TEXTURE_ENCODE_PNG:
    default:
            new_OpenGL_texture_2d = new_texture_from_nl_file;
        break;
    }
    sky_file = config->sky_file;
    int i;

    const int mesh_count = model->meshes.count;
    GL_NETLizard_3D_Item_Mesh *mesh = calloc(model->meshes.count, sizeof(GL_NETLizard_3D_Item_Mesh));
    for(i = 0; i < model->meshes.count; i++)
	{
        GL_NETLizard_3D_Item_Mesh *im = mesh + i;
        GL_NETLizard_3D_Mesh *m = &im->item_mesh;
        const NETLizard_RE3D_Mesh *mesh = model->meshes.data + i;
        int j;

        m->count = mesh->primitive.count;

        // vertex
        m->vertex_data.vertex_count = mesh->vertex_count;
        m->vertex_data.vertex = calloc(mesh->vertex_count, sizeof(GL_NETLizard_3D_Vertex));
        for(j = 0; j < mesh->vertex_count; j++)
        {
            GL_NETLizard_3D_Vertex *vertex = m->vertex_data.vertex + j;
            vertex->position[0] = mesh->vertex.data[j * 3];
            vertex->position[1] = mesh->vertex.data[j * 3 + 1];
            vertex->position[2] = mesh->vertex.data[j * 3 + 2];
            vertex->texcoord[0] = mesh->texcoord.data[j * 2];
            vertex->texcoord[1] = mesh->texcoord.data[j * 2 + 1];
            // normal
        }

        // index
        m->vertex_data.index_count = mesh->index.count;
        m->vertex_data.index = calloc(mesh->index.count, sizeof(GLushort));
        for(j = 0; j < mesh->index.count; j++)
        {
            m->vertex_data.index[j] = (GLushort)(mesh->index.data[j]);
        }

        // primitive
        int l = 0;
        m->materials = calloc(mesh->primitive.count, sizeof(GL_NETLizard_3D_Material));
        for(j = 0; j < mesh->primitive.count; j++)
        {
            GL_NETLizard_3D_Material *primitive = m->materials + j;
            NLuint s = mesh->primitive.data[j];
            primitive->tex_index = mesh->tex_index;
            primitive->index_start = l;
            primitive->index_count = s;
            primitive->mode = GL_TRIANGLE_STRIP;
            l += s;
        }

        // position
        im->pos[0] = mesh->translation[0];
        im->pos[1] = mesh->translation[1];
        im->pos[2] = mesh->translation[2];


//		vector3_s min = {0.0, 0.0, 0.0};
//		vector3_s max = {0.0, 0.0, 0.0};
//		Algo_ComputeAABBFromOriginalPointSet(mesh->vertex, &min, &max);
//		//nlCastAABB(mesh->vertex, &min.x, &min.y, &min.z, &max.x, &max.y, &max.z);
//		m->ortho[0] = max.x + m->translations[0];
//		m->ortho[1] = max.y + m->translations[1];
//		m->ortho[2] = max.z + m->translations[2];
//		m->ortho[3] = min.x + m->translations[0];
//		m->ortho[4] = min.y + m->translations[1];
//		m->ortho[5] = min.z + m->translations[2];
	}

    const int texes_count = model->texes.count;
    texture_s **texes = calloc(model->texes.count, sizeof(texture_s *));
    for(i = 0; i < model->texes.count; i++)
	{
        const char *file = model->texes.data[i];
		if(!file)
			continue;
		char *name = NULL;
        if(resource_path)
		{
            int rplen = strlen(file) + strlen(resource_path) + 1 + 1;
            name = calloc(rplen, 1);
            memset(name, '\0', rplen);
            sprintf(name, "%s/%s", resource_path, file);
		}
		else
		{
			name = strdup(file);
		}
        texture_s *tex = malloc(sizeof(texture_s));
        memset(tex, 0, sizeof(texture_s));
        new_OpenGL_texture_2d(tex, name);
        texes[i] = tex;
        free(name);
	}

    texture_s sky;
    memset(&sky, 0, sizeof(sky));
    if(sky_file)
    {
        char *name = NULL;
        if(resource_path)
        {
            int rplen = strlen(sky_file) + strlen(resource_path) + 1 + 1;
            name = calloc(rplen, 1);
            memset(name, '\0', rplen);
            sprintf(name, "%s/%s", resource_path, sky_file);
        }
        else
            name = strdup(sky_file);
        new_texture_from_nl_file(&sky, name);
        free(name);
    }

    memset(glmodel, 0, sizeof(GL_NETLizard_3D_Model));
    glmodel->item_meshes = mesh;
    glmodel->item_count = mesh_count;
    glmodel->texes = texes;
    glmodel->tex_count = texes_count;
    if(sky.texid)
    {
        glmodel->bg_tex = malloc(sizeof(texture_s));
        memcpy(glmodel->bg_tex, &sky, sizeof(sky));
    }
    glmodel->game = game;

    return GL_TRUE;
}

GLboolean NETLizard_ReadGLRE3DMapModelFile(const char *name, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!name)
        return GL_FALSE;
    NETLizard_RE3D_Model m;
    if(!nlReadRE3DMeshFile(name, &m))
        return GL_FALSE;
    NETLizard_MakeGLRE3DModel(&m, resource_path, model);
    delete_NETLizard_RE3D_Model(&m);
    return GL_TRUE;
}

GLboolean NETLizard_ReadGLRE3DCarModelFile(const char *car_file, const char *tex_file, const char *resource_path, GL_NETLizard_3D_Model *model)
{
    if(!car_file || !tex_file)
        return GL_FALSE;
    NETLizard_RE3D_Model m;
    if(!nlReadRE3DMeshFile(car_file, &m))
        return GL_FALSE;

    int i;
    for(i = 0; i < m.texes.count; i++)
        free(m.texes.data[i]);
    free(m.texes.data);
    m.texes.count = 1;
    m.texes.data = calloc(1, sizeof(char *));
    m.texes.data[0] = strdup(tex_file);

    NETLizard_MakeGLRE3DModel(&m, resource_path, model);
    delete_NETLizard_RE3D_Model(&m);
    return GL_TRUE;


//    RE3D_Model *m = nlReadRE3DMeshFile(car_file);
//    if(!m)
//        return NULL;
//    GL_RE3D_Model *model = NETLizard_MakeGL2RE3DModel(m);
//    model->texes[0] = new_texture_from_nl_file(tex_file);
//    delete_RE3D_Model(m);
//    free(m);
//    return model;
}
