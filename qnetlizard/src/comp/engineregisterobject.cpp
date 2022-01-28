#include "engineregisterobject.h"

#include <QDebug>

#include "mapeventhandlercomponent.h"
#include "simplecameracomponent.h"
#include "simplecontrol2dcomponent.h"
#include "simplecontrolcomponent.h"
#include "simpleimagecontrolcomponent.h"
#include "simplelightsourcecomponent.h"

#include "netlizardanimationmodelrenderer.h"
#include "netlizardfontrenderer.h"
#include "netlizarditemmodelrenderer.h"
#include "netlizardmapmodeldebugrenderer.h"
#include "netlizardmapmodelrenderer.h"
#include "netlizardshadowmodelrenderer.h"
#include "netlizardspriterenderer.h"
#include "netlizardtexturerenderer.h"

#include "simplecameraactor.h"
#include "simplelightsourceactor.h"

#define REG_METATYPE(T) qRegisterMetaType<T *>(#T " *")

void EngineRegisterObject::RegisterMetaType()
{
    REG_METATYPE(MapEventHandlerComponent);
    REG_METATYPE(SimpleCameraComponent);
    REG_METATYPE(SimpleControl2DComponent);
    REG_METATYPE(SimpleControlComponent);
    REG_METATYPE(SimpleImageControlComponent);
    REG_METATYPE(SimpleLightSourceComponent);
    REG_METATYPE(NETLizardAnimationModelRenderer);
    REG_METATYPE(NETLizardFontRenderer);
    REG_METATYPE(NETLizardItemModelRenderer);
    REG_METATYPE(NETLizardMapModelDebugRenderer);
    REG_METATYPE(NETLizardMapModelRenderer);
    REG_METATYPE(NETLizardShadowModelRenderer);
    REG_METATYPE(NETLizardSpriteRenderer);
    REG_METATYPE(NETLizardSpriteRenderer);
    REG_METATYPE(SimpleCameraActor);
    REG_METATYPE(SimpleLightSourceActor);
    qDebug() << "NETLizard register";
}
