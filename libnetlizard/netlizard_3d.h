#ifndef _KARIN_NETLIZARD_3D_H
#define _KARIN_NETLIZARD_3D_H

#ifdef __cplusplus
extern "C" {
#endif

#define CT3D_LEVEL 10
#define CT3DEP2_LEVEL 9
#define RE3D_LEVEL 7
#define EGYPT3D_LEVEL 19
#define CLONE3D_LEVEL 12
#define CT3DEP3_LEVEL 15
#define SPECNAZ3D_LEVEL 10

#define LVL_CODE 1819700224 // lvl
#define O_SPLASH_O_CODE 1865379584 // o/o

#define CLONE3D_TEX_COUNT 57
#define CT3D_TEX_COUNT 72
#define EGYPT3D_TEX_COUNT 58 // 50
#define SPECNAZ3D_TEX_COUNT 56 // 39
#define CT3DEP2_TEX_COUNT 52 // 55
#define CT3DEP3_TEX_COUNT 52 // 55

#define CLONE3D_TEX_SUBFIX "w/w%d.png"
#define CT3D_TEX_SUBFIX "tex/l%d.png"
#define CT3DEP2_TEX_SUBFIX "w/l%d.png"
#define SPECNAZ3D_TEX_SUBFIX "w/w%d.png"
#define EGYPT3D_TEX_SUBFIX "w/w%d.png"
#define CT3DEP3_TEX_SUBFIX "w/l%d.png"

#define CT3D_SKY_FILE "sky_0.png"
#define CT3DEP2_SKY_FILE "s.png"
#define CT3DEP3_SKY_FILE "s.png"
#define SPECNAZ3D_SKY_FILE "s.png"
#define RE3D_SKY_FILE "sky.png"

#define ROLE_FILE_SUBFIX "un%d.png"
#define V3_FP_FILE_SUBFIX "fp%d.png"
#define LEVEL_FILE_SUBFIX "lvl%d.png"
#define RE3D_CAR_FILE_SUBFIX "car%d.png"
#define RE3D_LEVEL_FILE_SUBFIX "track0%d.png"

#define SKYFILE_LENGTH 10
#define OBJ_SUBFIX_LENGTH 14

#define CLONE3D_OBJ_SUBFIX "o/o%d.png"
#define CT3D_OBJ_SUBFIX "obj/obj%d.png"
#define CT3DEP2_OBJ_SUBFIX "o/o%d.png"
#define SPECNAZ3D_OBJ_SUBFIX "o/o%d.png"
#define EGYPT3D_OBJ_SUBFIX "o/o%d.png"
#define CT3DEP3_OBJ_SUBFIX "o/o%d.png"

#define RE3D_MIDI_COUNT 4
#define CT3D_MIDI_COUNT 0
#define EGYPT3D_MIDI_COUNT 6
#define CLONE3D_MIDI_COUNT 3
#define CT3DEP2_MIDI_COUNT 5
#define CT3DEP3_MIDI_COUNT 5
#define SPECNAZ3D_MIDI_COUNT 5

char * make_resource_file_path(const char *format, int index, const char *resc_path);

#ifdef __cplusplus
}
#endif

#endif
