#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// ct3d ep2 no jad version v1?

#define dprintfsi(s, d) nlprintf("CT 3D Ep2 : "#s"->%d\n", d)
#define dprintfsii(s, d, d2) nlprintf("CT 3D Ep2 : ""%d - "#s"->%d\n", d, d2)
#define dprintfss(s) nlprintf("CT 3D Ep2 : "#s"\n")

typedef struct _class__h__scene // 地图模型场景
{
  array short_array_3__a__vertex; // short 3顶点坐标
  array int_array_7__b__primitive; // int 3索引 3纹理坐标索引(i, i + 1) 1纹理索引
  int c;
  int d;
  int int__e__end; // = 0; 最后一个物品索引
  int int__f__aabb; // AABB盒子
  int int__g__aabb; // AABB盒子
  int int__h__aabb; // AABB盒子
  int int__i__aabb; // AABB盒子
  int int__j__aabb; // AABB盒子
  int int__k__aabb; // AABB盒子
  array int_array_2__l__plane; // int 碰撞面 1法线索引(i, i + 1, i + 2) 1顶点索引(i, i + 1, i + 2)
  int m;
  int n;
  int int_array_7__o__bsp[7]; // -1 -1 -1 -1 -1 -1 -1
  int int__p__bsp; // = 0;
  int int__q__begin; // = 0; 第一个物品索引
} class__h__scene;

typedef struct _class__c__bsp
{
  byte byte__a__direction;
	int int_array_3__a__normal[3];
  int int_array_4x3__b__plane[4][3]; // n[] b = new n[4];
  int int__c__prev_scene;
  int int__d__next_scene;
  boolean e; // = false;
} class__c__bsp;

typedef struct _class__k__item // 物品
{
  array int_array_6__a__vertex; // int 顶点坐标
  array int_array_16__b__primitive; // int 纹理 纹理索引
  boolean c;
  boolean d;
  short e; // = 0;
  int int__f__aabb;
  int int__g__aabb;
  int int__h__aabb;
  int int__i__aabb;
  int int__j__aabb;
  int int__k__aabb;
  boolean l;
  int int__m__translation; // 场景坐标
  int int__n__translation; // 场景坐标
  int int__o__translation; // 场景坐标
  int int__p__rotation; // = 0; 旋转
  int int__q__rotation; // = 0; 旋转
  short r;
  boolean s;
  byte byte__t__obj;
  boolean u; // = true;
} class__k__item;

typedef struct _class__l__lvl
{
  array class_h_array__ar__scene; // 场景
    array int_array__ak__normal; // 法线
    array int_array__dP__texcoord; // 纹理坐标
  array class_k_array__an__item; // 物品
  int int_array__dw__translation[3]; // 主角初始坐标
  int int__dt__rotation; // 主角初始视角
  int int__dv__rotation; // 主角初始视角;
    array class_c_array__ap__bsp;
} class__l__lvl;

static void read_CT3DEp2_map_items(array *k_an);
static class__l__lvl class_l__function_h_1int__scene(const byte paramInt[]); // 场景解析
static void class_l__function_I_void__item(class__k__item *an, const byte arrayOfByte1[]); // 物品解析

NLboolean nlReadCT3DEp2ModelFile(const char* name, int level, NETLizard_3D_Model *model)
{
    array arr;
    int res;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;
	int paramInt = level;
	int dr = 0;
	if ((paramInt == 1) || (paramInt == 2) || (paramInt == 4) || (paramInt == 7)) {
		dr = 1;
	}
    byte *arrayOfByte = (byte *)(arr.array);
    class__l__lvl lvl = class_l__function_h_1int__scene(arrayOfByte);
    class__l__lvl *lv = &lvl;

	ZERO(model, NETLizard_3D_Model);
    array *h_ar = &lv->class_h_array__ar__scene;
    int *dP = (int *)(lv->int_array__dP__texcoord.array);
    int *ak = (int *)(lv->int_array__ak__normal.array);
    model->data.data = NEW_II(NETLizard_3D_Mesh, h_ar->length);
    model->data.length = h_ar->length;
	model->has_sky = dr;
	model->game = CT_3D_Ep2_Map;

	int i;
	for(i = 0; i < h_ar->length; i++)
	{
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
		class__h__scene *obj = ((class__h__scene *)(h_ar->array)) + i;

        // vertex
        mesh->vertex.length = obj->short_array_3__a__vertex.length;
        mesh->vertex.data = NEW_II(NLint, mesh->vertex.length);
        int *m_v = (int *)(mesh->vertex.data);
        short *s_a = (short *)(obj->short_array_3__a__vertex.array);
		int j;
        for(j = 0; j < obj->short_array_3__a__vertex.length; j++)
		{
			m_v[j] = s_a[j];
		}
		
        // index
        mesh->primitive.length = obj->int_array_7__b__primitive.length / 7;
        mesh->primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->vertex.length);
        int *s_b = (int *)(obj->int_array_7__b__primitive.array);
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
        mesh->plane.length = obj->int_array_2__l__plane.length / 2;
        mesh->plane.data = NEW_II(NETLizard_3D_Plane, mesh->plane.length);
        int *l = (int *)(obj->int_array_2__l__plane.array);
		k = 0;
        for(j = 0; j < obj->int_array_2__l__plane.length; j += 2)
		{
			int normal_index = l[j];
			int position_index = l[j + 1];
            NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
			plane->normal[0] = ak[normal_index];
			plane->normal[1] = ak[normal_index + 1];
			plane->normal[2] = ak[normal_index + 2];
			plane->position[0] = s_a[position_index];
			plane->position[1] = s_a[position_index + 1];
			plane->position[2] = s_a[position_index + 2];
			k++;
		}
		mesh->ortho[0] = obj->int__f__aabb >> 16;
		mesh->ortho[1] = obj->int__g__aabb >> 16;
		mesh->ortho[2] = obj->int__h__aabb >> 16;
		mesh->ortho[3] = obj->int__i__aabb >> 16;
		mesh->ortho[4] = obj->int__j__aabb >> 16;
		mesh->ortho[5] = obj->int__k__aabb >> 16;
		mesh->item_index_range[0] = obj->int__q__begin;
		mesh->item_index_range[1] = obj->int__e__end;

        // bsp
        mesh->bsp.data = NULL;
		if(obj->int__p__bsp)
		{
            mesh->bsp.length = obj->int__p__bsp;
            mesh->bsp.data = NEW_II(NLint, mesh->bsp.length);
            int *bsp_index = (int *)(mesh->bsp.data);
			for(j = 0; j < obj->int__p__bsp; j++)
			{
				bsp_index[j] = obj->int_array_7__o__bsp[j];
			}
		}
	}

    // item model
    array *k_an = &lv->class_k_array__an__item;
    model->item_data.length = k_an->length;
    model->item_data.data = NEW_II(NETLizard_3D_Item_Mesh, model->item_data.length);
	for(i = 0; i < k_an->length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
		class__k__item *obj = ((class__k__item *)(k_an->array)) + i;
        if(!ARRAY_DATA(obj->int_array_6__a__vertex) || !ARRAY_DATA(obj->int_array_16__b__primitive))
		{
            mesh->item_mesh.vertex.length = 0;
            mesh->item_mesh.primitive.length = 0;
			continue;
		}

        // position/rotation
		mesh->pos[0] = obj->int__m__translation >> 16;
		mesh->pos[1] = obj->int__n__translation >> 16;
		mesh->pos[2] = obj->int__o__translation >> 16;
		mesh->angle[0] = obj->int__p__rotation;
		mesh->angle[1] = obj->int__q__rotation;
		mesh->obj_index = obj->byte__t__obj;

        // box
		mesh->item_mesh.ortho[0] = obj->int__f__aabb >> 16;
		mesh->item_mesh.ortho[1] = obj->int__g__aabb >> 16;
		mesh->item_mesh.ortho[2] = obj->int__h__aabb >> 16;
		mesh->item_mesh.ortho[3] = obj->int__i__aabb >> 16;
		mesh->item_mesh.ortho[4] = obj->int__j__aabb >> 16;
		mesh->item_mesh.ortho[5] = obj->int__k__aabb >> 16;

        // vertex
        mesh->item_mesh.vertex.length = obj->int_array_6__a__vertex.length / 2;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.length);
		int j;
        int *im_v = (int *)(mesh->item_mesh.vertex.data);
        int *i_a = (int *)(obj->int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj->int_array_6__a__vertex.length; j += 6)
		{
			im_v[k] = i_a[j] >> 16;
			im_v[k + 1] = i_a[j + 1] >> 16;
			im_v[k + 2] = i_a[j + 2] >> 16;
			k += 3;
		}

        // index
        mesh->item_mesh.primitive.length = obj->int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.length);
        int *i_b = (int *)(obj->int_array_16__b__primitive.array);
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
    model->bsp_data.length = 0;
    if(ARRAY_DATA(lv->class_c_array__ap__bsp))
	{
        class__c__bsp *bsp = (class__c__bsp *)(lv->class_c_array__ap__bsp.array);
        model->bsp_data.length = lv->class_c_array__ap__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.length);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_c_array__ap__bsp.length; j++)
		{
			int k;
			for(k = 0; k < 4; k++)
			{
				bsp_data[j].plane[k][0] = bsp[k].int_array_4x3__b__plane[k][0] >> 16;
				bsp_data[j].plane[k][1] = bsp[k].int_array_4x3__b__plane[k][1] >> 16;
				bsp_data[j].plane[k][2] = bsp[k].int_array_4x3__b__plane[k][2] >> 16;
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
	model->start_angle[0] = lv->int__dt__rotation;
	model->start_angle[1] = lv->int__dv__rotation;
	model->start_pos[0] = lv->int_array__dw__translation[0] >> 16;
	model->start_pos[1] = lv->int_array__dw__translation[1] >> 16;
	model->start_pos[2] = lv->int_array__dw__translation[2] >> 16;

    // free
	for(i = 0; i < h_ar->length; i++)
	{
		class__h__scene *obj = ((class__h__scene *)(h_ar->array)) + i;
        delete_array(&obj->short_array_3__a__vertex);
        delete_array(&obj->int_array_7__b__primitive);
        delete_array(&obj->int_array_2__l__plane);
	}
    delete_array(h_ar);

	for(i = 0; i < k_an->length; i++)
	{
		class__k__item *obj = ((class__k__item *)(k_an->array)) + i;
        delete_array(&obj->int_array_6__a__vertex);
        delete_array(&obj->int_array_16__b__primitive);
	}
    delete_array(k_an);

    delete_array(&lv->int_array__ak__normal);
    delete_array(&lv->int_array__dP__texcoord);
    delete_array(&lv->class_c_array__ap__bsp);

    return NL_TRUE;
}

NLboolean nlReadCT3DEp2ItemModelFile(const char* name, int index, NETLizard_3D_Model *model)
{
    array arr;
    int res;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

	ZERO(model, NETLizard_3D_Model);

	class__k__item obj;
    class_l__function_I_void__item(&obj, arr.array);
    array item_meshes;
    new_array(&item_meshes, sizeof(NETLizard_3D_Item_Mesh), 1);
    NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(item_meshes.array));
    mesh->item_mesh.vertex.data = NULL;
    mesh->item_mesh.primitive.data = NULL;
	mesh->pos[0] = 0;
	mesh->pos[1] = 0;
	mesh->pos[2] = 0;
	mesh->angle[0] = 0;
	mesh->angle[1] = 0;
	mesh->obj_index = index;
    if(ARRAY_DATA(obj.int_array_6__a__vertex) && ARRAY_DATA(obj.int_array_16__b__primitive))
	{
        // vertex
        mesh->item_mesh.vertex.length = obj.int_array_6__a__vertex.length / 2;
        mesh->item_mesh.vertex.data = NEW_II(NLint, mesh->item_mesh.vertex.length);
		int j;
        int *im_v = (int *)(mesh->item_mesh.vertex.data);
        int *i_a = (int *)(obj.int_array_6__a__vertex.array);
		int k = 0;
        for(j = 0; j < obj.int_array_6__a__vertex.length; j += 6)
		{
			im_v[k] = i_a[j] >> 16;
			im_v[k + 1] = i_a[j + 1] >> 16;
			im_v[k + 2] = i_a[j + 2] >> 16;
			k += 3;
		}

        // index
        int *i_b = (int *)(obj.int_array_16__b__primitive.array);
        mesh->item_mesh.primitive.length = obj.int_array_16__b__primitive.length / 16;
        mesh->item_mesh.primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->item_mesh.primitive.length);
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
		mesh->item_mesh.ortho[0] = obj.int__f__aabb >> 16;
		mesh->item_mesh.ortho[1] = obj.int__g__aabb >> 16;
		mesh->item_mesh.ortho[2] = obj.int__h__aabb >> 16;
		mesh->item_mesh.ortho[3] = obj.int__i__aabb >> 16;
		mesh->item_mesh.ortho[4] = obj.int__j__aabb >> 16;
		mesh->item_mesh.ortho[5] = obj.int__k__aabb >> 16;
	}

    delete_array(&obj.int_array_6__a__vertex);
    delete_array(&obj.int_array_16__b__primitive);
    delete_array(&arr);
    model->data.data = NULL;
    model->data.length = 0;
    model->item_data.data = item_meshes.array;
    model->item_data.length = item_meshes.length;
	model->has_sky = 0;
	model->game = CT_3D_Ep2_Item;
    return NL_TRUE;
}

void read_CT3DEp2_map_items(array *k_an)
{
	int i;
	for(i = 0; i < k_an->length; /* 45 */i++)
	{
		class__k__item *an = ((class__k__item *)(k_an->array)) + i;
		int i11 = an->byte__t__obj;
		char subfix[strlen(CT3DEP2_OBJ_SUBFIX) + 1];
		memset(subfix, '\0', sizeof(char) * (strlen(subfix) + 1));
		sprintf(subfix, CT3DEP2_OBJ_SUBFIX, i11);
		char *name = NULL;
        if(game_resource_path[NL_CONTR_TERRORISM_3D_EPISODE_2])
		{
            name = NEW_II(char, strlen(subfix) + strlen(game_resource_path[NL_CONTR_TERRORISM_3D_EPISODE_2]) + 1 + 1);
            memset(name, '\0', sizeof(char) * ((strlen(subfix) + strlen(game_resource_path[NL_CONTR_TERRORISM_3D_EPISODE_2]) + 1 + 1)));
            sprintf(name, "%s/%s", game_resource_path[NL_CONTR_TERRORISM_3D_EPISODE_2], subfix);
		}
		else
            name = strdup(subfix);

        array arr;
        int res = file_get_contents(name, &arr);
		free(name);
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

class__l__lvl class_l__function_h_1int__scene(const byte paramInt[])
{
	int i2;
  int dw[3];
	//byte *cZ = NULL;
  short dh;
  short di;
  short dj;
  int dt;
  int dv;
  int jdField_do;
  array int_ak;
  array int_dP;
  array h_ar;
  //byte *cL = NULL;
  int cI;
  array k_an;

	int i1 = -2;
	i1 += 2;
	if ((i2 = marge_digit(paramInt[0], paramInt[1])) != 0) {
		//cZ = NEW_II(byte, i2);
	}
	int i3;
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

    new_array(&int_ak, 4, i2);
    int *ak = (int *)(int_ak.array);
	for (i3 = 0; i3 < i2; i3 += 3)
	{
		i1 += 2;
		ak[i3] = ((int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
		ak[(i3 + 1)] = ((int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
		i1 += 2;
		ak[(i3 + 2)] = ((int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L));
	}
	dprintfss("Get map normal");
	i1 += 2;
	i2 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
	dprintfsi("Texcoord count", i2);

    new_array(&int_dP, 4, i2);
    int *dP = (int *)(int_dP.array);
	for (i3 = 0; i3 < i2; i3 += 2)
	{
		i1 += 2;
		dP[i3] = ((int)(64 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
		i1 += 2;
		dP[(i3 + 1)] = ((int)(64 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");

    new_array(&h_ar, sizeof(class__h__scene), jdField_do);
    class__h__scene *ar = (class__h__scene *)(h_ar.array);
	//cL = NEW_II(byte, jdField_do);
	int i4;
	int i5;
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
        new_array(&ar[i4].short_array_3__a__vertex, ESIZE(short, 2), i3 * 3);
        short *a = (short *)(ar[i4].short_array_3__a__vertex.array);
        ar[i4].m = i3;
        new_array(&ar[i4].int_array_7__b__primitive, ESIZE(int, 4), i2 * 7);
        int *b = (int *)(ar[i4].int_array_7__b__primitive.array);
 		ar[i4].n = i2;
		int i7;
		for (i7 = 0; i7 < i3; i7++)
		{
			i1 += 2;
			a[(i7 * 3)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
			a[(i7 * 3 + 1)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			i1 += 2;
			a[(i7 * 3 + 2)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
		}
		dprintfss("Get scene vertex coord");
        int *arrayOfInt2 = (int *)(ar[i4].int_array_7__b__primitive.array);
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
        new_array(&ar[i4].int_array_2__l__plane, ESIZE(int, 4), i3 << 1);
		dprintfsi("Scene plane count", i3);
        int *l = (int *)(ar[i4].int_array_2__l__plane.array);
		int i9;
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
		int i8;
		for (i8 = 0; i8 < i2; i8++)
		{
			i3 = i8 * 7;
			i1 += 2;
			b[(i3 + 3)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
			i1 += 2;
			b[(i3 + 4)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
			i1 += 2;
			b[(i3 + 5)] = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]) << 1);
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
    array class_c_array__ap__bsp;
    new_array(&class_c_array__ap__bsp, sizeof(class__c__bsp), i5);
    class__c__bsp *ap = (class__c__bsp *)(class_c_array__ap__bsp.array);
	int i8;
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
		i2 = (int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
		i3 = (int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
		i1 += 2;
		i4 = (int)(65536 * marge_digit(paramInt[i1], paramInt[(i1 + 1)]) / 10000L);
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
		ar[i3].int_array_7__o__bsp = NEW_II(int, i9);
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
	int i9;
	i3 = 0;
	int i6;
	for (i6 = 0; i6 < jdField_do; i6++)
	{
		i1 += 2;
		i8 = marge_digit(paramInt[i1], paramInt[(i1 + 1)]);
		dprintfsii("Scene item count", i6, i8);
		ar[i6].int__q__begin = i3;
		ar[i6].int__e__end = (i3 + i8);
		for (i9 = 0; i9 < i8; i9++)
		{
			an[i3].r = ((short)i6);
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
			an[i3].e = ((short)marge_digit(paramInt[i1], paramInt[(i1 + 1)]));
			if (an[i3].e < 0)
			{
				class__k__item *tmp2952_2951 = an + i3;
				tmp2952_2951->e = ((short)(tmp2952_2951->e + 256));
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

    read_CT3DEp2_map_items(&k_an);

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
	int i3;
	int i1 = -2;
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
	int i2 = marge_digit(arrayOfByte1[22], arrayOfByte1[23]);
	i1 += 2;
	int i4 = marge_digit(arrayOfByte1[24], arrayOfByte1[25]);
	//am[i5].i = i3;
    new_array(&an->int_array_6__a__vertex, ESIZE(int, 4), i3 * 6);
    int *a = (int *)(an->int_array_6__a__vertex.array);
    new_array(&an->int_array_16__b__primitive, ESIZE(int, 4), i2 << 4);
    int *b = (int *)(an->int_array_16__b__primitive.array);
	i4 <<= 1;
	int *arrayOfInt = NEW_II(int, i4);
	int i6;
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
		b[((i3 << 4) + 10)] = ((int)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
		b[((i3 << 4) + 11)] = ((int)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
		b[((i3 << 4) + 12)] = ((int)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
	}
	int i7;
	for (i7 = 0; i7 < i4; i7 += 2)
	{
		i1 += 2;
		arrayOfInt[i7] = ((int)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
		arrayOfInt[(i7 + 1)] = ((int)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
	for (i4 = 0; i4 < i2; i4++)
	{
		i1 += 2;
		b[((i4 << 4) + 3)] = ((short)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
		b[((i4 << 4) + 4)] = ((short)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
		b[((i4 << 4) + 5)] = ((short)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
		b[((i4 << 4) + 6)] = ((short)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
		i1 += 2;
		b[((i4 << 4) + 7)] = ((short)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1)]);
		b[((i4 << 4) + 8)] = ((short)arrayOfInt[((marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 1) + 1)]);
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
