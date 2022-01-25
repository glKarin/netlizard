#include "nlrenderable.h"

#include <QDebug>

#include "nlactor.h"
#include "nldbg.h"

NLRenderable::NLRenderable(NLActor *parent) :
    NLObject(NLPROPERTIY_NAME(NLRenderable), parent)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLRenderable::NLRenderable(const NLProperties &prop, NLActor *parent) :
    NLObject(NLPROPERTIES_NAME(prop, NLRenderable), parent)
{
    Construct();
}

NLRenderable::NLRenderable(NLScene *scene, NLActor *parent) :
    NLObject(scene, NLPROPERTIY_NAME(NLRenderable), parent)
{
    Construct();
}

NLRenderable::NLRenderable(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(scene, NLPROPERTIES_NAME(prop, NLRenderable), parent)
{
    Construct();
}

NLRenderable::~NLRenderable()
{
    Destroy(); // !! vitrual
}

void NLRenderable::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLRenderable);
    setObjectName("NLRenderable");
    SetType(NLObject::Type_Renderer);
}

void NLRenderable::SetActor(NLActor *actor)
{
    setParent(actor);
}

NLActor * NLRenderable::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLRenderable::Update(float delta)
{
    if(!IsActived())
        return;
}

void NLRenderable::Destroy()
{
    SetActor(0);
    NLObject::Destroy();
}
