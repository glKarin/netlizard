#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// ct 3d konon version by serviak

#define dprintfsi(s, d) nllogfln("CT 3D : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("CT 3D : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("CT 3D : "#s)

typedef struct _class__f__scene
{
  int_array int_array_6__a__vertex;
  int_array int_array_10__b__primitive;
  jint c;
  jint d;
  jint int__e__end;// = 0; 最后一个物品索引
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  jint int__j__aabb;
  jint int__k__aabb;
  int_array int_array_6__l__plane;
  jint m;
  jint n;
  jint o;
  jint int_array_5__p__bsp[5];
  jint int__q__bsp;// = 0;
  jint int__r__begin;// = 0; 第一个物品索引
} class__f__scene;

typedef struct _class__b__bsp
{
  byte byte__a__direction;
  jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; //i[] b = new i[4];
  jint int__c__prev_scene;
  jint int__d__next_scene;
  boolean e; // = false;
} class__b__bsp;

typedef struct _class__g__item
{
  int_array int_array_6__a__vertex;
  int_array int_array_16__b__primitive;
  jint** c;
  jshort d;
  jint e;
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  jint int__j__aabb;
  jint int__k__aabb;
  byte l;// = -1;
  jint int__m__translation;// = 40960000;
  jint int__n__translation;// = 40960000;
  jint int__o__translation;// = 8192000;
  jint int__p__rotation;// = 0;
  jint int__q__rotation;// = 0;
  jshort r;
  byte byte__s__obj;
  jint t;//boolean t = true;
} class__g__item;

typedef struct _class__h__lvl
{
    T_array(class__f__scene) class_f_array__l__scene;
    T_array(class__g__item) class_g_array__h__item;
    jint int_array__dg__translation[3];
    jint int__dd__rotation;
    jint int__df__rotation;
    T_array(class__b__bsp) class_b_array__j__bsp;
} class__h__lvl;

static void delete_class__f__scene(class__f__scene *scene)
{
    delete_array(&scene->int_array_6__a__vertex);
    delete_array(&scene->int_array_10__b__primitive);
    delete_array(&scene->int_array_6__l__plane);
}

static void delete_class__g__item(class__g__item *item)
{
    delete_array(&item->int_array_6__a__vertex);
    delete_array(&item->int_array_16__b__primitive);
}

static void delete_class__h__lvl(class__h__lvl *lv)
{
    int i;

    T_array(class__f__scene) *l = &lv->class_f_array__l__scene;
    for(i = 0; i < l->length; i++)
    {
        class__f__scene *obj = ((class__f__scene *)(l->array)) + i;
        delete_class__f__scene(obj);
    }
    delete_array(l);

    T_array(class__g__item) *h = &lv->class_g_array__h__item;
    for(i = 0; i < h->length; i++)
    {
        class__g__item *obj = ((class__g__item *)(h->array)) + i;
        delete_class__g__item(obj);
    }
    delete_array(h);
    delete_array(&lv->class_b_array__j__bsp);
}

static class__h__lvl class_h__function_k_void__scene(const byte arrayOfByte[], const char *resc_path);
static void class_h__function_S_void__item(class__g__item *h, const byte arrayOfByte[]);
static void read_CT3D_map_items(T_array(class__g__item) *class_g_array__h__item, const char *resc_path);

NLboolean nlReadCT3DModelFile(const char *name, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DModelData(arr.array, arr.length, level, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DModelData(const char* data, NLsizei res, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
	int paramInt = level;
    int dr = 0; // has sky
	if ((paramInt == 3) || (paramInt == 4) || (paramInt == 5) || (paramInt == 7)) {
		dr = 1;
	}

    byte *arrayOfByte = (byte *)(data);
    class__h__lvl lvl = class_h__function_k_void__scene(arrayOfByte, resc_path);
    class__h__lvl *lv = &lvl;

	ZERO(model, NETLizard_3D_Model);
    array *l = &lv->class_f_array__l__scene;
    model->data.data = NEW_II(NETLizard_3D_Mesh, l->length);
    model->data.count = l->length;
	model->has_sky = dr;
    model->type = NL_CT_3D_MAP_MODEL;
    model->game = NL_CONTR_TERRORISM_3D;

    // map model
	int i;
	for(i = 0; i < l->length; i++)
	{
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
		class__f__scene *obj = ((class__f__scene *)(l->array)) + i;

        // vertex
        mesh->vertex.count = obj->int_array_6__a__vertex.length / 2;
        mesh->vertex.data = NEW_II(NLint, mesh->vertex.count);
		int j;
        NLint *m_v = (NLint *)(mesh->vertex.data);
        jint *s_a = (jint *)(obj->int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj->int_array_6__a__vertex.length; j += 6)
		{
			m_v[k] = s_a[j] >> 16;
			m_v[k + 1] = s_a[j + 1] >> 16;
			m_v[k + 2] = s_a[j + 2] >> 16;
			k += 3;
		}

        // index
        mesh->primitive.count = obj->int_array_10__b__primitive.length / 10;
        mesh->primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->primitive.count);
        jint *s_b = (jint *)(obj->int_array_10__b__primitive.array);
		k = 0;
        for(j = 0; j < obj->int_array_10__b__primitive.length; j += 10)
		{
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->primitive.data)) + k;
			p->index[0] = s_b[j] / 6;
			p->index[1] = s_b[j + 1] / 6;
			p->index[2] = s_b[j + 2] / 6;
			p->texcoord[0] = s_b[j + 3];
			p->texcoord[1] = s_b[j + 4];
			p->texcoord[2] = s_b[j + 5];
			p->texcoord[3] = s_b[j + 6];
			p->texcoord[4] = s_b[j + 7];
			p->texcoord[5] = s_b[j + 8];
			p->tex_index = s_b[j + 9];
			k++;
		}

        // plane
        mesh->plane.count = obj->int_array_6__l__plane.length / 6;
        mesh->plane.data = NEW_II(NETLizard_3D_Plane, mesh->plane.count);
        jint *s_l = (jint *)(obj->int_array_6__l__plane.array);
		k = 0;
        for(j = 0; j < obj->int_array_6__l__plane.length; j += 6)
        {
            NETLizard_3D_Plane *p = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
			p->position[0] = s_l[j] >> 16;
			p->position[1] = s_l[j + 1] >> 16;
			p->position[2] = s_l[j + 2] >> 16;
			p->normal[0] = s_l[j + 3];
			p->normal[1] = s_l[j + 4];
			p->normal[2] = s_l[j + 5];
			k++;
		}
        mesh->box.max[0] = obj->int__f__aabb >> 16;
        mesh->box.max[1] = obj->int__g__aabb >> 16;
        mesh->box.max[2] = obj->int__h__aabb >> 16;
        mesh->box.min[0] = obj->int__i__aabb >> 16;
        mesh->box.min[1] = obj->int__j__aabb >> 16;
        mesh->box.min[2] = obj->int__k__aabb >> 16;
		mesh->item_index_range[0] = obj->int__r__begin;
		mesh->item_index_range[1] = obj->int__e__end;

        // bsp
		if(obj->int__q__bsp)
		{
            mesh->bsp.count = obj->int__q__bsp;
            mesh->bsp.data = NEW_II(NLint, mesh->bsp.count);
            NLint *bsp_index = (NLint *)(mesh->bsp.data);
			for(j = 0; j < obj->int__q__bsp; j++)
			{
				bsp_index[j] = obj->int_array_5__p__bsp[j];
			}
		}
	}

    // item model
    T_array(class__g__item) *h = &lv->class_g_array__h__item;
    model->item_data.count = h->length;
    model->item_data.data = NEW_II(NETLizard_3D_Item_Mesh, model->item_data.count);
	for(i = 0; i < h->length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
		class__g__item *obj = ((class__g__item *)(h->array)) + i;
        if(!ARRAY_DATA(obj->int_array_6__a__vertex) || !ARRAY_DATA(obj->int_array_16__b__primitive))
		{
            mesh->item_mesh.vertex.data = NULL;
            mesh->item_mesh.vertex.count = 0;
            mesh->item_mesh.primitive.data = NULL;
            mesh->item_mesh.primitive.count = 0;
			continue;
		}

        // position/rotation
        mesh->position[0] = obj->int__m__translation >> 16;
        mesh->position[1] = obj->int__n__translation >> 16;
        mesh->position[2] = obj->int__o__translation >> 16;
        mesh->rotation[0] = obj->int__p__rotation;
        mesh->rotation[1] = obj->int__q__rotation;
		mesh->obj_index = obj->byte__s__obj;

        // vertex
        mesh->item_mesh.vertex.count = obj->int_array_6__a__vertex.length / 2;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
		int j;
        NLint *im_a = (NLint *)(mesh->item_mesh.vertex.data);
        jint *i_a = (jint *)(obj->int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj->int_array_6__a__vertex.length; j += 6)
		{
			im_a[k] = i_a[j] >> 16;
			im_a[k + 1] = i_a[j + 1] >> 16;
			im_a[k + 2] = i_a[j + 2] >> 16;
			k += 3;
		}

        // index
        mesh->item_mesh.primitive.count = obj->int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
		k = 0;
        jint *i_b = (jint *)(obj->int_array_16__b__primitive.array);
        for(j = 0; j < obj->int_array_16__b__primitive.length; j += 16)
		{
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data)) + k;
			p->index[0] = i_b[j] / 6;
			p->index[1] = i_b[j + 1] / 6;
			p->index[2] = i_b[j + 2] / 6;
			p->texcoord[0] = i_b[j + 3];
			p->texcoord[1] = i_b[j + 4];
			p->texcoord[2] = i_b[j + 5];
			p->texcoord[3] = i_b[j + 6];
			p->texcoord[4] = i_b[j + 7];
			p->texcoord[5] = i_b[j + 8];
			p->tex_index = i_b[j + 9];
            // plane
			p->plane.position[0] = i_a[p->index[0]] >> 16;
			p->plane.position[1] = i_a[p->index[0] + 1] >> 16;
			p->plane.position[2] = i_a[p->index[0] + 2] >> 16;
			p->plane.normal[0] = i_b[j + 10];
			p->plane.normal[1] = i_b[j + 11];
			p->plane.normal[2] = i_b[j + 12];
			k++;
		}
        // box
        mesh->item_mesh.box.max[0] = obj->int__f__aabb >> 16;
        mesh->item_mesh.box.max[1] = obj->int__g__aabb >> 16;
        mesh->item_mesh.box.max[2] = obj->int__h__aabb >> 16;
        mesh->item_mesh.box.min[0] = obj->int__i__aabb >> 16;
        mesh->item_mesh.box.min[1] = obj->int__j__aabb >> 16;
        mesh->item_mesh.box.min[2] = obj->int__k__aabb >> 16;
	}

    // bsp
    model->bsp_data.data = NULL;
    model->bsp_data.count = 0;
    if(ARRAY_DATA(lv->class_b_array__j__bsp))
	{
        class__b__bsp *bsp = (class__b__bsp *)(lv->class_b_array__j__bsp.array);
        model->bsp_data.count = lv->class_b_array__j__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_b_array__j__bsp.length; j++)
		{
			int k;
			for(k = 0; k < 4; k++)
			{
				bsp_data[j].plane[k][0] = bsp[j].int_array_4x3__b__plane[k][0] >> 16;
				bsp_data[j].plane[k][1] = bsp[j].int_array_4x3__b__plane[k][1] >> 16;
				bsp_data[j].plane[k][2] = bsp[j].int_array_4x3__b__plane[k][2] >> 16;
			}
			bsp_data[j].direction = bsp[j].byte__a__direction;
			bsp_data[j].prev_scene = bsp[j].int__c__prev_scene;
			bsp_data[j].next_scene = bsp[j].int__d__next_scene;
			bsp_data[j].normal[0] = bsp[j].int_array_3__a__normal[0];
			bsp_data[j].normal[1] = bsp[j].int_array_3__a__normal[1];
			bsp_data[j].normal[2] = bsp[j].int_array_3__a__normal[2];
		}
	}

    // player position/direction
	model->start_rotation[0] = lv->int__dd__rotation;
	model->start_rotation[1] = lv->int__df__rotation;
	model->start_position[0] = lv->int_array__dg__translation[0] >> 16;
	model->start_position[1] = lv->int_array__dg__translation[1] >> 16;
	model->start_position[2] = lv->int_array__dg__translation[2] >> 16;

    // free
    delete_class__h__lvl(lv);

    return NL_TRUE;
}

NLboolean nlReadCT3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DItemModelData(const char* data, NLsizei res, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__g__item obj;
    class_h__function_S_void__item(&obj, (const byte *)data);
    NETLizard_3D_Item_Mesh *mesh = NEW(NETLizard_3D_Item_Mesh);
    ZERO(mesh, NETLizard_3D_Item_Mesh);
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
    if(ARRAY_DATA(obj.int_array_6__a__vertex) && ARRAY_DATA(obj.int_array_16__b__primitive))
	{
        // vertex
        mesh->item_mesh.vertex.count = obj.int_array_6__a__vertex.length / 2;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
		int j;
        NLint *im_v = (NLint *)(mesh->item_mesh.vertex.data);
        jint *i_a = (jint *)(obj.int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj.int_array_6__a__vertex.length; j += 6)
		{
			im_v[k] = i_a[j] >> 16;
			im_v[k + 1] = i_a[j + 1] >> 16;
			im_v[k + 2] = i_a[j + 2] >> 16;
			k += 3;
		}

        // index
        mesh->item_mesh.primitive.count = obj.int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
        jint *i_b = (jint *)(obj.int_array_16__b__primitive.array);
		k = 0;
        for(j = 0; j < obj.int_array_16__b__primitive.length; j += 16)
		{
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->item_mesh.primitive.data)) + k;
			p->index[0] = i_b[j] / 6;
			p->index[1] = i_b[j + 1] / 6;
			p->index[2] = i_b[j + 2] / 6;
			p->texcoord[0] = i_b[j + 3];
			p->texcoord[1] = i_b[j + 4];
			p->texcoord[2] = i_b[j + 5];
			p->texcoord[3] = i_b[j + 6];
			p->texcoord[4] = i_b[j + 7];
			p->texcoord[5] = i_b[j + 8];
			p->tex_index = i_b[j + 9];
			p->plane.position[0] = i_a[p->index[0]] >> 16;
			p->plane.position[1] = i_a[p->index[0] + 1] >> 16;
			p->plane.position[2] = i_a[p->index[0] + 2] >> 16;
			p->plane.normal[0] = i_b[j + 10];
			p->plane.normal[1] = i_b[j + 11];
			p->plane.normal[2] = i_b[j + 12];
			k++;
		}

        // box
        mesh->item_mesh.box.max[0] = obj.int__f__aabb >> 16;
        mesh->item_mesh.box.max[1] = obj.int__g__aabb >> 16;
        mesh->item_mesh.box.max[2] = obj.int__h__aabb >> 16;
        mesh->item_mesh.box.min[0] = obj.int__i__aabb >> 16;
        mesh->item_mesh.box.min[1] = obj.int__j__aabb >> 16;
        mesh->item_mesh.box.min[2] = obj.int__k__aabb >> 16;
	}

    // free
    delete_class__g__item(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
	model->has_sky = 0;
    model->type = NL_CT_3D_ITEM_MODEL;
    model->game = NL_CONTR_TERRORISM_3D;
    return NL_TRUE;
}

void read_CT3D_map_items(T_array(class__g__item) *class_g_array__h__item, const char *resc_path)
{
	int i;
	for(i = 0; i < class_g_array__h__item->length; i++)
	{
		class__g__item *h = ((class__g__item *)(class_g_array__h__item->array)) + i;
        jint i11 = h->byte__s__obj;
		if ((i11 != 0) || (i11 == 55))
        {
            char *name = make_resource_file_path(CT3D_OBJ_SUBFIX, i11, resc_path);
			array arr;
			int res = file_get_contents(name, &arr);
            free(name);
            if(res > 0)
			{
                class_h__function_S_void__item(h, arr.array);
                delete_array(&arr);
            }
            else
            {
                ARRAY_NULL(h->int_array_6__a__vertex);
                ARRAY_NULL(h->int_array_16__b__primitive);
            }
		}
		else
        {
            ARRAY_NULL(h->int_array_6__a__vertex);
            ARRAY_NULL(h->int_array_16__b__primitive);
		}
	}
}

/* ****** static ****** */

class__h__lvl class_h__function_k_void__scene(const byte arrayOfByte[], const char *resc_path)
{
	byte cO;
    jint cZ;
    jint cA;
    jint dg[3];
    jint dd;
    jint df;

	jint i1 = -2;
	i1 += 2;
	cO = ((byte)(marge_digit(arrayOfByte[0], arrayOfByte[1]) - 1));
	i1 += 2;
	dg[0] = (marge_digit(arrayOfByte[2], arrayOfByte[3]) << 16);
	i1 += 2;
	dg[1] = (marge_digit(arrayOfByte[4], arrayOfByte[5]) << 16);
	i1 += 2;
	dg[2] = (marge_digit(arrayOfByte[6], arrayOfByte[7]) << 16);
	i1 += 2;
	dd = marge_digit(arrayOfByte[8], arrayOfByte[9]);
	i1 += 2;
	df = marge_digit(arrayOfByte[10], arrayOfByte[11]);
	if (df > 361) {
		df -= 65536;
	}
	if (dd > 361) {
		dd -= 65536;
	}
	if (df < 0) {
		df = -df;
	} else {
		df = 360 - df;
	}
	if (df == 360) {
		df = 0;
	}
	if (dd < 0) {
		dd = 270 + dd;
	} else {
		dd = 270 - dd;
	}
	if (dd == 360) {
		dd = 0;
	}
	dprintfss("Get start position and axis");
	i1 += 2;
	cZ = marge_digit(arrayOfByte[12], arrayOfByte[13]);
	dprintfsi("Map scene count", cZ);

    T_array(class__f__scene) class_f_array__l__scene;
    new_array(&class_f_array__l__scene, sizeof(class__f__scene), cZ);
    class__f__scene *l = (class__f__scene *)(class_f_array__l__scene.array);
    jint i20;
    jint i25;
    jint i15;
	for (i15 = 0; i15 < cZ; i15++)
	{
		dprintfsi("Get map scene data", i15);
        jint i13 = 0;
		i1 += 2;
		l[i15].int__f__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		l[i15].int__g__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		l[i15].int__h__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		l[i15].int__i__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		l[i15].int__j__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		l[i15].int__k__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
        jint i9 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i9);
		i1 += 2;
        jint i8 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene primitive count", i8);
		i1 += 2;
        jint i10 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
        new_array(&l[i15].int_array_6__a__vertex, ESIZE(jint, 4), i9 * 6);
        jint *a = (jint *)(l[i15].int_array_6__a__vertex.array);
		l[i15].n = i9;
        new_array(&l[i15].int_array_10__b__primitive, ESIZE(jint, 4), i8 * 10);
        jint *b = (jint *)(l[i15].int_array_10__b__primitive.array);
		l[i15].o = i8;
		i10 *= 2;
        jint *arrayOfInt2 = NEW_II(jint, i8 * 3);
        jint *arrayOfInt1 = NEW_II(jint, i10);
		dprintfsi("Map normal count", i8);
		dprintfsi("Texcoord count", i10);
        jint i16;
		for (i16 = 0; i16 < i9; i16++)
		{
			i1 += 2;
			a[(i16 * 6)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			a[(i16 * 6 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			a[(i16 * 6 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		}
		dprintfss("Get scene vertex coord");
        jint i17;
		for (i17 = 0; i17 < i8; i17++)
		{
			i1 += 2;
			b[(i17 * 10)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
			i1 += 2;
			b[(i17 * 10 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
			i1 += 2;
			b[(i17 * 10 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
		}
		dprintfss("Get scene primitive vertex coord index");
        jint i18;
		for (i18 = 0; i18 < i8; i18++)
		{
			i1 += 2;
            arrayOfInt2[(i18 * 3)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
			i1 += 2;
            arrayOfInt2[(i18 * 3 + 1)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
			i1 += 2;
            arrayOfInt2[(i18 * 3 + 2)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		}
		dprintfss("Get map normal");
        jint i12 = 0;
        jint i19;
		for (i19 = 0; i19 < i8; i19++)
		{
            jint i14 = 1;
			for (i20 = i19 + 1; i20 < i8; i20++) {
				if ((arrayOfInt2[(i19 * 3)] == arrayOfInt2[(i20 * 3)]) && (arrayOfInt2[(i19 * 3 + 1)] == arrayOfInt2[(i20 * 3 + 1)]) && (arrayOfInt2[(i19 * 3 + 2)] == arrayOfInt2[(i20 * 3 + 2)]))
				{
					arrayOfInt2[(i19 * 3)] = 0;
					arrayOfInt2[(i19 * 3 + 1)] = 0;
					arrayOfInt2[(i19 * 3 + 2)] = 0;
					i14 = 0;
					break;
				}
			}
			if (i14 != 0) {
				i12++;
			}
		}
        dprintfsi("Scene plane count", i12);
        new_array(&l[i15].int_array_6__l__plane, ESIZE(jint, 4), i12 * 6);
        jint *sub_l = (jint *)(l[i15].int_array_6__l__plane.array);
		l[i15].m = (i12 * 6 - 6);
        jint i20;
		for (i20 = 0; i20 < i8; i20++) {
			if ((arrayOfInt2[(i20 * 3)] != 0) || (arrayOfInt2[(i20 * 3 + 1)] != 0) || (arrayOfInt2[(i20 * 3 + 2)] != 0))
			{
				sub_l[(i13 * 6)] = a[b[(i20 * 10)]];
				sub_l[(i13 * 6 + 1)] = a[(b[(i20 * 10)] + 1)];
				sub_l[(i13 * 6 + 2)] = a[(b[(i20 * 10)] + 2)];
				sub_l[(i13 * 6 + 3)] = arrayOfInt2[(i20 * 3)];
				sub_l[(i13 * 6 + 4)] = arrayOfInt2[(i20 * 3 + 1)];
				sub_l[(i13 * 6 + 5)] = arrayOfInt2[(i20 * 3 + 2)];
				i13++;
			}
		}
		dprintfss("Get scene plane normal index and point vertex coord index");
		// ??? + >>
		l[i15].c = ((l[i15].int__f__aabb + l[i15].int__i__aabb) >> 1);
		l[i15].d = ((l[i15].int__g__aabb + l[i15].int__j__aabb) >> 1);
        jint *arrayOfInt3 = (jint *)(l[i15].int_array_6__l__plane.array);
        jint i25;
		for (i25 = l[i15].m; i25 >= 0; i25 -= 6)
		{
            jlong l1; //!! long !!
			if ((l1 = (arrayOfInt3[(i25 + 3)] * (l[i15].c - arrayOfInt3[i25]) + arrayOfInt3[(i25 + 4)] * (l[i15].d - arrayOfInt3[(i25 + 1)])) >> 16) < 8192000L)
			{
                jlong l2 = 8192000L - l1; //!! long !!
				class__f__scene *tmp1584_1583 = l + i15;
                tmp1584_1583->c = ((jint)(tmp1584_1583->c + (arrayOfInt3[(i25 + 3)] * l2 >> 16)));
				class__f__scene *tmp1614_1613 = l + i15;
                tmp1614_1613->d = ((jint)(tmp1614_1613->d + (arrayOfInt3[(i25 + 4)] * l2 >> 16)));
			}
		}
        jint i26;
		for (i26 = 0; i26 < i10; i26 += 2)
		{
			i1 += 2;
            arrayOfInt1[i26] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
			i1 += 2;
            arrayOfInt1[(i26 + 1)] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
		}
		dprintfss("Get map texcoord");
        jint i27;
		for (i27 = 0; i27 < i8; i27++)
		{
			i1 += 2;
            b[(i27 * 10 + 3)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
            b[(i27 * 10 + 4)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
			i1 += 2;
            b[(i27 * 10 + 5)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
            b[(i27 * 10 + 6)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
			i1 += 2;
            b[(i27 * 10 + 7)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
            b[(i27 * 10 + 8)] = ((jshort)arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
		}
		dprintfss("Get scene texcoord");
        jint i28;
		for (i28 = 0; i28 < i8; i28++)
		{
			i1 += 2;
			b[(i28 * 10 + 9)] = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			//bU[l[i15].b[(i28 * 10 + 9)]] = true;
		}
		dprintfss("Get primitive texture index");
		free(arrayOfInt2);
		free(arrayOfInt1);
	}
	i1 += 2;
    jint i11;
	i11 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
    T_array(class__b__bsp) class_b_array__j__bsp;
    new_array(&class_b_array__j__bsp, sizeof(class__b__bsp), i11);
    class__b__bsp *j = (class__b__bsp *)(class_b_array__j__bsp.array);
    jint i2;
    jint i3;
    jint i4;
    jint i17;
	dprintfsi("Map BSP tree node count", i11);
	for (i17 = 0; i17 < i11; i17++)
	{
        jint i18;
		for (i18 = 0; i18 < 4; i18++)
		{
			i1 += 2;
			j[i17].int_array_4x3__b__plane[i18][0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			j[i17].int_array_4x3__b__plane[i18][1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			j[i17].int_array_4x3__b__plane[i18][2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		}
		i1 += 2;
        i2 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        i3 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        i4 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
		j[i17].int__c__prev_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		i1 += 2;
		j[i17].int__d__next_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		j[i17].int_array_3__a__normal[0] = i2;
		j[i17].int_array_3__a__normal[1] = i3;
		j[i17].int_array_3__a__normal[2] = i4;
		if ((j[i17].int__c__prev_scene == 999) || (j[i17].int__d__next_scene == 999)) {
			//this.dr = true;
		}
		if (i2 != 0) {
			j[i17].byte__a__direction = 1;
		} else if (i3 != 0) {
			j[i17].byte__a__direction = 2;
		} else if (i4 != 0) {
			j[i17].byte__a__direction = 3;
		}
	}
    jint i18;
	for (i18 = 0; i18 < i11; i18++)
	{
		if (j[i18].int__c__prev_scene < 999)
		{
			l[j[i18].int__c__prev_scene].int_array_5__p__bsp[l[j[i18].int__c__prev_scene].int__q__bsp] = i18;
			l[j[i18].int__c__prev_scene].int__q__bsp += 1;
		}
		if (j[i18].int__d__next_scene < 999)
		{
			l[j[i18].int__d__next_scene].int_array_5__p__bsp[l[j[i18].int__d__next_scene].int__q__bsp] = i18;
			l[j[i18].int__d__next_scene].int__q__bsp += 1;
		}
	}
	dprintfss("Get map BSP tree node");
	//testtt(l, j, 10, cZ);
	i1 += 2;
	cA = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map item count", cA);
    T_array(class__g__item) class_g_array__h__item;
    new_array(&class_g_array__h__item, sizeof(class__g__item), cA + 1);
    class__g__item *h = (class__g__item *)(class_g_array__h__item.array);
    jint i21 = 0;
    jint i24;
    jint i22;
	for (i22 = 0; i22 < cZ; i22++)
	{
		i1 += 2;
		i20 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsii("Scene item count", i22, i20);
		l[i22].int__r__begin = i21;
		l[i22].int__e__end = (i21 + i20);
        jint i23;
		for (i23 = 0; i23 < i20; i23++)
		{
            h[i21].r = ((jshort)i22);
			i1 += 2;
			h[i21].int__m__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			h[i21].int__n__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			h[i21].int__o__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			h[i21].int__p__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			i1 += 2;
			h[i21].int__q__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			if (h[i21].int__q__rotation > 360) {
				h[i21].int__q__rotation = (65536 - h[i21].int__q__rotation);
			}
			if (h[i21].int__p__rotation > 360) {
				h[i21].int__p__rotation = (65536 - h[i21].int__p__rotation);
			}
			if (h[i21].int__q__rotation < 0) {
				h[i21].int__q__rotation = (-h[i21].int__q__rotation);
			}
			if (h[i21].int__q__rotation == 360) {
				h[i21].int__q__rotation = 0;
			}
			if (h[i21].int__p__rotation < 0) {
				h[i21].int__p__rotation = (360 + h[i21].int__p__rotation);
			} else {
				h[i21].int__p__rotation = (360 - h[i21].int__p__rotation);
			}
			if (h[i21].int__p__rotation == 360) {
				h[i21].int__p__rotation = 0;
			}
			i1 += 2;
			h[i21].byte__s__obj = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			//bS[h[i21].s] = true;
			i1 += 2;
            h[i21].d = ((jshort)(byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			if (h[i21].d != 0)
			{
                h[i21].c = NEW_II(jint *, h[i21].d);
				for (i24 = 0; i24 < h[i21].d; i24++) {
                    h[i21].c[i24] = NEW_II(jint, 4);
				}
				for (i25 = 0; i25 < h[i21].d; i25++)
				{
					i1 += 2;
					h[i21].c[i25][0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
					i1 += 2;
					h[i21].c[i25][1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
					i1 += 2;
					h[i21].c[i25][2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
					i1 += 2;
					h[i21].c[i25][3] = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
				}
                jint i26;
				for (i26 = 0; i26 < h[i21].d - 1; i26++)
				{
                    jint i6;
					if ((i6 = h[i21].c[(i26 + 1)][3] - h[i21].c[i26][3]) > 1)
					{
						i2 = h[i21].c[i26][0];
						i3 = h[i21].c[i26][1];
						i4 = h[i21].c[i26][2];
                        jint i5 = h[i21].c[i26][3];
						free(h[i21].c[i26]);
                        h[i21].c[i26] = NEW_II(jint, 7);
						h[i21].c[i26][0] = i2;
						h[i21].c[i26][1] = i3;
						h[i21].c[i26][2] = i4;
						h[i21].c[i26][3] = i5;
						h[i21].c[i26][4] = ((h[i21].c[(i26 + 1)][0] - h[i21].c[i26][0]) / i6);
						h[i21].c[i26][5] = ((h[i21].c[(i26 + 1)][1] - h[i21].c[i26][1]) / i6);
						h[i21].c[i26][6] = ((h[i21].c[(i26 + 1)][2] - h[i21].c[i26][2]) / i6);
					}
				}
                // free unused
				for (i24 = 0; i24 < h[i21].d; i24++) {
					free(h[i21].c[i24]);
				}
				free(h[i21].c);
			}
			i21++;
		}
	}
    read_CT3D_map_items(&class_g_array__h__item, resc_path);

	i1 += 2;
#if UNPARSED
	dR = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) + 1;
	al = new byte[dR];
	n = new j[dR];
    for (jint i23 = 0; i23 < dR; i23++) {
		n[i23] = new j(null);
	}
	i21 = 1;
	n[0].p = dg[0];
	n[0].q = dg[1];
	n[0].r = dg[2];
#endif
    jint i26;
	for (i26 = 0; i26 < cZ; i26++)
	{
		i1 += 2;
		i24 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
        jint i27;
		for (i27 = 0; i27 < i24; i27++)
		{
			i1 += 2;
			//n[i21].p = (a(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//n[i21].z = n[i21].p;
			i1 += 2;
			//n[i21].q = (a(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//n[i21].A = n[i21].q;
			i1 += 2;
			//n[i21].r = (a(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			//n[i21].C = ((byte)a(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
#if UNPARSED
			i25 = n[i21].C;
			bV[i25] = true;
			if ((i25 == 4) || (i25 == 5))
			{
				bt = (byte)(bt + 1);
				this.ei = ((byte)i21);
			}
			else
			{
				dK = (byte)(dK + 1);
			}
            n[i21].t = ((jshort)i26);
			i21++;
#endif
		}
	}
#if UNPARSED
    for (jint i27 = dR - 1; i27 >= 0; i27--)
	{
		f(n[i27]);
		if (i27 == 0)
		{
			cY = n[i27].t;
			b(n[i27].D);
		}
		else
		{
			c(n[i27], 10, 0, 0, 1);
		}
	}
    n[0].t = ((jshort)cY);
#endif

    class__h__lvl lv;
    ZERO(&lv, class__h__lvl);
    lv.class_f_array__l__scene = class_f_array__l__scene;
    lv.class_g_array__h__item = class_g_array__h__item;
    lv.int__dd__rotation = dd;
    lv.int__df__rotation = df;
    lv.int_array__dg__translation[0] = dg[0];
    lv.int_array__dg__translation[1] = dg[1];
    lv.int_array__dg__translation[2] = dg[2];
    lv.class_b_array__j__bsp = class_b_array__j__bsp;
	return lv;
}

void class_h__function_S_void__item(class__g__item *h, const byte arrayOfByte[])
{
    jint i1 = -2;
	i1 += 2;
	h->int__f__aabb = (marge_digit(arrayOfByte[0], arrayOfByte[1]) << 16);
	i1 += 2;
	h->int__g__aabb = (marge_digit(arrayOfByte[2], arrayOfByte[3]) << 16);
	i1 += 2;
	h->int__h__aabb = (marge_digit(arrayOfByte[4], arrayOfByte[5]) << 16);
	i1 += 2;
	h->int__i__aabb = (marge_digit(arrayOfByte[6], arrayOfByte[7]) << 16);
	i1 += 2;
	h->int__j__aabb = (marge_digit(arrayOfByte[8], arrayOfByte[9]) << 16);
	i1 += 2;
	h->int__k__aabb = (marge_digit(arrayOfByte[10], arrayOfByte[11]) << 16);
    jint i2 = h->int__f__aabb >> 16;
    jint i3 = h->int__g__aabb >> 16;
    jint i4 = h->int__h__aabb >> 16;
    jint i5 = i2 * i2 + i3 * i3 + i4 * i4;
    jint i6 = 65536;
    jint i7 = 0;
	while (i6 >>= 1 > 0) {
		if ((i7 + i6) * (i7 + i6) <= i5) {
			i7 += i6;
		}
	}
	//h->d = ((i7 << 1) * 240);
	//h->c = ((i7 << 1) * 320);
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i9 = marge_digit(arrayOfByte[20], arrayOfByte[21]);
	i1 += 2;
    jint i8 = marge_digit(arrayOfByte[22], arrayOfByte[23]);
	i1 += 2;
    jint i10 = marge_digit(arrayOfByte[24], arrayOfByte[25]);
    //h->k = i9;
    new_array(&h->int_array_6__a__vertex, ESIZE(jint, 4), i9 * 6);
    new_array(&h->int_array_16__b__primitive, ESIZE(jint, 4), i8 * 16);
    jint *a = (jint *)(h->int_array_6__a__vertex.array);
    jint *b = (jint *)(h->int_array_16__b__primitive.array);
	i10 *= 2;
    jint *arrayOfInt = NEW_II(jint, i10);
    jint i12;
	for (i12 = 0; i12 < i9; i12++)
	{
		i1 += 2;
		a[(i12 * 6)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i12 * 6 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i12 * 6 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	}
    jint i13;
	for (i13 = 0; i13 < i8; i13++)
	{
		i1 += 2;
		b[(i13 * 16)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i13 * 16 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i13 * 16 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
	}
    jint i14;
	for (i14 = 0; i14 < i8; i14++)
	{
		i1 += 2;
        b[(i14 * 16 + 10)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i14 * 16 + 11)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i14 * 16 + 12)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
	}
    jint i15;
	for (i15 = 0; i15 < i10; i15 += 2)
	{
		i1 += 2;
        arrayOfInt[i15] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i15 + 1)] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
	}
    jint i16;
	for (i16 = 0; i16 < i8; i16++)
	{
		i1 += 2;
        b[(i16 * 16 + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 4)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i16 * 16 + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 6)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i16 * 16 + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 8)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2 + 1)]);
	}
    jint i17;
	for (i17 = 0; i17 < i8; i17++)
	{
		i1 += 2;
		b[(i17 * 16 + 9)] = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		//bU[g[i11].b[(i17 * 16 + 9)]] = true;
	}
	free(arrayOfInt);
	/*
		 bU[60] = true;
		 for (int i12 = 0; i12 < h.length; i12++) {
		 if (i12 == h.length - 1)
		 {
		 h[i12].a = g[49].a;
		 h[i12].b = g[49].b;
		 h[i12].f = g[49].e;
		 h[i12].i = g[49].h;
		 h[i12].g = g[49].f;
		 h[i12].j = g[49].i;
		 h[i12].h = g[49].g;
		 h[i12].k = g[49].j;
		 }
		 else
		 {
		 h[i12].a = g[h[i12].s].a;
		 h[i12].b = g[h[i12].s].b;
		 h[i12].f = g[h[i12].s].e;
		 h[i12].i = g[h[i12].s].h;
		 h[i12].g = g[h[i12].s].f;
		 h[i12].j = g[h[i12].s].i;
		 h[i12].h = g[h[i12].s].g;
		 h[i12].k = g[h[i12].s].j;
		 if ((h[i12].s == 55) || (h[i12].s == 53)) {
		 h[i12].e = (h[i12].o - h[i12].h - 8192000);
		 }
		 if ((h[i12].s == 54) || (h[i12].s == 52)) {
		 h[i12].e = (h[i12].o + h[i12].h + 8192000);
		 }
		 }
		 }
		 */
}
