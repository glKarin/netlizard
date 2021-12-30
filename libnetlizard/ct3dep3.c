#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

#define dprintfsi(s, d) nllogfln("CT 3D Ep3 : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("CT 3D Ep3 : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("CT 3D Ep3 : "#s)
#define dprintfs(s) nllogfln("CT 3D Ep3 : %s", s)

// ct3d ep3 no jad

// test
static void ct3dep3_a(array *data, jint paramInt1, jint paramInt2);

typedef struct _class__f__scene // 地图模型场景
{
  short_array short_array_3__a__vertex;
  int_array int_array_7__b__primitive;
  jint c;
  jint d;
  jint int__e__end; // = 0;
  jint int__f__aabb;
  jint int__g__aabb;
  jint int__h__aabb;
  jint int__i__aabb;
  jint int__j__aabb;
  jint int__k__aabb;
  int_array int_array_2__l__plane;
  jint m;
  jint n;
  jint int_array_7__o__bsp[7]; // = new int[7];
  jint int__p__bsp; // = 0;
  jint int__q__begin; // = 0;
} class__f__scene;

typedef struct _class__a__bsp
{
  byte byte__a__direction;
    jint int_array_3__a__normal[3];
  jint int_array_4x3__b__plane[4][3]; // u[] b = new u[4];
  jint int__c__prev_scene;
  jint int__d__next_scene;
  jboolean e; // = false;
} class__a__bsp;

typedef struct _class__i__item // 物品
{
  int_array int_array_6__a__vertex;
  int_array int_array_16__b__primitive;
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
  jint int__m__translation;
  jint int__n__translation;
  jint int__o__translation;
  jint int__p__rotation; // = 0;
  jint int__q__rotation; // = 0;
  jshort r;
  jboolean s;
  byte byte__t__obj;
  jboolean u; // = true;
} class__i__item;

typedef struct _class__t__lvl
{
    jint int_array__fd__translation[3];
    jint int__fa__rotation;
    jint int__fc__rotation;
    int_array int_array__o__normal;
    int_array int_array__gg__texcoord;
    T_array(class__f__scene) class_f_array__v__scene;
    T_array(class__i__item) class_i_array__q__item;
    T_array(class__a__bsp) class_a_array__t__bsp;
} class__t__lvl;

static void delete_class__f__scene(class__f__scene *scene)
{
    delete_array(&scene->short_array_3__a__vertex);
    delete_array(&scene->int_array_7__b__primitive);
    delete_array(&scene->int_array_2__l__plane);
}

static void delete_class__i__item(class__i__item *item)
{
    delete_array(&item->int_array_6__a__vertex);
    delete_array(&item->int_array_16__b__primitive);
}

static void delete_class__t__lvl(class__t__lvl *lv)
{
    int i;

    T_array(class__f__scene) *f_v = &lv->class_f_array__v__scene;
    for(i = 0; i < f_v->length; i++)
    {
        class__f__scene *obj = ((class__f__scene *)(f_v->array)) + i;
        delete_class__f__scene(obj);
    }
    delete_array(f_v);

    T_array(class__i__item) *i_q = &lv->class_i_array__q__item;
    for(i = 0; i < i_q->length; i++)
    {
        class__i__item *obj = ((class__i__item *)(i_q->array)) + i;
        delete_class__i__item(obj);
    }
    delete_array(i_q);
    delete_array(&lv->class_a_array__t__bsp);
}

static class__t__lvl  class_t__function_b_1int_2bool__scene(const byte arrayOfByte[], boolean online, const char *resc_path);
static void class_t__function_av_void__item(class__i__item *p, const byte arrayOfByte1[]);
static void read_CT3DEp3_map_items(T_array(class__i__item) *i_q, const char *resc_path);
static void ct3dep3_god_mode_version__class_b__function_a_1string_2int__decode_recordstore_record(byte_array *arr);

NLboolean nlReadCT3DEp3ModelFile(const char* name, NLint level, const char *resc_path, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DEp3ModelData(arr.array, arr.length, level, resc_path, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DEp3ModelData(const char* data, NLsizei res, NLint paramInt, const char *resc_path, NETLizard_3D_Model *model)
{
    if(paramInt == 13 || paramInt == 15)
        return NL_FALSE; // TODO: Level 13 and 15 is not support now.
    int dr = 0; // has sky
    if (paramInt == 5 || paramInt == 6 || paramInt == 8 || paramInt == 9 || paramInt == 10) {
		dr = 1;
    }
    /*
            if (k.cC || k.cE) {
                if (k.cC) {
                    k.aC = c.c(k.hb);
                    b(k.dv, true);
                    k.cE = false;
                }
                if (k.cE) {
                    k.aC = c.j();
                    b(k.dv, true);
                    k.gr = System.currentTimeMillis();
                    k.go = k.gr + 20000L;
                }
            }
            else if (!k.dq) {
                b(k.dv, false);
            }
            else {
                if (k.dr >= k.cV) {
                    k.cC = true;
                    k.aC = c.c(k.dr);
                    b(k.dr, true);
                }
                else {
                    b(k.dr, false);
                }
                k.cE = false;
            }
      */
    jboolean b = (paramInt >= 8) ? jtrue : jfalse;
	/*
	if(b)
		ct3dep3_a(arr, 105, 20); // UNUSED online level(8 - 15), now not support to read.
		*/
    byte_array rms_arr;
    ARRAY_NULL(rms_arr);
    const char *parse_data = NULL;
    if(b)
    {
        new_array_with_data(&rms_arr, 1, res, data, res * 1);
        ct3dep3_god_mode_version__class_b__function_a_1string_2int__decode_recordstore_record(&rms_arr); // in j2me, it is get record from record store, and decode data.
        parse_data = rms_arr.array;
    }
    else
        parse_data = data;

    byte *arrayOfByte = (byte *)(parse_data);
    class__t__lvl lvl = class_t__function_b_1int_2bool__scene(arrayOfByte, b, resc_path);
    class__t__lvl *lv = &lvl;

	ZERO(model, NETLizard_3D_Model);
    T_array(class__f__scene) *f_v = &lv->class_f_array__v__scene;
    jint *gg = (jint *)(lv->int_array__gg__texcoord.array);
    jint *o = (jint *)(lv->int_array__o__normal.array);
    model->data.data = NEW_II(NETLizard_3D_Mesh, f_v->length);
    model->data.count = f_v->length;
	model->has_sky = dr;
    model->type = NL_CT_3D_EP3_MAP_MODEL;
    model->game = NL_CONTR_TERRORISM_3D_EPISODE_3;

	int i;
	for(i = 0; i < f_v->length; i++)
	{
        NETLizard_3D_Mesh *mesh = ((NETLizard_3D_Mesh *)(model->data.data)) + i;
		class__f__scene *obj = ((class__f__scene *)(f_v->array)) + i;

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
			p->texcoord[0] = gg[(s_b[j + 3])];
			p->texcoord[1] = gg[(s_b[j + 3]) + 1];
			p->texcoord[2] = gg[(s_b[j + 4])];
			p->texcoord[3] = gg[(s_b[j + 4]) + 1];
			p->texcoord[4] = gg[(s_b[j + 5])];
			p->texcoord[5] = gg[(s_b[j + 5]) + 1];
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
			plane->normal[0] = o[normal_index];
			plane->normal[1] = o[normal_index + 1];
			plane->normal[2] = o[normal_index + 2];
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
    T_array(class__i__item) *i_q = &lv->class_i_array__q__item;
    model->item_data.count = i_q->length;
    model->item_data.data = NEW_II(NETLizard_3D_Item_Mesh, model->item_data.count);
	for(i = 0; i < i_q->length; i++)
	{
        NETLizard_3D_Item_Mesh *mesh = ((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i;
		class__i__item *obj = ((class__i__item *)(i_q->array)) + i;
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
    if(ARRAY_DATA(lv->class_a_array__t__bsp))
	{
        class__a__bsp *bsp = (class__a__bsp *)(lv->class_a_array__t__bsp.array);
        model->bsp_data.count = lv->class_a_array__t__bsp.length;
        model->bsp_data.data = NEW_II(NETLizard_BSP_Tree_Node, model->bsp_data.count);
        NETLizard_BSP_Tree_Node *bsp_data = (NETLizard_BSP_Tree_Node *)(model->bsp_data.data);
		int j;
        for(j = 0; j < lv->class_a_array__t__bsp.length; j++)
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
	model->start_rotation[0] = lv->int__fa__rotation;
	model->start_rotation[1] = lv->int__fc__rotation;
	model->start_position[0] = lv->int_array__fd__translation[0] >> 16;
	model->start_position[1] = lv->int_array__fd__translation[1] >> 16;
	model->start_position[2] = lv->int_array__fd__translation[2] >> 16;

    // free
    delete_class__t__lvl(lv);
    delete_array(&rms_arr);

    return NL_TRUE;
}

NLboolean nlReadCT3DEp3ItemModelFile(const char* name, NLint index, NETLizard_3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadCT3DEp3ItemModelData(arr.array, arr.length, index, model);
    delete_array(&arr);
    return b;
}

NLboolean nlLoadCT3DEp3ItemModelData(const char* data, NLsizei res, NLint index, NETLizard_3D_Model *model)
{
	ZERO(model, NETLizard_3D_Model);

	class__i__item obj;
    class_t__function_av_void__item(&obj, (const byte *)data);
    NETLizard_3D_Item_Mesh *mesh = NEW(NETLizard_3D_Item_Mesh);
    ZERO(mesh, NETLizard_3D_Item_Mesh);
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
    delete_class__i__item(&obj);
    model->data.data = NULL;
    model->data.count = 0;
    model->item_data.data = mesh;
    model->item_data.count = 1;
	model->has_sky = 0;
    model->type = NL_CT_3D_EP3_ITEM_MODEL;
    model->game = NL_CONTR_TERRORISM_3D_EPISODE_3;
    return NL_TRUE;
}

void read_CT3DEp3_map_items(T_array(class__i__item) *i_q, const char *resc_path)
{
	if(!i_q)
		return;
	int i;
	for(i = 0; i < i_q->length; /* 45 */i++)
	{
		class__i__item *q = ((class__i__item *)(i_q->array)) + i;
		jint i11 = q->byte__t__obj;

        char *name = make_resource_file_path(CT3DEP3_OBJ_SUBFIX, i11, resc_path);
        array arr;
        int res = file_get_contents(name, &arr);
        free(name);
        if(res > 0)
        {
            ARRAY_NULL(q->int_array_6__a__vertex);
            ARRAY_NULL(q->int_array_16__b__primitive);
            class_t__function_av_void__item(q, arr.array);
            delete_array(&arr);
        }
        else
        {
            ARRAY_NULL(q->int_array_6__a__vertex);
            ARRAY_NULL(q->int_array_16__b__primitive);
        }
	}
}

/******** static ********/

class__t__lvl class_t__function_b_1int_2bool__scene(const byte arrayOfByte[], boolean online, const char *resc_path)
{
    jint fd[3];
    jint fa;
    jint fc;
    jint eU;
    int_array int_o;
    int_array int_gg;
    T_array(class__f__scene) f_v;
    T_array(class__i__item) i_q;
    jint dL;

    jint i1 = -2;
    if (online)
	{
		i1 += 20;
    }
	i1 += 2;
    jint i2 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	if (i2 != 0) {
		//eA = new short[i2];
    }
    jint i5;
    for (i5 = 0; i5 < i2; i5++)
	{
		i1 += 2;
		//eA[i5] = ((short)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
	}
	i1 += 2;
	fd[0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	i1 += 2;
	fd[1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	i1 += 2;
	fd[2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	//eJ = (short)(fd[0] >> 16);
	//eK = (short)(fd[1] >> 16);
	//eL = (short)(fd[2] >> 16);
	i1 += 2;
	fa = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	i1 += 2;
	fc = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	if (fc > 361) {
		fc -= 65536;
	}
	if (fa > 361) {
		fa -= 65536;
	}
	if (fc < 0) {
		fc = -fc;
	} else {
		fc = 360 - fc;
	}
	if (fc == 360) {
		fc = 0;
	}
	if (fa < 0) {
		fa = 270 + fa;
	} else {
		fa = 270 - fa;
	}
	if (fa == 360) {
		fa = 0;
	}
	dprintfss("Get start position and axis");
	i1 += 2;
	eU = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map scene count", eU);
	i1 += 2;
    jint i6 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map normal count", i6);

    new_array(&int_o, ESIZE(jint, 4), i6);
    jint *o = (jint *)(int_o.array);
    jint i7;
	for (i7 = 0; i7 < i6; i7 += 3)
	{
		i1 += 2;
        o[i7] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        o[(i7 + 1)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
		i1 += 2;
        o[(i7 + 2)] = ((jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L));
	}
	dprintfss("Get map normal");
	i1 += 2;
	i6 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Texcoord count", i6);

    new_array(&int_gg, ESIZE(jint, 4), i6);
    jint *gg = (jint *)(int_gg.array);
    jint i8;
	for (i8 = 0; i8 < i6; i8 += 2)
	{
		i1 += 2;
        gg[i8] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
		i1 += 2;
        gg[(i8 + 1)] = ((jint)(64 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 100L));
	}
	dprintfss("Get map texcoord");

    new_array(&f_v, sizeof(class__f__scene), eU);
    class__f__scene *v = (class__f__scene *)(f_v.array);
	//dT = new byte[eU];
    jint i20;
    jint i24;
    jint i13;
	for (i13 = 0; i13 < eU; i13++)
	{
		dprintfsi("Get map scene data", i13);
		i1 += 2;
		v[i13].int__f__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		v[i13].int__g__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		v[i13].int__h__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		v[i13].int__i__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		v[i13].int__j__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		v[i13].int__k__aabb = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		dprintfss("Get scene AABB");
		i1 += 2;
        jint i11 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene vertex coord count", i11);
		i1 += 2;
        jint i10 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene primitive count", i10);
        new_array(&v[i13].short_array_3__a__vertex, ESIZE(jshort, 2), i11 * 3);
        jshort *a = (jshort *)(v[i13].short_array_3__a__vertex.array);
		v[i13].m = i11;
        new_array(&v[i13].int_array_7__b__primitive, ESIZE(jint, 4), i10 * 7);
        jint *b = (jint *)(v[i13].int_array_7__b__primitive.array);
		v[i13].n = i10;
        jint i14;
		for (i14 = 0; i14 < i11; i14++)
		{
			i1 += 2;
            a[(i14 * 3)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			i1 += 2;
            a[(i14 * 3 + 1)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			i1 += 2;
            a[(i14 * 3 + 2)] = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		}
		dprintfss("Get scene vertex coord");
        jint *arrayOfInt1 = b;
        jint i16 = 0;
        jint i17;
		for (i17 = 0; i17 < i10; i17++)
		{
			i16 = i17 * 7;
			i1 += 2;
			b[i16] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3);
			i1 += 2;
			b[(i16 + 1)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3);
			i1 += 2;
			b[(i16 + 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3);
		}
		dprintfss("Get scene primitive vertex coord index");
		i1 += 2;
        jint i18 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsi("Scene plane count", i18);
        new_array(&v[i13].int_array_2__l__plane, ESIZE(jint, 4), i18 * 2);
        jint *l = (jint *)(v[i13].int_array_2__l__plane.array);
        jint i19;
		for (i19 = 0; i19 < i18; i19++)
		{
			i1 += 2;
			l[(i19 * 2)] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 3);
			i1 += 2;
			l[(i19 * 2 + 1)] = arrayOfInt1[(marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) * 7)];
		}
		dprintfss("Get scene plane normal index and point vertex coord index");
		v[i13].c = (v[i13].int__f__aabb + v[i13].int__i__aabb >> 1);
		v[i13].d = (v[i13].int__g__aabb + v[i13].int__j__aabb >> 1);
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
			i1++;
			// ??? TODO
			byte i21 = arrayOfByte[i1];
			if (i21 < 0) {
				i21 += 256;
			}
			b[(i23 * 7 + 6)] = i21;
			//cK[v[i13].b[(i23 * 7 + 6)]] = true;
		}
		for (i24 = 0; i24 < i10; i24++)
		{
			i1++;
            jint i22 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			if (i22 < 0) {
				i22 += 256;
			}
            jint i21 = b[(i24 * 7 + 6)] & 0xFF;
			b[(i24 * 7 + 6)] = ((i22 << 8) + i21);
		}
		dprintfss("Get primitive texture index");
		i1--;
	}
	i1 += 2;
    jint i12 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map BSP tree node count", i12);
    T_array(class__a__bsp) class_a_array__t__bsp;
    new_array(&class_a_array__t__bsp, sizeof(class__a__bsp), i12);
    class__a__bsp *t = (class__a__bsp *)(class_a_array__t__bsp.array);
    jint i15;
	for (i15 = 0; i15 < i12; i15++)
	{
        jint i16;
		for (i16 = 0; i16 < 4; i16++)
		{
			i1 += 2;
			t[i15].int_array_4x3__b__plane[i16][0] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			t[i15].int_array_4x3__b__plane[i16][1] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			t[i15].int_array_4x3__b__plane[i16][2] = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		}
		i1 += 2;
        i2 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        jint i3 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
        jint i4 = (jint)(65536 * marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) / 10000L);
		i1 += 2;
		t[i15].int__c__prev_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		i1 += 2;
		t[i15].int__d__next_scene = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) - 1);
		t[i15].int_array_3__a__normal[0] = i2;
		t[i15].int_array_3__a__normal[1] = i3;
		t[i15].int_array_3__a__normal[2] = i4;
		if ((t[i15].int__c__prev_scene == 999) || (t[i15].int__d__next_scene == 999)) {
			//fC = true;
		}
		if (i2 != 0) {
			t[i15].byte__a__direction = 1;
		} else if (i3 != 0) {
			t[i15].byte__a__direction = 2;
		} else if (i4 != 0) {
			t[i15].byte__a__direction = 3;
		}
	}
    jint i16;
	for (i16 = 0; i16 < i12; i16++)
	{
		if (t[i16].int__c__prev_scene < 999)
		{
			v[t[i16].int__c__prev_scene].int_array_7__o__bsp[v[t[i16].int__c__prev_scene].int__p__bsp] = i16;
			v[t[i16].int__c__prev_scene].int__p__bsp += 1;
		}
		if (t[i16].int__d__next_scene < 999)
		{
			v[t[i16].int__d__next_scene].int_array_7__o__bsp[v[t[i16].int__d__next_scene].int__p__bsp] = i16;
			v[t[i16].int__d__next_scene].int__p__bsp += 1;
		}
	}
#if 0
	int i18;
	for (i18 = 0; i18 < f_v->length; i18++)
	{
		int *arrayOfInt2 = null;
		arrayOfInt2 = v[i18].int_array_7__o__bsp;
		v[i18].int_array_7__o__bsp = null;
		i19 = v[i18].int__p__bsp;
		v[i18].int_array_7__o__bsp = NEW_II(int i19);
		for (i20 = 0; i20 < i19; i20++) {
			v[i18].int_array_7__o__bsp[i20] = arrayOfInt2[i20];
		}
	}
#endif
	dprintfss("Get map BSP tree node");
	i1 += 2;
	dL = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	dprintfsi("Map item count", dL);
    new_array(&i_q, sizeof(class__i__item), dL + 1);
    class__i__item *q = (class__i__item *)(i_q.array);
    jint i21 = 0;
    jint i22;
	for (i22 = 0; i22 < eU; i22++)
	{
		i1 += 2;
		i20 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		dprintfsii("Scene item count", i21, i20);
		v[i22].int__q__begin = i21;
		v[i22].int__e__end = (i21 + i20);
        jint i23;
		for (i23 = 0; i23 < i20; i23++)
		{
            q[i21].r = ((jshort)i22);
			i1 += 2;
			q[i21].int__m__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			q[i21].int__n__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			q[i21].int__o__translation = (marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			q[i21].int__p__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			i1 += 2;
			q[i21].int__q__rotation = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			if (q[i21].int__q__rotation > 360) {
				q[i21].int__q__rotation = (65536 - q[i21].int__q__rotation);
			}
			if (q[i21].int__p__rotation > 360) {
				q[i21].int__p__rotation = (65536 - q[i21].int__p__rotation);
			}
			if (q[i21].int__q__rotation < 0) {
				q[i21].int__q__rotation = (-q[i21].int__q__rotation);
			}
			if (q[i21].int__q__rotation == 360) {
				q[i21].int__q__rotation = 0;
			}
			if (q[i21].int__p__rotation < 0) {
				q[i21].int__p__rotation = (360 + q[i21].int__p__rotation);
			} else {
				q[i21].int__p__rotation = (360 - q[i21].int__p__rotation);
			}
			if (q[i21].int__p__rotation == 360) {
				q[i21].int__p__rotation = 0;
			}
			i1 += 2;
			q[i21].byte__t__obj = ((byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			if (q[i21].byte__t__obj == 7) {
				q[i21].byte__t__obj = q[i21].byte__t__obj;
			}
			if (q[i21].byte__t__obj < 120)
			{
				if ((q[i21].byte__t__obj > 37) && (q[i21].byte__t__obj < 42)) {
					//dT[i22] = 1;
				}
				if (q[i21].byte__t__obj >= 90)
				{
					class__i__item *tmp2972_2971 = q + i21;
					tmp2972_2971->byte__t__obj = ((byte)(tmp2972_2971->byte__t__obj - 50));
					//dT[i22] = -1;
				}
			}
			//cJ[q[i21].t] = true;
			i1 += 2;
            q[i21].e = ((jshort)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
			if (q[i21].e < 0)
			{
				class__i__item *tmp3048_3047 = q + i21;
                tmp3048_3047->e = ((jshort)(tmp3048_3047->e + 256));
			}
			if (q[i21].e < 100) {
				q[i21].e = 100;
			}
			i1 += 2;
			i21++;
		}
	}

	i1 += 2;
#if UNPARSED
	gC = b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) + 1;
	az = new byte[gC];
	C = new v[gC];
	for (int i23 = 0; i23 < gC; i23++) {
		C[i23] = new v(null);
	}
	i21 = 1;
	C[0].n = fd[0];
	C[0].o = fd[1];
	C[0].p = fd[2];
#endif
    jint i27;
	for (i27 = 0; i27 < eU; i27++)
	{
		i1 += 2;
		i24 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
        jint i28;
		for (i28 = 0; i28 < i24; i28++)
		{
			i1 += 2;
			//C[i21].n = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//C[i21].v = C[i21].n;
			i1 += 2;
			//C[i21].o = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			//C[i21].w = C[i21].o;
			i1 += 2;
			//C[i21].p = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
			i1 += 2;
			//C[i21].z = ((byte)b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
#if UNPARSED
			i25 = C[i21].z;
			if (i25 > 20)
			{
				i25 -= 20;
				C[i21].u = true;
				C[i21].B = false;
			}
			else if (i25 > 10)
			{
				i25 -= 10;
				C[i21].u = true;
			}
			C[i21].z = ((byte)i25);
			if (C[i21].z == 0) {
				C[i21].z = C[i21].z;
			}
			if (i25 != 7) {
				cL[i25] = true;
			}
#endif
			i1 += 2;
#if UNPARSED
			int i26 = b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
			if (i26 > 360) {
				i26 -= 65536;
			}
			if (i26 < 0) {
				i26 = -i26;
			} else {
				i26 = 360 - i26;
			}
			if (i26 == 360) {
				i26 = 0;
			}
			C[i21].s[0] = (-fA[i26]);
			C[i21].s[1] = (-aZ[i26]);
			C[i21].A[0] = (-fA[i26]);
			C[i21].A[1] = (-aZ[i26]);
			C[i21].r = ((short)i27);
			i21++;
#endif
		}
	}
	i1 += 2;
    jint i25 = (byte)marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	//cy = i25;
	//cz = new short[i25 * 3];
    jint i28;
	for (i28 = 0; i28 < i25; i28++)
	{
		i1 += 2;
		//cz[(i28 * 3)] = ((short)b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		i1 += 2;
		//cz[(i28 * 3 + 1)] = ((short)b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		i1 += 2;
		//cz[(i28 * 3 + 2)] = ((short)b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
	}
	i1 += 2;
	i25 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
	//em = false;
	if (i25 == 1)
	{
		//em = true;
		//s = new int[7];
		i1 += 2;
		//s[0] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[1] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[2] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[3] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[4] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[5] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
		i1 += 2;
		//s[6] = (b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]) << 16);
	}
	//fE = null;
	//eN = null;
	i1 += 2;
    jint i29 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
#if UNPARSED
	if (i29 != 0) {
		eN = new short[i29];
	} else {
		eN = null;
	}
#endif
	i1 += 2;
    jint i30;
	for (i30 = 0; i30 < i29; i30++)
	{
		//eN[i30] = ((short)b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]));
		i1 += 2;
	}
    jint i31 = marge_digit(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
#if UNPARSED
	if (i31 != 0) {
		fE = new byte[v.length];
	} else {
		fE = null;
	}
#endif
    jint i32;
	for (i32 = 0; i32 < i31; i32++)
	{
		i1 += 2;
		//int i33 = b(arrayOfByte[i1], arrayOfByte[(i1 + 1)]);
		//fE[i33] = 1;
	}
	i1 += 2;
	char *str = NULL;
    jint i34;
	for (i34 = 0; i34 < 30; i34++)
	{
		if (arrayOfByte[i1] == 0) {
			break;
		}
        jint i35 = arrayOfByte[i1] & 0xFF;
		if ((i35 >= 192) && (i35 <= 255)) {
			i35 += 848;
		}
		if(!str)
		{
			str = NEW_II(char, 1 + 1);
			memset(str, '\0', sizeof(char) * (1 + 1));
			str[0] = (char)i35;
		}
		else
		{
			char *p = str;
			size_t len = strlen(p);
			str = NEW_II(char, len + 1 + 1);
			memset(str, '\0', sizeof(char) * (len + 1 + 1));
			strcpy(str, p);
			free(p);
			str[len + 1] = (char)i35;
		}
		i1++;
	}
	dprintfs(str);
    DELETE(str);
#if UNPARSED
	cv = str;
	for (int i35 = gC - 1; i35 >= 0; i35--)
	{
		h(C[i35]);
		if (i35 == 0)
		{
			eR = C[i35].r;
			b(C[i35].A);
		}
	}
	C[0].r = ((short)eR);
	byte[] arrayOfByte = null;
	if (es) {
		i((du - 1) % 5);
	}
#endif
    read_CT3DEp3_map_items(&i_q, resc_path);

    class__t__lvl lv;
    ZERO(&lv, class__t__lvl);
    lv.int_array__fd__translation[0] = fd[0];
    lv.int_array__fd__translation[1] = fd[1];
    lv.int_array__fd__translation[2] = fd[2];
    lv.int__fa__rotation = fa;
    lv.int__fc__rotation = fc;
    lv.int_array__o__normal = int_o;
    lv.int_array__gg__texcoord = int_gg;
    lv.class_f_array__v__scene = f_v;
    lv.class_i_array__q__item = i_q;
    lv.class_a_array__t__bsp = class_a_array__t__bsp;
    return lv;
}

void class_t__function_av_void__item(class__i__item *p, const byte arrayOfByte1[])
{
	// 45
	if(!p || !arrayOfByte1)
		return;
	//if ((cJ[i11] != 0) || (i11 == 44))
    jint i1 = -2;
	i1 += 2;
	p->int__f__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // e
	i1 += 2;
	p->int__g__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // f
	i1 += 2;
	p->int__h__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // g
	i1 += 2;
	p->int__i__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // h
	i1 += 2;
	p->int__j__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // i
	i1 += 2;
	p->int__k__aabb = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16); // j
    jint i2 = p->int__f__aabb >> 16;
    jint i3 = p->int__g__aabb >> 16;
    jint i4 = p->int__h__aabb >> 16;
    jint i5 = i2 * i2 + i3 * i3 + i4 * i4;
    jint i6 = 65536;
    jint i7 = 0;
	while (i6 >>= 1 > 0) {
		if ((i7 + i6) * (i7 + i6) <= i5) {
			i7 += i6;
		}
	}
	p->d = ((i7 << 3) * 240 << 1);
	p->c = ((i7 << 1) * 260);
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
	i1 += 2;
    jint i9 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	i1 += 2;
    jint i8 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	i1 += 2;
    jint i10 = marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]);
	//p[i11].k = i9;
    new_array(&p->int_array_6__a__vertex, ESIZE(jint, 4), i9 * 6);
    jint *a = (jint *)(p->int_array_6__a__vertex.array);
    new_array(&p->int_array_16__b__primitive, ESIZE(jint, 4), i8 * 16);
    jint *b = (jint *)(p->int_array_16__b__primitive.array);
	i10 *= 2;
    jint *arrayOfInt = NEW_II(jint, i10);
    jint i12;
	for (i12 = 0; i12 < i9; i12++)
	{
		i1 += 2;
		a[(i12 * 6)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i12 * 6 + 1)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
		i1 += 2;
		a[(i12 * 6 + 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) << 16);
	}
    jint i13;
	for (i13 = 0; i13 < i8; i13++)
	{
		i1 += 2;
		b[(i13 * 16)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i13 * 16 + 1)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
		i1 += 2;
		b[(i13 * 16 + 2)] = (marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 6);
	}
    jint i14;
	for (i14 = 0; i14 < i8; i14++)
	{
		i1 += 2;
        b[(i14 * 16 + 10)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i14 * 16 + 11)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
		i1 += 2;
        b[(i14 * 16 + 12)] = ((jint)(65536 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 10000L));
	}
    jint i15;
	for (i15 = 0; i15 < i10; i15 += 2)
	{
		i1 += 2;
        arrayOfInt[i15] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
		i1 += 2;
        arrayOfInt[(i15 + 1)] = ((jint)(64 * marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) / 100L));
	}
    jint i16;
	for (i16 = 0; i16 < i8; i16++)
	{
		i1 += 2;
        b[(i16 * 16 + 3)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 4)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i16 * 16 + 5)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 6)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
		i1 += 2;
        b[(i16 * 16 + 7)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2)]);
        b[(i16 * 16 + 8)] = ((jshort)arrayOfInt[(marge_digit(arrayOfByte1[i1], arrayOfByte1[(i1 + 1)]) * 2 + 1)]);
	}
	i1 += 2;
    jint i17;
	for (i17 = 0; i17 < i8; i17++)
	{
		b[(i17 * 16 + 9)] = arrayOfByte1[i1];
		//cK[p[i11].b[(i17 * 16 + 9)]] = true;
		i1++;
	}
	free(arrayOfInt);
	/*
		 arrayOfByte1 = null;

		 byte[] arrayOfByte2 = s.k.b(s.a(H), -1);
		 for (int i16 = 0; i16 < q.length; i16++) {
		 if (i16 == q.length - 1)
		 {
		 q[i16].a = p[44].a;
		 q[i16].b = p[44].b;
		 q[i16].f = p[44].e;
		 q[i16].i = p[44].h;
		 q[i16].g = p[44].f;
		 q[i16].j = p[44].i;
		 q[i16].h = p[44].g;
		 q[i16].k = p[44].j;
		 q[i16].t = 44;
		 }
		 else
		 {
		 i locali = q[i16];
		 if ((!dp ? (locali.t > 0) && (locali.t < 8) : locali.t == 21) || (locali.t >= 120)) {
		 continue;
		 }
		 i14 = locali.t << 2;
		 if (arrayOfByte2[i14] == 1) {
		 locali.d = true;
		 }
		 if (arrayOfByte2[(i14 + 1)] == 1) {
		 locali.s = true;
		 }
		 if (arrayOfByte2[(i14 + 2)] == 1) {
		 locali.c = true;
		 }
		 if (arrayOfByte2[(i14 + 3)] == 1) {
		 locali.l = true;
		 }
		 k localk = p[locali.t];
		 locali.a = localk.a;
		 locali.b = localk.b;
		 locali.f = localk.e;
		 locali.i = localk.h;
		 locali.g = localk.f;
		 locali.j = localk.i;
		 locali.h = localk.g;
		 locali.k = localk.j;
		 }
		 }
		 arrayOfByte2 = null;
		 */
}

void ct3dep3_a(array *data, jint paramInt1, jint paramInt2)
{
	byte *paramArrayOfByte = (byte *)(data->array);
    jint i1 = data->length;
    jint i3 = (byte)paramInt1;
    jint i2;
	for (i2 = paramInt2; i2 < i1; i2++)
	{
		paramArrayOfByte[i2] = ((byte)(paramArrayOfByte[i2] ^ i3));
		printf("%c", paramArrayOfByte[i2]);
		i3 = (byte)(i3 + 1);
	}
}

void ct3dep3_god_mode_version__class_b__function_a_1string_2int__decode_recordstore_record(byte_array *arr)
{
	byte *localObject = (byte *)(arr->array);
    jint paramInt = 0;
	const char paramString[] = "SonyEricssonW900i175"; // 175 == ((byte)-81) & 0xff
    jint i1 = strlen(paramString);
    jint i2;
	for (i2 = 0; i2 < arr->length; i2++)
	{
		localObject[i2] = ((byte)(localObject[i2] ^ (byte)paramString[paramInt]));
		paramInt++;
		if (paramInt >= i1) {
			paramInt = 0;
		}
	}
}
