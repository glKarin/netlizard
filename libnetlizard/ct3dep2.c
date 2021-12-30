#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// ct3d ep2 no jad version v1?

#define dprintfsi(s, d) nllogfln("CT 3D Ep2 : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("CT 3D Ep2 : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("CT 3D Ep2 : "#s)

typedef struct _class__h__scene // 地图模型场景
{
  short_array short_array_3__a__vertex; // short 3顶点坐标
  int_array int_array_7__b__primitive; // int 3索引 3纹理坐标索引(i, i + 1) 1纹理索引
  jint c;
  jint d;
  jint int__e__end; // = 0; 最后一个物品索引
  jint int__f__aabb; // AABB盒子
  jint int__g__aabb; // AABB盒子
  jint int__h__aabb; // AABB盒子
  jint int__i__aabb; // AABB盒子
  jint int__j__aabb; // AABB盒子
  jint int__k__aabb; // AABB盒子
  int_array int_array_2__l__plane; // int 碰撞面 1法线索引(i, i + 1, i + 2) 1顶点索引(i, i + 1, i + 2)
  jint m;
  jint n;
  jint int_array_7__o__bsp[7]; // -1 -1 -1 -1 -1 -1 -1
  jint int__p__bsp; // = 0;
  jint int__q__begin; // = 0; 第一个物品索引
} class__h__scene;

typedef struct _class__c__bsp
{
  byte byte__a__direction;
  jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; // n[] b = new n[4];
  jint int__c__prev_scene;
  jint int__d__next_scene;
  jboolean e; // = false;
} class__c__bsp;

typedef struct _class__k__item // 物品
{
  int_array int_array_6__a__vertex; // int 顶点坐标
  int_array int_array_16__b__primitive; // int 纹理 纹理索引
  jboolean c;
  jboolean d;
  jshort e; // = 0;
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  jint int__j__aabb;
  jint int__k__aabb;
  jboolean l;
  jint int__m__translation; // 场景坐标
  jint int__n__translation; // 场景坐标
  jint int__o__translation; // 场景坐标
  jint int__p__rotation; // = 0; 旋转
  jint int__q__rotation; // = 0; 旋转
  jshort r;
  jboolean s;
  byte byte__t__obj;
  jboolean u; // = true;
} class__k__item;

typedef struct _class__l__lvl
{
  T_array(class__h__scene) class_h_array__ar__scene; // 场景
    int_array int_array__ak__normal; // 法线
    int_array int_array__dP__texcoord; // 纹理坐标
  T_array(class__k__item) class_k_array__an__item; // 物品
  jint int_array__dw__translation[3]; // 主角初始坐标
  jint int__dt__rotation; // 主角初始视角
  jint int__dv__rotation; // 主角初始视角;
  T_array(class__c__bsp) class_c_array__ap__bsp;
} class__l__lvl;

static void delete_class__h__scene(class__h__scene *scene)
{
    delete_array(&scene->short_array_3__a__vertex);
    delete_array(&scene->int_array_7__b__primitive);
    delete_array(&scene->int_array_2__l__plane);
}

static void delete_class__k__item(class__k__item *item)
{
    delete_array(&item->int_array_6__a__vertex);
    delete_array(&item->int_array_16__b__primitive);
}

static void delete_class__l__lvl(class__l__lvl *lv)
{
    int i;

    T_array(class__h__scene) *h_ar = &lv->class_h_array__ar__scene;
    for(i = 0; i < h_ar->length; i++)
    {
        class__h__scene *obj = ((class__h__scene *)(h_ar->array)) + i;
        delete_class__h__scene(obj);
    }
    delete_array(h_ar);

    T_array(class__k__item) *k_an = &lv->class_k_array__an__item;
    for(i = 0; i < k_an->length; i++)
    {
        class__k__item *obj = ((class__k__item *)(k_an->array)) + i;
        delete_class__k__item(obj);
    }
    delete_array(k_an);
    delete_array(&lv->class_c_array__ap__bsp);
}

static void read_CT3DEp2_map_items(T_array(class__k__item) *k_an, const char *resc_path);
static class__l__lvl class_l__function_h_1int__scene(const byte paramInt[], const char *resc_path); // 场景解析
static void class_l__function_I_void__item(class__k__item *an, const byte arrayOfByte1[]); // 物品解析

NLboolean nlReadCT3DEp2ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DEp2ModelData(arr.array, arr.length, level, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DEp2ModelData(const char* data, NLsizei res, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
	int paramInt = level;
    int dr = 0; // has sky
	if ((paramInt == 1) || (paramInt == 2) || (paramInt == 4) || (paramInt == 7)) {
		dr = 1;
	}
    byte *arrayOfByte = (byte *)(data);
    class__l__lvl lvl = class_l__function_h_1int__scene(arrayOfByte, resc_path);
    class__l__lvl *lv = &lvl;

	ZERO(model, NETLizard_3D_Model);
    T_array(class__h__scene) *h_ar = &lv->class_h_array__ar__scene;
    jint *dP = (jint *)(lv->int_array__dP__texcoord.array);
    jint *ak = (jint *)(lv->int_array__ak__normal.array);
    model->data.data = NEW_II(NETLizard_3D_Mesh, h_ar->length);
    model->data.count = h_ar->length;
	model->has_sky = dr;
    model->type = NL_CT_3D_EP2_MAP_MODEL;
    model->game = NL_CONTR_TERRORISM_3D_EPISODE_2;

	int i;
	for(i = 0; i < h_ar->length; i++)
	{
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
		class__h__scene *obj = ((class__h__scene *)(h_ar->array)) + i;

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
        mesh->primitive.count = obj->int_array_7__b__primitive.length / 7;
        mesh->primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->vertex.count);
        jint *s_b = (jint *)(obj->int_array_7__b__primitive.array);
		int k = 0;
        for(j = 0; j < obj->int_array_7__b__primitive.length; j += 7)
		{
            NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->primitive.data)) + k;
			p->index[0] = s_b[j];
			p->index[1] = s_b[j + 1]; 
			p->index[2] = s_b[j + 2];
			p->texcoord[0] = dP[(s_b[j + 3])];
			p->texcoord[1] = dP[(s_b[j + 3]) + 1];
			p->texcoord[2] = dP[(s_b[j + 4])];
			p->texcoord[3] = dP[(s_b[j + 4]) + 1];
			p->texcoord[4] = dP[(s_b[j + 5])];
			p->texcoord[5] = dP[(s_b[j + 5]) + 1];
			p->tex_index = (s_b[j + 6]) & 0xff;
			k++;
		}

        // plane
        mesh->plane.count = obj->int_array_2__l__plane.length / 2;
        mesh->plane.data = NEW_II(NETLizard_3D_Plane, mesh->plane.count);
        jint *l = (jint *)(obj->int_array_2__l__plane.array);
		k = 0;
        for(j = 0; j < obj->int_array_2__l__plane.length; j += 2)
		{
            jint normal_index = l[j];
            jint position_index = l[j + 1];
            NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
			plane->normal[0] = ak[normal_index];
			plane->normal[1] = ak[normal_index + 1];
			plane->normal[2] = ak[normal_index + 2];
			plane->position[0] = s_a[position_index];
			plane->position[1] = s_a[position_index + 1];
			plane->position[2] = s_a[position_index + 2];
			k++;
		}
        mesh->box.max[0] = obj->int__f__aabb >> 16;
        mesh->box.max[1] = obj->int__g__aabb >> 16;
        mesh->box.max[2] = obj->int__h__aabb >> 16;
        mesh->box.min[0] = obj->int__i__aabb >> 16;
        mesh->box.min[1] = obj->int__j__aabb >> 16;
        mesh->box.min[2] = obj->int__k__aabb >> 16;
		mesh->item_index_range[0] = obj->int__q__begin;
		mesh->item_index_range[1] = obj->int__e__end;

        // bsp
        mesh->bsp.data = NULL;
        mesh->bsp.count = 0;
		if(obj->int__p__bsp)
		{
            mesh->bsp.count = obj->int__p__bsp;
            mesh->bsp.data = NEW_II(NLint, mesh->bsp.count);
            NLint *bsp_index = (NLint *)(mesh->bsp.data);
			for(j = 0; j < obj->int__p__bsp; j++)
			{
				bsp_index[j] = obj->int_array_7__o__bsp[j];
			}
		}
	}

    // item model
    T_array(class__k__item) *k_an = &lv->class_k_array__an__item;
    model->item_data.count = k_an->length;
    model->item_data.data = NEW_II(NETLizard_3D_Item_Mesh, model->item_data.count);
	for(i = 0; i < k_an->length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
		class__k__item *obj = ((class__k__item *)(k_an->array)) + i;
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
		mesh->obj_index = obj->byte__t__obj;

        // box
        mesh->item_mesh.box.max[0] = obj->int__f__aabb >> 16;
        mesh->item_mesh.box.max[1] = obj->int__g__aabb >> 16;
        mesh->item_mesh.box.max[2] = obj->int__h__aabb >> 16;
        mesh->item_mesh.box.min[0] = obj->int__i__aabb >> 16;
        mesh->item_mesh.box.min[1] = obj->int__j__aabb >> 16;
        mesh->item_mesh.box.min[2] = obj->int__k__aabb >> 16;

        // vertex
        mesh->item_mesh.vertex.count = obj->int_array_6__a__vertex.length / 2;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.count);
		int j;
        NLint *im_v = (NLint *)(mesh->item_mesh.vertex.data);
        jint *i_a = (jint *)(obj->int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj->int_array_6__a__vertex.length; j += 6)
		{
			im_v[k] = i_a[j] >> 16;
			im_v[k + 1] = i_a[j + 1] >> 16;
			im_v[k + 2] = i_a[j + 2] >> 16;
			k += 3;
		}

        // index
        mesh->item_mesh.primitive.count = obj->int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
        jint *i_b = (jint *)(obj->int_array_16__b__primitive.array);
		k = 0;
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
			p->plane.position[0] = i_a[p->index[0] * 3] >> 16;
			p->plane.position[1] = i_a[p->index[0] * 3 + 1] >> 16;
			p->plane.position[2] = i_a[p->index[0] * 3 + 2] >> 16;
			p->plane.normal[0] = i_b[j + 10];
			p->plane.normal[1] = i_b[j + 11];
			p->plane.normal[2] = i_b[j + 12];
			k++;
		}
	}

    // bsp
    model->bsp_data.data = NULL;
    model->bsp_data.count = 0;
    if(ARRAY_DATA(lv->class_c_array__ap__bsp))
	{
        class__c__bsp *bsp = (class__c__bsp *)(lv->class_c_array__ap__bsp.array);
        model->bsp_data.count = lv->class_c_array__ap__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_c_array__ap__bsp.length; j++)
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
	model->start_rotation[0] = lv->int__dt__rotation;
	model->start_rotation[1] = lv->int__dv__rotation;
	model->start_position[0] = lv->int_array__dw__translation[0] >> 16;
	model->start_position[1] = lv->int_array__dw__translation[1] >> 16;
	model->start_position[2] = lv->int_array__dw__translation[2] >> 16;

    // free
    delete_class__l__lvl(lv);

    return NL_TRUE;
}

NLboolean nlReadCT3DEp2ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DEp2ItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DEp2ItemModelData(const char* data, NLsizei res, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__k__item obj;
    class_l__function_I_void__item(&obj, (const byte *)data);
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
        jint *i_b = (jint *)(obj.int_array_16__b__primitive.array);
        mesh->item_mesh.primitive.count = obj.int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.count);
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
			p->plane.position[0] = i_a[p->index[0] * 3] >> 16;
			p->plane.position[1] = i_a[p->index[0] * 3 + 1] >> 16;
			p->plane.position[2] = i_a[p->index[0] * 3 + 2] >> 16;
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
    delete_class__k__item(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
	model->has_sky = 0;
    model->type = NL_CT_3D_EP2_ITEM_MODEL;
    model->game = NL_CONTR_TERRORISM_3D_EPISODE_2;
    return NL_TRUE;
}

void read_CT3DEp2_map_items(T_array(class__k__item) *k_an, const char *resc_path)
{
	int i;
	for(i = 0; i < k_an->length; /* 45 */i++)
	{
		class__k__item *an = ((class__k__item *)(k_an->array)) + i;
        jint i11 = an->byte__t__obj;

        char *name = make_resource_file_path(CT3DEP2_OBJ_SUBFIX, i11, resc_path);
        array arr;
        int res = file_get_contents(name, &arr);
		free(name);
        ARRAY_NULL(an->int_array_6__a__vertex);
        ARRAY_NULL(an->int_array_16__b__primitive);
        if(res > 0)
        {
            ARRAY_NULL(an->int_array_6__a__vertex);
            ARRAY_NULL(an->int_array_16__b__primitive);
            class_l__function_I_void__item(an, arr.array);
            delete_array(&arr);
        }
        else
        {
            ARRAY_NULL(an->int_array_6__a__vertex);
            ARRAY_NULL(an->int_array_16__b__primitive);
        }
	}
}

/* ****** static ****** */

class__l__lvl class_l__function_h_1int__scene(const byte paramInt[], const char *resc_path)
{
    jint i2;
  jint dw[3];
	//byte *cZ = NULL;
  jshort dh;
  jshort di;
  jshort dj;
  jint dt;
  jint dv;
  jint jdField_do;
  int_array int_ak;
  int_array int_dP;
  T_array(class__h__scene) h_ar;
  //byte *cL = NULL;
  jint cI;
  T_array(class__k__item) k_an;

    jint i1 = -2;
	i1 += 2;
	if ((i2 = marge_digit(paramInt[0], paramInt[1])) != 0) {
		//cZ = NEW_II(byte, i2);
	}
    jint i3;
	for (i3 = 0; i3 < i2; i3++)
	{
		i1 += 2;
		//cZ[i3] = ((byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
	}
	i1 += 2;
	dw[0] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	i1 += 2;
	dw[1] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	i1 += 2;
	dw[2] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	dh = dw[0] >> 16;
	di = dw[1] >> 16;
	dj = dw[2] >> 16;
	i1 += 2;
	dt = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	i1 += 2;
	if ((dv = marge_digit(paramInt[i1], paramInt[(i1 + 1)])) > 361) {
		dv -= 65536;
	}
	if (dt > 361) {
		dt -= 65536;
	}
	if (dv < 0) {
		dv = -dv;
	} else {
		dv = 360 - dv;
	}
	if (dv == 360) {
		dv = 0;
	}
	if (dt < 0) {
		dt = 270 + dt;
	} else {
		dt = 270 - dt;
	}
	if (dt == 360) {
		dt = 0;
	}
	dprintfss("Get start position and axis");
	i1 += 2;
	jdField_do = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map scene count", jdField_do);
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map normal count", i2);

    new_array(&int_ak, ESIZE(jint, 4), i2);
    jint *ak = (jint *)(int_ak.array);
	for (i3 = 0; i3 < i2; i3 += 3)
	{
		i1 += 2;
        ak[i3] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
        ak[(i3 + 1)] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
        ak[(i3 + 2)] = ((jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
	}
	dprintfss("Get map normal");
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Texcoord count", i2);

    new_array(&int_dP, ESIZE(jint, 4), i2);
    jint *dP = (jint *)(int_dP.array);
	for (i3 = 0; i3 < i2; i3 += 2)
	{
		i1 += 2;
        dP[i3] = ((jint)(64 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
		i1 += 2;
        dP[(i3 + 1)] = ((jint)(64 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");

    new_array(&h_ar, sizeof(class__h__scene), jdField_do);
    class__h__scene *ar = (class__h__scene *)(h_ar.array);
	//cL = NEW_II(byte, jdField_do);
    jint i4;
    jint i5;
	for (i4 = 0; i4 < jdField_do; i4++)
	{
		dprintfsi("Get map scene data", i4);
		i1 += 2;
		ar[i4].int__f__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		ar[i4].int__g__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		ar[i4].int__h__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		ar[i4].int__i__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		ar[i4].int__j__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		ar[i4].int__k__aabb = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
		i3 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i3);
		i1 += 2;
		i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsi("Scene primitive count", i2);
        new_array(&ar[i4].short_array_3__a__vertex, ESIZE(jshort, 2), i3 * 3);
        jshort *a = (jshort *)(ar[i4].short_array_3__a__vertex.array);
        ar[i4].m = i3;
        new_array(&ar[i4].int_array_7__b__primitive, ESIZE(jint, 4), i2 * 7);
        jint *b = (jint *)(ar[i4].int_array_7__b__primitive.array);
 		ar[i4].n = i2;
        jint i7;
		for (i7 = 0; i7 < i3; i7++)
		{
			i1 += 2;
            a[(i7 * 3)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
            a[(i7 * 3 + 1)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
            a[(i7 * 3 + 2)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
		}
		dprintfss("Get scene vertex coord");
        jint *arrayOfInt2 = (jint *)(ar[i4].int_array_7__b__primitive.array);
		for (i5 = 0; i5 < i2; i5++)
		{
			i3 = i5 * 7;
			i1 += 2;
			b[i3] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3);
			i1 += 2;
			b[(i3 + 1)] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3);
			i1 += 2;
			b[(i3 + 2)] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3);
		}
		i1 += 2;
		dprintfss("Get scene primitive vertex coord index");
		i3 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
        ;
        new_array(&ar[i4].int_array_2__l__plane, ESIZE(jint, 4), i3 << 1);
		dprintfsi("Scene plane count", i3);
        jint *l = (jint *)(ar[i4].int_array_2__l__plane.array);
        jint i9;
		for (i9 = 0; i9 < i3; i9++)
		{
			i1 += 2;
			l[(i9 << 1)] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 3);
			i1 += 2;
			l[((i9 << 1) + 1)] = arrayOfInt2[(marge_digit(paramInt[i1], paramInt[(i1 + 1)]) * 7)];
		}
		dprintfss("Get scene plane normal index and point vertex coord index");
		ar[i4].c = (ar[i4].int__f__aabb + ar[i4].int__i__aabb >> 1);
		ar[i4].d = (ar[i4].int__g__aabb + ar[i4].int__j__aabb >> 1);
        jint i8;
		for (i8 = 0; i8 < i2; i8++)
		{
			i3 = i8 * 7;
			i1 += 2;
            b[(i3 + 3)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
			i1 += 2;
            b[(i3 + 4)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
			i1 += 2;
            b[(i3 + 5)] = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
		}
		dprintfss("Get scene texcoord index");
		i1++;
		for (i9 = 0; i9 < i2; i9++)
		{
			i1++;
			if ((i3 = paramInt[i1]) < 0) {
				i3 += 256;
			}
			b[(i9 * 7 + 6)] = i3;
			//cl[ar[i4].b[(i9 * 7 + 6)]] = true;
		}
		for (i8 = 0; i8 < i2; i8++)
		{
			i1++;
			if ((i5 = marge_digit(paramInt[i1], paramInt[(i1 + 1)])) < 0) {
				i5 += 256;
			}
			i3 = b[(i8 * 7 + 6)] & 0xFF;
			b[(i8 * 7 + 6)] = ((i5 << 8) + i3);
		}
		dprintfss("Get primitive texture index");
		i1--;
	}
	i1 += 2;
	i5 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map BSP tree node count", i5);
    T_array(class__c__bsp) class_c_array__ap__bsp;
    new_array(&class_c_array__ap__bsp, sizeof(class__c__bsp), i5);
    class__c__bsp *ap = (class__c__bsp *)(class_c_array__ap__bsp.array);
    jint i8;
	for (i8 = 0; i8 < i5; i8++)
	{
		for (i3 = 0; i3 < 4; i3++)
		{
			i1 += 2;
			ap[i8].int_array_4x3__b__plane[i3][0] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			ap[i8].int_array_4x3__b__plane[i3][1] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			ap[i8].int_array_4x3__b__plane[i3][2] = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		}
		i1 += 2;
        i2 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
        i3 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
        i4 = (jint)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
		ap[i8].int__c__prev_scene = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) - 1);
		i1 += 2;
		ap[i8].int__d__next_scene = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) - 1);
		ap[i8].int_array_3__a__normal[0] = i2;
		ap[i8].int_array_3__a__normal[1] = i3;
		ap[i8].int_array_3__a__normal[2] = i4;
		if ((ap[i8].int__c__prev_scene == 999) || (ap[i8].int__d__next_scene == 999)) {
			//this.dE = true;
		}
		if (i2 != 0) {
			ap[i8].byte__a__direction = 1;
		} else if (i3 != 0) {
			ap[i8].byte__a__direction = 2;
		} else if (i4 != 0) {
			ap[i8].byte__a__direction = 3;
		}
	}
	for (i3 = 0; i3 < i5; i3++)
	{
		if (ap[i3].int__c__prev_scene < 999)
		{
			ar[ap[i3].int__c__prev_scene].int_array_7__o__bsp[ar[ap[i3].int__c__prev_scene].int__p__bsp] = i3;
			ar[ap[i3].int__c__prev_scene].int__p__bsp += 1;
		}
		if (ap[i3].int__d__next_scene < 999)
		{
			ar[ap[i3].int__d__next_scene].int_array_7__o__bsp[ar[ap[i3].int__d__next_scene].int__p__bsp] = i3;
			ar[ap[i3].int__d__next_scene].int__p__bsp += 1;
		}
	}
#if 0
	for (i3 = 0; i3 < h_ar->length; i3++)
	{
		int *arrayOfInt1 = ar[i3].int_array_7__o__bsp;
		ar[i3].int_array_7__o__bsp = NULL;
		int i9 = ar[i3].int__p__bsp;
        ar[i3].int_array_7__o__bsp = NEW_II(jint, i9);
		for (i8 = 0; i8 < i9; i8++) {
			ar[i3].int_array_7__o__bsp[i8] = arrayOfInt1[i8];
		}
	}
#endif
	dprintfss("Get map BSP tree node");
	i1 += 2;
	cI = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Map item count", cI);
    new_array(&k_an, sizeof(class__k__item), cI + 1);
    class__k__item *an = (class__k__item *)(k_an.array);
    jint i9;
	i3 = 0;
    jint i6;
	for (i6 = 0; i6 < jdField_do; i6++)
	{
		i1 += 2;
		i8 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsii("Scene item count", i6, i8);
		ar[i6].int__q__begin = i3;
		ar[i6].int__e__end = (i3 + i8);
		for (i9 = 0; i9 < i8; i9++)
		{
            an[i3].r = ((jshort)i6);
			i1 += 2;
			an[i3].int__m__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			an[i3].int__n__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			an[i3].int__o__translation = (marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			an[i3].int__p__rotation = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
			i1 += 2;
			an[i3].int__q__rotation = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
			if (an[i3].int__q__rotation > 360) {
				an[i3].int__q__rotation = (65536 - an[i3].int__q__rotation);
			}
			if (an[i3].int__p__rotation > 360) {
				an[i3].int__p__rotation = (65536 - an[i3].int__p__rotation);
			}
			if (an[i3].int__q__rotation < 0) {
				an[i3].int__q__rotation = (-an[i3].int__q__rotation);
			}
			if (an[i3].int__q__rotation == 360) {
				an[i3].int__q__rotation = 0;
			}
			if (an[i3].int__p__rotation < 0) {
				an[i3].int__p__rotation = (360 + an[i3].int__p__rotation);
			} else {
				an[i3].int__p__rotation = (360 - an[i3].int__p__rotation);
			}
			if (an[i3].int__p__rotation == 360) {
				an[i3].int__p__rotation = 0;
			}
			i1 += 2;
			an[i3].byte__t__obj = ((byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (an[i3].byte__t__obj == 7) {
				an[i3].byte__t__obj = an[i3].byte__t__obj;
			}
			if (an[i3].byte__t__obj < 120)
			{
				/*
					 if ((an[i3].byte__t__obj > 37) && (an[i3].byte__t__obj < 42)) {
					 cL[i6] = 1;
					 }
					 */
				if (an[i3].byte__t__obj >= 90)
				{
					an[i3].byte__t__obj -= 50;
					class__k__item *tmp2878_2877 = an + i3;
					tmp2878_2877->byte__t__obj = ((byte)(tmp2878_2877->byte__t__obj - 50));
					//cL[i6] = -1;
				}
			}
			//ck[an[i3].t] = true;
			i1 += 2;
            an[i3].e = ((jshort)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (an[i3].e < 0)
			{
				class__k__item *tmp2952_2951 = an + i3;
                tmp2952_2951->e = ((jshort)(tmp2952_2951->e + 256));
				an[i3].e += 256;
			}
			i1 += 2;
			i3++;
		}
	}

	i1 += 2;
#if UNPARSED
	bi = new byte[dY = b(paramInt[i1], paramInt[(i1 + 1)]) + 1];
	h = new o[dY];
	for (i9 = 0; i9 < dY; i9++) {
		h[i9] = new o(0);
	}
	i3 = 1;
	h[0].n = dw[0];
	h[0].o = dw[1];
	h[0].p = dw[2];
#endif
	for (i4 = 0; i4 < jdField_do; i4++)
	{
		i1 += 2;
		i8 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		for (i6 = 0; i6 < i8; i6++)
		{
			i1 += 2;
			//h[i3].n = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			//h[i3].v = h[i3].n;
			i1 += 2;
			//h[i3].o = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			//h[i3].w = h[i3].o;
			i1 += 2;
			//h[i3].p = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
			i1 += 2;
			//h[i3].z = ((byte)b(paramInt[i1], paramInt[(i1 + 1)]));
#if UNPARSED
			if ((i2 = h[i3].z) > 20)
			{
				i2 -= 20;
				h[i3].u = true;
				h[i3].B = false;
			}
			else if (i2 > 10)
			{
				i2 -= 10;
				h[i3].u = true;
			}
			h[i3].z = ((byte)i2);
			if (h[i3].z == 0) {
				h[i3].z = h[i3].z;
			}
			if (i2 != 7) {
				cm[i2] = true;
			}
#endif
			i1 += 2;
#if UNPARSED
			if ((i2 = b(paramInt[i1], paramInt[(i1 + 1)])) > 360) {
				i2 -= 65536;
			}
			if (i2 < 0) {
				i2 = -i2;
			} else {
				i2 = 360 - i2;
			}
			if (i2 == 360) {
				i2 = 0;
			}
			h[i3].s[0] = (-dC[i2]);
			h[i3].s[1] = (-br[i2]);
			h[i3].A[0] = (-dC[i2]);
			h[i3].A[1] = (-br[i2]);
			h[i3].r = ((short)i4);
			i3++;
#endif
		}
	}
	i1 += 2;
	i2 = (byte)marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	//this.cf = i2;
	//cg = new short[i2 * 3];
	for (i6 = 0; i6 < i2; i6++)
	{
		i1 += 2;
		//cg[(i6 * 3)] = ((short)b(paramInt[i1], paramInt[(i1 + 1)]));
		i1 += 2;
		//cg[(i6 * 3 + 1)] = ((short)b(paramInt[i1], paramInt[(i1 + 1)]));
		i1 += 2;
		//cg[(i6 * 3 + 2)] = ((short)b(paramInt[i1], paramInt[(i1 + 1)]));
	}
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	//cW = false;
	if (i2 == 1)
	{
		//cW = true;
		//ao = new int[7];
		i1 += 2;
		//ao[0] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[1] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[2] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[3] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[4] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[5] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
		i1 += 2;
		//ao[6] = (b(paramInt[i1], paramInt[(i1 + 1)]) << 16);
	}
#if UNPARSED
	for (paramInt = dY - 1; paramInt >= 0; paramInt--)
	{
		b(h[paramInt]);
		if (paramInt == 0)
		{
			dm = h[paramInt].r;
			b(h[paramInt].A);
		}
	}
	h[0].r = ((short)dm);
	w();
	if (R) {
		k(0);
	}
#endif

    read_CT3DEp2_map_items(&k_an, resc_path);

    class__l__lvl lv;
    ZERO(&lv, class__l__lvl);
    lv.class_h_array__ar__scene = h_ar;
    lv.class_k_array__an__item = k_an;
    lv.int_array__ak__normal = int_ak;
    lv.int_array__dP__texcoord = int_dP;
    lv.int__dt__rotation = dt;
    lv.int__dv__rotation = dv;
    lv.int_array__dw__translation[0] = dw[0];
    lv.int_array__dw__translation[1] = dw[1];
    lv.int_array__dw__translation[2] = dw[2];
    lv.class_c_array__ap__bsp = class_c_array__ap__bsp;
	return lv;
}

void class_l__function_I_void__item(class__k__item *an, const byte arrayOfByte1[])
{
	if(!an || !arrayOfByte1)
		return;
    jint i3;
    jint i1 = -2;
	i1 += 2;
	an->int__f__aabb = (marge_digit(arrayOfByte1[0], arrayOfByte1[1]) << 16); //c
	i1 += 2;
	an->int__g__aabb = (marge_digit(arrayOfByte1[2], arrayOfByte1[3]) << 16); //d
	i1 += 2;
	an->int__h__aabb = (marge_digit(arrayOfByte1[4], arrayOfByte1[5]) << 16); //e
	i1 += 2;
	an->int__i__aabb = (marge_digit(arrayOfByte1[6], arrayOfByte1[7]) << 16); //f
	i1 += 2;
	an->int__j__aabb = (marge_digit(arrayOfByte1[8], arrayOfByte1[9]) << 16); //g
	i1 += 2;
	an->int__k__aabb = (marge_digit(arrayOfByte1[10], arrayOfByte1[11]) << 16); //h
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i3 = marge_digit(arrayOfByte1[20], arrayOfByte1[21]);
	i1 += 2;
    jint i2 = marge_digit(arrayOfByte1[22], arrayOfByte1[23]);
	i1 += 2;
    jint i4 = marge_digit(arrayOfByte1[24], arrayOfByte1[25]);
	//am[i5].i = i3;
    new_array(&an->int_array_6__a__vertex, ESIZE(jint, 4), i3 * 6);
    jint *a = (jint *)(an->int_array_6__a__vertex.array);
    new_array(&an->int_array_16__b__primitive, ESIZE(jint, 4), i2 << 4);
    jint *b = (jint *)(an->int_array_16__b__primitive.array);
	i4 <<= 1;
    jint *arrayOfInt = NEW_II(jint, i4);
    jint i6;
	for (i6 = 0; i6 < i3; i6++)
	{
		i1 += 2;
		a[(i6 * 6)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i6 * 6 + 1)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i6 * 6 + 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
	}
	for (i3 = 0; i3 < i2; i3++)
	{
		i1 += 2;
		b[(i3 << 4)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[((i3 << 4) + 1)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[((i3 << 4) + 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
	}
	for (i3 = 0; i3 < i2; i3++)
	{
		i1 += 2;
        b[((i3 << 4) + 10)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[((i3 << 4) + 11)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[((i3 << 4) + 12)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
	}
    jint i7;
	for (i7 = 0; i7 < i4; i7 += 2)
	{
		i1 += 2;
        arrayOfInt[i7] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i7 + 1)] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
	for (i4 = 0; i4 < i2; i4++)
	{
		i1 += 2;
        b[((i4 << 4) + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[((i4 << 4) + 4)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
        b[((i4 << 4) + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[((i4 << 4) + 6)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
        b[((i4 << 4) + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
        b[((i4 << 4) + 8)] = ((jshort)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
	}
	i1 += 2;
	for (i3 = 0; i3 < i2; i3++)
	{
		b[((i3 << 4) + 9)] = arrayOfByte1[i1];
		//cl[am[i5].b[((i3 << 4) + 9)]] = true;
		i1++;
	}
	free(arrayOfInt);
	/*
		 byte[] arrayOfByte2 = g.d.a(g.a(az), -1);
		 for (int i4 = 0; i4 < an.length; i4++) {
		 if (i4 == an.length - 1)
		 {
		 an[i4].a = am[44].a;
		 an[i4].b = am[44].b;
		 an[i4].f = am[44].c;
		 an[i4].i = am[44].f;
		 an[i4].g = am[44].d;
		 an[i4].j = am[44].g;
		 an[i4].h = am[44].e;
		 an[i4].k = am[44].h;
		 an[i4].t = 44;
		 }
		 else
		 {
		 k localk = an[i4];
		 if ((!F ? (localk.t <= 0) || (localk.t >= 8) : localk.t != 21) && (localk.t < 120))
		 {
		 i3 = localk.t << 2;
		 if (arrayOfByte2[i3] == 1) {
		 localk.d = true;
		 }
		 if (arrayOfByte2[(i3 + 1)] == 1) {
		 localk.s = true;
		 }
		 if (arrayOfByte2[(i3 + 2)] == 1) {
		 localk.c = true;
		 }
		 if (arrayOfByte2[(i3 + 3)] == 1) {
		 localk.l = true;
		 }
		 q localq = am[localk.t];
		 localk.a = localq.a;
		 localk.b = localq.b;
		 localk.f = localq.c;
		 localk.i = localq.f;
		 localk.g = localq.d;
		 localk.j = localq.g;
		 localk.h = localq.e;
		 localk.k = localq.h;
		 }
		 }
		 }
		 */
}
