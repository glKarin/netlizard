#include "netlizard.h"

#include "priv_local.h"

#define dprintfsi(s, d) nllogfln("RE 3D : "#s"->%d", d)
#define dprintfsii(s, d, d2) nllogfln("RE 3D : ""%d - "#s"->%d", d, d2)
#define dprintfss(s) nllogfln("RE 3D : "#s)
#define dprintfsss(s, s2) nllogfln("RE 3D : "#s"->%s", s2)

static jint class_b__function_a__1byte_2byte(byte paramByte1, byte paramByte2);
static jint class_b__function_c__1byte_array_2int(const byte paramArrayOfByte[], jint paramInt);
static jint class_e__function_a__1Appearance_2int_3byte_array_4int(char **name, jint paramInt1, const byte paramArrayOfByte[], jint paramInt2);
static jint class_e__function_a__1class_k_array_2int_array_3int_4byte_array_5int(NETLizard_RE3D_Mesh *m, jint paramArrayOfInt[], jint paramInt1, const byte paramArrayOfByte[], jint paramInt2);
static jfloat class_b__function_a__1byte_array_2int(const byte paramArrayOfByte[], jint paramInt);

NLboolean nlReadRE3DMeshFile(const char *name, NETLizard_RE3D_Model *model)
{
    array arr;
    int res;
    NLboolean b;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_FALSE;

    b = nlLoadRE3DMeshData(arr.array, arr.length, model);
    delete_array(&arr);
    return b;
}

#define MOVE_2(i1) ((i1) += 2);
#define CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1) class_b__function_a__1byte_2byte(paramArrayOfByte[((i1)/*++*/)], paramArrayOfByte[((i1)+1/*++*/)]); MOVE_2(i1);
#define CROSS_PLATFORM__class_b__function_a__1byte_2byte__temp(paramArrayOfByte, i1, x) class_b__function_a__1byte_2byte(paramArrayOfByte[((i1)/*++*/)], paramArrayOfByte[((i1)+1/*++*/)]) x; MOVE_2(i1);

NLboolean nlLoadRE3DMeshData(const char *data,  NLsizei size, NETLizard_RE3D_Model *model)
{
    NLboolean paramInt = nlIsRE3DMesh(data, size);
    if(!paramInt)
        return NL_FALSE;

    const byte *paramArrayOfByte = (byte *)(data);
    jint i1 = 7; // NL_MESH
    jint i2 = 0;
    jint i3 = paramArrayOfByte[(i1++)];
	dprintfsi("Map texture file count", i3);
    jint i4;
	//k[] arrayOfk = new k[i4 = b_a(paramArrayOfByte[(i1++)], paramArrayOfByte[(i1++)])];

    // scene
    /*
      in windows vc: parameter is right-to-left
      i1 = 8;
      printf("%d, %d\n", i1++, i1++); // -> 9, 8

      so

      i1 = 8; paramArrayOfByte[8] = 60; paramArrayOfByte[9] = 0;
      i4 = class_b__function_a__1byte_2byte(paramArrayOfByte[(i1++)], paramArrayOfByte[(i1++)]);
      i4 = 15360; // (60 << 8) + 0, not (0 << 8) + 60, but linux-arm-gcc is 60

      so

      i4 = class_b__function_a__1byte_2byte(paramArrayOfByte[(i1++)], paramArrayOfByte[(i1++)]);
      ->
      i4 = class_b__function_a__1byte_2byte(paramArrayOfByte[i1], paramArrayOfByte[i1 + 1]); i += 2;
      */
    i4 = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
    dprintfsi("Map scene count", i4);
	//Appearance[] arrayOfAppearance = new Appearance[i3];
    jint *arrayOfInt = NEW_II(jint, i4);
    const NLint tex_count = i3;
    char ** texes = NEW_II(char *, i3);
    char **filenames = texes;
	//t = new String[i3];
	//s = new Texture2D[i3];
	//r = 0;
    jint i5 = 0;
    jint i7;
    jint i6;

    // texture filename
	for (i6 = 0; i6 < i3; i6++) {
		if ((i2 = paramArrayOfByte[(i1++)] & 0xFF) == 225) {
			i1 = class_e__function_a__1Appearance_2int_3byte_array_4int(filenames + i6, i5++, paramArrayOfByte, i1);
            dprintfsss("Map texture file", texes[i6]);
		}
	}

    // mesh
	i5 = 0;
    const NLint mesh_count = i4;
    printf("mesh count %d\n", i4);
    NETLizard_RE3D_Mesh *meshes = NEW_II(NETLizard_RE3D_Mesh, i4);
    for (i7 = 0; i7 < i4; i7++) {
		if ((i2 = paramArrayOfByte[(i1++)] & 0xFF) == 241) {
            NETLizard_RE3D_Mesh *mesh = meshes + i7;
            i1 = class_e__function_a__1class_k_array_2int_array_3int_4byte_array_5int(mesh, arrayOfInt, i5++, paramArrayOfByte, i1);
        }
	}
	for (i6 = 0; i6 < i4; i6++) {
        NETLizard_RE3D_Mesh *mesh = meshes + i6;
		if ((i7 = arrayOfInt[i6]) < 127)
		{
			mesh->tex_index = i7;
			/*
			int i8;
			if (((i8 = arrayOfAppearance[i7].getUserID()) & 0x4) != 0) {
				arrayOfk[i6].a = true;
			}
			*/
		}
		else
            mesh->tex_index = -1;
	}

    free(arrayOfInt);

    ZERO(model, NETLizard_RE3D_Model);
    model->meshes.data = meshes;
    model->meshes.count = mesh_count;
    model->texes.data = texes;
    model->texes.count = tex_count;
    model->game = NL_RACING_EVOLUTION_3D;
    model->type = NL_RE_3D_MAP_MODEL;

    return NL_TRUE;
}

/* ****** static ****** */

jint class_b__function_a__1byte_2byte(byte paramByte1, byte paramByte2)
{
    jint i = paramByte1 & 0xFF;
    jint j;
    return (((j = paramByte2 & 0xFF) & 0xFF) << 8) + (i & 0xFF);
}

jint class_e__function_a__1Appearance_2int_3byte_array_4int(char **name, jint paramInt1, const byte paramArrayOfByte[], jint paramInt2)
{
    jint i1 = paramInt2;
	//Appearance localAppearance = new Appearance();
	//new Material();
	i1 += 13;
	//int i2 = class_b__function_c__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	char *arrayOfChar = NULL; // java char type
    jint i3;
	//Object localObject;
	if ((i3 = paramArrayOfByte[(i1++)] & 0xFF) > 0)
	{
		char localObject[32]; // java char type
        jint i4 = 0;
        jint i5;
		for (i5 = paramArrayOfByte[(i1++)]; i5 != 0; i5 = paramArrayOfByte[(i1++)])
		{
			if (i5 < 0) {
				i5 = 256 + i5;
			}
			if ((i5 >= 192) && (i5 <= 255)) {
				i5 += 848;
			}
			if ((i5 >= 65) && (i5 <= 90)) {
				i5 += 32;
			}
			localObject[(i4++)] = ((char)i5); // java char type
		}
		arrayOfChar = NEW_II(char, i4 + 1); // java char type
		memcpy(arrayOfChar, localObject, sizeof(char) * i4);
		arrayOfChar[i4] = '\0';
		//System.arraycopy(localObject, 0, arrayOfChar, 0, i4);
	}
	*name = arrayOfChar;
	/*
		 (localObject = new PolygonMode()).setCulling(160);
		 ((PolygonMode)localObject).setPerspectiveCorrectionEnable(false);
		 localAppearance.setPolygonMode((PolygonMode)localObject);
		 CompositingMode localCompositingMode;
		 (localCompositingMode = new CompositingMode()).setDepthWriteEnable(true);
		 localCompositingMode.setDepthTestEnable(true);
		 localAppearance.setCompositingMode(localCompositingMode);
		 localAppearance.setUserID(i2);
		 */
	/*
		 if (arrayOfChar != null) {
		 String str1 = new String(arrayOfChar);
		 String str2 = "/" + str1;
		 int i6 = 0;
		 if (str1.endsWith("a.png")) {
		 i6 = 1;
		 }
		 int i7 = -1;
		 for (int i8 = 0; i8 < r; i8++) {
		 if (str2.compareTo(t[i8]) == 0)
		 {
		 i7 = i8;
		 break;
		 }
		 }
		 Texture2D localTexture2D;
		 if (i7 >= 0)
		 {
		 localTexture2D = s[i7];
		 }
		 else
		 {
		 int i9 = 99;
		 if (i6 != 0) {
		 i9 = 100;
		 }
		 Image2D localImage2D = new Image2D(i9, d.a(str2));
		 localTexture2D = new Texture2D(localImage2D);
		 s[r] = localTexture2D;
		 t[r] = str2;
		 r += 1;
		 }
		 if (i6 != 0)
		 {
		 localCompositingMode.setAlphaWriteEnable(true);
		 localAppearance.getCompositingMode().setBlending(64);
		 }
		 localTexture2D.setBlending(228);
		 localTexture2D.setFiltering(210, 210);
		 localTexture2D.setWrapping(240, 240);
		 localAppearance.setTexture(0, localTexture2D);
		 }
		 paramArrayOfAppearance[paramInt1] = localAppearance;
		 */
	return i1;
}

jint class_b__function_c__1byte_array_2int(const byte paramArrayOfByte[], jint paramInt)
{
    jint i = paramArrayOfByte[paramInt] & 0xFF;
    jint j = paramArrayOfByte[(paramInt + 1)] & 0xFF;
    jint k = paramArrayOfByte[(paramInt + 2)] & 0xFF;
    jint m = paramArrayOfByte[(paramInt + 3)] & 0xFF;
	return i + (j << 8) + (k << 16) + (m << 24);
}

jint class_e__function_a__1class_k_array_2int_array_3int_4byte_array_5int(NETLizard_RE3D_Mesh *m, jint paramArrayOfInt[], jint paramInt1, const byte paramArrayOfByte[], jint paramInt2)
{
    jint i1 = paramInt2;
    jint i2 = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
	i1++;
	i1++;
    jint i3 = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
    jint i4 = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
    jint i5 = paramArrayOfByte[(i1++)] & 0xFF;
    jint i6;
    jfloat arrayOfFloat[3];
	for (i6 = 0; i6 < 3; i6++)
	{
		arrayOfFloat[i6] = class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
		i1 += 4;
    }
    m->translation[0] = arrayOfFloat[0];
    m->translation[1] = arrayOfFloat[1];
    m->translation[2] = arrayOfFloat[2];
	//printf("RE 3D map primitive position->x: %f, y: %f, z: %f\n", arrayOfFloat[0], arrayOfFloat[1], arrayOfFloat[2]);
    jfloat f1 = class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	//paramArrayOfk[paramInt1] = new k();
	//paramArrayOfk[paramInt1].h = b.a(paramArrayOfByte, i1);
	//float k_h = class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	//paramArrayOfk[paramInt1].i = b.a(paramArrayOfByte, i1);
	//float k_i = class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	//paramArrayOfk[paramInt1].f = b.a(paramArrayOfByte, i1);
	//float k_f = class_b__function_a__1byte_array_2int(paramArrayOfByte, i1);
	i1 += 4;
	//paramArrayOfk[paramInt1].b = ((int)(paramArrayOfk[paramInt1].h * 4095.0F));
	//paramArrayOfk[paramInt1].c = ((int)(paramArrayOfk[paramInt1].h * 4095.0F));
	//int k_b = ((int)(k_h * 4095.0F));
	//int k_c = ((int)(k_h * 4095.0F));
	//VertexBuffer localVertexBuffer = new VertexBuffer();
    jfloat f2 = 3.051851E-5F * f1;
	//printf("(%f, %f, %f) %f - %f\n", k_h, k_i, k_f, (float)k_b * f2, f2);
    jint i7 = paramArrayOfByte[(i1++)] & 0xFF;
    jint i8 = 0;
    jint i9;
    jshort *localObject = NULL;

    m->vertex_count = i2;
    // vertex
	if (i7 == 242)
	{
        //VertexArray localVertexArray1 = new VertexArray(i2, 3, 2);
		dprintfsi("Scene vertex count", i2);
        localObject = NEW_II(jshort, i2 * 3);
        const NLuint vertex_count = i2 * 3;
        jfloat *vertex = NEW_II(jfloat, i2 * 3);
		for (i9 = 0; i9 < i2 * 3; i9++)
		{
            i8 = CROSS_PLATFORM__class_b__function_a__1byte_2byte__temp(paramArrayOfByte, i1, - 32767);
            localObject[i9] = ((jshort)i8);
			/*
				 if(i9 % 3 == 0)
				 printf(" | ");
				 printf("%f ", f2 * (float)i8);
				 */
            vertex[i9] = f2 * (jfloat)i8;
		}
		//printf("<%f %f %f>\n", vertex[0], vertex[1], vertex[2]);
        m->vertex.count = vertex_count;
        m->vertex.data = (NLfloat *)vertex;
        free(localObject);
		//printf("RE 3D map primitive scale factory->%f\n", f2);
		//localVertexArray1.set(0, i2, (short[])localObject);
		//localVertexBuffer.setPositions(localVertexArray1, f2, arrayOfFloat);
		i7 = paramArrayOfByte[(i1++)] & 0xFF;
    }

    // texcoord
    if (i7 == 244)
	{
		//VertexArray localVertexArray2 = new VertexArray(i2, 2, 2);
		dprintfsi("Scene texcoord count", i2);
        localObject = NEW_II(jshort, i2 * 2);
        const NLuint texcoord_count = i2 * 2;
        jfloat *texcoord = NEW_II(jfloat, i2 * 2);
		for (i9 = 0; i9 < i2; i9++)
		{
            localObject[(i9 * 2)] = ((jshort)(paramArrayOfByte[(i1++)] & 0xFF));
            localObject[(i9 * 2 + 1)] = ((jshort)(255 - (paramArrayOfByte[(i1++)] & 0xFF)));
			/*
				 printf("%f %f | ", 0.003921569F * (float)localObject[i9 * 2], 0.003921569F * (float)localObject[i9 * 2 + 1]);
				 */
            texcoord[i9 * 2] = 0.003921569F * (jfloat)localObject[i9 * 2];
            texcoord[i9 * 2 + 1] = 0.003921569F * (jfloat)localObject[i9 * 2 + 1];
        }
        m->texcoord.count = texcoord_count;
        m->texcoord.data = (NLfloat *)texcoord;
        free(localObject);
		//localVertexArray2.set(0, i2, (short[])localObject);
		//localVertexBuffer.setTexCoords(0, localVertexArray2, 0.003921569F, null);
		i7 = paramArrayOfByte[(i1++)] & 0xFF;
	}

    // index
    jint *localObject2 = NEW_II(jint, i3);
	dprintfsi("Scene element index count", i3);
	if (i7 == 245)
	{
		for (i9 = 0; i9 < i3; i9++) {
            localObject2[i9] = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
			//printf("%d ", localObject2[i9]);
		}
		i7 = paramArrayOfByte[(i1++)] & 0xFF;
	}
    m->index.count = i3;
    m->index.data = (NLuint *)localObject2;

    // triangle strip primitive size
	//printf("i4 = %d\n", i4);
    jint *arrayOfInt = NEW_II(jint, i4);
    const NLuint primitive_count = i4;
    NLuint *primitive = NEW_II(NLuint, i4);
    dprintfsi("Scene element index size count", i4);
	if (i7 == 246) {
        jint i10;
		for (i10 = 0; i10 < i4; i10++) {
            arrayOfInt[i10] = CROSS_PLATFORM__class_b__function_a__1byte_2byte(paramArrayOfByte, i1);
            primitive[i10] = (NLuint)arrayOfInt[i10];
			//printf("%d ", arrayOfInt[i10]);
		}
	}
    m->primitive.count = primitive_count;
    m->primitive.data = primitive;
    free(arrayOfInt);

	//TriangleStripArray localTriangleStripArray = new TriangleStripArray((int[])localObject, arrayOfInt);
	//paramArrayOfk[paramInt1].d = new Mesh(localVertexBuffer, localTriangleStripArray, null);
	//printf("§§§§§§ %d\n", i5);
	paramArrayOfInt[paramInt1] = i5;
	return i1;
}

jfloat class_b__function_a__1byte_array_2int(const byte paramArrayOfByte[], jint paramInt)
{
    jint i = paramArrayOfByte[paramInt] & 0xFF;
    jint j = paramArrayOfByte[(paramInt + 1)] & 0xFF;
    jint k = paramArrayOfByte[(paramInt + 2)] & 0xFF;
    jint m = paramArrayOfByte[(paramInt + 3)] & 0xFF;
    jfloat f;
	return f = int_bits_to_float(i + (j << 8) + (k << 16) + (m << 24));
}
