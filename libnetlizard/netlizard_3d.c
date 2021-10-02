#include "netlizard.h"

#include "priv_local.h"
#include "netlizard_3d.h"

static const int Game_Count = NL_TOTAL_GAME;
static const char *Game_Names[] = {
	"Racing Evolution 3D",
	"Contr Terrorism 3D",
	"Army Rangers 3D : Operation Arctic",
	"Contr Terrorism 3D Episode 2",
	"Shadows of Egypt 3D",
	"Clone 3D",
	"Contr Terrorism 3D Episode 3"
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

static NETLizard_3D_Model_Config Game_Config[] = {
    {
        NL_RACING_EVOLUTION_3D,
        0,
        NL_TEXTURE_3D_ENGINE_V2,
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

void delete_NETLizard_3D_Mesh(NETLizard_3D_Mesh *mesh)
{
    free(mesh->vertex.data);
    free(mesh->primitive.data);
    free(mesh->plane.data);
    free(mesh->bsp.data);
}

void delete_NETLizard_3D_Item_Mesh(NETLizard_3D_Item_Mesh *mesh)
{
	delete_NETLizard_3D_Mesh(&mesh->item_mesh);
}

void delete_NETLizard_3D_Model(NETLizard_3D_Model *model)
{
    int i;

    for(i = 0; i < model->data.count; i++)
        delete_NETLizard_3D_Mesh(((NETLizard_3D_Mesh *)(model->data.data)) + i);
    free(model->data.data);

    for(i = 0; i < model->item_data.count; i++)
        delete_NETLizard_3D_Item_Mesh(((NETLizard_3D_Item_Mesh *)(model->item_data.data)) + i);
    free(model->item_data.data);

    free(model->bsp_data.data);
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
            delete_NETLizard_Texture(&tex);
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

const char * nlGet3DModelAnimationName(NETLizard_3D_Animation_Type anim)
{
	if(anim >= Animation_Total_Type)
		return NULL;
	static const char *names[Animation_Total_Type] = {
		"Idel Animation",
		"Walk Animation",
		"Run Animation",
		"Fighting-1 Animation",
		"Fighting-2 Animation",
		"Attack-1 Animation",
		"Attack-2 Animation",
		"Dead-1 Animation",
		"Dead-2 Animation"
	};
	return names[anim];
}


int nlGet3DModelAnimationRange(NETLizard_Game game, int index, int animation[])
{
	const int anim[2][8][Animation_Total_Type * 2] = {
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
    if(game == NL_SHADOW_OF_EGYPT_3D)
	{
		if(animation)
			memcpy(animation, anim[0][index], sizeof(int) * Animation_Total_Type * 2);
		return anim_count[0][index];
	}
    else if(game == NL_CLONE_3D)
	{
		if(animation)
			memcpy(animation, anim[1][index], sizeof(int) * Animation_Total_Type * 2);
		return anim_count[1][index];
	}
	return -1;
}

int nlGetItemType(NETLizard_Game game, int index)
{
	switch(game)
	{
        case NL_CLONE_3D:
			if(index == 3 || index == 4 || index == 5 || index == 6)
				return Item_DoorV_Type;
			else if(index == 34 || index == 35 || index == 36 || index == 37)
				return Item_DoorH_Type;
			else if(index == 52 || index == 53 || index == 54 || index == 55)
				return Item_Weapon_Type;
			else if(index == 38 || index == 39)
				return Item_Ladder_Type;
			else if(index == 40 || index == 41 || index == 42 || index == 43)
				return Item_Elevator_Type;
			else if(index == 19 || index == 20)
				return Item_DoorV_Type;
			else if(index == 29)
				return Item_FanH_Type;
			else if(index == 31) // 机枪
				return Item_FanV_Type;
			else if(index == 32 || index == 33)
				return Item_DoorH_Type;
			else if(index == 26)
				return Item_Switcher_Type;
			break;

        case NL_SHADOW_OF_EGYPT_3D:
			if(index == 20 || index == 21)
				return Item_DoorV_Type;
			if(index == 5 || index == 6)
				return Item_Skyline_Type;
			else if(index == 24 || index == 25 || index == 26 || index == 27 || index == 28 || index == 29)
				return Item_Weapon_Type | Item_FanV_Type;
			else if(index == 33 || index == 34 || index == 35 || index == 36)
				return Item_Box_Type;
			else if(index == 14)
				return Item_FanV_Type | Item_Portal_Type;
			else if(index == 8)
				return Item_FanV_Type;
			break;

        case NL_CONTR_TERRORISM_3D:
			if(index == 52 || index == 53 || index == 54 || index == 55)
				return Item_DoorV_Type;
			else if(index == 16 || index == 21 || index == 26)
				return Item_FanV_Type;
			else if(index == 23)
				return Item_FanH_Type;
			else if(index == 17)
				return Item_Other_Type;
			break;

        case NL_CONTR_TERRORISM_3D_EPISODE_2:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6 || index == 7)
				return Item_Weapon_Type;
			else if(index == 38 || index == 39 || index == 40 || index == 41)
				return Item_DoorV_Type;
			else if(index == 43)
				return Item_Ladder_Type;
			else if(index == 12 || index == 15)
				return Item_FanV_Type;
			else if(index == 13)
				return Item_FanH_Type;
			break;

        case NL_ARMY_RANGER_3D:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5)
				return Item_Weapon_Type | Item_FanV_Type;
			else if(index == 6 || index == 7 || index == 8 || index == 9 || index == 10 || index == 11)
				return Item_Tiny_Type | Item_FanV_Type;
			else if(index == 40 || index == 41 || index == 42 || index == 43)
				return Item_DoorV_Type;
			else if(index == 34 || index == 49)
				return Item_FanV_Type;
			else if(index == 35)
				return Item_FanH_Type;
			break;

        case NL_CONTR_TERRORISM_3D_EPISODE_3:
			if(index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6 || index == 7)
				return Item_Weapon_Type;
			else if(index == 38 || index == 39 || index == 40 || index == 41)
				return Item_DoorV_Type;
			else if(index == 12 || index == 15)
				return Item_FanV_Type;
			else if(index == 13)
				return Item_FanH_Type;
			else if(index == 43)
				return Item_Ladder_Type;
			break;

		default:
			break;
	}
	return Item_Other_Type;
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
