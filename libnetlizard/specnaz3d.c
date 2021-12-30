#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

// army ranger 3d guagua cracked glo version

#define dprintfsi(s, d) nllogfln("Specnaz 3D : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("Specnaz 3D : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("Specnaz 3D : "#s)

typedef struct _class__g__scene
{
  int_array int_array_6__a__vertex; // int 3顶点坐标 3UNUSED DYNAMIC
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
  int_array int_array_6__l__plane; // int 碰撞面 1法线索引(i, i + 1, i + 2) 1顶点索引(i, i + 1, i + 2)
  jint m;
  jint n;
  jint int_array_5__o__bsp[5];
  jint int__p__bsp; // = 0;
  jint int__q__begin; // = 0; // 第一个物品索引
} class__g__scene;

typedef struct _class__c__bsp
{
  byte byte__a__direction;
  jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; // l[] b = new l[4];
  jint int__c__prev_scene;
  jint int__d__next_scene;
  boolean e; // = false;
} class__c__bsp;

typedef struct _class__j__item
{
  int_array int_array_6__a__vertex; // int 3顶点坐标 3UNUSED DYNAMIC
  int_array int_array_16__b__primitive; // int 3索引 6纹理坐标索引 1纹理索引 3法线 3UNKNOW
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
  jint int__m__translation; // 坐标
  jint int__n__translation; // 坐标
  jint int__o__translation; // 坐标
  jint int__p__rotation; // = 0; 旋转
  jint int__q__rotation; // = 0; 旋转
  jshort r;
  jboolean s;
  byte byte__t__obj;
  jboolean u; // = true;
} class__j__item;

typedef struct _class__k__lvl
{
    T_array(class__g__scene) class_g_array__q__scene; // 场景
  int_array int_array__j__normal; // 法线
  int_array int_array__dm__texcoord; // 纹理坐标
  array class_j_array__l__item; // 物品
  jint int_array__cF__translation[3]; // 主角初始坐标
  jint int__cC__rotation; // 主角初始视角
  jint int__cE__rotation; // 主角初始视角;
  T_array(class__c__bsp) class_c_array__o__bsp;
} class__k__lvl;

static void delete_class__g__scene(class__g__scene *scene)
{
    delete_array(&scene->int_array_6__a__vertex);
    delete_array(&scene->int_array_7__b__primitive);
    delete_array(&scene->int_array_6__l__plane);
}

static void delete_class__j__item(class__j__item *item)
{
    delete_array(&item->int_array_6__a__vertex);
    delete_array(&item->int_array_16__b__primitive);
}

static void delete_class__k__lvl(class__k__lvl *lv)
{
    int i;

    T_array(class__g__scene) *g_q = &lv->class_g_array__q__scene;
    for(i = 0; i < g_q->length; i++)
    {
        class__g__scene *obj = ((class__g__scene *)(g_q->array)) + i;
        delete_class__g__scene(obj);
    }
    delete_array(g_q);

    T_array(class__j__item) *j_l = &lv->class_j_array__l__item;
    for(i = 0; i < j_l->length; i++)
    {
        class__j__item *obj = ((class__j__item *)(j_l->array)) + i;
        delete_class__j__item(obj);
    }
    delete_array(j_l);
    delete_array(&lv->int_array__j__normal);
    delete_array(&lv->int_array__dm__texcoord);
    delete_array(&lv->class_c_array__o__bsp);
}

static class__k__lvl class_k__function_h_1int__scene(const byte arrayOfByte[], const char *resc_path); // 场景解析
static void class_k__function_P_void__item(class__j__item *l, const byte arrayOfByte[]); // 场景物品解析
static void read_Specnaz3D_map_items(T_array(class__j__item) *j_l, const char *resc_path);

NLboolean nlReadSpecnaz3DModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadSpecnaz3DModelData(arr.array, arr.length, level, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadSpecnaz3DModelData(const char* data, NLsizei res, NLint paramInt, const char *resc_path, NETLizard_3D_Model *model)
{
    byte *arrayOfByte = (byte *)(data);
    class__k__lvl lvl = class_k__function_h_1int__scene(arrayOfByte, resc_path);
    class__k__lvl *lv = &lvl;
	int dr = 0;
	if ((paramInt == 1) || paramInt == 2 || paramInt == 6 || paramInt == 8 || paramInt == 9 || paramInt == 10) {
		dr = 1;
    }

	ZERO(model, NETLizard_3D_Model);
    array *g_q = &lv->class_g_array__q__scene;
    jint *dm = (jint *)(lv->int_array__dm__texcoord.array);
    jint *j_array = (jint *)(lv->int_array__j__normal.array);
    model->data.count = g_q->length;
    model->data.data = NEW_II(NETLizard_3D_Mesh, g_q->length);
	model->has_sky = dr;
    model->type = NL_ARMY_RANGER_3D_MAP_MODEL;
    model->game = NL_ARMY_RANGER_3D;

	int i;
	for(i = 0; i < g_q->length; i++)
	{
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
		class__g__scene *obj = ((class__g__scene *)(g_q->array)) + i;

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
        mesh->primitive.data = NULL;
        mesh->primitive.count = 0;
        if(ARRAY_DATA(obj->int_array_7__b__primitive))
		{
            mesh->primitive.count = obj->int_array_7__b__primitive.length / 7;
            mesh->primitive.data = NEW_II(NETLizard_3D_Primitive, mesh->vertex.count);
            jint *s_b = (jint *)(obj->int_array_7__b__primitive.array);
			int k = 0;
            for(j = 0; j < obj->int_array_7__b__primitive.length; j += 7)
			{
                NETLizard_3D_Primitive *p = ((NETLizard_3D_Primitive *)(mesh->primitive.data)) + k;
				p->index[0] = s_b[j] / 6;
				p->index[1] = s_b[j + 1] / 6; 
				p->index[2] = s_b[j + 2] / 6;
				p->texcoord[0] = dm[(s_b[j + 3])];
				p->texcoord[1] = dm[(s_b[j + 3]) + 1];
				p->texcoord[2] = dm[(s_b[j + 4])];
				p->texcoord[3] = dm[(s_b[j + 4]) + 1];
				p->texcoord[4] = dm[(s_b[j + 5])];
				p->texcoord[5] = dm[(s_b[j + 5]) + 1];
				p->tex_index = (s_b[j + 6]) & 0xff;
				k++;
			}
		}

        // plane
        mesh->plane.data = NULL;
        mesh->plane.count = 0;
        if(ARRAY_DATA(obj->int_array_6__l__plane))
		{
            mesh->plane.count = obj->int_array_6__l__plane.length / 2;
            mesh->plane.data = NEW_II(NETLizard_3D_Plane, mesh->plane.count);
            jint *s_p = (jint *)(obj->int_array_6__l__plane.array);
			int k = 0;
            for(j = 0; j < obj->int_array_6__l__plane.length; j += 2)
			{
                jint normal_index = s_p[j];
                jint position_index = s_p[j + 1];
                NETLizard_3D_Plane *plane = ((NETLizard_3D_Plane *)(mesh->plane.data)) + k;
				plane->normal[0] = j_array[normal_index];
				plane->normal[1] = j_array[normal_index + 1];
				plane->normal[2] = j_array[normal_index + 2];
				plane->position[0] = s_a[position_index] >> 16;
				plane->position[1] = s_a[position_index + 1] >> 16;
				plane->position[2] = s_a[position_index + 2] >> 16;
				k++;
			}
		}

        // box
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
				bsp_index[j] = obj->int_array_5__o__bsp[j];
			}
		}
	}

    // item model
    model->item_data.count = 0;
    model->item_data.data = NULL;
    if(ARRAY_DATA(lv->class_j_array__l__item))
	{
        T_array(class__j__item) *j_l = &lv->class_j_array__l__item;
        model->item_data.count = j_l->length;
        model->item_data.data = NEW_II(NETLizard_3D_Item_Mesh, model->item_data.count);
		for(i = 0; i < j_l->length; i++)
		{
            NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
			class__j__item *obj = ((class__j__item *)(j_l->array)) + i;
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
				p->plane.position[0] = i_a[p->index[0] * 3] >> 16;
				p->plane.position[1] = i_a[p->index[0] * 3 + 1] >> 16;
				p->plane.position[2] = i_a[p->index[0] * 3 + 2] >> 16;
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
	}

    // bsp
    model->bsp_data.data = NULL;
    model->bsp_data.count = 0;
    if(ARRAY_DATA(lv->class_c_array__o__bsp))
	{
        class__c__bsp *bsp = (class__c__bsp *)(lv->class_c_array__o__bsp.array);
        model->bsp_data.count = lv->class_c_array__o__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_c_array__o__bsp.length; j++)
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

	model->start_rotation[0] = lv->int__cC__rotation;
	model->start_rotation[1] = lv->int__cE__rotation;
	model->start_position[0] = lv->int_array__cF__translation[0] >> 16;
	model->start_position[1] = lv->int_array__cF__translation[1] >> 16;
	model->start_position[2] = lv->int_array__cF__translation[2] >> 16;

    // free
    delete_class__k__lvl(lv);
    return NL_TRUE;
}

NLboolean nlReadSpecnaz3DItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadSpecnaz3DItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadSpecnaz3DItemModelData(const char* data, NLsizei res, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__j__item obj;
    class_k__function_P_void__item(&obj, (const byte *)data);
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
    delete_class__j__item(&obj);

    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
	model->has_sky = 0;
    model->type = NL_ARMY_RANGER_3D_ITEM_MODEL;
    model->game = NL_ARMY_RANGER_3D;
    return NL_TRUE;
}

void read_Specnaz3D_map_items(T_array(class__j__item) *j_l, const char *resc_path)
{
	if(!j_l)
		return;
    int i;
	for(i = 0; i < j_l->length; i++)
	{
		class__j__item *l = ((class__j__item *)(j_l->array)) + i;
        jint i11 = l->byte__t__obj;
        char *name = make_resource_file_path(SPECNAZ3D_OBJ_SUBFIX, i11, resc_path);
        array arr;
        int res = file_get_contents(name, &arr);
        free(name);
        if(res > 0)
        {
            class_k__function_P_void__item(l, arr.array);
            delete_array(&arr);
        }
        else
        {
            ARRAY_NULL(l->int_array_6__a__vertex);
            ARRAY_NULL(l->int_array_16__b__primitive);
        }
	}
}
/* ****** static ****** */

class__k__lvl class_k__function_h_1int__scene(const byte arrayOfByte[], const char *resc_path)
{
	//byte *cn = NULL;
    jint cF[3];
    jint cC;
    jint cE;
    jint cx;
    int_array int_j;
    int_array int_dm;
    T_array(class__g__scene) g_q;
	//byte *bX = NULL;
    jint bU;
    T_array(class__j__item) j_l;

    jint i1 = -2;
	i1 += 2;
    jint i2;
	if ((i2 = marge_digit(arrayOfByte[0], arrayOfByte[1])) != 0) {
		//cn = NEW_II(byte, i2);
	}
    jint i5;
	for (i5 = 0; i5 < i2; i5++)
	{
		i1 += 2;
		//cn[i5] = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
	}
	i1 += 2;
	cF[0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	i1 += 2;
	cF[1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	i1 += 2;
	cF[2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	i1 += 2;
	cC = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	i1 += 2;
	cE = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	if (cE > 361) {
		cE -= 65536;
	}
	if (cC > 361) {
		cC -= 65536;
	}
	if (cE < 0) {
		cE = -cE;
	} else {
		cE = 360 - cE;
	}
	if (cE == 360) {
		cE = 0;
	}
	if (cC < 0) {
		cC = 270 + cC;
	} else {
		cC = 270 - cC;
	}
	if (cC == 360) {
		cC = 0;
	}
	dprintfss("Get start position and axis");
	i1 += 2;
	cx = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map scene count", cx);
	i1 += 2;
    jint i6 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
    new_array(&int_j, ESIZE(jint, 4), i6);
	dprintfsi("Map normal count", i6);
    jint *j_array = (jint *)(int_j.array);
    jint i7;
	for (i7 = 0; i7 < i6; i7 += 3)
	{
		i1 += 2;
        j_array[i7] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        j_array[(i7 + 1)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        j_array[(i7 + 2)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
	}
	dprintfss("Get map normal");
	i1 += 2;
	i6 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Texcoord count", i6);
    new_array(&int_dm, ESIZE(jint, 4), i6);
    jint *dm = (jint *)(int_dm.array);
    jint i8;
	for (i8 = 0; i8 < i6; i8 += 2)
	{
		i1 += 2;
        dm[i8] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
		i1 += 2;
        dm[(i8 + 1)] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");
    new_array(&g_q, sizeof(class__g__scene), cx);
    class__g__scene *q = (class__g__scene *)(g_q.array);
	//bX = NEW_II(byte, cx);
    jint i18;
    jint i24;
    jint i22;
    jint i13;
	for (i13 = 0; i13 < cx; i13++)
	{
		dprintfsi("Get map scene data", i13);
		i1 += 2;
		q[i13].int__f__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		q[i13].int__g__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		q[i13].int__h__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		q[i13].int__i__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		q[i13].int__j__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		q[i13].int__k__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
        jint i11 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i11);
		i1 += 2;
        jint i10 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene primitive count", i10);
        new_array(&q[i13].int_array_6__a__vertex, ESIZE(jint, 4), i11 * 6);
        jint *a = (jint *)(q[i13].int_array_6__a__vertex.array);
		q[i13].m = i11;
        new_array(&q[i13].int_array_7__b__primitive, ESIZE(jint, 4), i10 * 7);
        jint *b = NULL;
        if(ARRAY_DATA(q[i13].int_array_7__b__primitive))
            b = (jint *)(q[i13].int_array_7__b__primitive.array);
		q[i13].n = i10;
        jint i14;
		for (i14 = 0; i14 < i11; i14++)
		{
			i1 += 2;
			a[(i14 * 6)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			a[(i14 * 6 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			a[(i14 * 6 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		}
		dprintfss("Get scene vertex coord");
        jint *arrayOfInt = b;
        jint i17;
        jint i16;
		for (i17 = 0; i17 < i10; i17++)
		{
			i16 = i17 * 7;
			i1 += 2;
			b[i16] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
			i1 += 2;
			b[(i16 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
			i1 += 2;
			b[(i16 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 6);
		}
		dprintfss("Get scene primitive vertex coord index");
		i1 += 2;
		i18 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
        new_array(&q[i13].int_array_6__l__plane, ESIZE(jint, 4), i18 * 2);
		dprintfsi("Scene plane count", i8);
        jint *l = NULL;
        if(ARRAY_DATA(q[i13].int_array_6__l__plane))
            l = (jint *)(q[i13].int_array_6__l__plane.array);
        jint i19;
		for (i19 = 0; i19 < i18; i19++)
		{
			i1 += 2;
			l[(i19 * 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3);
			i1 += 2;
			l[(i19 * 2 + 1)] = arrayOfInt[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 7)];
		}
		dprintfss("Get scene plane normal index and point vertex coord index");
		q[i13].c = (q[i13].int__f__aabb + q[i13].int__i__aabb >> 1);
		q[i13].d = (q[i13].int__g__aabb + q[i13].int__j__aabb >> 1);
        jint i20;
		for (i20 = 0; i20 < i10; i20++)
		{
			i16 = i20 * 7;
			i1 += 2;
            b[(i16 + 3)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2);
			i1 += 2;
            b[(i16 + 4)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2);
			i1 += 2;
            b[(i16 + 5)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 2);
		}
		dprintfss("Get scene texcoord index");
		i1++;
        jint i23;
		for (i23 = 0; i23 < i10; i23++)
		{
            jint i21;
			i1++;
			if ((i21 = arrayOfByte[i1]) < 0) {
				i21 += 256;
			}
			b[(i23 * 7 + 6)] = i21;
			//bv[q[i13].b[(i23 * 7 + 6)]] = true;
		}
		for (i24 = 0; i24 < i10; i24++)
		{
			i1++;
			if ((i22 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)])) < 0) {
				i22 += 256;
			}
            jint i21 = b[(i24 * 7 + 6)] & 0xFF;
			b[(i24 * 7 + 6)] = ((i22 << 8) + i21);
		}
		dprintfss("Get primitive texture index");
		i1--;
	}
	i1 += 2;
    jint i12;
	i12 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map BSP tree node count", i12);
    T_array(class__c__bsp) class_c_array__o__bsp;
    new_array(&class_c_array__o__bsp, sizeof(class__c__bsp), i12);
    class__c__bsp *o = (class__c__bsp *)(class_c_array__o__bsp.array);
    jint i15;
	for (i15 = 0; i15 < i12; i15++)
	{
        jint i16;
		for (i16 = 0; i16 < 4; i16++)
		{
			i1 += 2;
			o[i15].int_array_4x3__b__plane[i16][0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			o[i15].int_array_4x3__b__plane[i16][1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			o[i15].int_array_4x3__b__plane[i16][2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		}
		i1 += 2;
        i2 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        jint i3 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        jint i4 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
		o[i15].int__c__prev_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		i1 += 2;
		o[i15].int__d__next_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		o[i15].int_array_3__a__normal[0] = i2;
		o[i15].int_array_3__a__normal[1] = i3;
		o[i15].int_array_3__a__normal[2] = i4;
		if ((o[i15].int__c__prev_scene == 999) || (o[i15].int__d__next_scene == 999)) {
			//this.cW = true;
		}
		if (i2 != 0) {
			o[i15].byte__a__direction = 1;
		} else if (i3 != 0) {
			o[i15].byte__a__direction = 2;
		} else if (i4 != 0) {
			o[i15].byte__a__direction = 3;
		}
	}
    jint i16;
	for (i16 = 0; i16 < i12; i16++)
	{
		if (o[i16].int__c__prev_scene < 999)
		{
			q[o[i16].int__c__prev_scene].int_array_5__o__bsp[q[o[i16].int__c__prev_scene].int__p__bsp] = i16;
			q[o[i16].int__c__prev_scene].int__p__bsp += 1;
		}
		if (o[i16].int__d__next_scene < 999)
		{
			q[o[i16].int__d__next_scene].int_array_5__o__bsp[q[o[i16].int__d__next_scene].int__p__bsp] = i16;
			q[o[i16].int__d__next_scene].int__p__bsp += 1;
		}
	}
	dprintfss("Get map BSP tree node");
	i1 += 2;
	bU = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map item count", bU);
    ARRAY_NULL(j_l);
    if(bU)
	{
        new_array(&j_l, sizeof(class__j__item), bU);
        class__j__item *l = (class__j__item *)(j_l.array);
        jint i19 = 0;
        jint i20;
		for (i20 = 0; i20 < cx; i20++)
		{
			i1 += 2;
			i18 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			dprintfsii("Scene item count", i20, i18);
			q[i20].int__q__begin = i19;
			q[i20].int__e__end = (i19 + i18);
            jint i21;
			for (i21 = 0; i21 < i18; i21++)
			{
                l[i19].r = ((jshort)i20);
				i1 += 2;
				l[i19].int__m__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
				i1 += 2;
				l[i19].int__n__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
				i1 += 2;
				l[i19].int__o__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
				i1 += 2;
				l[i19].int__p__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
				i1 += 2;
				l[i19].int__q__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
				if (l[i19].int__q__rotation > 360) {
					l[i19].int__q__rotation = (65536 - l[i19].int__q__rotation);
				}
				if (l[i19].int__p__rotation > 360) {
					l[i19].int__p__rotation = (65536 - l[i19].int__p__rotation);
				}
				if (l[i19].int__q__rotation < 0) {
					l[i19].int__q__rotation = (-l[i19].int__q__rotation);
				}
				if (l[i19].int__q__rotation == 360) {
					l[i19].int__q__rotation = 0;
				}
				if (l[i19].int__p__rotation < 0) {
					l[i19].int__p__rotation = (360 + l[i19].int__p__rotation);
				} else {
					l[i19].int__p__rotation = (360 - l[i19].int__p__rotation);
				}
				if (l[i19].int__p__rotation == 360) {
					l[i19].int__p__rotation = 0;
				}
				i1 += 2;
                l[i19].byte__t__obj = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
				if (l[i19].byte__t__obj < 120)
				{
					/*
						 if ((l[i19].byte__t__obj > 39) && (l[i19].byte__t__obj < 44)) {
						 bX[i20] = 1;
						 }
						 */
					if (l[i19].byte__t__obj >= 90)
                    {
						class__j__item *tmp2767_2766 = l + i19;
                        tmp2767_2766->byte__t__obj = ((byte)(tmp2767_2766->byte__t__obj - 50)); // l[i19].byte__t__obj -= 50;
                        //bX[i20] = -1;
					}
					//bu[l[i19].byte__t__obj] = true;
				}
				i1 += 2;
                l[i19].e = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
				if (l[i19].e < 0)
				{
					class__j__item *tmp2841_2840 = l + i19;
                    tmp2841_2840->e = ((jshort)(tmp2841_2840->e + 256));
					l[i19].e += 256;
				}
				i1 += 2;
				i19++;
			}
		}
	}
	//printfi(j_l != NULL);

	i1 += 2;
#if UNPARSED
	dv = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) + 1;
	X = new byte[dv];
	s = new m[dv];
	for (int i21 = 0; i21 < dv; i21++) {
		s[i21] = new m(null);
	}
	i19 = 1;
	if ((bM == 5) && (f.a[3] != 0))
	{
		cF[1] = 0;
		cF[2] = 51511296;
		if (f.a[3] == 1)
		{
			cE = 90;
			cF[0] = 380108800;
		}
		else
		{
			cE = 270;
			cF[0] = -380108800;
		}
	}
	s[0].n = cF[0];
	s[0].o = cF[1];
	s[0].p = cF[2];
#endif
    jint i25;
	for (i25 = 0; i25 < cx; i25++)
	{
		i1 += 2;
		i22 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
        jint i26;
		for (i26 = 0; i26 < i22; i26++)
		{
			i1 += 2;
			//s[i19].n = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//s[i19].v = s[i19].n;
			i1 += 2;
			//s[i19].o = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//s[i19].w = s[i19].o;
			i1 += 2;
			//s[i19].p = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			//s[i19].y = ((byte)c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
#if UNPARSED
			if ((i23 = s[i19].y) > 20)
			{
				i23 -= 20;
				s[i19].u = true;
				s[i19].A = false;
			}
			else if (i23 > 10)
			{
				i23 -= 10;
				s[i19].u = true;
			}
			s[i19].y = ((byte)i23);
			if (i23 != 7) {
				bw[i23] = true;
			}
#endif
			i1 += 2;
#if UNPARSED
			if ((i24 = c(arrayOfByte[i1], arrayOfByte[(i1 + 1)])) > 360) {
				i24 -= 65536;
			}
			if (i24 < 0) {
				i24 = -i24;
			} else {
				i24 = 360 - i24;
			}
			if (i24 == 360) {
				i24 = 0;
			}
			s[i19].s[0] = (-cU[i24]);
			s[i19].s[1] = (-aq[i24]);
			s[i19].z[0] = (-cU[i24]);
			s[i19].z[1] = (-aq[i24]);
			if ((i23 == 4) || (i23 == 5)) {
				aT = (byte)(aT + 1);
			} else {
				dl = (byte)(dl + 1);
			}
			s[i19].r = ((short)i25);
			i19++;
#endif
		}
	}
	i1 += 2;
    jint i23 = (byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	//this.bo = i23;
	//bp = new short[i23 * 3];
    jint i26;
	for (i26 = 0; i26 < i23; i26++)
	{
		i1 += 2;
		//bp[(i26 * 3)] = ((short)c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		i1 += 2;
		//bp[(i26 * 3 + 1)] = ((short)c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		i1 += 2;
		//bp[(i26 * 3 + 2)] = ((short)c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
	}
	i1 += 2;
	i23 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	//ci = false;
	if (i23 == 1)
	{
		//ci = true;
		//n = new int[7];
		i1 += 2;
		//n[0] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[1] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[2] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[3] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[4] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[5] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//n[6] = (c(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	}
#if UNPARSED
	for (int i27 = dv - 1; i27 >= 0; i27--)
	{
		g(s[i27]);
		if (i27 == 0)
		{
			cv = s[i27].r;
			b(s[i27].z);
		}
		if (s[i27].y == 4) {
			s[i27].l = 1250;
		}
	}
	s[0].r = ((short)cv);
	x();
	if (ck) {
		l(0);
	}
#endif

    read_Specnaz3D_map_items(&j_l, resc_path);

    class__k__lvl lv;
    ZERO(&lv, class__k__lvl);
    lv.class_g_array__q__scene = g_q;
    lv.class_j_array__l__item = j_l;
    lv.int_array__j__normal = int_j;
    lv.int_array__dm__texcoord = int_dm;
    lv.int__cC__rotation = cC;
    lv.int__cE__rotation = cE;
    lv.int_array__cF__translation[0] = cF[0];
    lv.int_array__cF__translation[1] = cF[1];
    lv.int_array__cF__translation[2] = cF[2];
    lv.class_c_array__o__bsp = class_c_array__o__bsp;
	return lv;
}

void class_k__function_P_void__item(class__j__item *l, const byte arrayOfByte1[])
{
	if(!l || !arrayOfByte1)
		return;
    jint i1 = -2;
	i1 += 2;
	l->int__f__aabb = (marge_digit(arrayOfByte1[0], arrayOfByte1[1]) << 16); // c
	i1 += 2;
	l->int__g__aabb = (marge_digit(arrayOfByte1[2], arrayOfByte1[3]) << 16); // d
	i1 += 2;
	l->int__h__aabb = (marge_digit(arrayOfByte1[4], arrayOfByte1[5]) << 16); // e
	i1 += 2;
	l->int__i__aabb = (marge_digit(arrayOfByte1[6], arrayOfByte1[7]) << 16); // f
	i1 += 2;
	l->int__j__aabb = (marge_digit(arrayOfByte1[8], arrayOfByte1[9]) << 16); // g
	i1 += 2;
	l->int__k__aabb = (marge_digit(arrayOfByte1[10], arrayOfByte1[11]) << 16); // h
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i3 = marge_digit(arrayOfByte1[20], arrayOfByte1[21]);
	i1 += 2;
    jint i2 = marge_digit(arrayOfByte1[22], arrayOfByte1[23]);
	i1 += 2;
    jint i4 = marge_digit(arrayOfByte1[24], arrayOfByte1[25]);
	//k[i5].i = i3;
    new_array(&l->int_array_6__a__vertex, ESIZE(jint, 4), i3 * 6);
    new_array(&l->int_array_16__b__primitive, ESIZE(jint, 4), i2 * 16);
    jint *a = (jint *)(l->int_array_6__a__vertex.array);
    jint *b = (jint *)(l->int_array_16__b__primitive.array);
	i4 *= 2;
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
    jint i7;
	for (i7 = 0; i7 < i2; i7++)
	{
		i1 += 2;
		b[(i7 * 16)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i7 * 16 + 1)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i7 * 16 + 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
	}
    jint i8;
	for (i8 = 0; i8 < i2; i8++)
	{
		i1 += 2;
        b[(i8 * 16 + 10)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i8 * 16 + 11)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i8 * 16 + 12)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
	}
    jint i9;
	for (i9 = 0; i9 < i4; i9 += 2)
	{
		i1 += 2;
        arrayOfInt[i9] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i9 + 1)] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
    jint i10;
	for (i10 = 0; i10 < i2; i10++)
	{
		i1 += 2;
        b[(i10 * 16 + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i10 * 16 + 4)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i10 * 16 + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i10 * 16 + 6)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i10 * 16 + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i10 * 16 + 8)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
	}
	i1 += 2;
    jint i11;
	for (i11 = 0; i11 < i2; i11++)
	{
		b[(i11 * 16 + 9)] = arrayOfByte1[i1];
		//bv[k[i5].b[(i11 * 16 + 9)]] = true;
		i1++;
	}
	free(arrayOfInt);
	/*
		 byte[] arrayOfByte2 = f.g.a(f.a(B), -1);
		 for (int i10 = 0; i10 < l.length; i10++)
		 {
		 j localj;
		 if ((localj = l[i10]).t < 120)
		 {
		 i8 = localj.t << 2;
		 if (arrayOfByte2[i8] == 1) {
		 localj.d = true;
		 }
		 if (arrayOfByte2[(i8 + 1)] == 1) {
		 localj.s = true;
		 }
		 if (arrayOfByte2[(i8 + 2)] == 1) {
		 localj.c = true;
		 }
		 if (arrayOfByte2[(i8 + 3)] == 1) {
		 localj.l = true;
		 }
		 o localo = k[localj.t];
		 localj.a = localo.a;
		 localj.b = localo.b;
		 localj.f = localo.c;
		 localj.i = localo.f;
		 localj.g = localo.d;
		 localj.j = localo.g;
		 localj.h = localo.e;
		 localj.k = localo.h;
		 if (localj.t == 50)
		 {
		 localj.f = 1300;
		 localj.g = 4000;
		 }
		 }
		 }
		 */
}

