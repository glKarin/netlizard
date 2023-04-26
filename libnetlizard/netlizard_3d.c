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
         "1-Катакомбы",
         "2-Коллектор",
         "3-Подвал",
         "4-Погреб",
         "5-Эвакуация",
         "6-Содействие",
         "7-Жестокость",
         "8-Залог",
         "9-Директор",
         "10-Побег",
         "11-Цыкл",
         "12-Убежище",
         "13-",
         "14-Догма",
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
        "",
        RE3D_LEVEL
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
        CT3D_OBJ_SUBFIX,
        CT3D_LEVEL
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
        SPECNAZ3D_OBJ_SUBFIX,
        SPECNAZ3D_LEVEL
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
        CT3DEP2_OBJ_SUBFIX,
        CT3DEP2_LEVEL
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
        EGYPT3D_OBJ_SUBFIX,
        EGYPT3D_LEVEL
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
        CLONE3D_OBJ_SUBFIX,
        CLONE3D_LEVEL
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
        CT3DEP3_OBJ_SUBFIX,
        CT3DEP3_LEVEL
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
                res |= NL_3D_ITEM_TYPE_FAN_X_AXIS;
			else if(index == 31) // 鏈烘灙
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS;
			else if(index == 32 || index == 33)
                res |= NL_3D_ITEM_TYPE_DOOR_HORIZONTAL;
			else if(index == 26)
                res |= NL_3D_ITEM_TYPE_SWITCH;
            else if(index == 19 || index == 20)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL | NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
            else if(index == 51 || index == 50 || index == 49 || index == 48 || index == 47 || index == 46 || index == 45 || index == 44 || index == 10 || index == 57 || index == 11 || index == 58 || index == 56)
                res |= NL_3D_ITEM_TYPE_PARTICLE;
			break;

        case NL_SHADOW_OF_EGYPT_3D:
			if(index == 20 || index == 21)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			if(index == 5 || index == 6)
                res |= NL_3D_ITEM_TYPE_SKYLINE;
			else if(index == 24 || index == 25 || index == 26 || index == 27 || index == 28 || index == 29)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_FAN_Z_AXIS;
			else if(index == 33 || index == 34 || index == 35 || index == 36)
                res |= NL_3D_ITEM_TYPE_SKYBOX;
			else if(index == 14)
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS | NL_3D_ITEM_TYPE_PORTAL;
            else if(index == 15)
                res |= NL_3D_ITEM_TYPE_PORTAL;
			else if(index == 8)
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS;
            else if(index == 22) // tower
                res |= NL_3D_ITEM_TYPE_EMPTY;
            else if(index == 9 || index == 14)
                res |= NL_3D_ITEM_TYPE_SWITCH;
            else if(index == 2 || index == 3 || index == 1)
                res |= NL_3D_ITEM_TYPE_PARTICLE;
			break;

        case NL_CONTR_TERRORISM_3D:
			if(index == 52 || index == 53 || index == 54 || index == 55)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 16 || index == 21 || index == 26)
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS;
//			else if(index == 23)
//                res |= NL_3D_ITEM_TYPE_FAN_Y_AXIS;
			else if(index == 17)
                res |= NL_3D_ITEM_TYPE_FAN_Y_AXIS;
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
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS;
//			else if(index == 13)
//                res |= NL_3D_ITEM_TYPE_FAN_Y_AXIS;
            else if(index == 25 || index == 26)
                res |= NL_3D_ITEM_TYPE_TRANSPARENT | NL_3D_ITEM_TYPE_THIN;
            else if(index == 28 || index == 27)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL | NL_3D_ITEM_TYPE_THIN;
			break;

        case NL_ARMY_RANGER_3D:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_FAN_Z_AXIS;
			else if(index == 6 || index == 7 || index == 8 || index == 9 || index == 10 || index == 11)
                res |= NL_3D_ITEM_TYPE_EMPTY | NL_3D_ITEM_TYPE_FAN_Z_AXIS;
			else if(index == 40 || index == 41 || index == 42 || index == 43)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 34 || index == 49)
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS | NL_3D_ITEM_TYPE_THIN;
			else if(index == 35)
                res |= NL_3D_ITEM_TYPE_FAN_Y_AXIS | NL_3D_ITEM_TYPE_THIN;
            else if(index == 44)
                res |= NL_3D_ITEM_TYPE_ELEVATOR | NL_3D_ITEM_TYPE_SWITCH | NL_3D_ITEM_TYPE_THIN;
            else if(index == 39)
                res |= NL_3D_ITEM_TYPE_ELEVATOR | NL_3D_ITEM_TYPE_SWITCH | NL_3D_ITEM_TYPE_SCENE;
			break;

        case NL_CONTR_TERRORISM_3D_EPISODE_3:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6 || index == 7)
                res |= NL_3D_ITEM_TYPE_WEAPON | NL_3D_ITEM_TYPE_THIN;
			else if(index == 38 || index == 39 || index == 40 || index == 41)
                res |= NL_3D_ITEM_TYPE_DOOR_VERTICAL;
			else if(index == 12 || index == 15)
                res |= NL_3D_ITEM_TYPE_FAN_Z_AXIS;
			else if(index == 13)
                res |= NL_3D_ITEM_TYPE_FAN_Y_AXIS;
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
    const int max_level = Game_Level[game];
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
    {NL_SHADOW_OF_EGYPT_3D, 2, 13, 15, {-100466688, -141295616, -16384000}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, 12, 15, {-100466688, -141295616, -16384000}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, 15, 7, {-78643200, 147456000, 9830400}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, 14, 7, {-78643200, 147456000, 9830400}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, 11, 7, {-579993600, 8847360, -36044800}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 2, 10, 7, {-579993600, 8847360, -36044800}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_4[] = {
    {NL_SHADOW_OF_EGYPT_3D, 4, 11, 7, {205914112, -144179200, 9830400}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 4, 10, 7, {205914112, -144179200, 9830400}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_6[] = {
    {NL_SHADOW_OF_EGYPT_3D, 6, 2, 7, {-68812800, -503119872, -55705600}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 6, 3, 7, {-68812800, -503119872, -55705600}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_7[] = {
    {NL_SHADOW_OF_EGYPT_3D, 7, 0, 7, {720896, -23003136, 370540544}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_8[] = {
    {NL_SHADOW_OF_EGYPT_3D, 8, 0, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 1, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 81, 7, {-550502400, 139853824, 13631488}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 82, 7, {-550502400, 139853824, 13631488}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 126, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 127, 3, {-49152000, -76349440, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 32, 7, {462028800, 474087424, 6422528}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 31, 7, {462028800, 474087424, 6422528}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 69, 7, {-167116800, -587530240, 14417920}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 70, 7, {-167116800, -587530240, 14417920}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 135, 7, {56164352, 82575360, 13959168}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 136, 7, {56164352, 82575360, 13959168}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 115, 7, {56164352, 80871424, 14483456}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 8, 116, 7, {56164352, 80871424, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_9[] = {
    {NL_SHADOW_OF_EGYPT_3D, 9, 27, 7, {-152305664, 397213696, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 28, 7, {-152305664, 397213696, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 100, 7, {742195200, 595132416, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 101, 7, {742195200, 595132416, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 82, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 83, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 35, 7, {289996800, -622460928, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 36, 7, {289996800, -622460928, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 72, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 73, 7, {466419712, -32768000, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 40, 7, {-44171264, -43778048, 14483456}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 9, 41, 7, {-44171264, -43778048, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_10[] = {
    {NL_SHADOW_OF_EGYPT_3D, 10, 0, 7, {55181312, 86507520, 12713984}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 10, 1, 7, {55181312, 86507520, 12713984}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_11[] = {
    {NL_SHADOW_OF_EGYPT_3D, 11, 10, 7, {111411200, -97648640, 8192000}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 11, 11, 7, {111411200, -97648640, 8192000}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_12[] = {
    {NL_SHADOW_OF_EGYPT_3D, 12, 43, 7, {325255168, 72679424, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, 42, 7, {325255168, 72679424, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, 26, 7, {571015168, -310640640, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, 25, 7, {571015168, -310640640, 13107200}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, 72, 7, {305594368, -477756440, 14483456}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 12, 73, 7, {305594368, -477756440, 14483456}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_13[] = {
    {NL_SHADOW_OF_EGYPT_3D, 13, 7, 11, {-78643200, -149094400, 0}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, 8, 11, {-78643200, -149094400, 0}, {180, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, 11, 7, {39321600, 144637952, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, 10, 7, {39321600, 144637952, 0}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, 13, 7, {50593792, -68943872, 9830400}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 13, 14, 7, {50593792, -68943872, 9830400}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_14[] = {
    {NL_SHADOW_OF_EGYPT_3D, 14, 1, 7, {44433408, -29032448, 8454144}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 14, 2, 7, {44433408, -29032448, 8454144}, {0, 0}},
};
static const NETLizard_Level_Teleport Egypt3D_Level_Teleport_15[] = {
    {NL_SHADOW_OF_EGYPT_3D, 15, 9, 7, {0, -69664768, 8192000}, {0, 0}},
    {NL_SHADOW_OF_EGYPT_3D, 15, 8, 7, {0, -69664768, 8192000}, {0, 0}},
};

static const NETLizard_Level_Teleport *Egypt3D_Level_Teleport[] = {
    NULL,
    NULL, Egypt3D_Level_Teleport_2, NULL, Egypt3D_Level_Teleport_4, NULL,
    Egypt3D_Level_Teleport_6, Egypt3D_Level_Teleport_7, Egypt3D_Level_Teleport_8, Egypt3D_Level_Teleport_9, Egypt3D_Level_Teleport_10,
    Egypt3D_Level_Teleport_11, Egypt3D_Level_Teleport_12, Egypt3D_Level_Teleport_13, Egypt3D_Level_Teleport_14, Egypt3D_Level_Teleport_15,
    NULL, NULL,
    NULL,
};
static const NLint Egypt3D_Level_Teleport_Count[] = {
    0,
    0, COUNTOF(Egypt3D_Level_Teleport_2), 0, COUNTOF(Egypt3D_Level_Teleport_4), 0,
    COUNTOF(Egypt3D_Level_Teleport_6), COUNTOF(Egypt3D_Level_Teleport_7), COUNTOF(Egypt3D_Level_Teleport_8), COUNTOF(Egypt3D_Level_Teleport_9), COUNTOF(Egypt3D_Level_Teleport_10),
    COUNTOF(Egypt3D_Level_Teleport_11), COUNTOF(Egypt3D_Level_Teleport_12), COUNTOF(Egypt3D_Level_Teleport_13), COUNTOF(Egypt3D_Level_Teleport_14), COUNTOF(Egypt3D_Level_Teleport_15),
    0, 0,
    0,
};

static const NETLizard_Level_Teleport **Game3D_Level_Teleport[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    Egypt3D_Level_Teleport,
    NULL,
    NULL,
};

static const NLint *Game3D_Level_Teleport_Count[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    Egypt3D_Level_Teleport_Count,
    NULL,
    NULL,
};

const NETLizard_Level_Teleport * nlGet3DGameTeleport(NLenum game, NLint level, NLint item_id, NLint *length)
{
    const NETLizard_Level_Teleport **objs = Game3D_Level_Teleport[game];
    const NETLizard_Level_Teleport *ret = NULL;
    NLint len = 0;
    if(objs)
    {
        ret = objs[level];
        len = Game3D_Level_Teleport_Count[game][level];
        if(ret && item_id >= 0)
        {
            int i;
            for(i = 0; i < len; i++)
            {
                const NETLizard_Level_Teleport *o = ret + i;
                if(o->item == item_id)
                {
                    ret = o;
                    len = 1;
                    break;
                }
            }
        }
    }
    if(length)
        *length = len;
    return ret;
}

static const NETLizard_Level_Elevator Specnaz3D_Level_Elevator_4[] = {
    {NL_ARMY_RANGER_3D, 4, 14, 14, 3, 0, 1500, NL_FALSE},
};

static const NETLizard_Level_Elevator Specnaz3D_Level_Elevator_9[] = {
    {NL_ARMY_RANGER_3D, 9, 4, 4, 3, 0, 4500, NL_FALSE},
};

static const NETLizard_Level_Elevator Specnaz3D_Level_Elevator_10[] = {
    {NL_ARMY_RANGER_3D, 10, 0, 0, 3, -400, 0, NL_FALSE},
};

static const NETLizard_Level_Elevator *Specnaz3D_Level_Elevator[] = {
    NULL, NULL, NULL, NULL,
    Specnaz3D_Level_Elevator_4,
    NULL, NULL, NULL, NULL,
    Specnaz3D_Level_Elevator_9, Specnaz3D_Level_Elevator_10,
};
static const NLint Specnaz3D_Level_Elevator_Count[] = {
    0, 0, 0, 0,
    COUNTOF(Specnaz3D_Level_Elevator_4),
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

static const NETLizard_Level_Elevator *Egypt3D_Level_Elevator[] = {
    NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, Egypt3D_Level_Elevator_10, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL,
};
static const NLint Egypt3D_Level_Elevator_Count[] = {
    0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, COUNTOF(Egypt3D_Level_Elevator_10), 0, 0,
    0, 0, 0, 0, 0,
    0,
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

static const NETLizard_Level_Elevator **Game3D_Level_Elevator[] = {
    NULL,
    NULL,
    Specnaz3D_Level_Elevator,
    NULL,
    Egypt3D_Level_Elevator,
    Clone3D_Level_Elevator,
    NULL,
};

static const NLint *Game3D_Level_Elevator_Count[] = {
    NULL,
    NULL,
    Specnaz3D_Level_Elevator_Count,
    NULL,
    Egypt3D_Level_Elevator_Count,
    Clone3D_Level_Elevator_Count,
    NULL,
};

const NETLizard_Level_Elevator * nlGet3DGameElevator(NLenum game, NLint level, NLint item_id, NLint *length)
{
    const NETLizard_Level_Elevator **objs = Game3D_Level_Elevator[game];
    const NETLizard_Level_Elevator *ret = NULL;
    NLint len = 0;
    if(objs)
    {
        ret = objs[level];
        len = Game3D_Level_Elevator_Count[game][level];
        if(ret && item_id >= 0)
        {
            int i;
            for(i = 0; i < len; i++)
            {
                const NETLizard_Level_Elevator *o = ret + i;
                if(o->switch_item == item_id)
                {
                    ret = o;
                    len = 1;
                    break;
                }
            }
        }
    }
    if(length)
        *length = len;
    return ret;
}

static const NETLizard_Level_Door Egypt3D_Level_Door_3[] = {
    {NL_SHADOW_OF_EGYPT_3D, 3, {{2, 800, 1100}, {-1, 0, 0}}, 1, 1, {{2700, 3900, 800}, {3300, 4000, 1100}}},
    {NL_SHADOW_OF_EGYPT_3D, 3, {{3, 800, 1100}, {-1, 0, 0}}, 1, 1, {{2700, 5800, 800}, {3300, 5900, 1100}}},
    {NL_SHADOW_OF_EGYPT_3D, 3, {{8, 800, 1100}, {-1, 0, 0}}, 1, 1, {{2700, 7400, 800}, {3300, 7500, 1100}}},
    {NL_SHADOW_OF_EGYPT_3D, 3, {{9, 800, 1100}, {-1, 0, 0}}, 1, 1, {{1200, 9000, 800}, {1800, 9100, 1100}}},
    {NL_SHADOW_OF_EGYPT_3D, 3, {{10, 800, 1100}, {-1, 0, 0}}, 1, 1, {{1200, 10900, 800}, {1800, 11000, 1100}}}
};
static const NETLizard_Level_Door Egypt3D_Level_Door_4[] = {
    {NL_SHADOW_OF_EGYPT_3D, 4, {{15, 0, 300}, {-1, 0, 0}}, 1, 1, {{2100, 4200, 0}, {2700, 4300, 300}}}
};
static const NETLizard_Level_Door Egypt3D_Level_Door_5[] = {
    {NL_SHADOW_OF_EGYPT_3D, 5, {{46, 900, 1200}, {-1, 0, 0}}, 1, 1, {{7500, -900, 900}, {8100, -800, 1200}}},
};
static const NETLizard_Level_Door Egypt3D_Level_Door_11[] = {
    {NL_SHADOW_OF_EGYPT_3D, 11, {{13, 0, 300}, {-1, 0, 0}}, 1, 1, {{0, -100, 0}, {600, 0, 300}}},
    {NL_SHADOW_OF_EGYPT_3D, 11, {{12, 0, 300}, {-1, 0, 0}}, 1, 1, {{2400, -100, 0}, {3000, 0, 300}}},
};
static const NETLizard_Level_Door Egypt3D_Level_Door_15[] = {
    {NL_SHADOW_OF_EGYPT_3D, 15, {{10, -1000, -700}, {-1, 0, 0}}, 1, 1, {{5699, 12600, -1000}, {6299, 12700, -700}}},
};
static const NETLizard_Level_Door Egypt3D_Level_Door_16[] = {
    {NL_SHADOW_OF_EGYPT_3D, 16, {{4, -1000, -700}, {-1, 0, 0}}, 1, 1, {{-300, 12000, -1000}, {300, 12100, -700}}},
};
static const NETLizard_Level_Door *Egypt3D_Level_Door[] = {
    NULL,
    NULL, NULL, Egypt3D_Level_Door_3, Egypt3D_Level_Door_4, Egypt3D_Level_Door_5,
    NULL, NULL, NULL, NULL, NULL,
    Egypt3D_Level_Door_11, NULL, NULL, NULL, Egypt3D_Level_Door_15,
    Egypt3D_Level_Door_16, NULL,
    NULL,
};
static const NLint Egypt3D_Level_Door_Count[] = {
    0,
    0, 0, COUNTOF(Egypt3D_Level_Door_3), COUNTOF(Egypt3D_Level_Door_4), COUNTOF(Egypt3D_Level_Door_5),
    0, 0, 0, 0, 0,
    COUNTOF(Egypt3D_Level_Door_11), 0, 0, 0, COUNTOF(Egypt3D_Level_Door_15),
    COUNTOF(Egypt3D_Level_Door_16), 0,
    0,
};

static const NETLizard_Level_Door CT3D_Level_Door_1[] = {
    {NL_CONTR_TERRORISM_3D, 1, {{19, 150, 300}, {18, 0, -150}}, 3, 1, {{2750, 960, 0}, {3250, 1000, 300}}},
    {NL_CONTR_TERRORISM_3D, 1, {{16, 150, 300}, {17, 0, -150}}, 3, 1, {{-250, 6230, 0}, {250, 6270, 300}}},
};
static const NETLizard_Level_Door CT3D_Level_Door_2[] = {
    {NL_CONTR_TERRORISM_3D, 2, {{28, 750, 900}, {27, 600, 450}}, 3, 1, {{1460, -5250, 600}, {1500, -4750, 900}}},
};
static const NETLizard_Level_Door CT3D_Level_Door_6[] = {
    {NL_CONTR_TERRORISM_3D, 0, {{48, 150, 300}, {47, 0, -150}}, 3, 1, {{3250, 2210, 0}, {3750, 2250, 300}}},
    {NL_CONTR_TERRORISM_3D, 0, {{50, 150, 300}, {49, 0, -150}}, 3, 1, {{6750, 1710, 0}, {7250, 1750, 300}}},
    {NL_CONTR_TERRORISM_3D, 0, {{52, 150, 300}, {51, 0, -150}}, 3, 1, {{2000, 3210, 0}, {2500, 3250, 300}}},
};
static const NETLizard_Level_Door *CT3D_Level_Door[] = {
    NULL,
    CT3D_Level_Door_1, CT3D_Level_Door_2, NULL, NULL, NULL,
    CT3D_Level_Door_6, NULL, NULL, NULL, NULL,
};
static const NLint CT3D_Level_Door_Count[] = {
    0,
    COUNTOF(CT3D_Level_Door_1), COUNTOF(CT3D_Level_Door_2), 0, 0, 0,
    COUNTOF(CT3D_Level_Door_6), 0, 0, 0, 0,
};

static const NETLizard_Level_Door Specnaz3D_Level_Door_2[] = {
    {NL_ARMY_RANGER_3D, 2, {{4, 150, 299}, {5, 0, -150}}, 3, 1, {{7450, 6600, 0}, {7550, 7200, 299}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_3[] = {
    {NL_ARMY_RANGER_3D, 3, {{1, 150, 299}, {2, 0, -150}}, 3, 1, {{900, -1500, 0}, {1000, -900, 299}}},
    {NL_ARMY_RANGER_3D, 3, {{4, 150, 299}, {5, 0, -150}}, 3, 1, {{2100, 1200, 0}, {2700, 1300, 299}}},
    {NL_ARMY_RANGER_3D, 3, {{17, 150, 299}, {16, 0, -150}}, 3, 1, {{6000, 1600, 0}, {6100, 2200, 299}}},
    {NL_ARMY_RANGER_3D, 3, {{18, 150, 299}, {19, 0, -150}}, 3, 1, {{6700, 1600, 0}, {6800, 2200, 299}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_4[] = {
    {NL_ARMY_RANGER_3D, 4, {{1, 150, 299}, {0, 0, -150}}, 3, 1, {{300, -800, 0}, {900, -700, 299}}},
    {NL_ARMY_RANGER_3D, 4, {{3, 150, 299}, {2, 0, -150}}, 3, 1, {{300, -100, 0}, {900, 0, 299}}},
    {NL_ARMY_RANGER_3D, 4, {{12, 150, 299}, {13, 0, -150}}, 3, 1, {{900, 3600, 0}, {1500, 3700, 299}}},
    {NL_ARMY_RANGER_3D, 4, {{16, 1650, 1799}, {15, 1500, 1350}}, 3, 1, {{900, 3600, 1500}, {1500, 3700, 1799}}},
    {NL_ARMY_RANGER_3D, 4, {{19, 1650, 1799}, {20, 1500, 1350}}, 3, 1, {{3900, 3000, 1500}, {4500, 3100, 1799}}},
    {NL_ARMY_RANGER_3D, 4, {{21, 1650, 1799}, {22, 1500, 1350}}, 3, 1, {{3900, 3700, 1500}, {4500, 3800, 1799}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_5[] = {
    {NL_ARMY_RANGER_3D, 5, {{0, -350, -201}, {1, -500, -650}}, 3, 1, {{700, -5300, -500}, {1300, -5200, -201}}},
    {NL_ARMY_RANGER_3D, 5, {{3, -350, -201}, {2, -500, -650}}, 3, 1, {{700, -4600, -500}, {1300, -4500, -201}}},
    {NL_ARMY_RANGER_3D, 5, {{9, 750, 899}, {10, 600, 450}}, 3, 1, {{-5500, -300, 600}, {-5400, 300, 899}}},
    {NL_ARMY_RANGER_3D, 5, {{11, 750, 899}, {12, 600, 450}}, 3, 1, {{-6200, -300, 600}, {-6100, 300, 899}}},
    {NL_ARMY_RANGER_3D, 5, {{15, 750, 899}, {16, 600, 450}}, 3, 1, {{5400, -300, 600}, {5500, 300, 899}}},
    {NL_ARMY_RANGER_3D, 5, {{17, 750, 899}, {18, 600, 450}}, 3, 1, {{6100, -300, 600}, {6200, 300, 899}}},
    {NL_ARMY_RANGER_3D, 5, {{43, 1150, 1299}, {44, 1000, 850}}, 3, 1, {{-300, 4500, 1000}, {300, 4600, 1299}}},
    {NL_ARMY_RANGER_3D, 5, {{45, 1150, 1299}, {46, 1000, 850}}, 3, 1, {{-300, 5200, 1000}, {300, 5300, 1299}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_6[] = {
    {NL_ARMY_RANGER_3D, 6, {{25, 150, 299}, {26, 0, -150}}, 3, 1, {{-1700, -300, 0}, {-1600, 300, 299}}},
    {NL_ARMY_RANGER_3D, 6, {{23, 150, 299}, {24, 0, -150}}, 3, 1, {{-1000, -300, 0}, {-900, 300, 299}}},
    {NL_ARMY_RANGER_3D, 6, {{21, 2950, 3099}, {22, 2800, 2650}}, 3, 1, {{-300, 1200, 2800}, {300, 1300, 3099}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_7[] = {
    {NL_ARMY_RANGER_3D, 7, {{1, 150, 299}, {0, 0, -150}}, 3, 1, {{-300, -4200, 0}, {300, -4100, 299}}},
    {NL_ARMY_RANGER_3D, 7, {{3, 150, 299}, {2, 0, -150}}, 3, 1, {{-300, -3500, 0}, {300, -3400, 299}}},
    {NL_ARMY_RANGER_3D, 7, {{9, 750, 899}, {8, 600, 450}}, 3, 1, {{-300, 1400, 600}, {300, 1500, 899}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_8[] = {
    {NL_ARMY_RANGER_3D, 8, {{1, -50, 99}, {0, -200, -350}}, 3, 1, {{2700, -4700, -200}, {3300, -4600, 99}}},
    {NL_ARMY_RANGER_3D, 8, {{3, -50, 99}, {2, -200, -350}}, 3, 1, {{2700, -4000, -200}, {3300, -3900, 99}}},
    {NL_ARMY_RANGER_3D, 8, {{25, 550, 699}, {26, 400, 250}}, 3, 1, {{-300, -6700, 400}, {300, -6600, 699}}},
    {NL_ARMY_RANGER_3D, 8, {{27, 550, 699}, {28, 400, 250}}, 3, 1, {{-300, -7400, 400}, {300, -7300, 699}}},
};
static const NETLizard_Level_Door Specnaz3D_Level_Door_9[] = {
    {NL_ARMY_RANGER_3D, 0, {{1, 150, 299}, {0, 0, -150}}, 3, 1, {{-2000, 2400, 0}, {-1900, 3000, 299}}},
    {NL_ARMY_RANGER_3D, 0, {{3, 150, 299}, {2, 0, -150}}, 3, 1, {{-1300, 2400, 0}, {-1200, 3000, 299}}},
    {NL_ARMY_RANGER_3D, 0, {{10, 1050, 1199}, {11, 900, 750}}, 3, 1, {{1200, -600, 900}, {1300, 0, 1199}}},
    {NL_ARMY_RANGER_3D, 0, {{13, 1050, 1199}, {12, 900, 750}}, 3, 1, {{1200, 0, 900}, {1300, 600, 1199}}},
    {NL_ARMY_RANGER_3D, 0, {{7, 2250, 2399}, {6, 2100, 1950}}, 3, 1, {{-600, -1300, 2100}, {0, -1200, 2399}}},
    {NL_ARMY_RANGER_3D, 0, {{8, 2250, 2399}, {9, 2100, 1950}}, 3, 1, {{0, -1300, 2100}, {600, -1200, 2399}}},
    {NL_ARMY_RANGER_3D, 0, {{17, 3450, 3599}, {16, 3300, 3150}}, 3, 1, {{-1300, 0, 3300}, {-1200, 600, 3599}}},
    {NL_ARMY_RANGER_3D, 0, {{14, 3450, 3599}, {15, 3300, 3150}}, 3, 1, {{-1300, -599, 3300}, {-1200, 1, 3599}}},
};
static const NETLizard_Level_Door *Specnaz3D_Level_Door[] = {
    NULL,
    NULL, Specnaz3D_Level_Door_2, Specnaz3D_Level_Door_3, Specnaz3D_Level_Door_4, Specnaz3D_Level_Door_5,
    Specnaz3D_Level_Door_6, Specnaz3D_Level_Door_7, Specnaz3D_Level_Door_8, Specnaz3D_Level_Door_9, NULL,
};
static const NLint Specnaz3D_Level_Door_Count[] = {
    0,
    0, COUNTOF(Specnaz3D_Level_Door_2), COUNTOF(Specnaz3D_Level_Door_3), COUNTOF(Specnaz3D_Level_Door_4), COUNTOF(Specnaz3D_Level_Door_5),
    COUNTOF(Specnaz3D_Level_Door_6), COUNTOF(Specnaz3D_Level_Door_7), COUNTOF(Specnaz3D_Level_Door_8), COUNTOF(Specnaz3D_Level_Door_9), 0,
};

static const NETLizard_Level_Door CT3DEp2_Level_Door_4[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 4, {{25, 1150, 1300}, {26, 1000, 850}}, 3, 1, {{-3100, 900, 1000}, {-3000, 1500, 1300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 4, {{31, 1550, 1700}, {32, 1400, 1250}}, 3, 1, {{-5200, 4200, 1400}, {-4600, 4300, 1700}}},
};
static const NETLizard_Level_Door CT3DEp2_Level_Door_5[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{1, 150, 300}, {2, 0, -150}}, 3, 1, {{2200, -1800, 0}, {2300, -1200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{20, 150, 300}, {21, 0, -150}}, 3, 1, {{-300, -1800, 0}, {-200, -1200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{38, 150, 300}, {37, 0, -150}}, 3, 1, {{-2800, -1800, 0}, {-2700, -1200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{45, 751, 901}, {46, 601, 451}}, 3, 1, {{-2800, -1000, 601}, {-2700, -400, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{51, 751, 901}, {52, 601, 451}}, 3, 1, {{-300, -1000, 601}, {-200, -400, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{58, 751, 901}, {57, 601, 451}}, 3, 1, {{1200, -2500, 601}, {1300, -1900, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 5, {{61, 751, 901}, {60, 601, 451}}, 3, 1, {{-400, -2500, 601}, {-300, -1900, 901}}},
};
static const NETLizard_Level_Door CT3DEp2_Level_Door_8[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{2, 0, 300}, {-1, 0, 0}}, 1, 1, {{3001, 1200, 0}, {3001, 1800, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{0, 0, 300}, {-1, 0, 0}}, 1, 1, {{3001, 2100, 0}, {3001, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{27, 450, 750}, {-1, 0, 0}}, 1, 1, {{3001, 1800, 450}, {3001, 2400, 750}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{22, 50, 350}, {-1, 0, 0}}, 1, 1, {{1201, -2700, 50}, {1201, -2100, 350}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{25, 0, 300}, {-1, 0, 0}}, 1, 1, {{599, 2100, 0}, {599, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{23, 0, 300}, {-1, 0, 0}}, 1, 1, {{599, 1200, 0}, {599, 1800, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{18, 450, 750}, {-1, 0, 0}}, 1, 1, {{599, 2100, 450}, {599, 2700, 750}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{17, 450, 750}, {-1, 0, 0}}, 1, 1, {{599, 1200, 450}, {599, 1800, 750}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 8, {{16, 850, 1150}, {-1, 0, 0}}, 1, 1, {{-300, 4199, 850}, {300, 4199, 1150}}},
};
static const NETLizard_Level_Door CT3DEp2_Level_Door_9[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 9, {{1, 150, 300}, {0, 0, -150}}, 3, 1, {{1199, 2100, 0}, {1299, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 9, {{15, 150, 300}, {16, 0, -150}}, 3, 1, {{4599, 2100, 0}, {4699, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_2, 9, {{25, 550, 700}, {26, 400, 250}}, 3, 1, {{8300, 600, 400}, {8400, 1200, 700}}},
};
static const NETLizard_Level_Door *CT3DEp2_Level_Door[] = {
    NULL,
    NULL, NULL, NULL, CT3DEp2_Level_Door_4, CT3DEp2_Level_Door_5,
    NULL, NULL, CT3DEp2_Level_Door_8, CT3DEp2_Level_Door_9,
};
static const NLint CT3DEp2_Level_Door_Count[] = {
    0,
    0, 0, 0, COUNTOF(CT3DEp2_Level_Door_4), COUNTOF(CT3DEp2_Level_Door_5),
    0, 0, COUNTOF(CT3DEp2_Level_Door_8), COUNTOF(CT3DEp2_Level_Door_9),
};

static const NETLizard_Level_Door Clone3D_Level_Door_1[] = {
    {NL_CLONE_3D, 1, {{13, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -3600, 0}, {950, -3300, 300}}},
    {NL_CLONE_3D, 1, {{15, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -4200, 0}, {950, -3900, 300}}},
    {NL_CLONE_3D, 1, {{14, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -3900, 0}, {950, -3600, 300}}},
    {NL_CLONE_3D, 1, {{11, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -3600, 0}, {-950, -3300, 300}}},
    {NL_CLONE_3D, 1, {{10, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -3900, 0}, {-950, -3600, 300}}},
    {NL_CLONE_3D, 1, {{9, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -4200, 0}, {-950, -3900, 300}}},
    {NL_CLONE_3D, 1, {{24, 150, 450}, {-1, 0, 0}}, 1, 1, {{949, -8100, 0}, {949, -7800, 300}}},
    {NL_CLONE_3D, 1, {{20, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -8700, 0}, {-950, -8400, 300}}},
    {NL_CLONE_3D, 1, {{21, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -8400, 0}, {-950, -8100, 300}}},
    {NL_CLONE_3D, 1, {{22, 150, 450}, {-1, 0, 0}}, 1, 1, {{-950, -8100, 0}, {-950, -7800, 300}}},
    {NL_CLONE_3D, 1, {{25, 150, 450}, {-1, 0, 0}}, 1, 1, {{949, -8400, 0}, {949, -8100, 300}}},
    {NL_CLONE_3D, 1, {{26, 150, 450}, {-1, 0, 0}}, 1, 1, {{949, -8700, 0}, {949, -8400, 300}}},
    {NL_CLONE_3D, 1, {{33, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -9600, 0}, {950, -9300, 300}}},
    {NL_CLONE_3D, 1, {{34, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -9900, 0}, {950, -9600, 300}}},
    {NL_CLONE_3D, 1, {{35, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -10200, 0}, {950, -9900, 300}}},
    {NL_CLONE_3D, 1, {{28, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -10200, 0}, {-949, -9900, 300}}},
    {NL_CLONE_3D, 1, {{29, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -9900, 0}, {-949, -9600, 300}}},
    {NL_CLONE_3D, 1, {{30, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -9600, 0}, {-949, -9300, 300}}},
    {NL_CLONE_3D, 1, {{42, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -11100, 0}, {950, -10800, 300}}},
    {NL_CLONE_3D, 1, {{43, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -11400, 0}, {950, -11100, 300}}},
    {NL_CLONE_3D, 1, {{44, 150, 450}, {-1, 0, 0}}, 1, 1, {{950, -11700, 0}, {950, -11400, 300}}},
    {NL_CLONE_3D, 1, {{37, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -11700, 0}, {-949, -11400, 300}}},
    {NL_CLONE_3D, 1, {{38, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -11400, 0}, {-949, -11100, 300}}},
    {NL_CLONE_3D, 1, {{39, 150, 450}, {-1, 0, 0}}, 1, 1, {{-949, -11100, 0}, {-949, -10800, 300}}},
    {NL_CLONE_3D, 1, {{72, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{949, -8700, 900}, {949, -8400, 1200}}},
    {NL_CLONE_3D, 1, {{71, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{949, -8400, 900}, {949, -8100, 1200}}},
    {NL_CLONE_3D, 1, {{70, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{949, -8100, 900}, {949, -7800, 1200}}},
    {NL_CLONE_3D, 1, {{66, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{950, -9900, 900}, {950, -9600, 1200}}},
    {NL_CLONE_3D, 1, {{65, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{950, -9600, 900}, {950, -9300, 1200}}},
    {NL_CLONE_3D, 1, {{67, 1050, 1350}, {-1, 0, 0}}, 1, 1, {{950, -10200, 900}, {950, -9900, 1200}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_2[] = {
    {NL_CLONE_3D, 2, {{71, 250, 400}, {72, 250, 100}}, 3, 1, {{12900, 200, 100}, {13500, 350, 400}}},
    {NL_CLONE_3D, 2, {{73, 250, 400}, {74, 250, 100}}, 3, 1, {{11700, -1447, 100}, {12300, -1297, 400}}},
    {NL_CLONE_3D, 2, {{66, 250, 400}, {67, 250, 100}}, 3, 1, {{11700, 200, 100}, {12300, 350, 400}}},
    {NL_CLONE_3D, 2, {{105, 250, 400}, {106, 250, 100}}, 3, 1, {{10050, -700, 100}, {10200, -100, 400}}},
    {NL_CLONE_3D, 2, {{107, 250, 400}, {108, 250, 100}}, 3, 1, {{7200, -700, 100}, {7350, -100, 400}}},
    {NL_CLONE_3D, 2, {{21, 250, 400}, {20, 250, 100}}, 3, 1, {{4350, -700, 100}, {4500, -100, 400}}},
    {NL_CLONE_3D, 2, {{30, -1150, -1000}, {31, -1150, -1300}}, 3, 1, {{-4650, 3000, -1300}, {-4500, 3600, -1000}}},
    {NL_CLONE_3D, 2, {{82, -1850, -1700}, {81, -1850, -2000}}, 3, 3, {{5900, -2000, 410}, {6200, -1700, 420}}},
    {NL_CLONE_3D, 2, {{86, 1050, 1200}, {85, 1050, 900}}, 3, 3, {{5800, 900, 410}, {6100, 1200, 420}}},
    {NL_CLONE_3D, 2, {{89, 1200, 1350}, {88, 1200, 1050}}, 3, 3, {{150, 1050, -990}, {450, 1350, -980}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_3[] = {
    {NL_CLONE_3D, 3, {{17, 150, 300}, {18, 150, 0}}, 3, 1, {{600, 550, 0}, {1200, 700, 300}}},
    {NL_CLONE_3D, 3, {{30, 1950, 2100}, {31, 1950, 1800}}, 3, 1, {{-1500, -600, 1800}, {-900, -450, 2100}}},
    {NL_CLONE_3D, 3, {{45, 2150, 2300}, {46, 2150, 2000}}, 3, 1, {{300, -1800, 2000}, {450, -1200, 2300}}},
    {NL_CLONE_3D, 3, {{52, 1257, 1407}, {53, 1257, 1107}}, 3, 3, {{720, 1107, 310}, {1020, 1407, 320}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_4[] = {
    {NL_CLONE_3D, 4, {{10, 0, 300}, {9, 0, -300}}, 3, 3, {{-1800, -300, 0}, {-1700, 300, 300}}},
    {NL_CLONE_3D, 4, {{63, 0, 300}, {64, 0, -300}}, 3, 2, {{-300, 1400, 0}, {300, 1500, 300}}},
    {NL_CLONE_3D, 4, {{28, 0, 300}, {29, 0, -300}}, 3, 2, {{-300, -1800, 0}, {300, -1700, 300}}},
    {NL_CLONE_3D, 4, {{75, 1527, 1677}, {74, 1527, 1377}}, 3, 3, {{-1638, 1377, 310}, {-1338, 1677, 320}}},
    {NL_CLONE_3D, 4, {{78, 2593, 2743}, {77, 2593, 2443}}, 3, 3, {{-2725, 2443, 310}, {-2425, 2743, 320}}},
    {NL_CLONE_3D, 4, {{80, 1819, 1969}, {81, 1819, 1669}}, 3, 3, {{-1109, 1669, 1510}, {-809, 1969, 1520}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_5[] = {
    {NL_CLONE_3D, 5, {{86, 0, 300}, {87, 0, -300}}, 3, 2, {{-300, 2700, 0}, {300, 2800, 300}}},
    {NL_CLONE_3D, 5, {{1, 300, 600}, {0, 300, 0}}, 3, 3, {{-900, 0, 0}, {-800, 600, 300}}},
    {NL_CLONE_3D, 5, {{7, -1500, -1200}, {6, -1500, -1800}}, 3, 3, {{-300, -1800, 0}, {-200, -1200, 300}}},
    {NL_CLONE_3D, 5, {{15, -1500, -1200}, {14, -1500, -1800}}, 3, 3, {{1200, -1800, 0}, {1300, -1200, 300}}},
    {NL_CLONE_3D, 5, {{24, -2700, -2400}, {23, -2700, -3000}}, 3, 3, {{1200, -3000, 0}, {1300, -2400, 300}}},
    {NL_CLONE_3D, 5, {{27, -4500, -4200}, {26, -4500, -4800}}, 3, 3, {{600, -4800, 300}, {700, -4200, 600}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_6[] = {
    {NL_CLONE_3D, 6, {{33, 1050, 1200}, {32, 1050, 900}}, 3, 1, {{900, 1800, 900}, {1500, 1950, 1200}}},
    {NL_CLONE_3D, 6, {{17, 1050, 1200}, {16, 1050, 900}}, 3, 1, {{300, -1650, 900}, {900, -1500, 1200}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_8[] = {
    {NL_CLONE_3D, 8, {{4, 150, 300}, {3, 150, 0}}, 3, 1, {{-1200, -6450, 0}, {-600, -6300, 300}}},
    {NL_CLONE_3D, 8, {{52, -9875, -9725}, {51, -9875, -10025}}, 3, 3, {{450, -10025, 1810}, {750, -9725, 1820}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_9[] = {
    {NL_CLONE_3D, 9, {{31, 4350, 4500}, {30, 4350, 4200}}, 3, 1, {{900, -8250, 4200}, {1500, -8100, 4500}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_10[] = {
    {NL_CLONE_3D, 10, {{94, 2550, 2700}, {93, 2550, 2400}}, 3, 1, {{-300, -2100, 2400}, {300, -1950, 2700}}},
    {NL_CLONE_3D, 10, {{92, 2550, 2700}, {91, 2550, 2400}}, 3, 1, {{-300, 5250, 2400}, {300, 5400, 2700}}},
    {NL_CLONE_3D, 10, {{39, 2550, 2700}, {40, 2550, 2400}}, 3, 1, {{-900, 9300, 2400}, {-300, 9450, 2700}}},
    {NL_CLONE_3D, 10, {{61, 2550, 2700}, {62, 2550, 2400}}, 3, 1, {{600, 10800, 2400}, {750, 11400, 2700}}},
    {NL_CLONE_3D, 10, {{53, 2550, 2700}, {54, 2550, 2400}}, 3, 1, {{-900, 12300, 2400}, {-300, 12450, 2700}}},
    {NL_CLONE_3D, 10, {{49, 2550, 2700}, {50, 2550, 2400}}, 3, 1, {{-1950, 10800, 2400}, {-1800, 11400, 2700}}},
    {NL_CLONE_3D, 10, {{72, 7050, 7200}, {71, 7050, 6900}}, 3, 3, {{-2250, 6900, 2710}, {-1950, 7200, 2720}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_11[] = {
    {NL_CLONE_3D, 11, {{86, 9150, 9300}, {85, 9150, 9000}}, 3, 1, {{2100, -1500, 9000}, {2250, -900, 9300}}},
    {NL_CLONE_3D, 11, {{88, 3300, 3450}, {87, 3300, 3150}}, 3, 3, {{900, 3150, 4810}, {1200, 3450, 4820}}},
    {NL_CLONE_3D, 11, {{90, 600, 750}, {91, 600, 450}}, 3, 3, {{900, 450, 6010}, {1200, 750, 6020}}},
};
static const NETLizard_Level_Door Clone3D_Level_Door_12[] = {
    {NL_CLONE_3D, 12, {{16, 150, 300}, {15, 150, 0}}, 3, 1, {{-300, -150, 0}, {300, 0, 300}}},
};
static const NETLizard_Level_Door *Clone3D_Level_Door[] = {
    NULL,
    Clone3D_Level_Door_1, Clone3D_Level_Door_2, Clone3D_Level_Door_3, Clone3D_Level_Door_4, Clone3D_Level_Door_5,
    Clone3D_Level_Door_6, NULL, Clone3D_Level_Door_8, Clone3D_Level_Door_9, Clone3D_Level_Door_10,
    Clone3D_Level_Door_11, Clone3D_Level_Door_12
};
static const NLint Clone3D_Level_Door_Count[] = {
    0,
    COUNTOF(Clone3D_Level_Door_1), COUNTOF(Clone3D_Level_Door_2), COUNTOF(Clone3D_Level_Door_3), COUNTOF(Clone3D_Level_Door_4), COUNTOF(Clone3D_Level_Door_5),
    COUNTOF(Clone3D_Level_Door_6), 0, COUNTOF(Clone3D_Level_Door_8), COUNTOF(Clone3D_Level_Door_9), COUNTOF(Clone3D_Level_Door_10),
    COUNTOF(Clone3D_Level_Door_11), COUNTOF(Clone3D_Level_Door_12)
};

static const NETLizard_Level_Door CT3DEp3_Level_Door_1[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{5, 150, 300}, {6, 0, -150}}, 3, 1, {{200, 1500, 0}, {300, 2100, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{8, 150, 300}, {7, 0, -150}}, 3, 1, {{200, -600, 0}, {300, 0, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{18, 150, 300}, {19, 0, -150}}, 3, 1, {{2400, -2400, 0}, {2500, -1800, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{21, 150, 300}, {20, 0, -150}}, 3, 1, {{3900, -4800, 0}, {4000, -4200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{34, -150, 0}, {33, -300, -450}}, 3, 1, {{500, -9300, -300}, {600, -8700, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{49, -150, 0}, {48, -300, -450}}, 3, 1, {{500, -6900, -300}, {600, -6300, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{58, 450, 600}, {57, 300, 150}}, 3, 1, {{-1000, -2400, 300}, {-900, -1800, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 1, {{64, 450, 600}, {63, 300, 150}}, 3, 1, {{-5600, -2400, 300}, {-5500, -1800, 600}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_2[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{3, 450, 600}, {4, 300, 150}}, 3, 1, {{1500, 1200, 300}, {2100, 1300, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{5, -150, 0}, {6, -300, -450}}, 3, 1, {{-300, 1200, -300}, {300, 1300, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{37, 450, 600}, {38, 300, 150}}, 3, 1, {{5100, -1600, 300}, {5700, -1500, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{45, 450, 600}, {46, 300, 150}}, 3, 1, {{5100, -6200, 300}, {5700, -6100, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{53, 450, 600}, {54, 300, 150}}, 3, 1, {{3900, -8100, 300}, {4500, -8000, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 2, {{55, 450, 600}, {56, 300, 150}}, 3, 1, {{6300, -8100, 300}, {6900, -8000, 600}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_3[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{8, 150, 300}, {7, 0, -150}}, 3, 1, {{100, -1500, 0}, {200, -900, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{15, 150, 300}, {14, 0, -150}}, 3, 1, {{100, -3300, 0}, {200, -2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{17, -150, 0}, {16, -300, -450}}, 3, 1, {{-1700, -5100, -300}, {-1600, -4500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{26, -150, 0}, {25, -300, -450}}, 3, 1, {{800, -5100, -300}, {900, -4500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{30, -150, 0}, {29, -300, -450}}, 3, 1, {{2700, -5100, -300}, {2800, -4500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{42, 150, 300}, {41, 0, -150}}, 3, 1, {{9100, -900, 0}, {9700, -800, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{49, 450, 600}, {48, 300, 150}}, 3, 1, {{6700, 1300, 300}, {7300, 1400, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{53, 450, 600}, {54, 300, 150}}, 3, 1, {{5200, 2600, 300}, {5800, 2700, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{62, 150, 300}, {61, 0, -150}}, 3, 1, {{2000, -2100, 0}, {2100, -1500, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{64, 150, 300}, {63, 0, -150}}, 3, 1, {{2000, -900, 0}, {2100, -300, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{70, 450, 600}, {69, 300, 150}}, 3, 1, {{2300, 900, 300}, {2400, 1500, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 3, {{81, 450, 600}, {82, 300, 150}}, 3, 1, {{-200, 900, 300}, {-100, 1500, 600}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_4[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{5, -150, 0}, {4, -300, -450}}, 3, 1, {{2200, -2100, -300}, {2300, -1500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{13, -150, 0}, {12, -300, -450}}, 3, 1, {{7100, -2100, -300}, {7200, -1500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{32, 150, 300}, {31, 0, -150}}, 3, 1, {{12900, 2900, 0}, {13500, 3000, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{54, -450, -300}, {53, -600, -750}}, 3, 1, {{11700, 4700, -600}, {12300, 4800, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{65, -450, -300}, {64, -600, -750}}, 3, 1, {{9600, 4700, -600}, {10200, 4800, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{73, -150, 0}, {74, -300, -450}}, 3, 1, {{7100, 6900, -300}, {7200, 7500, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{104, -150, 0}, {105, -300, -450}}, 3, 1, {{2200, 1500, -300}, {2300, 2100, 0}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 4, {{106, -150, 0}, {107, -300, -450}}, 3, 1, {{13500, 7500, -300}, {14100, 7600, 0}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_5[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{5, 450, 600}, {6, 300, 150}}, 3, 1, {{-1800, 2800, 300}, {-1700, 3400, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{19, 450, 600}, {20, 300, 150}}, 3, 1, {{-1800, -200, 300}, {-1700, 400, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{21, 150, 300}, {22, 0, -150}}, 3, 1, {{-4200, -1800, 0}, {-3600, -1700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{27, 150, 300}, {28, 0, -150}}, 3, 1, {{-3300, -5100, 0}, {-3200, -4500, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{42, 150, 300}, {43, 0, -150}}, 3, 1, {{100, -5100, 0}, {200, -4500, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{53, 150, 300}, {52, 0, -150}}, 3, 1, {{3200, -2700, 0}, {3300, -2100, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{54, 750, 900}, {55, 600, 450}}, 3, 1, {{7200, -7600, 600}, {7300, -7000, 900}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 5, {{80, 750, 900}, {81, 600, 450}}, 3, 1, {{-300, -12100, 600}, {-200, -11500, 900}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_6[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{1, 150, 300}, {0, 0, -150}}, 3, 1, {{-4000, -300, 0}, {-3900, 300, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{20, -450, -300}, {19, -600, -750}}, 3, 1, {{300, -600, -600}, {900, -500, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{30, -450, -300}, {31, -600, -750}}, 3, 1, {{3500, 100, -600}, {3600, 700, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{44, -450, -300}, {43, -600, -750}}, 3, 1, {{6000, 1600, -600}, {6600, 1700, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{51, -450, -300}, {50, -600, -750}}, 3, 1, {{3300, 3500, -600}, {3900, 3600, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{61, -450, -300}, {60, -600, -750}}, 3, 1, {{6000, 5400, -600}, {6600, 5500, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 6, {{69, -450, -300}, {70, -600, -750}}, 3, 1, {{2400, 5400, -600}, {3000, 5500, -300}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_7[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{0, 151, 301}, {1, 1, -149}}, 3, 1, {{0, 2100, 1}, {600, 2200, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{10, 151, 301}, {11, 1, -149}}, 3, 1, {{-3900, -1600, 1}, {-3300, -1500, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{17, 151, 301}, {18, 1, -149}}, 3, 1, {{-5700, -5200, 1}, {-5100, -5100, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{44, 151, 301}, {45, 1, -149}}, 3, 1, {{-5700, -7700, 1}, {-5100, -7600, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{46, 151, 301}, {47, 1, -149}}, 3, 1, {{-4800, -11600, 1}, {-4700, -11000, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{50, -149, 1}, {51, -299, -449}}, 3, 1, {{-300, -9500, -299}, {300, -9400, 1}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{70, 751, 901}, {71, 601, 451}}, 3, 1, {{300, -1900, 601}, {900, -1800, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 7, {{72, 751, 901}, {73, 601, 451}}, 3, 1, {{200, -600, 601}, {300, 0, 901}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_8[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{2, -450, -300}, {3, -600, -750}}, 3, 1, {{3300, -4800, -600}, {3400, -4200, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{24, -450, -300}, {23, -600, -750}}, 3, 1, {{10200, -7200, -600}, {10300, -6600, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{27, -450, -300}, {26, -600, -750}}, 3, 1, {{10200, -9600, -600}, {10300, -9000, -300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{53, 150, 300}, {52, 0, -150}}, 3, 1, {{6300, -9600, 0}, {6400, -9000, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{66, 150, 300}, {67, 0, -150}}, 3, 1, {{8800, -9600, 0}, {8900, -9000, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{77, 150, 300}, {78, 0, -150}}, 3, 1, {{10700, -12000, 0}, {10800, -11400, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{91, 150, 300}, {90, 0, -150}}, 3, 1, {{9100, -6600, 0}, {9200, -6000, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{92, 150, 300}, {93, 0, -150}}, 3, 1, {{7300, -7500, 0}, {7900, -7400, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{96, 750, 900}, {95, 600, 450}}, 3, 1, {{5500, -5400, 600}, {6100, -5300, 900}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{114, 750, 900}, {113, 600, 450}}, 3, 1, {{7900, -9100, 600}, {8500, -9000, 900}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 8, {{119, 750, 900}, {118, 600, 450}}, 3, 1, {{10600, -9100, 600}, {11200, -9000, 900}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_9[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{14, 151, 301}, {15, 1, -149}}, 3, 1, {{300, 1200, 1}, {900, 1300, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{16, 151, 301}, {17, 1, -149}}, 3, 1, {{300, -100, 1}, {900, 0, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{21, 151, 301}, {22, 1, -149}}, 3, 1, {{-3000, -100, 1}, {-2400, 0, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{30, 751, 901}, {31, 601, 451}}, 3, 1, {{-4900, 1200, 601}, {-4300, 1300, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{42, 751, 901}, {41, 601, 451}}, 3, 1, {{-4900, 2500, 601}, {-4300, 2600, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{44, 751, 901}, {45, 601, 451}}, 3, 1, {{-5800, 2500, 601}, {-5200, 2600, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{65, 751, 901}, {66, 601, 451}}, 3, 1, {{-5800, -100, 601}, {-5200, 0, 901}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 9, {{79, 751, 901}, {80, 601, 451}}, 3, 1, {{-7600, -2600, 601}, {-7000, -2500, 901}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_10[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{1, 151, 301}, {0, 1, -149}}, 3, 1, {{-5500, -3800, 1}, {-4900, -3700, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{12, 151, 301}, {13, 1, -149}}, 3, 1, {{-4600, -6200, 1}, {-4500, -5600, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{15, 151, 301}, {14, 1, -149}}, 3, 1, {{-3300, -5600, 1}, {-2700, -5500, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{19, 451, 601}, {18, 301, 151}}, 3, 1, {{-300, -4400, 151}, {300, -4300, 451}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{20, 151, 301}, {21, 1, -149}}, 3, 1, {{-2400, -4300, 1}, {-2300, -3700, 301}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{45, 451, 601}, {44, 301, 151}}, 3, 1, {{-300, -100, 301}, {300, 0, 601}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{60, 451, 601}, {61, 301, 151}}, 3, 1, {{300, -4300, 301}, {400, -3700, 601}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{64, 451, 601}, {63, 301, 151}}, 3, 1, {{3000, -2500, 301}, {3600, -2400, 601}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{70, 451, 601}, {71, 301, 151}}, 3, 1, {{3900, 300, 301}, {4000, 900, 601}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 10, {{83, 1351, 1501}, {84, 1201, 1051}}, 3, 1, {{5400, -2400, 1201}, {5500, -1800, 1501}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_11[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{5, 150, 300}, {6, 0, -150}}, 3, 1, {{-300, -2200, 0}, {300, -2100, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{14, 150, 300}, {15, 0, -150}}, 3, 1, {{2100, -3400, 0}, {2700, -3300, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{20, 150, 300}, {21, 0, -150}}, 3, 1, {{4800, -1200, 0}, {4900, -600, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{29, 150, 300}, {30, 0, -150}}, 3, 1, {{2100, -300, 0}, {2200, 300, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{31, 150, 300}, {32, 0, -150}}, 3, 1, {{-300, 2100, 0}, {300, 2200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{40, 150, 300}, {41, 0, -150}}, 3, 1, {{-5100, 5100, 0}, {-4500, 5200, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{46, 150, 300}, {47, 0, -150}}, 3, 1, {{-3400, 2100, 0}, {-3300, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 11, {{55, 150, 300}, {56, 0, -150}}, 3, 1, {{-2200, -300, 0}, {-2100, 300, 300}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_12[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 12, {{4, 450, 600}, {3, 300, 150}}, 3, 1, {{-2700, 2700, 300}, {-2100, 2800, 600}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 12, {{17, 150, 300}, {18, 0, -150}}, 3, 1, {{3900, -1200, 0}, {4000, -600, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 12, {{23, 150, 300}, {24, 0, -150}}, 3, 1, {{5400, 0, 0}, {5500, 600, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 12, {{28, 450, 600}, {27, 300, 150}}, 3, 1, {{3300, 2700, 300}, {3900, 2800, 600}}},
};
static const NETLizard_Level_Door CT3DEp3_Level_Door_14[] = {
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 14, {{1, 150, 300}, {0, 0, -150}}, 3, 1, {{1199, 2100, 0}, {1299, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 14, {{15, 150, 300}, {16, 0, -150}}, 3, 1, {{4599, 2100, 0}, {4699, 2700, 300}}},
    {NL_CONTR_TERRORISM_3D_EPISODE_3, 14, {{25, 550, 700}, {26, 400, 250}}, 3, 1, {{8300, 600, 400}, {8400, 1200, 700}}},
};
static const NETLizard_Level_Door *CT3DEp3_Level_Door[] = {
    NULL,
    CT3DEp3_Level_Door_1, CT3DEp3_Level_Door_2, CT3DEp3_Level_Door_3, CT3DEp3_Level_Door_4, CT3DEp3_Level_Door_5,
    CT3DEp3_Level_Door_6, CT3DEp3_Level_Door_7, CT3DEp3_Level_Door_8, CT3DEp3_Level_Door_9, CT3DEp3_Level_Door_10,
    CT3DEp3_Level_Door_11, CT3DEp3_Level_Door_12, NULL, CT3DEp3_Level_Door_14, NULL
};
static const NLint CT3DEp3_Level_Door_Count[] = {
    0,
    COUNTOF(CT3DEp3_Level_Door_1), COUNTOF(CT3DEp3_Level_Door_2), COUNTOF(CT3DEp3_Level_Door_3), COUNTOF(CT3DEp3_Level_Door_4), COUNTOF(CT3DEp3_Level_Door_5),
    COUNTOF(CT3DEp3_Level_Door_6), COUNTOF(CT3DEp3_Level_Door_7), COUNTOF(CT3DEp3_Level_Door_8), COUNTOF(CT3DEp3_Level_Door_9), COUNTOF(CT3DEp3_Level_Door_10),
    COUNTOF(CT3DEp3_Level_Door_11), COUNTOF(CT3DEp3_Level_Door_12), 0 , COUNTOF(CT3DEp3_Level_Door_14), 0
};

static const NETLizard_Level_Door **Game3D_Level_Door[] = {
    NULL,
    CT3D_Level_Door,
    Specnaz3D_Level_Door,
    CT3DEp2_Level_Door,
    Egypt3D_Level_Door,
    Clone3D_Level_Door,
    CT3DEp3_Level_Door,
};

static const NLint *Game3D_Level_Door_Count[] = {
    NULL,
    CT3D_Level_Door_Count,
    Specnaz3D_Level_Door_Count,
    CT3DEp2_Level_Door_Count,
    Egypt3D_Level_Door_Count,
    Clone3D_Level_Door_Count,
    CT3DEp3_Level_Door_Count,
};

const NETLizard_Level_Door * nlGet3DGameDoor(NLenum game, NLint level, NLint item_id, NLint *length)
{
    const NETLizard_Level_Door **objs = Game3D_Level_Door[game];
    const NETLizard_Level_Door *ret = NULL;
    NLint len = 0;
    if(objs)
    {
        ret = objs[level];
        len = Game3D_Level_Door_Count[game][level];
        if(ret && item_id >= 0)
        {
            int i;
            for(i = 0; i < len; i++)
            {
                const NETLizard_Level_Door *o = ret + i;
                int j;
                NLboolean has = NL_FALSE;
                for(j = 0; j < COUNTOF(o->item); j++)
                {
                    if(o->item[j].item == item_id)
                    {
                        ret = o;
                        len = 1;
                        has = NL_TRUE;
                        break;
                    }
                }
                if(has)
                    break;
            }
        }
    }
    if(length)
        *length = len;
    return ret;
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
