#ifndef _KARIN_NLDEF_H
#define _KARIN_NLDEF_H

#include <QVariant>

#include "lib/vector3.h"
#include "lib/mesa_gl_math.h"

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

typedef QString NLName;
#if 0
typedef QVariantHash NLProperties;
typedef QVariant NLProperty;
#endif

typedef struct vector3_s NLVector3;
typedef GLmatrix NLMatrix4;

#define NLINTERFACE
#define NLGETTER(x)
#define NLSETTER(x)
#define NLPROPERTYD(x)
#define NLPROPERTY(T, x)
#define NLACTOR(x)
#define NLSCENE(x)
#define NLCOMPONENT(x)
#define NLRENDERER(x)

#define NLDEBUG_VECTOR3(v) qDebug() << QString(#v "(%1, %2, %3)").arg(VECTOR3_X(v), 0, 'f', 6).arg(VECTOR3_Y(v), 0, 'f', 6).arg(VECTOR3_Z(v), 0, 'f', 6);

#define NLDEBUG_MATRIX4(v) qDebug() << QString(#v "[\n|%1, %2, %3, %4|\n|%5, %6, %7, %8|\n|%9, %10, %11, %12|\n|%13, %14, %15, %16|\n]")\
.arg(GL_MATRIX_M_CR4(v, 1, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 1), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 2), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 3), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 4), 16, 'f', 6)\
;
#define NLDEBUG_MATRIX3(v) qDebug() << QString(#v "[\n|%1, %2, %3|\n|%4, %5, %6|\n|%7, %8, %9|\n|%10, %11, %12|\n]")\
.arg(GL_MATRIX_M_CR4(v, 1, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 1), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 2), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 3), 16, 'f', 6)\
;

Q_DECLARE_METATYPE(NLVector3)

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

#endif // _KARIN_NLDEF_H
