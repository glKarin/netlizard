#include "netlizard.h"
#include "netlizard_3d.h"

#include "priv_local.h"

char * nlGet3DGameLevelMusicFileName(NETLizard_Game game, NLint level, char *file)
{
	if(!nlCheck3DGameLevelIsAvailable(game, level))
		return NULL;

#define GET_MIDI_NAME_BY_LEVEL(str, level, count) \
    { \
		int l = level % count - 1; \
		if(l == -1) \
		l = count - 1; \
		sprintf(str, "mm%d.mid", l); \
	}

    int l;

	switch(game)
	{
        case NL_RACING_EVOLUTION_3D:
            l = level % RE3D_MIDI_COUNT;
			if(l == -1)
				l = RE3D_MIDI_COUNT + 1;
			sprintf(file, "%d.mid", l);
			break;
        case NL_CONTR_TERRORISM_3D:
            strcpy(file, "mm.mid");
			break;
        case NL_ARMY_RANGER_3D:
			GET_MIDI_NAME_BY_LEVEL(file, level, SPECNAZ3D_MIDI_COUNT)
				break;
        case NL_CONTR_TERRORISM_3D_EPISODE_2:
			GET_MIDI_NAME_BY_LEVEL(file, level, CT3DEP2_MIDI_COUNT)
				break;
        case NL_SHADOW_OF_EGYPT_3D:
            if(level == 0)
                strcpy(file, "mm.mid");
			else if(level == 18)
                strcpy(file, "mm1.mid");
			else
				GET_MIDI_NAME_BY_LEVEL(file, level, EGYPT3D_MIDI_COUNT)
			break;
        case NL_CLONE_3D:
			GET_MIDI_NAME_BY_LEVEL(file, level, CLONE3D_MIDI_COUNT)
				break;
        case NL_CONTR_TERRORISM_3D_EPISODE_3:
			GET_MIDI_NAME_BY_LEVEL(file, level, CT3DEP3_MIDI_COUNT)
				break;
		default:
			break;
	}
	return file;
}

char * nlGet3DGameMenuMusicFileName(NETLizard_Game game, char *file)
{
	switch(game)
	{
        case NL_RACING_EVOLUTION_3D:
            strcpy(file, "1.mid");
			break;
        case NL_CONTR_TERRORISM_3D:
        case NL_ARMY_RANGER_3D:
        case NL_CONTR_TERRORISM_3D_EPISODE_2:
        case NL_SHADOW_OF_EGYPT_3D:
        case NL_CLONE_3D:
        case NL_CONTR_TERRORISM_3D_EPISODE_3:
            strcpy(file, "mm.mid");
			break;
		default:
			break;
	}
	return file;
}
