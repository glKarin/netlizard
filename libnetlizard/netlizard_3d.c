#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

static const int Game_Count = NL_TOTAL_GAME;
static const char *Game_Names[] = {
	"Racing Evolution 3D",
	"Contr Terrorism 3D",
    "Army Rangers 3D: Operation Arctic",
    "Contr Terrorism 3D: Episode 2",
	"Shadows of Egypt 3D",
	"Clone 3D",
    "Contr Terrorism 3D: Episode 3"
};

static const char *CT3D_Level[] = {
	"Complex",
	"Underground",
	"Yard",
	"Street",
	"Garage",
	"Controls",
	"Station",
	"Office",
	"HeadQuarters",
	"Escape"
};

static const char *Specnaz3D_Level[] = {
	"Landing",
	"Gorge",
	"CheckPoint",
	"Passage",
	"Cross-Road",
	"Tower",
	"Storage",
	"Station",
	"Mine",
	"Limit"
};
static const char *CT3DEp2_Level[] = {
	"Mine",
	"Yard",
	"Cellar",
	"To The Roof",
	"WareHouse",
	"Office",
	"Street",
	"Prison",
	"Controls"
};
static const char *Egypt3D_Level[] = {
	"Shadows of Egypt",
	"Landslide",
	"Spider Lair",
	"Trap",
	"Riddle",
	"Tomb",
	"Lair",
	"Maggot",
	"SkyLine 1",
	"SkyLine 2",
	"Torches",
	"Fight",
	"SkyLine 3",
	"Border",
	"Break-Through",
	"Sanctuary",
	"Highest",
	"Home",
	"Servive"
};
static const char *Clone3D_Level[] = {
	"Escape",
	"GreenHouse",
	"Plantation",
	"Mine",
	"Office",
	"Commercial",
	"Commercial 2",
	"Passage",
	"Ventilation",
	"Departament",
	"Exit",
	"Exit"
};
static const char *RE3D_Level[] = {
	"Track 1",
	"Track 2",
	"Track 3",
	"Track 4",
	"Track 5",
	"Track 6",
	"Track 7"
};
static const char *CT3DEp3_Level[] = {
	"1-Catacombs",
	"2-Collector",
	"3-Cellar",
	"4-Cellar",
	"5-Evacuation",
	"6-Assistance",
	"7-Cruelty",
	"8-Pledge",
	"9-Director",
	"10-Escape",
	"11-Cykl",
	"12-Vault",
	"Level 13",
	"14-Dogma",
    "Level 15"
    /*
         "1-§¬§Ñ§ä§Ñ§Ü§à§Þ§Ò§í",
         "2-§¬§à§Ý§Ý§Ö§Ü§ä§à§â",
         "3-§±§à§Õ§Ó§Ñ§Ý",
         "4-§±§à§Ô§â§Ö§Ò",
         "5-§¿§Ó§Ñ§Ü§å§Ñ§è§Ú§ñ",
         "6-§³§à§Õ§Ö§Û§ã§ä§Ó§Ú§Ö",
         "7-§¨§Ö§ã§ä§à§Ü§à§ã§ä§î",
         "8-§©§Ñ§Ý§à§Ô",
         "9-§¥§Ú§â§Ö§Ü§ä§à§â",
         "10-§±§à§Ò§Ö§Ô",
         "11-§¸§í§Ü§Ý",
         "12-§µ§Ò§Ö§Ø§Ú§ë§Ö",
         "13-",
         "14-§¥§à§Ô§Þ§Ñ",
         "15-"
         */
};

static const char **Game_Level_Name[] = {
	RE3D_Level,
	CT3D_Level,
	Specnaz3D_Level,
	CT3DEp2_Level,
	Egypt3D_Level,
	Clone3D_Level,
	CT3DEp3_Level
};

static const int Game_Level[] = {
    RE3D_LEVEL,
    CT3D_LEVEL,
    SPECNAZ3D_LEVEL,
    CT3DEP2_LEVEL,
    EGYPT3D_LEVEL,
    CLONE3D_LEVEL,
    CT3DEP3_LEVEL
};

static const NETLizard_3D_Model_Config Game_Config[] = {
    {
        NL_RACING_EVOLUTION_3D,
        0,
        NL_TEXTURE_ENCODE_PNG,
        1,
        1,
        NL_FALSE,
        NL_FALSE,
        RE3D_SKY_FILE,
        "",
        ""
    },
    {
        NL_CONTR_TERRORISM_3D,
        CT3D_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V2,
        1,
        1,
        NL_TRUE,
        NL_TRUE,
        CT3D_SKY_FILE,
        CT3D_TEX_SUBFIX,
        CT3D_OBJ_SUBFIX
    },
    {
        NL_ARMY_RANGER_3D,
        SPECNAZ3D_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V2,
        1,
        1,
        NL_TRUE,
        NL_TRUE,
        SPECNAZ3D_SKY_FILE,
        SPECNAZ3D_TEX_SUBFIX,
        SPECNAZ3D_OBJ_SUBFIX
    },
    {
        NL_CONTR_TERRORISM_3D_EPISODE_2,
        SPECNAZ3D_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V2,
        3,
        1,
        NL_TRUE,
        NL_TRUE,
        CT3DEP2_SKY_FILE,
        CT3DEP2_TEX_SUBFIX,
        CT3DEP2_OBJ_SUBFIX
    },
    {
        NL_SHADOW_OF_EGYPT_3D,
        EGYPT3D_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V3,
        3,
        3,
        NL_TRUE,
        NL_TRUE,
        "",
        EGYPT3D_TEX_SUBFIX,
        EGYPT3D_OBJ_SUBFIX
    },
    {
        NL_CLONE_3D,
        CLONE3D_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V3,
        3,
        3,
        NL_FALSE,
        NL_FALSE,
        "",
        CLONE3D_TEX_SUBFIX,
        CT3DEP3_OBJ_SUBFIX
    },
    {
        NL_CONTR_TERRORISM_3D_EPISODE_3,
        CT3DEP3_TEX_COUNT,
        NL_TEXTURE_3D_ENGINE_V3,
        3,
        1,
        NL_FALSE,
        NL_FALSE,
        CT3DEP3_SKY_FILE,
        CT3DEP3_TEX_SUBFIX,
        CT3DEP3_OBJ_SUBFIX
    },
};

#define FRAME_ANIM(type, start, end) {type, start, end, end - start + 1}
#define FRAME_ANIM_NULL(type) {type, -1, -1, 0}
static const NETLizard_3D_Frame_Animation Egypt_3D_Frame_Animation[8][NL_FRAME_ANIMATION_TOTAL] = {
    {
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_IDLE),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_WALK),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_RUN),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_2),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_2),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_DEAD_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_DEAD_2),
    }, // 0 unused
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 8, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 8, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 8, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 8, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 12, 13),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 12, 13),
    }, // 1
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 8, 10),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 8, 10),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 8, 10),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 8, 10),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 11, 13), // 12
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 11, 13),
    }, // 2
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 1, 5),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 1, 5),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 1, 5),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 1, 5),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 6, 6),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 6, 6),
    }, // 3
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 2, 2),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 2, 2),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 2, 2),
    }, // 4
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 10, 14),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 10, 14),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 10, 14),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 10, 14),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 14, 17),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 14, 20),
    }, // 5
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 8, 13),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 8, 13),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 8, 13),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 8, 13),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 14, 17),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 14, 17),
    }, // 6
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 0, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 8, 18),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 8, 18),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 18, 26),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 18, 26),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 12, 12),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 17, 17),
    }, // 7
};

static const NETLizard_3D_Frame_Animation Clone_3D_Frame_Animation[5][NL_FRAME_ANIMATION_TOTAL] = {
    {
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_IDLE),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_WALK),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_RUN),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_2),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_2),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_DEAD_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_DEAD_2),
    }, // 0 unused
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 26, 26),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 27, 35),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 36, 42),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 1, 7),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 9, 15),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 8, 8),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 16, 20),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 21, 25),
    }, // 1
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 10, 19),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 20, 29),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 10, 10),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 20, 20),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 30, 34),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 30, 34),
    }, // 2
    {
        FRAME_ANIM(NL_FRAME_ANIMATION_IDLE, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_WALK, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_RUN, 1, 9),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_1, 10, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_FIGHTING_2, 10, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_1, 10, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_ATTACK_2, 10, 11),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 0, 0),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 0, 0),
    }, // 3
    {
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_IDLE),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_WALK),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_RUN),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_FIGHTING_2),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_1),
        FRAME_ANIM_NULL(NL_FRAME_ANIMATION_ATTACK_2),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_1, 0, 3),
        FRAME_ANIM(NL_FRAME_ANIMATION_DEAD_2, 0, 3),
    }, // 4
};

static const NETLizard_3D_Frame_Animation **Frame_Animation[2] = {
    Egypt_3D_Frame_Animation,
    Clone_3D_Frame_Animation,
};

static const char *Frame_Animation_Name[NL_FRAME_ANIMATION_TOTAL] = {
    "Idel",
    "Walk",
    "Run",
    "Fighting-1",
    "Fighting-2",
    "Attack-1",
    "Attack-2",
    "Dead-1",
    "Dead-2"
};

void nlDeleteNETLizard3DMesh(NETLizard_3D_Mesh *mesh)
{
    free(mesh->vertex.data);
    free(mesh->primitive.data);
    free(mesh->plane.data);
    free(mesh->bsp.data);
}

void nlDeleteNETLizard3DItemMesh(NETLizard_3D_Item_Mesh *mesh)
{
	nlDeleteNETLizard3DMesh(&mesh->item_mesh);
}

void nlDeleteNETLizard3DModel(NETLizard_3D_Model *model)
{
    int i;

    for(i = 0; i < model->data.count; i++)
        nlDeleteNETLizard3DMesh(((NETLizard_3D_Mesh *)(model->data.data)) + i);
    free(model->data.data);

    for(i = 0; i < model->item_data.count; i++)
        nlDeleteNETLizard3DItemMesh(((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i);
    free(model->item_data.data);

    free(model->bsp_data.data);
}

void nlDeleteNETLizardRE3DMesh(NETLizard_RE3D_Mesh *mesh)
{
    free(mesh->vertex.data);
    free(mesh->texcoord.data);
    free(mesh->index.data);
    free(mesh->primitive.data);
}

void nlDeleteNETLizardRE3DModel(NETLizard_RE3D_Model *model)
{
    int i;
    for(i = 0; i < model->meshes.count; i++)
    {
        nlDeleteNETLizardRE3DMesh(model->meshes.data + i);
    }
    free(model->meshes.data);

    for(i = 0; i < model->texes.count; i++)
        free(model->texes.data[i]);
    free(model->texes.data);
}

NETLizard_Texture_Type nlGetPNGType(const char *data, NLsizei length)
{
    if(length >= 3 && nlIsPNG(data, length))
		return NL_TEXTURE_NORMAL_PNG;
    else if(length >= 3 && nlIsNLPNG(data, length))
		return NL_TEXTURE_ENCODE_PNG;
    else if(length >= 3 && nlIsNL3DTextureV2(data, length))
		return NL_TEXTURE_3D_ENGINE_V2;
    else if(length >= 3 && nlIsNL3DTextureV3(data, length))
    {
        NETLizard_Texture tex;
        NLboolean res = nlLoadTextureV3Data(data, length, -1, &tex);
        if(res)
        {
            NETLizard_Texture_Type type = NL_TEXTURE_3D_ENGINE_V3;
			int i;
            for(i = 0; i < tex.color_index.count; i++)
			{
                if(((signed char*)(tex.color_index.data))[i] < 0)
				{
					type = NL_TEXTURE_3D_ENGINE_V3_COMPRESS;
					break;
				}
			}
            nlDeleteNETLizardTexture(&tex);
			return type;
		}
		else
		{
            return NL_TEXTURE_UNKNOWN;
        }
	}
	else
		return NL_TEXTURE_UNKNOWN;
}

NETLizard_Texture_Type nlGetPNGFileType(const char *name)
{
    array arr;
    int res;

    res = file_get_contents(name, &arr);
    if(res <= 0)
        return NL_TEXTURE_UNKNOWN;
    NETLizard_Texture_Type type = nlGetPNGType((char *)arr.array, arr.length);
    delete_array(&arr);
    return type;
}

const char * nlGet3DModelFrameAnimationName(NETLizard_3D_Animation_Type anim)
{
	if(anim >= NL_FRAME_ANIMATION_TOTAL)
        return NULL;
    return Frame_Animation_Name[anim];
}

const NETLizard_3D_Frame_Animation * nlGet3DModelFrameAnimationConfig(NETLizard_Game game, NLuint index)
{
#if 0
	const int anim[2][8][NL_FRAME_ANIMATION_TOTAL * 2] = {
		{
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			}, // unused
			{
				0, 0, -1, -1, 1, 7, 8, 11, -1, -1, 8, 11, -1, -1, 12, 13, -1, -1
			}, // 1
			{
				0, 0, -1, -1, 1, 7, 8, 10, -1, -1, 8, 10, -1, -1, 12, 13, -1, -1
			}, // 2
			{
				0, 0, -1, -1, 0, 0, 1, 5, -1, -1, 1, 5, -1, -1, 6, 6, -1, -1
			}, // 3
			{
				2, 2, -1, -1, 0, 8, 0, 8, -1, -1, 0, 8, -1, -1, 2, 2, -1, -1
			}, // 4
			{
				0, 0, -1, -1, 1, 9, 10, 14, -1, -1, 10, 14, -1, -1, 14, 17, 14, 20
			}, // 5
			{
				0, 0, -1, -1, 1, 7, 8, 13, -1, -1, 8, 13, -1, -1, 14, 17, -1, -1
			}, // 6
			{
				0, 8, -1, -1, 0, 8, 8, 18, -1, -1, 18, 26, -1, -1, 12, 12, 17, 17
			} // 7
		}, // egypt 3d 7
		{
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			}, // unused
			{
				26, 26, 27, 35, 36, 42, 1, 7, 9, 15, 0, 0, 8, 8, 16, 20, 21, 25
			}, // 1
			{
				0, 0, -1, -1, 1, 9, 10, 19, 20, 29, 10, 10, 20, 20, 30, 34, -1, -1
			}, // 2
			{
				0, 0, -1, -1, 1, 9, 10, 11, -1, -1, 10, 11, -1, -1, 0, 0, -1, -1
			}, // 3
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 3, -1, -1
			}, // 4
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			}, // unused
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			}, // unused
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			} // unused
		} // clone 3d 4
	};

    const int anim_count[2][8] = {
		{0, 5, 5, 5, 5, 6, 5, 6}, // egypt 3d 7
		{0, 9, 7, 5, 1, 0, 0, 0}, // clone 3d 4
	};
#endif
    if(game == NL_SHADOW_OF_EGYPT_3D && index <= 8)
    {
        return Egypt_3D_Frame_Animation[index];
	}
    else if(game == NL_CLONE_3D && index <= 4)
    {
        return Clone_3D_Frame_Animation[index];
	}
    return NULL;
}

int nlGetItemType(NETLizard_Game game, int index)
{
    int res = NL_3D_ITEM_TYPE_GENERAL;
	switch(game)
	{
        case NL_CLONE_3D:
			if(index == 3 || index == 4 || index == 5 || index == 6)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 34 || index == 35 || index == 36 || index == 37)
                res |= NL_3D_ITEM_TYPE_DOOR_HORIZONTAL;
			else if(index == 52 || index == 53 || index == 54 || index == 55)
                res |= NL_3D_ITEM_TYPE_WEAPON;
			else if(index == 38 || index == 39)
                res |= NL_3D_ITEM_TYPE_LADDER;
			else if(index == 40 || index == 41 || index == 42 || index == 43)
                res |= NL_3D_ITEM_TYPE_ELEVATOR | NL_3D_ITEM_TYPE_THIN;
            else if(index == 20)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 29)
                res |= NL_3D_ITEM_TYPE_FAN_HORIZONTAL;
			else if(index == 31) // æœºæžª
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 32 || index == 33)
                res |= NL_3D_ITEM_TYPE_DOOR_HORIZONTAL;
			else if(index == 26)
                res |= NL_3D_ITEM_TYPE_SWITCH;
            else if(index == 19 || index == 20)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL | NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
			break;

        case NL_SHADOW_OF_EGYPT_3D:
			if(index == 20 || index == 21)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			if(index == 5 || index == 6)
                res |= NL_3D_ITEM_TYPE_SKYLINE;
			else if(index == 24 || index == 25 || index == 26 || index == 27 || index == 28 || index == 29)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 33 || index == 34 || index == 35 || index == 36)
                res |= NL_3D_ITEM_TYPE_SKYBOX;
			else if(index == 14)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL | NL_3D_ITEM_TYPE_PORTAL;
            else if(index == 15)
                res |= NL_3D_ITEM_TYPE_PORTAL;
			else if(index == 8)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
            else if(index == 22) // tower
                res |= NL_3D_ITEM_TYPE_EMPTY;
            else if(index == 9 || index == 14)
                res |= NL_3D_ITEM_TYPE_SWITCH;
			break;

        case NL_CONTR_TERRORISM_3D:
			if(index == 52 || index == 53 || index == 54 || index == 55)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 16 || index == 21 || index == 26)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 23)
                res |= NL_3D_ITEM_TYPE_FAN_HORIZONTAL;
			else if(index == 17)
                res |= NL_3D_ITEM_TYPE_GENERAL; //
            else if(index == 45 || index == 46)
                res |= NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
            else if(index == 5)
                res |= NL_3D_ITEM_TYPE_2D;
			break;

        case NL_CONTR_TERRORISM_3D_EPISODE_2:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6 || index == 7)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_THIN;
			else if(index == 38 || index == 39 || index == 40 || index == 41)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 43)
                res |= NL_3D_ITEM_TYPE_LADDER | NL_3D_ITEM_TYPE_TRANSPARENT;
			else if(index == 12 || index == 15)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 13)
                res |= NL_3D_ITEM_TYPE_FAN_HORIZONTAL;
            else if(index == 25 || index == 26)
                res |= NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
			break;

        case NL_ARMY_RANGER_3D:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 6 || index == 7 || index == 8 || index == 9 || index == 10 || index == 11)
                res |= NL_3D_ITEM_TYPE_EMPTY | NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 40 || index == 41 || index == 42 || index == 43)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 34 || index == 49)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 35)
                res |= NL_3D_ITEM_TYPE_FAN_HORIZONTAL;
            else if(index == 44)
                res |= NL_3D_ITEM_TYPE_ELEVATOR | NL_3D_ITEM_TYPE_SWITCH | NL_3D_ITEM_TYPE_THIN;
			break;

        case NL_CONTR_TERRORISM_3D_EPISODE_3:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6 || index == 7)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_THIN;
			else if(index == 38 || index == 39 || index == 40 || index == 41)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 12 || index == 15)
                res |= NL_3D_ITEM_TYPE_FAN_VERTICAL;
			else if(index == 13)
                res |= NL_3D_ITEM_TYPE_FAN_HORIZONTAL;
			else if(index == 43)
                res |= NL_3D_ITEM_TYPE_LADDER | NL_3D_ITEM_TYPE_TRANSPARENT;
            else if(index == 25 || index == 26)
                res |= NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
			break;

		default:
			break;
	}
    return res;
}

NLboolean nlCheck3DGameLevelIsAvailable(NETLizard_Game game, int level)
{
	switch(game)
	{
        case NL_RACING_EVOLUTION_3D:
        case NL_CONTR_TERRORISM_3D:
        case NL_ARMY_RANGER_3D:
        case NL_CONTR_TERRORISM_3D_EPISODE_2:
        case NL_CLONE_3D:
        case NL_CONTR_TERRORISM_3D_EPISODE_3:
            return(level >= 1 && level <= Game_Level[game] ? NL_TRUE : NL_FALSE);
        case NL_SHADOW_OF_EGYPT_3D:
            return(level >= 0 && level < EGYPT3D_LEVEL ? NL_TRUE : NL_FALSE);
			break;
		default:
			break;
	}
    return NL_FALSE;
}

const char * nlGet3DGameLevelName(NETLizard_Game game, NLuint level)
{
    if(game > NL_CONTR_TERRORISM_3D_EPISODE_3)
        return NULL;
    int max_level = Game_Level[game];
    if(level > max_level)
        return NULL;
    if(game == NL_SHADOW_OF_EGYPT_3D) // Main menu 3D level only in `3D Egypt`
    {
        return Game_Level_Name[game][level];
    }
    else
    {
        if(level == 0)
            return NULL;
        else
            return Game_Level_Name[game][level - 1];
    }
}

NLboolean nlGet3DGameLevelRange(NETLizard_Game game, NLint *start, NLint *count)
{
    if(game > NL_CONTR_TERRORISM_3D_EPISODE_3)
        return NL_FALSE;
    int max_level = Game_Level[game];
    if(count)
        *count = max_level;
    if(start)
    {
        *start = game == NL_SHADOW_OF_EGYPT_3D ? 0 : 1;
    }
    return NL_TRUE;
}

const char * nlGet3DGameName(NETLizard_Game game)
{
    if(game > NL_CONTR_TERRORISM_3D_EPISODE_3)
        return NULL;
    return Game_Names[game];
}

const NETLizard_3D_Model_Config * nlGet3DGameModelConfig(NETLizard_Game game)
{
    if(game > NL_CONTR_TERRORISM_3D_EPISODE_3)
        return NULL;
    return Game_Config + game;
}

NLboolean nlIsRE3DMeshFile(const char *name)
{
    int len;
    array arr;
    NLboolean ret;
    char data[7] = {0};

    ret = NL_FALSE;
    make_array(&arr, 1, 7, data);
    len = file_get_contents_s(name, &arr);
    if(len == 7)
        ret = nlIsRE3DMesh((char *)arr.array, arr.length);
    return ret;
}

NLboolean nlIsRE3DMesh(const char *data, NLsizei len)
{
    if(len < 7)
        return NL_FALSE;
    const unsigned char NL_RE3D_Mesh_Dec[] = {
        78, 76, 95, 77, 69, 83, 72
    };

    int res = memcmp(data, NL_RE3D_Mesh_Dec, sizeof(NL_RE3D_Mesh_Dec));
    return res == 0 ? NL_TRUE : NL_FALSE;
}

static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_2[] = {
    {NL_SHADOW_OF_EGYPT_3D, 2, {13, 12}, 15, {-100466688, -141295616, -16384000}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, {15, 14}, 7, {-78643200, 147456000, 9830400}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, {11, 10}, 7, {-579993600, 8847360, -36044800}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_4[] = {
    {NL_SHADOW_OF_EGYPT_3D, 4, {11, 10}, 7, {205914112, -144179200, 9830400}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_6[] = {
    {NL_SHADOW_OF_EGYPT_3D, 6, {2, 3}, 7, {-68812800, -503119872, -55705600}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_8[] = {
    {NL_SHADOW_OF_EGYPT_3D, 8, {0, 1}, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {80, 82}, 7, {-550502400, 139853824, 13631488}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {120, 127}, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {34, 31}, 7, {462028800, 474087424, 6422528}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {68, 70}, 7, {-167116800, -587530240, 14417920}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {132, 136}, 7, {56164352, 82575360, 13959168}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, {114, 116}, 7, {56164352, 80871424, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_9[] = {
    {NL_SHADOW_OF_EGYPT_3D, 9, {26, 28}, 7, {-152305664, 397213696, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, {96, 101}, 7, {742195200, 595132416, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, {81, 83}, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, {34, 36}, 7, {289996800, -622460928, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, {71, 73}, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, {38, 41}, 7, {-44171264, -43778048, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_10[] = {
    {NL_SHADOW_OF_EGYPT_3D, 10, {0, 1}, 7, {55181312, 86507520, 12713984}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_11[] = {
    {NL_SHADOW_OF_EGYPT_3D, 11, {10, 11}, 7, {111411200, -97648640, 8192000}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_12[] = {
    {NL_SHADOW_OF_EGYPT_3D, 12, {30, 42}, 7, {325255168, 72679424, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, {19, 25}, 7, {571015168, -310640640, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, {63, 73}, 7, {305594368, -477756440, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_13[] = {
    {NL_SHADOW_OF_EGYPT_3D, 13, {7, 8}, 11, {-78643200, -149094400, 0}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, {11, 10}, 7, {39321600, 144637952, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, {13, 14}, 7, {50593792, -68943872, 9830400}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_14[] = {
    {NL_SHADOW_OF_EGYPT_3D, 14, {1, 2}, 7, {44433408, -29032448, 8454144}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_15[] = {
    {NL_SHADOW_OF_EGYPT_3D, 15, {1, 8}, 7, {0, -69664768, 8192000}, {0, 0}},
};

static const NETLizard_Level_Teleport *Egypt3D_Level_Teleport[] = {
    NULL,
    NULL, Egypt3D_Level_Teleport_2, NULL, Egypt3D_Level_Teleport_4, NULL,
    Egypt3D_Level_Teleport_6, NULL, Egypt3D_Level_Teleport_8, Egypt3D_Level_Teleport_9, Egypt3D_Level_Teleport_10,
    Egypt3D_Level_Teleport_11, Egypt3D_Level_Teleport_12, Egypt3D_Level_Teleport_13, Egypt3D_Level_Teleport_14, Egypt3D_Level_Teleport_15,
    NULL, NULL,
    NULL,
};
static const NLint Egypt3D_Level_Teleport_Count[] = {
    0,
    0, COUNTOF(Egypt3D_Level_Teleport_2), 0, COUNTOF(Egypt3D_Level_Teleport_4), 0,
    COUNTOF(Egypt3D_Level_Teleport_6), 0, COUNTOF(Egypt3D_Level_Teleport_8), COUNTOF(Egypt3D_Level_Teleport_9), COUNTOF(Egypt3D_Level_Teleport_10),
    COUNTOF(Egypt3D_Level_Teleport_11), COUNTOF(Egypt3D_Level_Teleport_12), COUNTOF(Egypt3D_Level_Teleport_13), COUNTOF(Egypt3D_Level_Teleport_14), COUNTOF(Egypt3D_Level_Teleport_15),
    0, 0,
    0,
};

const NETLizard_Level_Teleport * nlGet3DGameTeleport(NLenum game, NLint level, NLint item_id, NLint *length)
{
    if(game != NL_SHADOW_OF_EGYPT_3D)
        return NULL;
    const NETLizard_Level_Teleport *ret = Egypt3D_Level_Teleport[level];
    if(length)
        *length = Egypt3D_Level_Teleport_Count[level];
    return ret;
}

static const NETLizard_Level_Elevator Specnaz3D_Level_Elevator_9[] = {
    {NL_ARMY_RANGER_3D, 9, 4, 4, 3, 0, 4500, NL_FALSE},
};

static const NETLizard_Level_Elevator Specnaz3D_Level_Elevator_10[] = {
    {NL_ARMY_RANGER_3D, 10, 0, 0, 3, -400, 0, NL_FALSE},
};

static const NETLizard_Level_Elevator *Specnaz3D_Level_Elevator[] = {
    NULL, NULL, NULL, NULL,
    NULL,
    NULL, NULL, NULL, NULL,
    Specnaz3D_Level_Elevator_9, Specnaz3D_Level_Elevator_10,
};
static const NLint Specnaz3D_Level_Elevator_Count[] = {
    0, 0, 0, 0,
    0,
    0, 0, 0, 0,
    COUNTOF(Specnaz3D_Level_Elevator_9), COUNTOF(Specnaz3D_Level_Elevator_10),
};

static const NETLizard_Level_Elevator Egypt3D_Level_Elevator_10[] = {
    {NL_SHADOW_OF_EGYPT_3D, 10, 51, 14, 1, 0, 1200, NL_TRUE},
    {NL_SHADOW_OF_EGYPT_3D, 10, 52, 14, 1, 0, 1200, NL_TRUE},
    {NL_SHADOW_OF_EGYPT_3D, 10, 64, 14, 1, 0, 1200, NL_TRUE},
    {NL_SHADOW_OF_EGYPT_3D, 10, 51, 13, 1, 0, 1200, NL_TRUE},
    {NL_SHADOW_OF_EGYPT_3D, 10, 52, 13, 1, 0, 1200, NL_TRUE},
    {NL_SHADOW_OF_EGYPT_3D, 10, 64, 13, 1, 0, 1200, NL_TRUE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_3[] = {
    {NL_CLONE_3D, 3, 26, 25, 3, 600, 1800, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_4[] = {
    {NL_CLONE_3D, 4, 66, 65, 3, 0, 1200, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_6[] = {
    {NL_CLONE_3D, 6, 35, 34, 3, 900, 2100, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_8[] = {
    {NL_CLONE_3D, 8, 29, 28, 3, 600, 1500, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_9[] = {
    {NL_CLONE_3D, 9, 7, 6, 3, 0, 900, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_10[] = {
    {NL_CLONE_3D, 10, 21, 20, 3, 0, 1200, NL_FALSE},
    {NL_CLONE_3D, 10, 32, 31, 3, 1200, 2400, NL_FALSE},
    {NL_CLONE_3D, 10, 34, 33, 3, 1200, 2400, NL_FALSE},
};

static const NETLizard_Level_Elevator Clone3D_Level_Elevator_11[] = {
    {NL_CLONE_3D, 11, 7, 6, 3, 0, 1200, NL_FALSE},
    {NL_CLONE_3D, 11, 29, 28, 3, 3300, 4500, NL_FALSE},
    {NL_CLONE_3D, 11, 40, 39, 3, 4500, 5700, NL_FALSE},
    {NL_CLONE_3D, 11, 42, 41, 3, 5700, 6900, NL_FALSE},
    {NL_CLONE_3D, 11, 50, 49, 3, 6900, 9000, NL_FALSE},

    {NL_CLONE_3D, 11, 54, 53, 1, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 57, 56, 1, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 60, 59, 1, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 55, 53, 2, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 58, 56, 2, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 61, 59, 2, 9000, 9900, NL_FALSE},

    {NL_CLONE_3D, 11, 63, 62, 1, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 66, 65, 1, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 64, 62, 2, 9000, 9900, NL_FALSE},
    {NL_CLONE_3D, 11, 67, 65, 2, 9000, 9900, NL_FALSE},

    {NL_CLONE_3D, 11, 69, 68, 3, 9000, 12000, NL_FALSE},
};

static const NETLizard_Level_Elevator *Clone3D_Level_Elevator[] = {
    NULL, NULL, NULL,
    Clone3D_Level_Elevator_3, Clone3D_Level_Elevator_4, NULL,
    Clone3D_Level_Elevator_6, NULL,
    Clone3D_Level_Elevator_8, Clone3D_Level_Elevator_9, Clone3D_Level_Elevator_10,
    Clone3D_Level_Elevator_11,
    NULL,
};
static const NLint Clone3D_Level_Elevator_Count[] = {
    0, 0, 0,
    COUNTOF(Clone3D_Level_Elevator_3), COUNTOF(Clone3D_Level_Elevator_4), 0,
    COUNTOF(Clone3D_Level_Elevator_6), 0,
    COUNTOF(Clone3D_Level_Elevator_8), COUNTOF(Clone3D_Level_Elevator_9), COUNTOF(Clone3D_Level_Elevator_10),
    COUNTOF(Clone3D_Level_Elevator_11),
    0,
};

const NETLizard_Level_Elevator * nlGet3DGameElevator(NLenum game, NLint level, NLint item_id, NLint *length)
{
    if(game == NL_CLONE_3D)
    {
        const NETLizard_Level_Elevator *ret = Clone3D_Level_Elevator[level];
        if(length)
            *length = Clone3D_Level_Elevator_Count[level];
        return ret;
    }
    if(game == NL_SHADOW_OF_EGYPT_3D)
    {
        if(level == 10)
        {
            const NETLizard_Level_Elevator *ret = Egypt3D_Level_Elevator_10;
            if(length)
                *length = COUNTOF(Egypt3D_Level_Elevator_10);
            return ret;
        }
        return NULL;
    }
    if(game == NL_ARMY_RANGER_3D)
    {
        const NETLizard_Level_Elevator *ret = Specnaz3D_Level_Elevator[level];
        if(length)
            *length = Specnaz3D_Level_Elevator_Count[level];
        return ret;
    }
    return NULL;
}

char * make_resource_file_path(const char *format, int index, const char *resc_path)
{
    size_t fl = strlen(format);
    char *suffix = NEW_II(char, fl + 1);
    memset(suffix, '\0', sizeof(char) * (fl + 1));
    sprintf(suffix, format, index);
    char *name = NULL;
    if(resc_path)
    {
        size_t len = fl + strlen(resc_path) + 1 + 1;
        name = NEW_II(char, len);
        memset(name, '\0', sizeof(char) * len);
        sprintf(name, "%s/%s", resc_path, suffix);
        free(suffix);
    }
    else
        name = suffix;
    return name;
}
