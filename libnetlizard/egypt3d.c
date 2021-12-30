#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// egypt 3d

#define dprintfsi(s, d) nllogfln("Egypt 3D : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("Egypt 3D : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("Egypt 3D : "#s)

typedef struct _class__a__scene // map
{
  short_array short_array_3__a__vertex; // short 3顶点坐标
  short_array short_array_7__b__primitive; // short 3索引 3纹理坐标索引(i, i + 1) 1纹理索引
  jshort short__c__end; // = 0; // 最后一个物品索引
  jint int__d__aabb; // AABB盒子
  jint int__e__aabb; // AABB盒子
  jint int__f__aabb; // AABB盒子
  jint int__g__aabb; // AABB盒子
  jint int__h__aabb; // AABB盒子
  jint int__i__aabb; // AABB盒子
  int_array int_array_2__j__plane; //int []; int 碰撞面 1法线索引(i, i + 1, i + 2) 2顶点索引(i, i + 1, i + 2)
  jint k;
  jint l; // = 0;
  byte byte_array_9__m__bsp[9]; // -1, -1, -1, -1, -1, -1, -1, ?, ?
  jint int__n__bsp; // = 0;
  jshort short__o__begin; // = 0; // 第一个物品索引
} class__a__scene;

typedef struct _class__h__bsp
{
  byte byte__a__direction;
  jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; // c[] b = new c[4];
  jshort short__c__prev_scene;
  jshort short__d__next_scene;
  jboolean e; // = false;
} class__h__bsp;

typedef struct _class__g__item // item
{
  short_array short_array_3__a__vertex; // short
  short_array short_array_16__b__primitive; //short
  jboolean c;
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
  byte byte__s__obj; // 33 - 36 大背景盒子
  jboolean t; // = true;
} class__g__item;

typedef struct _class__r__role // role
{
    T_array(short_array) short_2_array_3__a__vertex; // short[][]
  short_array short_array_16__b__primitive; // short[]
  T_array(byte_array) c; // byte[][]
  jint d;
} class__r__role;

typedef struct _class__e__lvl
{
    T_array(class__a__scene) class_a_array__h__scene; // 场景
    short_array short_array__bh__texcoord; // 纹理坐标
    T_array(class__g__item) class_g_array__f__item; // 物品
    int_array int_array__d__normal; // 法线
  jint int_array__aN__translation[3];
  jint int__aL__rotation;
  jint int__aM__rotation;
  T_array(class__h__bsp) class_h_array__g__bsp;
} class__e__lvl;

static void delete_class__a__scene(class__a__scene *scene)
{
    delete_array(&scene->short_array_3__a__vertex);
    delete_array(&scene->short_array_7__b__primitive);
    delete_array(&scene->int_array_2__j__plane);
}

static void delete_class__g__item(class__g__item *item)
{
    delete_array(&item->short_array_3__a__vertex);
    delete_array(&item->short_array_16__b__primitive);
}

static void delete_class__r__role(class__r__role *role)
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

static void delete_class__e__lvl(class__e__lvl *lv)
{
    int i;

    T_array(class__a__scene) *a_h = &lv->class_a_array__h__scene;
    for(i = 0; i < a_h->length; i++)
    {
        class__a__scene *obj = ((class__a__scene *)(a_h->array)) + i;
        delete_class__a__scene(obj);
    }
    delete_array(a_h);

    T_array(class__g__item) *g_f = &lv->class_g_array__f__item;
    for(i = 0; i < g_f->length; i++)
    {
        class__g__item *obj = ((class__g__item *)(g_f->array)) + i;
        delete_class__g__item(obj);
    }
    delete_array(g_f);
    delete_array(&lv->int_array__d__normal);
    delete_array(&lv->short_array__bh__texcoord);
    delete_array(&lv->class_h_array__g__bsp);
}

static class__e__lvl class_e__function_h_1int__scene(const byte arrayOfByte1[], const char *resc_path);
static void read_Egypt3D_map_items(array *g_f, const char *resc_path);
static void class_e__function_Y_void__item(class__g__item *f, const byte arrayOfByte1[]);
static void class_e__function_aa_void__role(class__r__role *obj, const byte arrayOfByte[]);

NLboolean nlReadEgypt3DRoleModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadEgypt3DRoleModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadEgypt3DRoleModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__r__role obj;
    class_e__function_aa_void__role(&obj, (const byte *)data);
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
    delete_class__r__role(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = item_meshes;
    model->item_data.count = item_mesh_count;
	model->has_sky = 0;
    model->type = NL_EGYPT_3D_ROLE_MODEL;
    model->game = NL_SHADOW_OF_EGYPT_3D;
    return NL_TRUE;
}

NLboolean nlReadEgypt3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadEgypt3DItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadEgypt3DItemModelData(const char* data, NLsizei size, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__g__item obj;
    class_e__function_Y_void__item(&obj, (byte *)(data));
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
    delete_class__g__item(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
	model->has_sky = 0;
    model->type = NL_EGYPT_3D_ITEM_MODEL;
    model->game = NL_SHADOW_OF_EGYPT_3D;
    return NL_TRUE;
}

NLboolean nlReadEgypt3DModelFile(const char* name, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadEgypt3DModelData(arr.array, arr.length, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadEgypt3DModelData(const char* data, NLsizei size, const char *resc_path, NETLizard_3D_Model *model)
{
    byte *arrayOfByte = (byte *)(data);
    class__e__lvl lvl = class_e__function_h_1int__scene(arrayOfByte, resc_path);
    class__e__lvl *lv = &lvl;

	ZERO(model, NETLizard_3D_Model);
    T_array(class__a__scene) *a_h = &lv->class_a_array__h__scene;
    const int mesh_count = a_h->length;
    NETLizard_3D_Mesh *meshes = NEW_II(NETLizard_3D_Mesh, a_h->length);
    jshort *bh = NULL;

	int i;
    if(ARRAY_DATA(lv->short_array__bh__texcoord))
        bh = (jshort *)(lv->short_array__bh__texcoord.array);
	for(i = 0; i < a_h->length; i++)
	{
        class__a__scene *obj = (class__a__scene *)(a_h->array) + i;
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
				p->texcoord[0] = bh[(s_b[j + 3])];
				p->texcoord[1] = bh[(s_b[j + 3]) + 1];
				p->texcoord[2] = bh[(s_b[j + 4])];
				p->texcoord[3] = bh[(s_b[j + 4]) + 1];
				p->texcoord[4] = bh[(s_b[j + 5])];
				p->texcoord[5] = bh[(s_b[j + 5]) + 1];
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
            jint *d = (jint *)(lv->int_array__d__normal.array);
            jint *j_array = (jint *)(obj->int_array_2__j__plane.array);
			int k = 0;
            for(j = 0; j < obj->int_array_2__j__plane.length; j += 2)
			{
                jint normal_index = j_array[j];
                jint position_index = j_array[j + 1];
                NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
				plane->normal[0] = d[normal_index];
				plane->normal[1] = d[normal_index + 1];
				plane->normal[2] = d[normal_index + 2];
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
    T_array(class__g__item) *g_f = &lv->class_g_array__f__item;
    const int item_mesh_count = g_f->length;
    NETLizard_3D_Item_Mesh *item_meshes = NEW_II(NETLizard_3D_Item_Mesh, g_f->length);
	for(i = 0; i < g_f->length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(item_meshes)) + i;
		class__g__item *obj = ((class__g__item *)(g_f->array)) + i;
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
    if(ARRAY_DATA(lv->class_h_array__g__bsp))
	{
        class__h__bsp *bsp = (class__h__bsp *)(lv->class_h_array__g__bsp.array);
        model->bsp_data.count = lv->class_h_array__g__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_h_array__g__bsp.length; j++)
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
	model->start_rotation[0] = lv->int__aL__rotation;
	model->start_rotation[1] = lv->int__aM__rotation;
	model->start_position[0] = lv->int_array__aN__translation[0] >> 16;
	model->start_position[1] = lv->int_array__aN__translation[1] >> 16;
	model->start_position[2] = lv->int_array__aN__translation[2] >> 16;
    model->type = NL_EGYPT_3D_MAP_MODEL;
    model->game = NL_SHADOW_OF_EGYPT_3D;
    model->has_sky = 0;

    // free
    delete_class__e__lvl(lv);

    return NL_TRUE;
}

void read_Egypt3D_map_items(array *g_f, const char *resc_path)
{
	if(!g_f)
		return;
	int i;
	for(i = 0; i < g_f->length; i++)
	{
		class__g__item *f = ((class__g__item *)(g_f->array)) + i;
        jint i11 = f->byte__s__obj;
        char *name = make_resource_file_path(EGYPT3D_OBJ_SUBFIX, i11, resc_path);
        array arr;
        int res = file_get_contents(name, &arr);
        free(name);
        if(res > 0)
        {
            class_e__function_Y_void__item(f, arr.array);
            delete_array(&arr);
        }
        else
        {
            ARRAY_NULL(f->short_array_3__a__vertex);
            ARRAY_NULL(f->short_array_16__b__primitive);
        }
	}
}

/* ****** static ****** */

class__e__lvl class_e__function_h_1int__scene(const byte arrayOfByte1[], const char *resc_path)
{
	//byte *av = NULL;
    jint aL;
    jint aM;
    jint aN[3];
    jshort aF;
    T_array(class__a__scene) a_h;
	//byte *ao = NULL;
    int_array int_d;
    short_array short_bh;
    jshort ak;
    T_array(class__g__item) g_f;

    jint i1 = -2;
	i1 += 2;
    jint i2;
	if ((i2 = marge_digit(arrayOfByte1[0], arrayOfByte1[1])) != 0) {
		//av = NEW_II(byte, i2);
	}
    jint i15;
    jint i5;
	for (i5 = 0; i5 < i2; i5++)
	{
		i1 += 2;
		//av[i5] = ((byte)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
	}
	i1 += 2;
	aN[0] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
	i1 += 2;
	aN[1] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
	i1 += 2;
	aN[2] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
	i1 += 2;
	aL = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	i1 += 2;
	aM = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	if (aM > 361) {
		aM -= 65536;
	}
	if (aL > 361) {
		aL -= 65536;
	}
	if (aM < 0) {
		aM = -aM;
	} else {
		aM = 360 - aM;
	}
	if (aM == 360) {
		aM = 0;
	}
	if (aL < 0) {
		aL = 270 + aL;
	} else {
		aL = 270 - aL;
	}
	if (aL == 360) {
		aL = 0;
	}
	dprintfss("Get start position and axis");
	i1 += 2;
    aF = (jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	dprintfsi("Map scene count", aF);
	i1 += 2;
    jint i6;
	i6 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
    dprintfsi("Map normal count", i6);
    ARRAY_NULL(int_d);
	if(i6)
	{
        new_array(&int_d, ESIZE(jint, 4), i6);
        jint *d = (jint *)(int_d.array);
        jint i7;
		for (i7 = 0; i7 < i6; i7 += 3)
		{
			i1 += 2;
            d[i7] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
			i1 += 2;
            d[(i7 + 1)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
			i1 += 2;
            d[(i7 + 2)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
        }
	}
    dprintfss("Get map normal");
	i1 += 2;
	i6 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
    dprintfsi("Texcoord count", i6);
    new_array(&short_bh, ESIZE(jshort, 2), i6);
    jshort *bh = NULL;
    if(ARRAY_DATA(short_bh))
        bh = (jshort *)(short_bh.array);
    jint i8;
	for (i8 = 0; i8 < i6; i8 += 2)
	{
		i1 += 2;
        bh[i8] = ((jshort)(jint)(128 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        bh[(i8 + 1)] = ((jshort)(jint)(128 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");
    new_array(&a_h, sizeof(class__a__scene), aF);
    class__a__scene *h = (class__a__scene *)(a_h.array);
	//ao = NEW_II(byte, aF);
    jint i18;
    jint i13;
	for (i13 = 0; i13 < aF; i13++)
	{
		dprintfsi("Get map scene data", i13);
		i1 += 2;
		h[i13].int__d__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		h[i13].int__e__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		h[i13].int__f__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		h[i13].int__g__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		h[i13].int__h__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		h[i13].int__i__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
        jint i11 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i11);
		i1 += 2;
        jint i10 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
		dprintfsi("Scene primitive count", i10);
        new_array(&h[i13].short_array_3__a__vertex, ESIZE(jshort, 2), i11 * 3);
        jshort *a = ((jshort *)(h[i13].short_array_3__a__vertex.array));
		h[i13].k = i11;
        new_array(&h[i13].short_array_7__b__primitive, ESIZE(jshort, 2), i10 * 7);
        jshort *b = NULL;
        if(ARRAY_DATA(h[i13].short_array_7__b__primitive))
            b = ((jshort *)(h[i13].short_array_7__b__primitive.array));
		h[i13].l = i10;
        jint i14;
		for (i14 = 0; i14 < i11; i14++)
		{
			i1 += 2;
            a[(i14 * 3)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
			i1 += 2;
            a[(i14 * 3 + 1)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
			i1 += 2;
            a[(i14 * 3 + 2)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		}
		dprintfss("Get scene vertex coord");
        jshort *arrayOfShort = ARRAY_DATA(h[i13].short_array_7__b__primitive) ? (jshort *)h[i13].short_array_7__b__primitive.array : NULL;
        jint i17;
		for (i17 = 0; i17 < i10; i17++)
		{
            jint i16 = i17 * 7;
			i1 += 2;
            b[i16] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
			i1 += 2;
            b[(i16 + 1)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
			i1 += 2;
            b[(i16 + 2)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		}
		dprintfss("Get scene primitive vertex coord index");
		i1 += 2;
		i18 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
        dprintfsi("Scene plane count", i18);
        ARRAY_NULL(h[i13].int_array_2__j__plane);
		//h[i13].j = NEW_II(int, i18 * 2);
		if(i18)
		{
            new_array(&h[i13].int_array_2__j__plane, ESIZE(jint, 4), i18 * 2);
            jint *j = (jint *)(h[i13].int_array_2__j__plane.array);
            jint i19;
			for (i19 = 0; i19 < i18; i19++)
			{
				i1 += 2;
				j[(i19 * 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3);
				i1 += 2;
				j[(i19 * 2 + 1)] = arrayOfShort[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 7)];
			}
			dprintfss("Get scene plane normal index and point vertex coord index");
		}
        jint i20;
        jint i16;
		for (i20 = 0; i20 < i10; i20++)
		{
			i16 = i20 * 7;
			i1 += 2;
            b[(i16 + 3)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2));
			i1 += 2;
            b[(i16 + 4)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2));
			i1 += 2;
            b[(i16 + 5)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2));
		}
		dprintfss("Get scene texcoord index");
		i1++;
        jint i23;
        jint i21;
		for (i23 = 0; i23 < i10; i23++)
		{
			i1++;
			if ((i21 = arrayOfByte1[i1]) < 0) {
				i21 += 256;
			}
            b[(i23 * 7 + 6)] = ((jshort)i21);
			if (b[(i23 * 7 + 6)] < 50) {
				//T[h[i13].b[(i23 * 7 + 6)]] = true;
			}
			//printf("%d * 7 + 6 = %d\n", i23, ((short *)(h[i13].b->array))[(i23 * 7 + 6)]);
		}
        jint i24;
		for (i24 = 0; i24 < i10; i24++)
		{
			i1++;
            jint i22;
			if ((i22 = (i22 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)])) & 0xFF) < 70) {
				i22 = 70;
			}
			i21 = b[(i24 * 7 + 6)] & 0xFF;
            b[(i24 * 7 + 6)] = ((jshort)(i22 << 8 | i21));
		}
		dprintfss("Get primitive texture index");
		i1--;
	}

	i1 += 2;
    jint i12;
	i12 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	dprintfsi("Map BSP tree node count", i12);
    T_array(class__h__bsp) class_h_array__g__bsp;
    ARRAY_NULL(class_h_array__g__bsp);
	if(i12 > 0)
	{
        new_array(&class_h_array__g__bsp, sizeof(class__h__bsp), i12);
        class__h__bsp *g = (class__h__bsp *)(class_h_array__g__bsp.array);
		for (i15 = 0; i15 < i12; i15++)
		{
            jint i16;
			for (i16 = 0; i16 < 4; i16++)
			{
				i1 += 2;
				g[i15].int_array_4x3__b__plane[i16][0] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
				i1 += 2;
				g[i15].int_array_4x3__b__plane[i16][1] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
				i1 += 2;
				g[i15].int_array_4x3__b__plane[i16][2] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			}
			i1 += 2;
            i2 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
			i1 += 2;
            jint i3 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
			i1 += 2;
            jint i4 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
			i1 += 2;
            g[i15].short__c__prev_scene = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) - 1));
			i1 += 2;
            g[i15].short__d__next_scene = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) - 1));
			g[i15].int_array_3__a__normal[0] = i2;
			g[i15].int_array_3__a__normal[1] = i3;
			g[i15].int_array_3__a__normal[2] = i4;
			if (i2 != 0) {
				g[i15].byte__a__direction = 1;
			} else if (i3 != 0) {
				g[i15].byte__a__direction = 2;
			} else if (i4 != 0) {
				g[i15].byte__a__direction = 3;
			}
		}
        jint i16;
		for (i16 = 0; i16 < i12; i16++)
		{
			if (g[i16].short__c__prev_scene < 999)
			{
				h[g[i16].short__c__prev_scene].byte_array_9__m__bsp[h[g[i16].short__c__prev_scene].int__n__bsp] = ((byte)i16);
				h[g[i16].short__c__prev_scene].int__n__bsp += 1;
			}
			if (g[i16].short__d__next_scene < 999)
			{
				h[g[i16].short__d__next_scene].byte_array_9__m__bsp[h[g[i16].short__d__next_scene].int__n__bsp] = ((byte)i16);
				h[g[i16].short__d__next_scene].int__n__bsp += 1;
			}
		}
		dprintfss("Get map BSP tree node");
	}
	i1 += 2;
    ak = (jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	dprintfsi("Map item count", ak);
    new_array(&g_f, sizeof(class__g__item), ak);
    class__g__item *f = (class__g__item *)(g_f.array);
	//short *an = NEW_II(short, ak);
    jint i19 = 0;
    jint i20;
	for (i20 = 0; i20 < aF; i20++)
	{
		i1 += 2;
		i18 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
		dprintfsii("Scene item count", i20, i18);
        h[i20].short__o__begin = ((jshort)i19);
        h[i20].short__c__end = ((jshort)(i19 + i18));
		jint i21;
		for (i21 = 0; i21 < i18; i21++)
		{
            f[i19].q = ((jshort)i20);
			i1 += 2;
			f[i19].int__l__translation = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			f[i19].int__m__translation = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			f[i19].int__n__translation = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			f[i19].int__o__rotation = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
			i1 += 2;
			f[i19].int__p__rotation = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
			if (f[i19].int__p__rotation > 360) {
				f[i19].int__p__rotation = (65536 - f[i19].int__p__rotation);
			}
			if (f[i19].int__o__rotation > 360) {
				f[i19].int__o__rotation = (65536 - f[i19].int__o__rotation);
			}
			if (f[i19].int__p__rotation < 0) {
				f[i19].int__p__rotation = (360 + f[i19].int__p__rotation);
			}
			if (f[i19].int__o__rotation < 0) {
				f[i19].int__o__rotation = (360 + f[i19].int__o__rotation);
			} else {
				f[i19].int__o__rotation = (360 - f[i19].int__o__rotation);
			}
			if (f[i19].int__o__rotation == 360) {
				f[i19].int__o__rotation = 0;
			}
			i1 += 2;
			f[i19].byte__s__obj = ((byte)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
			if (f[i19].byte__s__obj > 50)
			{
				class__g__item *tmp2661_2660 = f + i19;
				tmp2661_2660->byte__s__obj = ((byte)(tmp2661_2660->byte__s__obj - 50));
                f[i19].t = jtrue;
			}
			if (f[i19].byte__s__obj == 20)
			{
				// door 石门
				//ao[i20] = 1;
				//an[i19] = ((short)((f[i19].n >> 16) + 300));
			}
			//S[f[i19].byte__s__obj] = true;
			i1 += 2;
			f[i19].d = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
			if ((f[i19].byte__s__obj == 14) || (f[i19].byte__s__obj == 15)) {
				// 14 传送门光柱
				// 15 传送门光柱底座
				f[i19].d = 255;
			}
			if (f[i19].d < 0)
			{
				class__g__item *tmp2822_2821 = f + i19;
                tmp2822_2821->d = ((jshort)(tmp2822_2821->d + 256));
			}
			class__g__item *tmp2840_2839 = f + i19;
            tmp2840_2839->d = ((jshort)(tmp2840_2839->d & 0xFF));
			if (f[i19].d < 70) {
				f[i19].d = 70;
			}
			i1 += 2;
			i19++;
		}
	}

#if UNPARSED
	aE = -1;
	aj = -1;
	for (int i21 = 0; i21 < ak; i21++) {
		if ((f[i21].s > 32) && (f[i21].s < 37))
		{
			aE = (byte)f[i21].q;
			aj = (byte)i21;
			break;
		}
	}
	int i25;
	for (int i23 = 0; i23 < h.length; i23++)
	{
		arrayOfByte2 = null;
		arrayOfByte2 = h[i23].m;
		h[i23].m = null;
		i24 = h[i23].n;
		h[i23].m = new byte[i24];
		for (i25 = 0; i25 < i24; i25++) {
			h[i23].m[i25] = arrayOfByte2[i25];
		}
	}
	byte[] arrayOfByte2 = null;
#endif
	i1 += 2;
#if UNPARSED
	bn = (short)(f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) + 1);
	s = new byte[bn];
	j = new p[bn];
	for (int i24 = 0; i24 < bn; i24++)
	{
		j[i24] = new p();
		j[i24].l = ((byte)i24);
	}
	i19 = 1;
	j[0].p = aN[0];
	j[0].q = aN[1];
	j[0].r = aN[2];
#endif
    jint i26;
    jint i28;
	for (i28 = 0; i28 < aF; i28++)
	{
		i1 += 2;
        jint i25;
		i25 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
        jint i29;
		for (i29 = 0; i29 < i25; i29++)
		{
			i1 += 2;
			//j[i19].p = (f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			//j[i19].q = (f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			//j[i19].r = (f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
			i1 += 2;
			//j[i19].A = ((byte)f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
#if UNPARSED
			if ((i26 = j[i19].A) > 30)
			{
				j[i19].w = true;
				j[i19].C = false;
				j[i19].k = true;
				j[i19].m = 0;
				i26 -= 30;
			}
			else if (i26 > 20)
			{
				i26 -= 20;
				j[i19].w = true;
				j[i19].C = false;
			}
			else if (i26 > 10)
			{
				i26 -= 10;
				j[i19].w = true;
			}
			j[i19].A = ((byte)i26);
			U[i26] = true;
#endif
			i1 += 2;
#if UNPARSED
			int i27;
			if ((i27 = f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)])) > 360) {
				i27 -= 65536;
			}
			if (i27 < 0) {
				i27 = -i27;
			} else {
				i27 = 360 - i27;
			}
			if (i27 == 360) {
				i27 = 0;
			}
			j[i19].u[0] = (-f.f[i27]);
			j[i19].u[1] = (-f.a[i27]);
			j[i19].B[0] = (-f.f[i27]);
			j[i19].B[1] = (-f.a[i27]);
			j[i19].t = ((short)i28);
			i19++;
#endif
		}
	}
#if UNPARSED
	for (int i29 = 0; i29 < j.length; i29++) {
		j[i29].m = O[j[i29].A];
	}
#endif
	i1 += 2;
	/*P = */i26 = (byte)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	//Q = new short[i26 * 3];
    jint i30;
	for (i30 = 0; i30 < i26; i30++)
	{
		i1 += 2;
		//Q[(i30 * 3)] = ((short)f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
		//Q[(i30 * 3 + 1)] = ((short)f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
		//Q[(i30 * 3 + 2)] = ((short)f.a(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
	}
#if UNPARSED
	aD = 0;
	for (int i31 = bn - 1; i31 >= 0; i31--)
	{
		f.a(j[i31]);
		f.b(j[i31]);
		if (i31 == 0)
		{
			aD = j[i31].t;
			f.a(j[i31].B, aL, aM);
			aN[0] = j[0].p;
			aN[1] = j[0].q;
			aN[2] = (j[0].r + 2293760);
		}
	}
	j[0].t = ((short)aD);
#endif

    read_Egypt3D_map_items(&g_f, resc_path);
    class__e__lvl lv;
    ZERO(&lv, class__e__lvl);
    lv.class_a_array__h__scene = a_h;
    lv.short_array__bh__texcoord = short_bh;
    lv.class_g_array__f__item = g_f;
    lv.int_array__d__normal = int_d;
    lv.int__aL__rotation = aL;
    lv.int__aM__rotation = aM;
    lv.int_array__aN__translation[0] = aN[0];
    lv.int_array__aN__translation[1] = aN[1];
    lv.int_array__aN__translation[2] = aN[2];
    lv.class_h_array__g__bsp = class_h_array__g__bsp;
	return lv;
}

void class_e__function_Y_void__item(class__g__item *f, const byte arrayOfByte1[])
{
    jint i14;
    jint i17;
    jint i1 = -2;
	i1 += 2;
	f->int__e__aabb = (marge_digit(arrayOfByte1[0], arrayOfByte1[1]) << 16);
	i1 += 2;
	f->int__f__aabb = (marge_digit(arrayOfByte1[2], arrayOfByte1[3]) << 16);
	i1 += 2;
	f->int__g__aabb = (marge_digit(arrayOfByte1[4], arrayOfByte1[5]) << 16);
	i1 += 2;
	f->int__h__aabb = (marge_digit(arrayOfByte1[6], arrayOfByte1[7]) << 16);
	i1 += 2;
	f->int__i__aabb = (marge_digit(arrayOfByte1[8], arrayOfByte1[9]) << 16);
	i1 += 2;
	f->int__j__aabb = (marge_digit(arrayOfByte1[10], arrayOfByte1[11]) << 16);
    jint i2 = f->int__e__aabb >> 16;
    jint i3 = f->int__f__aabb >> 16;
    jint i4 = f->int__g__aabb >> 16;
    jint i5 = i2 * i2 + i3 * i3 + i4 * i4;
    jint i6 = 65536;
    jint i7 = 0;
	while (i6 >>= 1 > 0) {
		if ((i7 + i6) * (i7 + i6) <= i5) {
			i7 += i6;
		}
	}
	f->d = ((i7 << 3) * 240 << 1);
	f->c = ((i7 << 1) * 260);
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i9 = marge_digit(arrayOfByte1[20], arrayOfByte1[21]);
	i1 += 2;
    jint i8 = marge_digit(arrayOfByte1[22], arrayOfByte1[23]);
	i1 += 2;
    jint i10 = marge_digit(arrayOfByte1[24], arrayOfByte1[25]);
	f->k = i9;
    new_array(&f->short_array_3__a__vertex, ESIZE(jshort, 2), i9 * 3);
    new_array(&f->short_array_16__b__primitive, ESIZE(jshort, 2), i8 * 16);
    jshort *a = (jshort *)(f->short_array_3__a__vertex.array);
    jshort *b = (jshort *)(f->short_array_16__b__primitive.array);
	i10 *= 2;
    jint *arrayOfInt = NEW_II(jint, i10);
    jint i12;
	for (i12 = 0; i12 < i9; i12++)
	{
		i1 += 2;
        a[(i12 * 3)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
        a[(i12 * 3 + 1)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
		i1 += 2;
        a[(i12 * 3 + 2)] = ((jshort)marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]));
	}
    jint i13;
	for (i13 = 0; i13 < i8; i13++)
	{
		i1 += 2;
        b[(i13 * 16)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i13 * 16 + 1)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i13 * 16 + 2)] = ((jshort)(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 3));
	}
    jint i15;
	for (i15 = 0; i15 < i8; i15++)
	{
		i1 += 2;
        i14 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
        b[(i15 * 16 + 10)] = ((jshort)(byte)(jint)(i14 * 127L >> 16));
		i1 += 2;
        i14 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
        b[(i15 * 16 + 11)] = ((jshort)(byte)(jint)(i14 * 127L >> 16));
		i1 += 2;
        i14 = (jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L);
        b[(i15 * 16 + 12)] = ((jshort)(byte)(jint)(i14 * 127L >> 16));
	}
    jint i16;
	for (i16 = 0; i16 < i10; i16 += 2)
	{
		i1 += 2;
        arrayOfInt[i16] = ((jint)(128 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i16 + 1)] = ((jint)(128 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
	for (i17 = 0; i17 < i8; i17++)
	{
		i1 += 2;
        b[(i17 * 16 + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i17 * 16 + 4)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i17 * 16 + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i17 * 16 + 6)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i17 * 16 + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i17 * 16 + 8)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
	}
	i1 += 2;
    jint i18;
	for (i18 = 0; i18 < i8; i18++)
	{
        b[(i18 * 16 + 9)] = ((jshort)arrayOfByte1[i1]);
		/*
			 if (e[(i18 * 16 + 9)] < 50) {
			 T[e[i11].b[(i18 * 16 + 9)]] = true;
			 }
			 */
		i1++;
	}

	free(arrayOfInt);
	/*
		 byte[] arrayOfByte2 = n.g.b("oi", -1);
		 for (int i16 = 0; i16 < f.length; i16++)
		 {
		 g localg;
		 if ((localg = f[i16]).s < 120)
		 {
		 i14 = localg.s << 2;
		 if (arrayOfByte2[(i14 + 1)] == 1) {
		 localg.r = true;
		 }
		 if (arrayOfByte2[(i14 + 2)] == 1) {
		 localg.c = true;
		 }
		 if (arrayOfByte2[(i14 + 3)] == 1) {
		 localg.k = true;
		 }
		 v localv = e[localg.s];
		 localg.a = localv.a;
		 localg.b = localv.b;
		 localg.e = localv.e;
		 localg.h = localv.h;
		 localg.f = localv.f;
		 localg.i = localv.i;
		 localg.g = localv.g;
		 localg.j = localv.j;
		 }
		 }
		 if (q.H == 10) {
		 for (i17 = 0; i17 < ak; i17++) {
		 if (f[i17].s == 9) {
		 f[i17].c = true;
		 }
		 }
		 }
		 */
}

void class_e__function_aa_void__role(class__r__role *obj, const byte arrayOfByte[])
{
    jint i1 = -2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i3 = marge_digit(arrayOfByte[12], arrayOfByte[13]);
	i1 += 2;
    jint i2 = marge_digit(arrayOfByte[14], arrayOfByte[15]);
	i1 += 2;
    jint i4 = marge_digit(arrayOfByte[16], arrayOfByte[17]);
	i1 += 2;
    jint i5 = marge_digit(arrayOfByte[18], arrayOfByte[19]);
    new_array(&obj->short_2_array_3__a__vertex, sizeof(short_array), i5); // short[][]
    new_array(&obj->c, sizeof(byte_array), i5); // byte[][]
	obj->d = i3;
	jint i7;
	for (i7 = 0; i7 < i5; i7++)
	{
        short_array *a = (short_array *)(obj->short_2_array_3__a__vertex.array);
        byte_array *c = (byte_array *)(obj->c.array);
        new_array(&c[i7], ESIZE(byte, 1), i2 * 3);
        new_array(&a[i7], ESIZE(jshort, 2), i3 * 3);
	}
    new_array(&obj->short_array_16__b__primitive, ESIZE(jshort, 2), i2 * 16);
    jshort *b = (jshort *)(obj->short_array_16__b__primitive.array);
	i4 *= 2;
    jshort *arrayOfShort = NEW_II(jshort, i4);
    jint i8;
	for (i8 = 0; i8 < i5; i8++) {
        short_array *arr = &((short_array *)(obj->short_2_array_3__a__vertex.array))[i8];
        jshort *a = (jshort *)(arr->array);
        jint i9;
		for (i9 = 0; i9 < i3; i9++)
        {
			i1 += 2;
            a[(i9 * 3)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			i1 += 2;
            a[(i9 * 3 + 1)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			i1 += 2;
            a[(i9 * 3 + 2)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		}
	}
    jint i10 = i2 * 16;
    jint i11;
	for (i11 = 0; i11 < i10; i11 += 16)
	{
		i1 += 2;
        b[i11] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i11 + 1)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
		i1 += 2;
        b[(i11 + 2)] = ((jshort)(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3));
	}
	i10 = i2 * 3;
	i1++;
    jint i12;
	for (i12 = 0; i12 < i5; i12++) {
        jint i13;
		for (i13 = 0; i13 < i10; i13 += 3)
		{
            byte *c = (byte *)((((array *)(obj->c.array))[i12]).array);
			i1++;
			c[i13] = arrayOfByte[i1];
			i1++;
			c[(i13 + 1)] = arrayOfByte[i1];
			i1++;
			c[(i13 + 2)] = arrayOfByte[i1];
		}
	}
	i1--;
    jint i14;
	for (i14 = 0; i14 < i4; i14 += 2)
	{
		i1 += 2;
        arrayOfShort[i14] = ((jshort)(jint)(256L * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 1000L));
		i1 += 2;
        arrayOfShort[(i14 + 1)] = ((jshort)(jint)(256L * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 1000L));
	}
	i2 *= 16;
    jint i15;
	for (i15 = 0; i15 < i2; i15 += 16)
	{
		i1 += 2;
		b[(i15 + 3)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)];
		b[(i15 + 4)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)];
		i1 += 2;
		b[(i15 + 5)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)];
		b[(i15 + 6)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)];
		i1 += 2;
		b[(i15 + 7)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)];
		b[(i15 + 8)] = arrayOfShort[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)];
	}
    jint i16;
	for (i16 = 0; i16 < i2; i16 += 16)
	{
		i1 += 2;
        b[(i16 + 9)] = ((jshort)(byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		//T[localr.b[(i16 + 9)]] = true;
	}
	free(arrayOfShort);
	//U();
}
