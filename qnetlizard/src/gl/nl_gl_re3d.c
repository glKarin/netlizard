#include "nl_gl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "netlizard.h"
#include "netlizard_3d.h"
#include "lib/vector3.h"
#include "lib/bound.h"
#include "lib/triangle.h"
#include "nl_texture.h"
#include "nl_util.h"

static void NETLizard_TriangleStripToTriangles(GL_NETLizard_3D_Mesh *mesh)
{
    int count = 0;
    int i, j, n, c, s;
    GLushort index, index_1, index_2;

    for(i = 0; i < mesh->count; i++)
    {
        const GL_NETLizard_3D_Material *m = mesh->materials + i;
        count += (m->index_count - 2) * 3;
    }

    GL_NETLizard_3D_Vertex *vertexes = calloc(count, sizeof(GL_NETLizard_3D_Vertex));

    n = 0;
    c = 0;
    s = 0;
    for(i = 0; i < mesh->count; i++)
    {
        GL_NETLizard_3D_Material *m = mesh->materials + i;
        GLushort *indexes = calloc((m->index_count - 2) * 3, sizeof(GLushort));
        for(j = 2; j < m->index_count; j++)
        {
            // odd: n-1, n-2, n ÆæÊý
            // even: n-2, n-1, n Å¼Êý
            index = m->index[j];
            index_1 = m->index[j - 1];
            index_2 = m->index[j - 2];
            if(j % 2) // odd
            {
                memcpy(vertexes + n, mesh->vertex_data.vertex + (index_1), sizeof(GL_NETLizard_3D_Vertex));
                memcpy(vertexes + n + 1, mesh->vertex_data.vertex + (index_2), sizeof(GL_NETLizard_3D_Vertex));
                memcpy(vertexes + n + 2, mesh->vertex_data.vertex + (index), sizeof(GL_NETLizard_3D_Vertex));
            }
            else // even
            {
                memcpy(vertexes + n, mesh->vertex_data.vertex + (index_2), sizeof(GL_NETLizard_3D_Vertex));
                memcpy(vertexes + n + 1, mesh->vertex_data.vertex + (index_1), sizeof(GL_NETLizard_3D_Vertex));
                memcpy(vertexes + n + 2, mesh->vertex_data.vertex + (index), sizeof(GL_NETLizard_3D_Vertex));
            }
            indexes[c] = n;
            indexes[c + 1] = n + 1;
            indexes[c + 2] = n + 2;

            vector3_t v_normal;
            triangle_t tri = TRIANGLEV(vertexes[n].position, vertexes[n + 1].position, vertexes[n + 2].position);
            triangle_cale_normal(&tri, &v_normal);
            vertexes[n].normal[0] = VECTOR3_X(v_normal);
            vertexes[n].normal[1] = VECTOR3_Y(v_normal);
            vertexes[n].normal[2] = VECTOR3_Z(v_normal);
            vertexes[n + 1].normal[0] = VECTOR3_X(v_normal);
            vertexes[n + 1].normal[1] = VECTOR3_Y(v_normal);
            vertexes[n + 1].normal[2] = VECTOR3_Z(v_normal);
            vertexes[n + 2].normal[0] = VECTOR3_X(v_normal);
            vertexes[n + 2].normal[1] = VECTOR3_Y(v_normal);
            vertexes[n + 2].normal[2] = VECTOR3_Z(v_normal);

            n += 3;
            c += 3;
        }
        m->mode = GL_TRIANGLES;
        m->index_count = c;
        m->index_start = s;

        s += c;
        c = 0;
        free(m->index);
        m->index = indexes;
    }

    free(mesh->vertex_data.vertex);
    mesh->vertex_data.vertex = vertexes;
    mesh->vertex_data.vertex_count = count;
}

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
    GL_NETLizard_3D_Mesh *mesh = calloc(model->meshes.count, sizeof(GL_NETLizard_3D_Mesh));
    for(i = 0; i < model->meshes.count; i++)
	{
        GL_NETLizard_3D_Mesh *m = mesh + i;
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
//        m->vertex_data.index_count = mesh->index.count;
//        m->vertex_data.index = calloc(mesh->index.count, sizeof(GLushort));
//        for(j = 0; j < mesh->index.count; j++)
//        {
//            m->vertex_data.index[j] = (GLushort)(mesh->index.data[j]);
//        }

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
            primitive->index = calloc(s, sizeof(GLushort));
            int k;
            for(k = 0; k < s; k++)
                primitive->index[k] = mesh->index.data[l + k];
            l += s;
        }

        // position
        m->position[0] = mesh->translation[0];
        m->position[1] = mesh->translation[1];
        m->position[2] = mesh->translation[2];

        bound_t bound;

        NETLizard_GetNETLizard3DMeshBound(m, 1, &bound);
        m->box.min[0] = BOUND_MIN_X(bound)/* + mesh->translation[0]*/;
        m->box.min[1] = BOUND_MIN_Y(bound)/* + mesh->translation[1]*/;
        m->box.min[2] = BOUND_MIN_Z(bound)/* + mesh->translation[2]*/;
        m->box.max[0] = BOUND_MAX_X(bound)/* + mesh->translation[0]*/;
        m->box.max[1] = BOUND_MAX_Y(bound)/* + mesh->translation[1]*/;
        m->box.max[2] = BOUND_MAX_Z(bound)/* + mesh->translation[2]*/;

        NETLizard_TriangleStripToTriangles(m);
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
    glmodel->meshes = mesh;
    glmodel->count = mesh_count;
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
