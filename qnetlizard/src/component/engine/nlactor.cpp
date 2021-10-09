#include "nlactor.h"

#include <QDebug>

#include "qdef.h"
#include "nlrenderable.h"
#include "nlactorcontainer.h"
#include "nlcomponentcontainer.h"
#include "nlcomponent.h"

NLActor::NLActor(const NLPropperties &prop, NLActor *parent) :
    NLObject(prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0)
{
    setObjectName("NLActor");
    SetType(NLObject::Type_Actor);
}

NLActor::~NLActor()
{
}

void NLActor::Init()
{
    if(IsInited())
        return;
    if(m_renderable)
        m_renderable->InitRender();
    NLObject::Init();
}

void NLActor::Update(float delta)
{
    if(!IsInited())
        return;
    if(m_components)
        m_components->Update(delta);
    NLObject::Update(delta);
}

void NLActor::Render()
{
    if(!IsInited())
        return;
    if(m_renderable)
        m_renderable->Render();
    //qDebug() << objectName() + ": " + Name() + " -> render";
}

void NLActor::Destroy()
{
    if(!IsInited())
        return;
    if(m_renderable)
        m_renderable->DeinitRender();
    if(m_components)
        m_components->Destroy();
    NLObject::Destroy();
}

bool NLActor::keyev(int key, bool pressed, int modify)
{
    if(m_components)
        return m_components->KeyEventHandler(key, pressed, modify);
    return false;
}

bool NLActor::mouseev(int mouse, bool pressed, int x, int y, int modify)
{
    if(m_components)
        return m_components->MouseEventHandler(mouse, pressed, x, y, modify);
    return false;
}

bool NLActor::motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modify)
{
    if(m_components)
        return m_components->MouseMotionHandler(mouse, pressed, x, y, oldx, oldy, modify);
    return false;
}

bool NLActor::wheelev(int orientation, int delta, int x, int y, int modify)
{
    if(m_components)
        return m_components->WheelHandler(orientation, delta, x, y, modify);
    return false;
}

NLActor * NLActor::ParentActor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLActor::SetParentActor(NLActor *actor)
{
    setParent(actor);
}

NLActorContainer * NLActor::Container()
{
    return static_cast<NLActorContainer *>(NLObject::Container());
}

void NLActor::SetContainer(NLActorContainer *container)
{
    NLObject::SetContainer(container);
}

bool NLActor::AddComponent(NLComponent *item)
{
    if(!item)
        return false;
    if(!m_components)
    {
        m_components = new NLComponentContainer(this);
        m_components->SetScene(Scene());
    }
    return m_components->Add(item);
}

bool NLActor::RemoveComponent(NLComponent *item)
{
    if(!item)
        return false;
    if(!m_components)
        return false;
    bool res = m_components->Remove(item);
    item->Destroy();
    return res;
}

void NLActor::SetRenderable(NLRenderable *renderable)
{
    m_renderable = renderable;
    if(m_renderable)
        m_renderable->setParent(this);
}

NLRenderable * NLActor::Renderable()
{
    return m_renderable;
}
