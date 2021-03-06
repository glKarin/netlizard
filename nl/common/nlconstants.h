#ifndef _KARIN_NLCONSTANTS_H
#define _KARIN_NLCONSTANTS_H

#include "common/nlvec.h"

enum NLAction_e
{
    NLAction_Move_Forward = 0,
    NLAction_Move_Backward,
    NLAction_Move_Left,
    NLAction_Move_Right,
    NLAction_Move_Up,
    NLAction_Move_Down,
    NLAction_Turn_Left,
    NLAction_Turn_Right,
    NLAction_Turn_Up,
    NLAction_Turn_Down,
    NLAction_Roll_Left,
    NLAction_Roll_Right,
    NLAction_Zoom_Out,
    NLAction_Zoom_In,
    NLAction_Crough,
    NLAction_Jump,
    NLAction_Fire,
    NLAction_Fire_2,
    NLAction_Fire_3,
    NLAction_Reload,
    NLAction_Switch_Weapon,
    NLAction_Drop_Weapon,
    NLAction_Prev_Weapon,
    NLAction_Next_Weapon,
    NLAction_Weapon_1,
    NLAction_Weapon_2,
    NLAction_Weapon_3,
    NLAction_Weapon_4,
    NLAction_Weapon_5,
    NLAction_Weapon_6,
    NLAction_Weapon_7,
    NLAction_Weapon_8,
    NLAction_Weapon_9,
    NLAction_Weapon_10,
    NLAction_Map,
    NLAction_Use,
    NLAction_Run,
    NLAction_Walk,
    NLAction_Console,
    NLAction_Scope,
    NLAction_Buy,
    NLAction_Lighting,
    NLAction_User_1,
    NLAction_User_2,
    NLAction_User_3,
    NLAction_User_4,
    NLAction_User_5,
    NLAction_User_6,
    NLAction_User_7,
    NLAction_User_8,
    NLAction_User_9,
    NLAction_User_10,
    NLAction_Total
};
typedef NLAction_e NLActionType;

namespace NL
{
extern const NLVector3 Init_Up_z;
extern const NLVector3 Init_Up_y;
extern const NLVector3 Init_Direction_z;
extern const NLVector3 Init_Direction_y;
extern const NLVector3 Init_Right;
extern const NLVector3 Init_Position;
extern const NLVector3 Init_Rotation;
extern const NLVector3 Init_Scale;
}

#endif // _KARIN_NLCONSTANTS_H
