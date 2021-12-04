#include "nl_gl.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "netlizard.h"
#include "netlizard_3d.h"
#include "lib/vector3.h"
#include "nl_texture.h"
#include "lib/triangle.h"
#include "lib/plane.h"
#include "lib/bound.h"

#define PRINT(fmt, args...) { fprintf(stderr, fmt, ##args); fprintf(stderr, "\n"); fflush(stderr); }

GLvoid delete_GL_NETLizard_3D_Material(GL_NETLizard_3D_Material *mat)
{
    if(!mat)
        return;
    if(mat->index)
        free(mat->index);
}

void delete_GL_NETLizard_3D_Mesh(GL_NETLizard_3D_Mesh *mesh)
{
    GLuint i;
	if(!mesh)
        return;

    if(mesh->vertex_data.vertex)
        free(mesh->vertex_data.vertex);
//    if(mesh->vertex_data.index)
//        free(mesh->vertex_data.index);

	if(mesh->plane)
		free(mesh->plane);
	if(mesh->bsp)
		free(mesh->bsp);
	if(mesh->tex_index)
		free(mesh->tex_index);

    if(mesh->materials)
    {
        for(i = 0; i < mesh->count; i++)
        {
            delete_GL_NETLizard_3D_Material(mesh->materials + i);
        }
        free(mesh->materials);
    }
}

//void delete_GL_NETLizard_3D_Item_Mesh(GL_NETLizard_3D_Item_Mesh *mesh)
//{
//    delete_GL_NETLizard_3D_Mesh(mesh);
//}

void delete_GL_NETLizard_3D_Model(GL_NETLizard_3D_Model *model)
{
	if(!model)
		return;
	if(model->meshes)
	{
		unsigned int i;
		for(i = 0; i < model->count; i++)
			delete_GL_NETLizard_3D_Mesh(model->meshes + i);
		free(model->meshes);
	}
	if(model->item_meshes)
	{
		unsigned int i;
		for(i = 0; i < model->item_count; i++)
            delete_GL_NETLizard_3D_Mesh(model->item_meshes + i);
		free(model->item_meshes);
	}
	if(model->texes)
	{
		unsigned int i;
		for(i = 0; i < model->tex_count; i++)
        {
            if(!model->texes[i])
                continue;
            if(glIsTexture(model->texes[i]->texid))
                glDeleteTextures(1, &model->texes[i]->texid);
            free(model->texes[i]);
		}
		free(model->texes);
	}
    if(model->bg_tex)
	{
        if(glIsTexture(model->bg_tex->texid))
            glDeleteTextures(1, &model->bg_tex->texid);
        free(model->bg_tex);
	}
	if(model->bsp_data)
	{
		free(model->bsp_data);
	}
}

GLboolean NETLizard_MakeGL3DModel(const NETLizard_3D_Model *model, const char *resource_path, GL_NETLizard_3D_Model *glmodel)
{
	if(!model)
        return GL_FALSE;

	new_netlizard_texture_from_file new_OpenGL_texture_2d;
	GLint tex_count; // 最大纹理数
    NETLizard_Game game = model->game;
	int invert_texcoord_y; // 纹理坐标y反转
	int item_invert_texcoord_y; // 纹理坐标y反转
	int index_factory; // 场景顶点索引因子
	int item_index_factory; // 物品顶点索引因子
    const char *subfix_str;
    const char *sky_file;

    const NETLizard_3D_Model_Config *config = nlGet3DGameModelConfig(game);
    tex_count = config->tex_count;
    subfix_str = config->tex_path_format;
    invert_texcoord_y = config->invert_texcoord_y;
    item_invert_texcoord_y = config->item_invert_texcoord_y;
    index_factory = config->index_factory;
    item_index_factory = config->item_index_factory;
    sky_file = config->sky_file;
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


    texture_s **texes = calloc(tex_count, sizeof(texture_s *));

	GL_NETLizard_3D_Mesh *meshes = NULL;
    if(model->data.data)
	{
        meshes = calloc(model->data.count, sizeof(GL_NETLizard_3D_Mesh));
		GLint i;
        for(i = 0; i < model->data.count; i++)
		{
            NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
			GL_NETLizard_3D_Mesh *m = meshes + i;
			m->count = 0;
			m->materials = NULL;
			m->tex_index = NULL;
			m->plane_count = 0;
            m->plane = NULL;
            m->vertex_data.vertex = NULL;
            m->vertex_data.vertex_count = 0;
//            m->vertex_data.index = NULL;
//            m->vertex_data.index_count = 0;
			m->bsp_count = 0;
			m->bsp = NULL;
            m->obj_index = -1;
            m->item_type = 0;
            m->position[0] = 0;
            m->position[1] = 0;
            m->position[2] = 0;
            m->rotation[0] = 0;
            m->rotation[1] = 0;

            if(mesh->primitive.data)
			{
				GLint o;
				GLuint tex_count_i = 0;
                GLint *tex_index_set = calloc(mesh->primitive.count, sizeof(GLint));
                for(o = 0; o < mesh->primitive.count; o++)
				{
					tex_index_set[o] = -1;
				}
                for(o = 0; o < mesh->primitive.count; o++)
				{
                    int tex_i = ((NETLizard_3D_Primitive *)(mesh->primitive.data))[o].tex_index;
					if(tex_i < 0)
						continue;
					GLuint n;
					for(n = 0; n < tex_count_i; n++)
					{
						if(tex_index_set[n] == -1)
							break;
						if(tex_index_set[n] == tex_i)
							break;
					}
					if(n == tex_count_i)
					{
						tex_index_set[tex_count_i] = tex_i;
						tex_count_i++;
					}
				}
				m->count = tex_count_i;
                m->tex_index = calloc(m->count, sizeof(GLint));
				for(o = 0; o < (GLint)m->count; o++)
				{
					m->tex_index[o] = tex_index_set[o];
				}
				free(tex_index_set);

                GLuint vertex_count = mesh->primitive.count * 3;
                GLuint index_count = mesh->primitive.count * 3;
                GL_NETLizard_3D_Vertex *vertex = calloc(vertex_count, sizeof(GL_NETLizard_3D_Vertex));
                GLushort *indexs = calloc(index_count, sizeof(GLushort));
                GL_NETLizard_3D_Material *materials = calloc(m->count, sizeof(GL_NETLizard_3D_Material));
                NLint *mesh_vertex = (NLint *)(mesh->vertex.data);
				GLint c = 0;
				for(o = 0; o < (GLint)m->count; o++)
				{
                    texture_s *tex = texes[m->tex_index[o]];
                    if(!tex && m->tex_index[o] < tex_count && m->tex_index[o] > 0)
					{
						char subfix[SUBFIX_LENGTH];
						memset(subfix, '\0', SUBFIX_LENGTH);
						sprintf(subfix, subfix_str, m->tex_index[o]);
						char *name = NULL;
                        if(resource_path)
						{
                            int rplen = strlen(subfix) + strlen(resource_path) + 1 + 1;
                            name = calloc(rplen, 1);
                            memset(name, '\0', rplen);
                            sprintf(name, "%s/%s", resource_path, subfix);
						}
						else
							name = strdup(subfix);
                        tex = malloc(sizeof(texture_s));
                        memset(tex, 0, sizeof(texture_s));
                        new_OpenGL_texture_2d(tex, name);
						free(name);
                        texes[m->tex_index[o]] = tex;
					}

                    NETLizard_3D_Primitive *p = (NETLizard_3D_Primitive *)(mesh->primitive.data);
					GLint a = c;
					GLint n;
                    for(n = 0; n < mesh->primitive.count; n++)
					{
						if(m->tex_index[o] == p[n].tex_index)
						{
							int i0 = p[n].index[0] / index_factory;
							int i1 = p[n].index[1] / index_factory;
							int i2 = p[n].index[2] / index_factory;

							vertex[a].position[0] = (GLfloat)mesh_vertex[i0 * 3];
							vertex[a].position[1] = (GLfloat)mesh_vertex[i0 * 3 + 1];
							vertex[a].position[2] = (GLfloat)mesh_vertex[i0 * 3 + 2];

							vertex[a + 1].position[0] = (GLfloat)mesh_vertex[i1 * 3];
							vertex[a + 1].position[1] = (GLfloat)mesh_vertex[i1 * 3 + 1];
							vertex[a + 1].position[2] = (GLfloat)mesh_vertex[i1 * 3 + 2];

							vertex[a + 2].position[0] = (GLfloat)mesh_vertex[i2 * 3];
							vertex[a + 2].position[1] = (GLfloat)mesh_vertex[i2 * 3 + 1];
                            vertex[a + 2].position[2] = (GLfloat)mesh_vertex[i2 * 3 + 2];
                            vector3_t v_normal;
                            triangle_t tri = TRIANGLEV(vertex[a].position, vertex[a + 1].position, vertex[a + 2].position);
                            triangle_cale_normal(&tri, &v_normal);
                            vertex[a].normal[0] = VECTOR3_X(v_normal);
                            vertex[a].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a].normal[2] = VECTOR3_Z(v_normal);
                            vertex[a + 1].normal[0] = VECTOR3_X(v_normal);
                            vertex[a + 1].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a + 1].normal[2] = VECTOR3_Z(v_normal);
                            vertex[a + 2].normal[0] = VECTOR3_X(v_normal);
                            vertex[a + 2].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a + 2].normal[2] = VECTOR3_Z(v_normal);

							GLfloat w = tex ? tex->width : 1.0;
							vertex[a].texcoord[0] = (GLfloat)p[n].texcoord[0] / w; 
							vertex[a].texcoord[1] = (GLfloat)p[n].texcoord[1] / w; 

							vertex[a + 1].texcoord[0] = (GLfloat)p[n].texcoord[2] / w; 
							vertex[a + 1].texcoord[1] = (GLfloat)p[n].texcoord[3] / w; 

							vertex[a + 2].texcoord[0] = (GLfloat)p[n].texcoord[4] / w; 
							vertex[a + 2].texcoord[1] = (GLfloat)p[n].texcoord[5] / w; 

							if(invert_texcoord_y && tex)
							{
								vertex[a].texcoord[1] = 1.0 - vertex[a].texcoord[1];
								vertex[a + 1].texcoord[1] = 1.0 - vertex[a + 1].texcoord[1];
								vertex[a + 2].texcoord[1] = 1.0 - vertex[a + 2].texcoord[1];
							}
							indexs[a] = a;
							indexs[a + 1] = a + 1;
							indexs[a + 2] = a + 2;
							a += 3;
						}
					}
					materials[o].tex_index = m->tex_index[o];
					materials[o].index_start = c;
					materials[o].index_count = a - c;
                    materials[o].mode = GL_TRIANGLES;
                    materials[o].index = calloc(a - c, sizeof(GLushort));
                    GLint q;
                    for(q = 0; q < materials[o].index_count; q++)
                        materials[o].index[q] = indexs[c + q];

					c = a;
				}

                m->materials = materials;
                m->vertex_data.vertex = vertex;
                m->vertex_data.vertex_count = vertex_count;
//                m->vertex_data.index_count = index_count;
//                m->vertex_data.index = indexs;
                free(indexs);
			}

            if(mesh->plane.data)
			{
                GLuint plane_count = mesh->plane.count;
                GL_NETLizard_3D_Plane *planes = calloc(mesh->plane.count, sizeof(GL_NETLizard_3D_Plane));
				GLint j;
                for(j = 0; j < mesh->plane.count; j++)
				{
					GL_NETLizard_3D_Plane *glplane = planes + j;
                    NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + j;
					vector3_t normal = VECTOR3V(plane->normal);
					glplane->position[0] = (GLfloat)plane->position[0];
					glplane->position[1] = (GLfloat)plane->position[1];
					glplane->position[2] = (GLfloat)plane->position[2];
                    vector3_normalizev(&normal);
                    glplane->normal[0] = (GLfloat)VECTOR3_X(normal);
                    glplane->normal[1] = (GLfloat)VECTOR3_Y(normal);
                    glplane->normal[2] = (GLfloat)VECTOR3_Z(normal);
				}
				m->plane_count = plane_count;
				m->plane = planes;
                m->plane_type = 1;
			}
            else if(mesh->primitive.data) // If not plane data, make planes with vertex data.
			{
				GLuint c = 0;
				GLuint j;
				for(j = 0; j < m->count; j++)
                    c += m->materials[j].index_count / 3;
				GLuint plane_count = c;
                GL_NETLizard_3D_Plane *planes = calloc(c, sizeof(GL_NETLizard_3D_Plane));
                GL_NETLizard_3D_Vertex *vertex = m->vertex_data.vertex;
				GLuint n = 0;
				for(j = 0; j < m->count; j++)
				{
					GLuint k;
                    for(k = 0; k < m->materials[j].index_count; k += 3)
					{
                        GLuint index = m->materials[j].index[k];
                        triangle_t tri = TRIANGLEV(vertex[index].position, vertex[index + 1].position, vertex[index + 2].position);
                        vector3_t center;
                        triangle_center_point(&tri, &center);
                        planes[n].position[0] = VECTOR3_X(center); // vertex[index].position[0];
                        planes[n].position[1] = VECTOR3_Y(center); // vertex[index].position[1];
                        planes[n].position[2] = VECTOR3_Z(center); // vertex[index].position[2];
                        planes[n].normal[0] = vertex[index].normal[0];
                        planes[n].normal[1] = vertex[index].normal[1];
                        planes[n].normal[2] = vertex[index].normal[2];
						n++;
					}
				}
				m->plane_count = plane_count;
				m->plane = planes;
                m->plane_type = 2;
            }
            else // If not plane data, make a floor plane with bound data.
			{
                GLuint plane_count = 1;
                plane_t ps[6];
				bound_t item_box = {
                    VECTOR3(
                        (GLfloat)mesh->box.min[0],
                        (GLfloat)mesh->box.min[1],
                        (GLfloat)mesh->box.min[2]
                    ),
                    VECTOR3(
                        (GLfloat)mesh->box.max[0],
                        (GLfloat)mesh->box.max[1],
                        (GLfloat)mesh->box.max[2]
                    )
				};
                GL_NETLizard_3D_Plane *planes = calloc(plane_count, sizeof(GL_NETLizard_3D_Plane));
                bound_get_box_plane(&item_box, ps);
				int o;
				int q = 0;
				for(o = 0; o < 6; o++)
				{
                    if(o != 0)
                        continue; // only floor plane
                    planes[q].position[0] = VECTOR3_X(ps[o].position);
                    planes[q].position[1] = VECTOR3_Y(ps[o].position);
                    planes[q].position[2] = VECTOR3_Z(ps[o].position);
                    planes[q].normal[0] = VECTOR3_X(ps[o].normal);
                    planes[q].normal[1] = VECTOR3_Y(ps[o].normal);
                    planes[q].normal[2] = VECTOR3_Z(ps[o].normal);
					q++;
				}
				m->plane_count = plane_count;
				m->plane = planes;
                m->plane_type = 3;
            }

            m->box.min[0] = (GLfloat)mesh->box.min[0];
            m->box.min[1] = (GLfloat)mesh->box.min[1];
            m->box.min[2] = (GLfloat)mesh->box.min[2];
            m->box.max[0] = (GLfloat)mesh->box.max[0];
            m->box.max[1] = (GLfloat)mesh->box.max[1];
            m->box.max[2] = (GLfloat)mesh->box.max[2];
			m->item_index_range[0] = (GLuint)mesh->item_index_range[0];
			m->item_index_range[1] = (GLuint)mesh->item_index_range[1];

            if(mesh->bsp.count)
			{
                m->bsp_count = mesh->bsp.count;
                m->bsp = calloc(mesh->bsp.count, sizeof(GLint));
				GLint k;
                for(k = 0; k < mesh->bsp.count; k++)
				{
                    m->bsp[k] = ((int *)(mesh->bsp.data))[k];
				}
			}
		}
	}

	GL_NETLizard_3D_Item_Mesh *item_meshes = NULL;
	GLuint item_data_length = 0;

    if(model->item_data.data)
	{
        item_meshes = calloc(model->item_data.count, sizeof(GL_NETLizard_3D_Item_Mesh));
        item_data_length = model->item_data.count;

		GLint i;
        for(i = 0; i < model->item_data.count; i++)
		{
            NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
			GL_NETLizard_3D_Item_Mesh *m = item_meshes + i;
            m->count = 0;
            m->materials = NULL;
            m->tex_index = NULL;
            m->vertex_data.vertex = NULL;
            m->vertex_data.vertex_count = 0;
//            m->vertex_data.index = NULL;
//            m->vertex_data.index_count = 0;
            m->plane_count = 0;
            m->plane = NULL;

            m->position[0] = (GLfloat)mesh->position[0];
            m->position[1] = (GLfloat)mesh->position[1];
            m->position[2] = (GLfloat)mesh->position[2];
            m->rotation[0] = (GLfloat)mesh->rotation[0];
            m->rotation[1] = (GLfloat)mesh->rotation[1];
			m->item_type = nlGetItemType(game, mesh->obj_index);
            m->obj_index = mesh->obj_index;
//            if(mesh->obj_index == 19)
//            {
//                fprintf(stderr, "item_type -> count: %d, %d\n", mesh->item_mesh.vertex.count,mesh->item_mesh.primitive.count); fflush(stderr);
//            }
            if(mesh->item_mesh.vertex.count && mesh->item_mesh.primitive.count)
			{
				GLint o;
				GLuint tex_count_i = 0;
                GLint *tex_index_set = calloc(mesh->item_mesh.primitive.count, sizeof(GLint));
                for(o = 0; o < mesh->item_mesh.primitive.count; o++)
                {
					tex_index_set[o] = -1;
				}
                for(o = 0; o < mesh->item_mesh.primitive.count; o++)
				{
                    int tex_i = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data))[o].tex_index;
					if(tex_i < 0)
                        continue;
                    GLuint n;
                    for(n = 0; n < tex_count_i; n++)
                    {
                        if(tex_index_set[n] == -1)
                            break;
                        if(tex_index_set[n] == tex_i)
                            break;
                    }
                    if(n == tex_count_i)
                    {
                        tex_index_set[tex_count_i] = tex_i;
                        tex_count_i++;
                    }
				}
                m->count = tex_count_i;
                m->tex_index = calloc(m->count, sizeof(GLint));
                for(o = 0; o < (GLint)m->count; o++)
				{
                    m->tex_index[o] = tex_index_set[o];
				}
				free(tex_index_set);

                GLuint vertex_count = mesh->item_mesh.primitive.count * 3;
                GLuint index_count = mesh->item_mesh.primitive.count * 3;
                GL_NETLizard_3D_Vertex *vertex = calloc(vertex_count, sizeof(GL_NETLizard_3D_Vertex));
                GLushort *indexs = calloc(index_count, sizeof(GLushort));
                GL_NETLizard_3D_Material *materials = calloc(m->count, sizeof(GL_NETLizard_3D_Material));
                GLuint plane_count = mesh->item_mesh.primitive.count;
                GL_NETLizard_3D_Plane *planes = calloc(plane_count, sizeof(GL_NETLizard_3D_Plane));
                NLint *mesh_vertex = (NLint *)(mesh->item_mesh.vertex.data);

				GLint b = 0;
				GLint c = 0;
                for(o = 0; o < (GLint)m->count; o++)
                {
                    texture_s *tex = texes[m->tex_index[o]];
                    if(!tex && m->tex_index[o] < tex_count && m->tex_index[o] > 0)
					{
						char subfix[SUBFIX_LENGTH];
						memset(subfix, '\0', SUBFIX_LENGTH);
                        sprintf(subfix, subfix_str, m->tex_index[o]);
						char *name = NULL;
                        if(resource_path)
                        {
                            int rplen = strlen(subfix) + strlen(resource_path) + 1 + 1;
                            name = calloc(rplen, 1);
                            memset(name, '\0', rplen);
                            sprintf(name, "%s/%s", resource_path, subfix);
						}
						else
							name = strdup(subfix);
                        tex = malloc(sizeof(texture_s));
                        memset(tex, 0, sizeof(texture_s));
                        new_OpenGL_texture_2d(tex, name);
						free(name);
                        texes[m->tex_index[o]] = tex;
                    }

                    NETLizard_3D_Primitive *p = (NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data);
					GLint a = c;
					GLint n;
                    for(n = 0; n < mesh->item_mesh.primitive.count; n++)
					{
                        if(m->tex_index[o] == p[n].tex_index)
						{
							int i0 = p[n].index[0] / item_index_factory;
							int i1 = p[n].index[1] / item_index_factory;
							int i2 = p[n].index[2] / item_index_factory;

							vertex[a].position[0] = (GLfloat)mesh_vertex[i0 * 3];
							vertex[a].position[1] = (GLfloat)mesh_vertex[i0 * 3 + 1];
							vertex[a].position[2] = (GLfloat)mesh_vertex[i0 * 3 + 2];
							vertex[a + 1].position[0] = (GLfloat)mesh_vertex[i1 * 3];
							vertex[a + 1].position[1] = (GLfloat)mesh_vertex[i1 * 3 + 1];
							vertex[a + 1].position[2] = (GLfloat)mesh_vertex[i1 * 3 + 2];
							vertex[a + 2].position[0] = (GLfloat)mesh_vertex[i2 * 3];
							vertex[a + 2].position[1] = (GLfloat)mesh_vertex[i2 * 3 + 1];
							vertex[a + 2].position[2] = (GLfloat)mesh_vertex[i2 * 3 + 2];
                            vector3_t v_normal;
                            triangle_t tri = TRIANGLEV(vertex[a].position, vertex[a + 1].position, vertex[a + 2].position);
                            triangle_cale_normal(&tri, &v_normal);
                            vertex[a].normal[0] = VECTOR3_X(v_normal);
                            vertex[a].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a].normal[2] = VECTOR3_Z(v_normal);
                            vertex[a + 1].normal[0] = VECTOR3_X(v_normal);
                            vertex[a + 1].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a + 1].normal[2] = VECTOR3_Z(v_normal);
                            vertex[a + 2].normal[0] = VECTOR3_X(v_normal);
                            vertex[a + 2].normal[1] = VECTOR3_Y(v_normal);
                            vertex[a + 2].normal[2] = VECTOR3_Z(v_normal);

							GLfloat w = tex ? tex->width : 1.0;
							vertex[a].texcoord[0] = (GLfloat)p[n].texcoord[0] / w; 
							vertex[a].texcoord[1] = (GLfloat)p[n].texcoord[1] / w; 
							vertex[a + 1].texcoord[0] = (GLfloat)p[n].texcoord[2] / w; 
							vertex[a + 1].texcoord[1] = (GLfloat)p[n].texcoord[3] / w; 
							vertex[a + 2].texcoord[0] = (GLfloat)p[n].texcoord[4] / w; 
							vertex[a + 2].texcoord[1] = (GLfloat)p[n].texcoord[5] / w; 

							if(item_invert_texcoord_y && tex)
							{
								vertex[a].texcoord[1] = 1.0 - vertex[a].texcoord[1];
								vertex[a + 1].texcoord[1] = 1.0 - vertex[a + 1].texcoord[1];
								vertex[a + 2].texcoord[1] = 1.0 - vertex[a + 2].texcoord[1];
							}

							indexs[a] = a;
							indexs[a + 1] = a + 1;
							indexs[a + 2] = a + 2;

                            vector3_t center;
                            triangle_center_point(&tri, &center);
                            planes[b].position[0] = VECTOR3_X(center); // vertex[a].position[0];
                            planes[b].position[1] = VECTOR3_Y(center); // vertex[a].position[1];
                            planes[b].position[2] = VECTOR3_Z(center); // vertex[a].position[2];
							planes[b].normal[0] = vertex[a].normal[0];
							planes[b].normal[1] = vertex[a].normal[1];
							planes[b].normal[2] = vertex[a].normal[2];
							b++;
							a += 3;
						}
					}
                    materials[o].tex_index = m->tex_index[o];
					materials[o].index_start = c;
					materials[o].index_count = a - c;
                    materials[o].mode = GL_TRIANGLES;
                    materials[o].index = calloc(a - c, sizeof(GLushort));
                    GLint q;
                    for(q = 0; q < materials[o].index_count; q++)
                        materials[o].index[q] = indexs[c + q];

					c = a;
				}

                m->materials = materials;
                m->vertex_data.vertex_count = vertex_count;
                m->vertex_data.vertex = vertex;
                m->plane_count = plane_count;
                m->plane = planes;
                m->plane_type = 2;
//                m->vertex_data.index_count = index_count;
//                m->vertex_data.index = indexs;
                free(indexs);
			}
            m->box.min[0] = (GLfloat)mesh->item_mesh.box.min[0];
            m->box.min[1] = (GLfloat)mesh->item_mesh.box.min[1];
            m->box.min[2] = (GLfloat)mesh->item_mesh.box.min[2];
            m->box.max[0] = (GLfloat)mesh->item_mesh.box.max[0];
            m->box.max[1] = (GLfloat)mesh->item_mesh.box.max[1];
            m->box.max[2] = (GLfloat)mesh->item_mesh.box.max[2];
		}
	}

	GL_NETLizard_BSP_Tree_Node *bsp_data = NULL;
	GLuint bsp_count = 0;
    if(model->bsp_data.count)
	{
        bsp_data = calloc(model->bsp_data.count, sizeof(GL_NETLizard_BSP_Tree_Node));
        NETLizard_BSP_Tree_Node *bsp = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
        bsp_count = model->bsp_data.count;
		GLint j;
        for(j = 0; j < model->bsp_data.count; j++)
		{
			GLint k;
			for(k = 0; k < 4; k++)
			{
				bsp_data[j].plane[k][0] = (GLfloat)bsp[j].plane[k][0];
				bsp_data[j].plane[k][1] = (GLfloat)bsp[j].plane[k][1];
				bsp_data[j].plane[k][2] = (GLfloat)bsp[j].plane[k][2];
			}
			bsp_data[j].direction = bsp[j].direction;
			bsp_data[j].prev_scene = bsp[j].prev_scene;
			bsp_data[j].next_scene = bsp[j].next_scene;
			vector3_t normal = VECTOR3V((float)bsp[j].normal);
            vector3_normalizev(&normal);
            bsp_data[j].normal[0] = VECTOR3_X(normal);
            bsp_data[j].normal[1] = VECTOR3_X(normal);
            bsp_data[j].normal[2] = VECTOR3_X(normal);
		}
	}

    texture_s sky;
    memset(&sky, 0, sizeof(sky));
	if(model->has_sky && strlen(sky_file))
	{
		char *name = NULL;
        if(resource_path)
		{
            name = calloc(strlen(sky_file) + strlen(resource_path) + 1 + 1, 1);
            memset(name, '\0', sizeof(char) * ((strlen(sky_file) + strlen(resource_path) + 1 + 1)));
            sprintf(name, "%s/%s", resource_path, sky_file);
		}
		else
			name = strdup(sky_file);
        new_texture_from_nl_v2_3d_file(&sky, name);
		free(name);
	}

    memset(glmodel, 0, sizeof(GL_NETLizard_3D_Model));
	glmodel->meshes = meshes;
    glmodel->count = model->data.count ? model->data.count : 0;
	glmodel->item_meshes = item_meshes;
	glmodel->item_count = item_data_length;
	glmodel->texes = texes;
	glmodel->tex_count = tex_count;
	glmodel->start_pos[0] = (GLfloat)model->start_pos[0];
	glmodel->start_pos[1] = (GLfloat)model->start_pos[1];
	glmodel->start_pos[2] = (GLfloat)model->start_pos[2];
	glmodel->start_angle[0] = (GLfloat)model->start_angle[0];
	glmodel->start_angle[1] = (GLfloat)model->start_angle[1];
	glmodel->bsp_data = bsp_data;
	glmodel->bsp_count = bsp_count;
    if(sky.texid)
    {
        glmodel->bg_tex = malloc(sizeof(texture_s));
        memcpy(glmodel->bg_tex, &sky, sizeof(sky));
    }
    glmodel->game = game;

    return GL_TRUE;
}
