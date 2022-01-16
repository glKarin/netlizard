#ifndef _KARIN_NLDEF_H
#define _KARIN_NLDEF_H

#include <QVariant>

#include "math/vector3.h"
#include "math/mesa_gl_math.h"

#if defined(_NL_LIBRARY)
#  define NLLIB_EXPORT Q_DECL_EXPORT
#else
#  define NLLIB_EXPORT Q_DECL_IMPORT
#endif

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

typedef struct vector3_s NLVector3;
typedef GLmatrix NLMatrix4;
typedef QPair<QString, QVariant> QVariantPair;

#define NLINTERFACE
#define NLGETTER(x)
#define NLSETTER(x)
#define NLPROPERTYD(x)
#define NLPROPERTY(T, x)
#define NLACTOR(x)
#define NLSCENE(x)
#define NLCOMPONENT(x)
#define NLRENDERER(x)

#define NLClamp(t, min, max) qMax(min, qMin(t, max))

#ifndef nlinstanceof
#define nlinstanceof(obj, T) ((dynamic_cast<T *>(&obj)) != 0)
#define nlinstanceofv(obj, T) ((dynamic_cast<T *>(obj)) != 0)
#endif

#ifndef nlcountof
#define nlcountof(arr) (sizeof(arr) / sizeof(arr[0]))
#define nllengthof nlcountof
#endif

#define NLSINGLE_INSTANCE_DEF(C) static C * Instance();
#define NLSINGLE_INSTANCE_DECL(C) C * C::Instance() \
{\
    static C _instance; \
    return &_instance; \
}
#define NLSINGLE_INSTANCE_OBJ(C) C::Instance()

#define NLBITS(x) (1 << (x))
#define NLBITS_ALL (~0U)

#define NLSWAP(a, b, T) \
{ \
    T __x = (a); \
    a = (b) ; \
    b = __x; \
}

#define NLSWAPV(a, b, T) \
{ \
    T __x = *(a); \
    *(a) = *(b) ; \
    *(b) = __x; \
}

#define NLPTR_MOVE(dst, src) \
{ \
    (dst) = (src); \
    (src) = NULL; \
}

#define NLPTR_DELETE(T, x) \
if(x) { \
    T *_Tx = x; \
    x = 0; \
    delete _Tx; \
}

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
