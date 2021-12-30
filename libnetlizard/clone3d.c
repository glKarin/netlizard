#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// clone 3d glo version

#define dprintfsi(s, d) nllogfln("Clone 3D : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("Clone 3D : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("Clone 3D : "#s)

typedef struct _class__c__scene
{
  short_array short_array_3__a__vertex; // short[]
  short_array short_array_7__b__primitive; // short[]
  jshort short__c__end; // = 0;  最后一个物品索引
  jint int__d__aabb;
  jint int__e__aabb;
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  int_array int_array_2__j__plane; // int[]
  jint int__k__vertex_count; // a
  jint int__l__primitive_count; // b = 0;
  byte byte_array_9__m__bsp[9]; // -1, -1, -1, -1, -1, -1, -1, ?, ?
  jint int__n__bsp; // = 0;
  jshort short__o__begin; // = 0; 第一个物品索引
} class__c__scene;

typedef struct _class__k__bsp
{
  byte byte__a__direction;
  jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; //e[] b = new e[4];
  jshort short__c__prev_scene;
  jshort short__d__next_scene;
  jboolean e; // = false;
} class__k__bsp;

typedef struct _class__i__item
{
  short_array short_array_3__a__vertex; // short[]
  short_array short_array_16__b__primitive; // short[]
  boolean c;
  jshort d;
  jint int__e__aabb;
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  jint int__j__aabb;
  jboolean k;
  jint int__l__translation;
  jint int__m__translation;
  jint int__n__translation;
  jint int__o__rotation;
  jint int__p__rotation;
  jshort q;
  jboolean r;
  byte byte__s__obj;
  jboolean t; // = true;
} class__i__item;

typedef struct _class__t__role
{
  T_array(short_array) short_2_array_3__a__vertex; // short[][]
  short_array short_array_16__b__primitive; // short[]
  T_array(byte_array) c; // byte[][]
  jint d;
  jint e;
} class__t__role;

typedef struct _class__g__lvl
{
  jint int__aa__rotation;
  jint int__Z__rotation;
  jint int_array__ab__translation[3];
    T_array(class__c__scene) class_c_array__g__scene;
    short_array short_array__bG__texcoord;
    T_array(class__i__item) class_i_array__e__item;
    int_array int_array__c__normal;
    T_array(class__k__bsp) class_k_array__f__bsp;
} class__g__lvl;

static void delete_class__c__scene(class__c__scene *scene)
{
    delete_array(&scene->short_array_3__a__vertex);
    delete_array(&scene->short_array_7__b__primitive);
    delete_array(&scene->int_array_2__j__plane);
}

static void delete_class__i__item(class__i__item *item)
{
    delete_array(&item->short_array_3__a__vertex);
    delete_array(&item->short_array_16__b__primitive);
}

static void delete_class__t__role(class__t__role *role)
{
    int i;

    for(i = 0; i < role->short_2_array_3__a__vertex.length; i++)
    {
        delete_array((short_array *)role->short_2_array_3__a__vertex.array + i);
    }
    delete_array(&role->short_2_array_3__a__vertex);

    for(i = 0; i < role->c.length; i++)
    {
        delete_array((byte_array *)role->c.array + i);
    }
    delete_array(&role->c);

    delete_array(&role->short_array_16__b__primitive);
}

static void delete_class__g__lvl(class__g__lvl *lv)
{
    int i;

    T_array(class__c__scene) *c_g = &lv->class_c_array__g__scene;
    for(i = 0; i < c_g->length; i++)
    {
        class__c__scene *obj = ((class__c__scene *)(c_g->array)) + i;
        delete_class__c__scene(obj);
    }
    delete_array(c_g);

    T_array(class__i__item) *i_e = &lv->class_i_array__e__item;
    for(i = 0; i < i_e->length; i++)
    {
        class__i__item *obj = ((class__i__item *)(i_e->array)) + i;
        delete_class__i__item(obj);
    }
    delete_array(i_e);
    delete_array(&lv->int_array__c__normal);
    delete_array(&lv->short_array__bG__texcoord);
    delete_array(&lv->class_k_array__f__bsp);
}

static class__g__lvl class_g__function_i_1int__scene(const byte paramInt[], const char *resc_path);
static void class_g__function_P_void__item(class__i__item *e, const byte arrayOfByte1[], jint i7);
static void class_g__function_b_1int__role(class__t__role *obj, const byte arrayOfByte[], jint index);
static void read_Clone3D_map_items(array *i_e, const char *resc_path);

NLboolean nlReadClone3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadClone3DItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadClone3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__i__item obj;
    class_g__function_P_void__item(&obj, (byte *)(data), index);
    NETLizard_3D_Item_Mesh *mesh = NEW(NETLizard_3D_Item_Mesh);
    mesh->item_mesh.vertex.data = NULL;
    mesh->item_mesh.vertex.count = 0;
    mesh->item_mesh.primitive.data = NULL;
    mesh->item_mesh.primitive.count = 0;
    mesh->position[0] = 0;
    mesh->position[1] = 0;
    mesh->position[2] = 0;
    mesh->rotation[0] = 0;
    mesh->rotation[1] = 0;
	mesh->obj_index = index;
    if(ARRAY_DATA(obj.short_array_3__a__vertex) && ARRAY_DATA(obj.short_array_16__b__primitive))
	{
        // vertex
        mesh->item_mesh.vertex.count = obj.short_array_3__a__vertex.length;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
        NLint *im_v = (NLint *)(mesh->item_mesh.vertex.data);
        jshort *i_a = (jshort *)(obj.short_array_3__a__vertex.array);
		int j;
        for(j = 0; j < obj.short_array_3__a__vertex.length; j++)
		{
			im_v[j] = i_a[j];
		}

        // index
        mesh->item_mesh.primitive.count = obj.short_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
        jshort *i_b = (jshort *)(obj.short_array_16__b__primitive.array);
		int k = 0;
        for(j = 0; j < obj.short_array_16__b__primitive.length; j += 16)
		{
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data)) + k;
			p->index[0] = i_b[j];
			p->index[1] = i_b[j + 1];
			p->index[2] = i_b[j + 2];
			p->texcoord[0] = i_b[j + 3];
			p->texcoord[1] = i_b[j + 4];
			p->texcoord[2] = i_b[j + 5];
			p->texcoord[3] = i_b[j + 6];
			p->texcoord[4] = i_b[j + 7];
			p->texcoord[5] = i_b[j + 8];
			p->tex_index = i_b[j + 9];
			p->plane.position[0] = i_a[p->index[0]];
			p->plane.position[1] = i_a[p->index[0] + 1];
			p->plane.position[2] = i_a[p->index[0] + 2];
			p->plane.normal[0] = i_b[j + 10];
			p->plane.normal[1] = i_b[j + 11];
			p->plane.normal[2] = i_b[j + 12];
			k++;
		}

        // box
        mesh->item_mesh.box.max[0] = obj.int__e__aabb >> 16;
        mesh->item_mesh.box.max[1] = obj.int__f__aabb >> 16;
        mesh->item_mesh.box.max[2] = obj.int__g__aabb >> 16;
        mesh->item_mesh.box.min[0] = obj.int__h__aabb >> 16;
        mesh->item_mesh.box.min[1] = obj.int__i__aabb >> 16;
        mesh->item_mesh.box.min[2] = obj.int__j__aabb >> 16;
	}

    // free
    delete_class__i__item(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
    model->game = NL_CLONE_3D;
    model->type = NL_CLONE_3D_ITEM_MODEL;
	model->has_sky = 0;
    return NL_TRUE;
}

NLboolean nlReadClone3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadClone3DModelData(arr.array, arr.length, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadClone3DModelData(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model)
{
    byte *arrayOfByte = (byte *)(data);
    class__g__lvl lvl = class_g__function_i_1int__scene(arrayOfByte, resc_path);
    class__g__lvl *lv = &lvl;

//	if(!lv)
    ZERO(model, NETLizard_3D_Model);
    T_array(class__c__scene) *c_g = &lv->class_c_array__g__scene;
    const int mesh_count = c_g->length;
    NETLizard_3D_Mesh *meshes = NEW_II(NETLizard_3D_Mesh, c_g->length);
    jshort *bG = (jshort *)(lv->short_array__bG__texcoord.array);
    int i;
    for(i = 0; i < c_g->length; i++)
    {
        class__c__scene *obj = (class__c__scene *)(c_g->array) + i;
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(meshes)) + i;

        // vertex
        mesh->vertex.count = obj->short_array_3__a__vertex.length;
        mesh->vertex.data = NEW_II(NLint, mesh->vertex.count);
        NLint *m_v = (NLint *)(mesh->vertex.data);
        jshort *s_a = (jshort *)(obj->short_array_3__a__vertex.array);
        int j;
        for(j = 0; j < obj->short_array_3__a__vertex.length; j++)
        {
            m_v[j] = s_a[j];
        }

        // index
        mesh->primitive.data = NULL;
        mesh->primitive.count = 0;
        if(ARRAY_DATA(obj->short_array_7__b__primitive))
        {
            mesh->primitive.count = obj->short_array_7__b__primitive.length / 7;
            mesh->primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->vertex.count);
            jshort *s_b = (jshort *)(obj->short_array_7__b__primitive.array);
            int k = 0;
            for(j = 0; j < obj->short_array_7__b__primitive.length; j += 7)
            {
                NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->primitive.data)) + k;
                p->index[0] = s_b[j];
                p->index[1] = s_b[j + 1];
                p->index[2] = s_b[j + 2];
                p->texcoord[0] = bG[(s_b[j + 3])];
                p->texcoord[1] = bG[(s_b[j + 3]) + 1];
                p->texcoord[2] = bG[(s_b[j + 4])];
                p->texcoord[3] = bG[(s_b[j + 4]) + 1];
                p->texcoord[4] = bG[(s_b[j + 5])];
                p->texcoord[5] = bG[(s_b[j + 5]) + 1];
                p->tex_index = (s_b[j + 6]) & 0xff;
                k++;
            }
        }

        // plane
        mesh->plane.data = NULL;
        mesh->plane.count = 0;
        if(ARRAY_DATA(obj->int_array_2__j__plane))
        {
            mesh->plane.count = obj->int_array_2__j__plane.length / 2;
            mesh->plane.data = NEW_II(NETLizard_3D_Plane, mesh->plane.count);
            jint *c = (jint *)(lv->int_array__c__normal.array);
            jint *j_array = (jint *)(obj->int_array_2__j__plane.array);
            int k = 0;
            for(j = 0; j < obj->int_array_2__j__plane.length; j += 2)
            {
                jint normal_index = j_array[j];
                jint position_index = j_array[j + 1];
                NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
                plane->normal[0] = c[normal_index];
                plane->normal[1] = c[normal_index + 1];
                plane->normal[2] = c[normal_index + 2];
                plane->position[0] = s_a[position_index];
                plane->position[1] = s_a[position_index + 1];
                plane->position[2] = s_a[position_index + 2];
                k++;
            }
        }

        // box
        mesh->box.max[0] = obj->int__d__aabb >> 16;
        mesh->box.max[1] = obj->int__e__aabb >> 16;
        mesh->box.max[2] = obj->int__f__aabb >> 16;
        mesh->box.min[0] = obj->int__g__aabb >> 16;
        mesh->box.min[1] = obj->int__h__aabb >> 16;
        mesh->box.min[2] = obj->int__i__aabb >> 16;
        mesh->item_index_range[0] = obj->short__o__begin;
        mesh->item_index_range[1] = obj->short__c__end;

        // bsp
        mesh->bsp.data = NULL;
        mesh->bsp.count = 0;
        if(obj->int__n__bsp)
        {
            mesh->bsp.count = obj->int__n__bsp;
            mesh->bsp.data = NEW_II(NLint, mesh->bsp.count);
            NLint *bsp_index = (NLint *)(mesh->bsp.data);
            for(j = 0; j < obj->int__n__bsp; j++)
            {
                bsp_index[j] = obj->byte_array_9__m__bsp[j];
            }
        }
    }

    // item model
    T_array(class__i__item) *i_e = &lv->class_i_array__e__item;
    const int item_mesh_count = i_e->length;
    NETLizard_3D_Item_Mesh *item_meshes = NEW_II(NETLizard_3D_Item_Mesh, i_e->length);
    for(i = 0; i < i_e->length; i++)
    {
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(item_meshes)) + i;
        class__i__item *obj = ((class__i__item *)(i_e->array)) + i;
        if(!ARRAY_DATA(obj->short_array_3__a__vertex) || !ARRAY_DATA(obj->short_array_16__b__primitive))
        {
            mesh->item_mesh.vertex.data = NULL;
            mesh->item_mesh.vertex.count = 0;
            mesh->item_mesh.primitive.data = NULL;
            mesh->item_mesh.primitive.count = 0;
            continue;
        }

        // position/rotation
        mesh->position[0] = obj->int__l__translation >> 16;
        mesh->position[1] = obj->int__m__translation >> 16;
        mesh->position[2] = obj->int__n__translation >> 16;
        mesh->rotation[0] = obj->int__o__rotation;
        mesh->rotation[1] = obj->int__p__rotation;
        mesh->obj_index = obj->byte__s__obj;

        // vertex
        mesh->item_mesh.vertex.count = obj->short_array_3__a__vertex.length;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
        NLint *im_v = (NLint *)(mesh->item_mesh.vertex.data);
        jshort *i_a = (jshort *)(obj->short_array_3__a__vertex.array);
        int j;
        for(j = 0; j < obj->short_array_3__a__vertex.length; j++)
        {
            im_v[j] = i_a[j];
        }

        // index
        mesh->item_mesh.primitive.count = obj->short_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
        jshort *i_b = (jshort *)(obj->short_array_16__b__primitive.array);
        int k = 0;
        for(j = 0; j < obj->short_array_16__b__primitive.length; j += 16)
        {
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data)) + k;
            p->index[0] = i_b[j];
            p->index[1] = i_b[j + 1];
            p->index[2] = i_b[j + 2];
            p->texcoord[0] = i_b[j + 3];
            p->texcoord[1] = i_b[j + 4];
            p->texcoord[2] = i_b[j + 5];
            p->texcoord[3] = i_b[j + 6];
            p->texcoord[4] = i_b[j + 7];
            p->texcoord[5] = i_b[j + 8];
            p->tex_index = i_b[j + 9];
            p->plane.position[0] = i_a[p->index[0]];
            p->plane.position[1] = i_a[p->index[0] + 1];
            p->plane.position[2] = i_a[p->index[0] + 2];
            p->plane.normal[0] = i_b[j + 10];
            p->plane.normal[1] = i_b[j + 11];
            p->plane.normal[2] = i_b[j + 12];
            k++;
        }

        // box
        mesh->item_mesh.box.max[0] = obj->int__e__aabb >> 16;
        mesh->item_mesh.box.max[1] = obj->int__f__aabb >> 16;
        mesh->item_mesh.box.max[2] = obj->int__g__aabb >> 16;
        mesh->item_mesh.box.min[0] = obj->int__h__aabb >> 16;
        mesh->item_mesh.box.min[1] = obj->int__i__aabb >> 16;
        mesh->item_mesh.box.min[2] = obj->int__j__aabb >> 16;
    }

    // bsp

    model->bsp_data.data = NULL;
    model->bsp_data.count = 0;
    if(ARRAY_DATA(lv->class_k_array__f__bsp))
    {
        class__k__bsp *bsp = (class__k__bsp *)(lv->class_k_array__f__bsp.array);
        model->bsp_data.count = lv->class_k_array__f__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
        int j;
        for(j = 0; j < lv->class_k_array__f__bsp.length; j++)
        {
            int k;
            for(k = 0; k < 4; k++)
            {
                bsp_data[j].plane[k][0] = bsp[j].int_array_4x3__b__plane[k][0] >> 16;
                bsp_data[j].plane[k][1] = bsp[j].int_array_4x3__b__plane[k][1] >> 16;
                bsp_data[j].plane[k][2] = bsp[j].int_array_4x3__b__plane[k][2] >> 16;
            }
            bsp_data[j].direction = bsp[j].byte__a__direction;
            bsp_data[j].prev_scene = bsp[j].short__c__prev_scene;
            bsp_data[j].next_scene = bsp[j].short__d__next_scene;
            bsp_data[j].normal[0] = bsp[j].int_array_3__a__normal[0];
            bsp_data[j].normal[1] = bsp[j].int_array_3__a__normal[1];
            bsp_data[j].normal[2] = bsp[j].int_array_3__a__normal[2];
        }
    }

    model->data.data = meshes;
    model->data.count = mesh_count;
    model->item_data.data = item_meshes;
    model->item_data.count = item_mesh_count;
    model->start_rotation[0] = lv->int__Z__rotation;
    model->start_rotation[1] = lv->int__aa__rotation;
    model->start_position[0] = lv->int_array__ab__translation[0] >> 16;
    model->start_position[1] = lv->int_array__ab__translation[1] >> 16;
    model->start_position[2] = lv->int_array__ab__translation[2] >> 16;
    model->game = NL_CLONE_3D;
    model->type = NL_CLONE_3D_MAP_MODEL;
    model->has_sky = 0;

    // free
    delete_class__g__lvl(lv);

    return NL_TRUE;
}

NLboolean nlReadClone3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadClone3DRoleModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadClone3DRoleModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__t__role obj;
    class_g__function_b_1int__role(&obj, (const byte *)data, index);
    const int item_mesh_count = obj.short_2_array_3__a__vertex.length;
    NETLizard_3D_Item_Mesh *item_meshes = NEW_II(NETLizard_3D_Item_Mesh, obj.short_2_array_3__a__vertex.length);
	int i;
    for(i = 0; i < obj.short_2_array_3__a__vertex.length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(item_meshes)) + i;
        mesh->item_mesh.vertex.data = NULL;
        mesh->item_mesh.vertex.count = 0;
        mesh->item_mesh.primitive.data = NULL;
        mesh->item_mesh.primitive.count = 0;
        mesh->position[0] = 0;
        mesh->position[1] = 0;
        mesh->position[2] = 0;
        mesh->rotation[0] = 0;
        mesh->rotation[1] = 0;
		mesh->obj_index = index;
        short_array *a = &((short_array *)(obj.short_2_array_3__a__vertex.array))[i];
        if(/*a && */ARRAY_DATA(obj.short_array_16__b__primitive))
		{
            // vertex
            mesh->item_mesh.vertex.count = a->length;
            mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
            NLint *m_v = (NLint *)(mesh->item_mesh.vertex.data);
            jshort *r_a = (jshort *)(a->array);
			int j;
			for(j = 0; j < a->length; j++)
			{
				m_v[j] = r_a[j];
			}

            // index
            mesh->item_mesh.primitive.count = obj.short_array_16__b__primitive.length / 16;
            mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
            jshort *r_b = (jshort *)(obj.short_array_16__b__primitive.array);
			int k = 0;
            for(j = 0; j < obj.short_array_16__b__primitive.length; j += 16)
			{
                NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data)) + k;
				p->index[0] = r_b[j];
				p->index[1] = r_b[j + 1];
				p->index[2] = r_b[j + 2];
				p->texcoord[0] = r_b[j + 3];
				p->texcoord[1] = r_b[j + 4];
				p->texcoord[2] = r_b[j + 5];
				p->texcoord[3] = r_b[j + 6];
				p->texcoord[4] = r_b[j + 7];
				p->texcoord[5] = r_b[j + 8];
				p->tex_index = r_b[j + 9];
				k++;
			}
		}
    }

    // free
    delete_class__t__role(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = item_meshes;
    model->item_data.count = item_mesh_count;
    model->game = NL_CLONE_3D;
    model->type = NL_CLONE_3D_ROLE_MODEL;
	model->has_sky = 0;
    return NL_TRUE;
}

void read_Clone3D_map_items(array *i_e, const char *resc_path)
{
    if(!i_e)
        return;
    int i;
    for(i = 0; i < i_e->length; i++)
    {
        class__i__item *e = ((class__i__item *)(i_e->array)) + i;
        jint i11 = e->byte__s__obj;
        char *name = make_resource_file_path(CLONE3D_OBJ_SUBFIX, i11, resc_path);
        array arr;
        int res = file_get_contents(name, &arr);
        free(name);
        if(res > 0)
        {
            class_g__function_P_void__item(e, arr.array, i11);
            delete_array(&arr);
        }
        else
        {
            ARRAY_NULL(e->short_array_3__a__vertex);
            ARRAY_NULL(e->short_array_16__b__primitive);
        }
    }
}

/* ****** static ****** */

class__g__lvl class_g__function_i_1int__scene(const byte paramInt[], const char *resc_path)
{
	//byte *bg = NULL;
    jint Z = 0;
    jint aa = 0;
    jint ab[3] = {0, 0, 0};
    jint bn = 0;
    jshort Y = 0;
    int_array int_c;
    T_array(class__c__scene) c_g;
	//byte *Q = NULL;
    T_array(class__i__item) i_e;
	//short *bb = NULL;
    jshort N = 0;
    short_array short_bG;
	/*
		 b = 0;
		 if (paramInt == 9) {
		 b = 61;
		 }
		 */
    jint i1 = -2;
    jint i4;
	i1 += 2;
	if ((i4 = marge_digit(paramInt[0], paramInt[1])) != 0) {
		//bg = NEW_II(byte, i4);
	}
    jint i2;
	for (i2 = 0; i2 < i4; i2++)
	{
		i1 += 2;
		//bg[i2] = ((byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
	}
	i1 += 2;
	ab[0] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	i1 += 2;
	ab[1] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	i1 += 2;
	ab[2] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	i1 += 2;
	Z = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	i1 += 2;
	if ((aa = marge_digit(paramInt[i1], paramInt[(i1 + 1)])) > 361) {
		aa -= 65536;
	}
	if (Z > 361) {
		Z -= 65536;
	}
	if (aa < 0) {
		aa = -aa;
	} else {
		aa = 360 - aa;
	}
	if (aa == 360) {
		aa = 0;
	}
	if (Z < 0) {
		Z = 270 + Z;
	} else {
		Z = 270 - Z;
	}
	if (Z == 360) {
		Z = 0;
	}
	bn = 270;
	dprintfss("Get start position and axis");
	i1 += 2;
    Y = (jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map scene count", Y);
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map normal count", i2);
    new_array(&int_c, ESIZE(jint, 4), i2);
    jint *c = (jint *)(int_c.array);
	for (i4 = 0; i4 < i2; i4 += 3)
	{
		i1 += 2;
        c[i4] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
        c[(i4 + 1)] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
        c[(i4 + 2)] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
	}
	dprintfss("Get map normal");
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Texcoord count", i2);
    new_array(&short_bG, ESIZE(jshort, 2), i2);
    jshort *bG = (jshort *)(short_bG.array);
	for (i4 = 0; i4 < i2; i4 += 2)
	{
		i1 += 2;
        bG[i4] = ((jshort)(jint)(128 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
		i1 += 2;
        bG[(i4 + 1)] = ((jshort)(jint)(128 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");
    new_array(&c_g, sizeof(class__c__scene), Y);
    class__c__scene *g = (class__c__scene *)(c_g.array);
	//Q = NEW_II(byte, Y);
    jint i5;
	for (i5 = 0; i5 < Y; i5++)
	{
		dprintfsi("Get map scene data", i5);
		i1 += 2;
		g[i5].int__d__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		g[i5].int__e__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		g[i5].int__f__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		g[i5].int__g__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		g[i5].int__h__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		g[i5].int__i__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
		i4 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i4);
		i1 += 2;
		i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsi("Scene primitive count", i2);
        new_array(&g[i5].short_array_3__a__vertex, ESIZE(jshort, 2), i4 * 3);
        jshort *a = (jshort *)(g[i5].short_array_3__a__vertex.array);
		g[i5].int__k__vertex_count = i4;
        ARRAY_NULL(g[i5].short_array_7__b__primitive);
        jshort *b = NULL;
		if(i2 != 0)
		{
            new_array(&g[i5].short_array_7__b__primitive, ESIZE(jshort, 2), i2 * 7);
            b = (jshort *)(g[i5].short_array_7__b__primitive.array);
		}
		g[i5].int__l__primitive_count = i2;
        jint i6;
		for (i6 = 0; i6 < i4; i6++)
		{
			i1 += 2;
            a[(i6 * 3)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
            a[(i6 * 3 + 1)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
            a[(i6 * 3 + 2)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
		}
		dprintfss("Get scene vertex coord");
        jshort *arrayOfShort = NULL;
        if(ARRAY_DATA(g[i5].short_array_7__b__primitive))
            arrayOfShort = ((jshort *)(g[i5].short_array_7__b__primitive.array));
		for (i6 = 0; i6 < i2; i6++)
		{
			i4 = i6 * 7;
			i1 += 2;
            b[i4] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3));
			i1 += 2;
            b[(i4 + 1)] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3));
			i1 += 2;
            b[(i4 + 2)] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3));
		}
		dprintfss("Get scene primitive vertex coord index");
		i1 += 2;
		i4 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsi("Scene plane count", i4);
        ARRAY_NULL(g[i5].int_array_2__j__plane);
		if(i4)
		{
            new_array(&g[i5].int_array_2__j__plane, ESIZE(jint, 4), i4 << 1);
            jint *j = (jint *)(g[i5].int_array_2__j__plane.array);
			for (i6 = 0; i6 < i4; i6++)
			{
				i1 += 2;
				j[(i6 << 1)] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3);
				i1 += 2;
				j[((i6 << 1) + 1)] = arrayOfShort[(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 7)];
			}
			dprintfss("Get scene plane normal index and point vertex coord index");
		}
        jint i7;
		for (i7 = 0; i7 < i2; i7++)
		{
			i4 = i7 * 7;
			i1 += 2;
            b[(i4 + 3)] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1));
			i1 += 2;
            b[(i4 + 4)] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1));
			i1 += 2;
            b[(i4 + 5)] = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1));
		}
		dprintfss("Get scene texcoord index");
		i1++;
        jint i4;
		for (i4 = 0; i4 < i2; i4++)
		{
			i1++;
            jint i8;
			if ((i8 = paramInt[i1]) < 0) {
				i8 += 256;
			}
            b[(i4 * 7 + 6)] = ((jshort)i8);
			if (b[(i4 * 7 + 6)] < 50) {
				//aN[g[i5].b[(i4 * 7 + 6)]] = true;
			}
		}
		for (i7 = 0; i7 < i2; i7++)
		{
			i1++;
			if ((i4 = (i4 = marge_digit(paramInt[i1], paramInt[(i1 + 1)])) & 0xFF) < 70) {
				i4 = 70;
			}
            jint i8;
			i8 = b[(i7 * 7 + 6)] & 0xFF;
            b[(i7 * 7 + 6)] = ((jshort)(i4 << 8 | i8));
		}
		dprintfss("Get primitive texture index");
		i1--;
	}
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
    T_array(class__k__bsp) class_k_array__f__bsp;
    new_array(&class_k_array__f__bsp, sizeof(class__k__bsp), i2);
    class__k__bsp *f = (class__k__bsp *)(class_k_array__f__bsp.array);
	dprintfsi("Map BSP tree node count", i2);
    jint i7;
	for (i7 = 0; i7 < i2; i7++)
	{
		for (i4 = 0; i4 < 4; i4++)
		{
			i1 += 2;
			f[i7].int_array_4x3__b__plane[i4][0]= (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			f[i7].int_array_4x3__b__plane[i4][1]= (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			f[i7].int_array_4x3__b__plane[i4][2]= (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		}
		i1 += 2;
        i4 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
        i5 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
        jint i6 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
        f[i7].short__c__prev_scene = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) - 1));
		i1 += 2;
        f[i7].short__d__next_scene = ((jshort)(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) - 1));
		if (i4 != 0) {
			f[i7].byte__a__direction = 1;
		} else if (i5 != 0) {
			f[i7].byte__a__direction = 2;
		} else if (i6 != 0) {
			f[i7].byte__a__direction = 3;
		}
	}
	for (i4 = 0; i4 < i2; i4++)
	{
		if (f[i4].short__c__prev_scene < 999)
		{
			g[f[i4].short__c__prev_scene].byte_array_9__m__bsp[g[f[i4].short__c__prev_scene].int__n__bsp] = ((byte)i4);
			g[f[i4].short__c__prev_scene].int__n__bsp += 1;
		}
		if (f[i4].short__d__next_scene < 999)
		{
			g[f[i4].short__d__next_scene].byte_array_9__m__bsp[g[f[i4].short__d__next_scene].int__n__bsp] = ((byte)i4);
			g[f[i4].short__d__next_scene].int__n__bsp += 1;
		}
	}
	dprintfss("Get map BSP tree node");
	i1 += 2;
    N = (jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map item count", N);
    new_array(&i_e, sizeof(class__i__item), N);
    class__i__item *e = (class__i__item *)(i_e.array);
	//bb = NEW_II(short, Y);
    jint i6 = 0;
	for (i7 = 0; i7 < Y; i7++)
	{
		i1 += 2;
		i4 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsii("Scene item count", i7, i4);
        g[i7].short__o__begin = ((jshort)i6);
        g[i7].short__c__end = ((jshort)(i6 + i4));
        jint i8;
		for (i8 = 0; i8 < i4; i8++)
		{
            e[i6].q = ((jshort)i7);
			i1 += 2;
			e[i6].int__l__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			e[i6].int__m__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			e[i6].int__n__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			e[i6].int__o__rotation = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
			i1 += 2;
            e[i6].int__p__rotation = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (e[i6].int__p__rotation > 360) {
				e[i6].int__p__rotation = (65536 - e[i6].int__p__rotation);
			}
			if (e[i6].int__o__rotation > 360) {
				e[i6].int__o__rotation = (65536 - e[i6].int__o__rotation);
			}
			if (e[i6].int__p__rotation < 0) {
				e[i6].int__p__rotation = (360 + e[i6].int__p__rotation);
			}
			if (e[i6].int__o__rotation < 0) {
				e[i6].int__o__rotation = (360 + e[i6].int__o__rotation);
			} else {
				e[i6].int__o__rotation = (360 - e[i6].int__o__rotation);
			}
			if (e[i6].int__o__rotation == 360) {
				e[i6].int__o__rotation = 0;
			}
			i1 += 2;
			e[i6].byte__s__obj = ((byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (((e[i6].byte__s__obj > 2) && (e[i6].byte__s__obj < 7)) || ((e[i6].byte__s__obj > 33) && (e[i6].byte__s__obj < 38)))
			{
				//Q[i7] = 1;
				if (e[i6].byte__s__obj < 10) {
					//bb[i7] = ((short)((e[i6].n >> 16) + 150));
				} else if (e[i6].byte__s__obj < 36) {
					//bb[i7] = ((short)((e[i6].m >> 16) + 300));
				} else {
					//bb[i7] = ((short)((e[i6].l >> 16) + 300));
				}
			}
			//aL[e[i6].byte__s__obj] = true;
			i1 += 2;
            e[i6].d = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (e[i6].d < 0)
			{
				class__i__item *tmp2756_2755 = e + i6;
                tmp2756_2755->d = ((jshort)(tmp2756_2755->d + 256));
			}
			class__i__item *tmp2774_2773 = e + i6;
            tmp2774_2773->d = ((jshort)(tmp2774_2773->d & 0xFF));
			if (e[i6].d < 70) {
				e[i6].d = 70;
			}
			i1 += 2;
			i6++;
		}
	}

#if UNPARSED
	V = null;
	W = null;
	int i8 = 0;
	for (i4 = 0; i4 < e.length; i4++) {
		if (e[i4].s == 31) {
			i8++;
		}
	}
	V = new byte[i8 * 5];
	W = new short[i8];
	i8 = 0;
	for (i4 = 0; i4 < e.length; i4++) {
		if (e[i4].s == 31)
		{
			V[(i8 * 5)] = ((byte)i4);
			V[(i8 * 5 + 1)] = ((byte)e[i4].q);
			V[(i8 * 5 + 3)] = 0;
			V[(i8 * 5 + 4)] = 1;
			i8++;
		}
	}
	Object localObject;
	for (i7 = 0; i7 < g.length; i7++)
	{
		localObject = g[i7].m;
		g[i7].m = null;
		i2 = g[i7].n;
		g[i7].m = new byte[i2];
		for (i5 = 0; i5 < i2; i5++) {
			g[i7].m[i5] = localObject[i5];
		}
	}
#endif
	i1 += 2;
#if UNPARSED
	ah = (short)(h.b(paramInt[i1], paramInt[(i1 + 1)]) + 1);
	if (p) {
		ah = (short)(ah + 1);
	}
	aA = new byte[ah];
	i = new q[ah];
	for (i2 = 0; i2 < ah; i2++)
	{
		i[i2] = new q();
		i[i2].n = ((byte)i2);
	}
	i6 = 1;
	i[0].w = ab[0];
	i[0].x = ab[1];
	i[0].y = ab[2];
#endif
    jint i9;
	for (i9 = 0; i9 < Y; i9++)
	{
		i1 += 2;
		i5 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
        jint i10;
		for (i10 = 0; i10 < i5; i10++)
		{
			//localObject = i[i6];
			i1 += 2;
			//((q)localObject).w = (h.b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			//((q)localObject).x = (h.b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			//((q)localObject).y = (h.b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			//((q)localObject).M = ((byte)h.b(paramInt[i1], paramInt[(i1 + 1)]));
			//i8 = ((q)localObject).M;
#if UNPARSED
			for (i7 = 0; i7 < N; i7++)
			{
				i locali;
				if (((locali = e[i7]).s > 51) && (locali.s < 56) && (locali.l == ((q)localObject).w) && (locali.m == ((q)localObject).x) && (Math.abs(locali.n - ((q)localObject).y >> 16) <= 30))
				{
					((q)localObject).O = ((byte)(locali.s - 51));
					locali.t = false;
				}
			}
			if (i8 > 30)
			{
				i8 -= 30;
				if (i8 == 1)
				{
					((q)localObject).K = 4;
					aN[4] = true;
				}
			}
			else if (i8 > 20)
			{
				i8 -= 20;
				if (i8 == 1)
				{
					((q)localObject).K = 3;
					aN[3] = true;
				}
			}
			else if (i8 > 10)
			{
				i8 -= 10;
				if (i8 == 1)
				{
					((q)localObject).K = 2;
					aN[2] = true;
				}
			}
			if (i8 == 2) {
				((q)localObject).K = 5;
			}
			if ((i8 == 3) || (i8 == 4)) {
				((q)localObject).K = 6;
			}
			((q)localObject).M = ((byte)i8);
			aO[i8] = true;
#endif
			i1 += 2;
#if UNPARSED
			if ((i3 = h.b(paramInt[i1], paramInt[(i1 + 1)])) > 360) {
				i3 -= 65536;
			}
			if (i3 < 0) {
				i3 = -i3;
			} else {
				i3 = 360 - i3;
			}
			if (i3 == 360) {
				i3 = 0;
			}
			((q)localObject).E[0] = (-h.d[i3]);
			((q)localObject).E[1] = (-h.a[i3]);
			((q)localObject).D = ((short)i3);
			((q)localObject).B = ((short)i9);
			i6++;
#endif
		}
	}
#if UNPARSED
	for (int i10 = 0; i10 < i.length; i10++) {
		i[i10].s = H[i[i10].M];
	}
#endif
	i1 += 2;
    /*aJ = */jint i8 = (byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	//aK = new short[i8 * 3];
    jint i3;
	for (i3 = 0; i3 < i8; i3++)
	{
		i1 += 2;
		//aK[(i3 * 3)] = ((short)h.b(paramInt[i1], paramInt[(i1 + 1)]));
		i1 += 2;
		//aK[(i3 * 3 + 1)] = ((short)h.b(paramInt[i1], paramInt[(i1 + 1)]));
		i1 += 2;
		//aK[(i3 * 3 + 2)] = ((short)h.b(paramInt[i1], paramInt[(i1 + 1)]));
	}
	i1 += 2;
	i1 += 2;
	//m = new byte[i8 = h.b(paramInt[i1], paramInt[(i1 + 1)])];
	i8 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	for (i3 = 0; i3 < i8; i3++)
	{
		i1 += 2;
		//m[i3] = ((byte)h.b(paramInt[i1], paramInt[(i1 + 1)]));
	}
#if UNPARSED
	X = 0;
	for (paramInt = ah - 1; paramInt >= 0; paramInt--)
	{
		h.a(i[paramInt]);
		h.b(i[paramInt]);
		if (paramInt == 0)
		{
			X = i[paramInt].B;
			ab[0] = i[0].w;
			ab[1] = i[0].x;
			ab[2] = (i[0].y + 4587520);
			i[0].D = ((short)aa);
		}
		else
		{
			i[paramInt].A = ((byte)h.a(10));
		}
	}
	i[0].B = ((short)X);
#endif

    read_Clone3D_map_items(&i_e, resc_path);

    class__g__lvl lv;
    ZERO(&lv, class__g__lvl);
    lv.int__aa__rotation = aa;
    lv.int__Z__rotation = Z;
    lv.int_array__ab__translation[0] = ab[0];
    lv.int_array__ab__translation[1] = ab[1];
    lv.int_array__ab__translation[2] = ab[2];
    lv.class_c_array__g__scene = c_g;
    lv.short_array__bG__texcoord = short_bG;
    lv.class_i_array__e__item = i_e;
    lv.class_k_array__f__bsp = class_k_array__f__bsp;
    lv.int_array__c__normal = int_c;
	return lv;
}

void class_g__function_P_void__item(class__i__item *e, const byte arrayOfByte1[], jint i7)
{
    jint i1 = -2;
	i1 += 2;
	e->int__e__aabb = (marge_digit(arrayOfByte1[0], arrayOfByte1[1]) << 16);
	i1 += 2;
	e->int__f__aabb = (marge_digit(arrayOfByte1[2], arrayOfByte1[3]) << 16);
	i1 += 2;
	e->int__g__aabb = (marge_digit(arrayOfByte1[4], arrayOfByte1[5]) << 16);
	i1 += 2;
	e->int__h__aabb = (marge_digit(arrayOfByte1[6], arrayOfByte1[7]) << 16);
	i1 += 2;
	e->int__i__aabb = (marge_digit(arrayOfByte1[8], arrayOfByte1[9]) << 16);
	i1 += 2;
	e->int__j__aabb = (marge_digit(arrayOfByte1[10], arrayOfByte1[11]) << 16);
    jint i2 = e->int__e__aabb >> 16;
    jint i3 = e->int__f__aabb >> 16;
    jint i5 = e->int__g__aabb >> 16;
	i2 = i2 * i2 + i3 * i3 + i5 * i5;
	i3 = 65536;
	i5 = 0;
	while (i3 >>= 1 > 0) {
		if ((i5 + i3) * (i5 + i3) <= i2) {
			i5 += i3;
		}
	}
	e->d = ((i5 << 3) * 240 << 1);
	e->c = ((i5 << 1) * 252);
	//printf("%d %d %d\n", i5, ((i5 << 3) * 240 << 1), ((i5 << 1) * 252));
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i3 = marge_digit(arrayOfByte1[20], arrayOfByte1[21]);
	i1 += 2;
	i2 = marge_digit(arrayOfByte1[22], arrayOfByte1[23]);
	i1 += 2;
	i5 = marge_digit(arrayOfByte1[24], arrayOfByte1[25]);
	e->k = i3;
    new_array(&e->short_array_3__a__vertex, ESIZE(jshort, 2), i3 * 3);
    new_array(&e->short_array_16__b__primitive, ESIZE(jshort, 2), i2 << 4);
    jshort *a = (jshort *)(e->short_array_3__a__vertex.array);
    jshort *b = (jshort *)(e->short_array_16__b__primitive.array);
	i5 <<= 1;
    jint *arrayOfInt = NEW_II(jint, i5);
    jint i6 = 0;
    jint i8;
	for (i8 = 0; i8 < i3; i8++)
	{
		i1 += 2;
        a[i6] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
        a[(i6 + 1)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
        a[(i6 + 2)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i6 += 3;
	}
	i6 = 0;
	for (i3 = 0; i3 < i2; i3++)
	{
		i1 += 2;
        b[i6] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i6 + 1)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i6 + 2)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		i6 += 16;
	}
	i6 = 0;
	i1++;
    jint i9;
	for (i9 = 0; i9 < i2; i9++)
	{
		i1++;
        b[(i6 + 10)] = ((jshort)arrayOfByte1[i1]);
		i1++;
        b[(i6 + 11)] = ((jshort)arrayOfByte1[i1]);
		i1++;
        b[(i6 + 12)] = ((jshort)arrayOfByte1[i1]);
		i6 += 16;
	}
	i1--;
	i6 = 128;
	if ((i7 >= 44) && (i7 <= 51)) {
		i6 = 256;
	}
	for (i3 = 0; i3 < i5; i3 += 2)
	{
		i1 += 2;
        arrayOfInt[i3] = ((jint)(i6 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i3 + 1)] = ((jint)(i6 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
	i6 = 0;
	for (i5 = 0; i5 < i2; i5++)
	{
		i1 += 2;
        b[(i6 + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[(i6 + 4)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
        b[(i6 + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[(i6 + 6)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
        b[(i6 + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[(i6 + 8)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i6 += 16;
	}
	i6 = 0;
	i1 += 2;
	for (i5 = 0; i5 < i2; i5++)
	{
        b[(i6 + 9)] = ((jshort)arrayOfByte1[i1]);
		//aN[localx.b[(i6 + 9)]] = true;
		i1++;
		i6 += 16;
	}
	free(arrayOfInt);
	/*
		 byte[] arrayOfByte2 = p.d.b("oi", -1);
		 for (int i6 = 0; i6 < e.length; i6++)
		 {
		 i locali;
		 if ((locali = e[i6]).s < 120)
		 {
		 i9 = locali.s << 2;
		 if (arrayOfByte2[i9] == 1) {
		 locali.c = true;
		 }
		 if (arrayOfByte2[(i9 + 1)] == 1) {
		 locali.r = true;
		 }
		 if (arrayOfByte2[(i9 + 3)] == 1) {
		 locali.k = true;
		 }
		 localx = d[locali.s];
		 locali.a = localx.a;
		 locali.b = localx.b;
		 locali.e = localx.e;
		 locali.h = localx.h;
		 locali.f = localx.f;
		 locali.i = localx.i;
		 locali.g = localx.g;
		 locali.j = localx.j;
		 }
		 }
		 int i4 = 0;
		 for (int i5 = 0; i5 < e.length; i5++) {
		 if ((e[i5].s > 39) && (e[i5].s < 44)) {
		 i4++;
		 }
		 }
		 I = new short[i4 * 3];
		 i4 = 0;
		 for (i5 = 0; i5 < e.length; i5++) {
		 if ((e[i5].s > 39) && (e[i5].s < 44))
		 {
		 I[(i4 * 3)] = ((short)i5);
		 I[(i4 * 3 + 1)] = 0;
		 I[(i4 * 3 + 2)] = 0;
		 i4++;
		 }
		 }
		 */
}

void class_g__function_b_1int__role(class__t__role *obj, const byte arrayOfByte[], jint i8)
{
    jint i1 = -2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i4 = marge_digit(arrayOfByte[12], arrayOfByte[13]);
	i1 += 2;
    jint i3 = marge_digit(arrayOfByte[14], arrayOfByte[15]);
	i1 += 2;
    jint i5 = marge_digit(arrayOfByte[16], arrayOfByte[17]);
	i1 += 2;
    jint i6 = marge_digit(arrayOfByte[18], arrayOfByte[19]);
    new_array(&obj->short_2_array_3__a__vertex, sizeof(byte_array), i6); // short[][];
    new_array(&obj->c, sizeof(byte_array), i6); // byte[][];
	obj->d = i4;
	obj->e = i3;
    jint i7;
	for (i7 = 0; i7 < i6; i7++)
	{
        short_array *a = (short_array *)(obj->short_2_array_3__a__vertex.array);
        byte_array *c = (byte_array *)(obj->c.array);
        new_array(&c[i7], ESIZE(byte, 1), i3 * 3);
        new_array(&a[i7], ESIZE(jshort, 2), (i4 + 13) * 3);
	}
    new_array(&obj->short_array_16__b__primitive, ESIZE(jshort, 2), (i3 + 22) << 4);
    jshort *b = (jshort *)(obj->short_array_16__b__primitive.array);
	i5 <<= 1;
    jshort *arrayOfShort = NEW_II(jshort, i5);
    jint i9 = 1;
    jint i10 = 1;
	if (i8 < 3) {
		i9 = -1;
	}
	if (i8 < 3) {
		i10 = -1;
	}
    jint i12;
    jint i11;
	for (i11 = 0; i11 < i6; i11++) {
        short_array *arr = &((short_array *)(obj->short_2_array_3__a__vertex.array))[i11];
        jshort *a = (jshort *)(arr->array);
		for (i12 = 0; i12 < i4; i12++)
		{
			i1 += 2;
            a[(i12 * 3)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * i9));
			i1 += 2;
            a[(i12 * 3 + 1)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * i10));
			i1 += 2;
            a[(i12 * 3 + 2)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		}
	}
	i4 = i3 << 4;
	for (i11 = 0; i11 < i4; i11 += 16)
	{
		i1 += 2;
        b[i11] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i11 + 1)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i11 + 2)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
	}
	i4 = i3 * 3;
	i1++;
	for (i11 = 0; i11 < i6; i11++) {
		for (i12 = 0; i12 < i4; i12 += 3)
		{
            byte *c = (byte *)((((array *)(obj->c.array))[i11]).array);
			i1++;
			c[i12] = ((byte)(arrayOfByte[i1] * i9));
			i1++;
			c[(i12 + 1)] = ((byte)(arrayOfByte[i1] * i10));
			i1++;
			c[(i12 + 2)] = arrayOfByte[i1];
		}
	}
	i1--;
	for (i4 = 0; i4 < i5; i4 += 2)
	{
		i1 += 2;
        arrayOfShort[i4] = ((jshort)(jint)(256L * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 1000L));
		i1 += 2;
        arrayOfShort[(i4 + 1)] = ((jshort)(jint)(256L * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 1000L));
	}
	i3 <<= 4;
	for (i4 = 0; i4 < i3; i4 += 16)
	{
		i1 += 2;
		b[(i4 + 3)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1)];
		b[(i4 + 4)] = arrayOfShort[((marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1) + 1)];
		i1 += 2;
		b[(i4 + 5)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1)];
		b[(i4 + 6)] = arrayOfShort[((marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1) + 1)];
		i1 += 2;
		b[(i4 + 7)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1)];
		b[(i4 + 8)] = arrayOfShort[((marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 1) + 1)];
	}
	for (i4 = 0; i4 < i3; i4 += 16)
	{
		i1 += 2;
		b[(i4 + 9)] = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		//aN[localt.b[(i4 + 9)]] = true;
	}
	free(arrayOfShort);
}
