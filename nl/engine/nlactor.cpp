#include "nlactor.h"

#include <QDebug>

#include <math.h>

#include "nldbg.h"
#include "nlrenderable.h"
#include "nlactorcontainer.h"
#include "nlcomponentcontainer.h"
#include "nlcomponent.h"
#include "utils/nlmath.h"
#include "nlscript.h"

NLActor::NLActor(NLActor *parent) :
    NLObject(NLObject::Type_Actor, 0, NLProperties(), parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_scripts(0)
{
    Construct();
}

NLActor::NLActor(const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Actor, 0, prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_scripts(0)
{
    Construct();
}

NLActor::NLActor(NLScene *scene, NLActor *parent) :
    NLObject(NLObject::Type_Actor, scene, NLProperties(), parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_scripts(0)
{
    Construct();
}

NLActor::NLActor(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Actor, scene, prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_scripts(0)
{
    Construct();
}

NLActor::~NLActor()
{
    Destroy(); // !! vitrual
    Mesa_FreeGLMatrix(&m_matrix);
    Mesa_FreeGLMatrix(&m_globalMatrix);
    Mesa_FreeGLMatrix(&m_normalMatrix);
}

void NLActor::Construct()
{
    NLObject *p = ParentObject();
    if(p)
        SetScene(p->Scene());
    NLActor *a = ParentActor();
    if(a)
        SetParentActor(a);

    CLASS_NAME(NLActor);
    if(objectName().isEmpty() || !GetInitProperty("name").isValid())
        setObjectName("NLActor");
    SetType(NLObject::Type_Actor);
    Mesa_AllocGLMatrix(&m_matrix);
    Mesa_AllocGLMatrix(&m_globalMatrix);
    Mesa_AllocGLMatrix(&m_normalMatrix);
    m_position = NL::Init_Position;
    m_rotation = NL::Init_Rotation;
    m_scale = NL::Init_Scale;
    m_up = NL::Init_Up_y;
    UpdateMatrix();
    UpdateDirection();
}

void NLActor::Init()
{
    if(IsInited())
        return;
    if(m_renderable)
        m_renderable->Init();
    if(m_scripts)
        m_scripts->Init();
    if(m_components)
        m_components->Init();
    if(m_children)
        m_children->Init();
    NLObject::Init();
}

void NLActor::Update(float delta)
{
    if(!IsActived())
        return;
    if(m_scripts)
        m_scripts->Update(delta);
    if(m_components)
        m_components->Update(delta);
    if(m_children)
        m_children->Update(delta);

    if(m_renderable)
        m_renderable->Update(delta);
    //NLObject::Update(delta);
}

void NLActor::Render()
{
    if(!IsActived())
        return;
    if(m_renderable)
    {
        glPushMatrix();
        {
            glMultMatrixf(GL_MATRIX_M(m_globalMatrix));
            m_renderable->Render();
        }
        glPopMatrix();
    }
    if(m_children)
        m_children->Render();
    //qDebug() << objectName() + ": " + Name() + " -> render" << m_position.v[0] << m_position.v[1] << m_position.v[2];
}

void NLActor::Destroy()
{
    if(!IsInited())
        return;
    NLActor *actor = ParentActor();
    NLObject::Destroy();
    if(actor)
        actor->TellChildRemoved();
    if(m_renderable)
    {
        m_renderable->Destroy();
        delete m_renderable;
        m_renderable = 0;
    }
    if(m_scripts)
    {
        m_scripts->Destroy();
        delete m_scripts;
        m_scripts = 0;
    }
    if(m_components)
    {
        m_components->Destroy();
        delete m_components;
        m_components = 0;
    }
    if(m_children)
    {
        m_children->Destroy();
        delete m_children;
        m_children = 0;
    }
}

bool NLActor::keyev(int key, bool pressed, int modifier)
{
    if(!IsActived())
        return false;
    bool res = false;
    if(m_components)
        res = m_components->KeyEventHandler(key, pressed, modifier);
    if(m_children)
        res = m_children->KeyEventHandler(key, pressed, modifier) || res;
    return res;
}

bool NLActor::mouseev(int mouse, bool pressed, int x, int y, int modifier)
{
    if(!IsActived())
        return false;
    bool res = false;
    if(m_components)
        res = m_components->MouseEventHandler(mouse, pressed, x, y, modifier);
    if(m_children)
        res = m_children->MouseEventHandler(mouse, pressed, x, y, modifier) || res;
    return res;
}

bool NLActor::motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    if(!IsActived())
        return false;
    bool res = false;
    if(m_components)
        res = m_components->MouseMotionHandler(mouse, pressed, x, y, oldx, oldy, modifier);
    if(m_children)
        res = m_children->MouseMotionHandler(mouse, pressed, x, y, oldx, oldy, modifier) || res;
    return res;
}

bool NLActor::wheelev(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    if(!IsActived())
        return false;
    bool res = false;
    if(m_components)
        res = m_components->WheelEventHandler(mouse,  orientation, delta, x, y, modifier);
    if(m_children)
        res = m_children->WheelEventHandler(mouse, orientation, delta, x, y, modifier) || res;
    return res;
}

NLActor * NLActor::ParentActor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

const NLActor * NLActor::ParentActor() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLActor *>(p);
    return 0;
}

void NLActor::SetParentActor(NLActor *actor)
{
    setParent(actor);
    UpdateGlobalMatrix();
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
    bool res = m_components->Add(item);
    emit componentChanged(item);
    return res;
}

bool NLActor::RemoveComponent(NLComponent *item)
{
    if(!item)
        return false;
    if(!m_components)
        return false;
    bool res = m_components->Remove(item);
    if(res)
    {
        delete item;
        //emit componentChanged();
    }
    return res;
}

bool NLActor::RemoveComponent(int index)
{
    if(!m_components)
        return false;
    NLComponent *comp = GetComponent(index);
    return RemoveComponent(comp);
}

bool NLActor::RemoveComponent(const QString &name)
{
    if(!m_components)
        return false;
    NLComponent *comp = GetComponent(name);
    return RemoveComponent(comp);
}

bool NLActor::AddChild(NLActor *actor)
{
    if(!actor)
        return false;
    if(actor->ParentActor())
        return false;
    if(!m_children)
    {
        m_children = new NLActorContainer(this);
        m_children->SetScene(Scene());
    }
    bool res = m_children->Add(actor);
    emit childChanged(actor);
    return res;
}

bool NLActor::RemoveChild(NLActor *actor)
{
    if(!actor)
        return false;
    if(!m_children)
        return false;
    bool res = m_children->Remove(actor);
    if(res)
    {
        delete actor;
        //emit childChanged();
    }
    return res;
}

bool NLActor::RemoveChild(int index)
{
    if(!m_children)
        return false;
    NLActor *actor = GetChild(index);
    return RemoveChild(actor);
}

bool NLActor::RemoveChild(const QString &name)
{
    if(!m_children)
        return false;
    NLActor *actor = GetChild(name);
    return RemoveChild(actor);
}

NLActor * NLActor::GetChild(const QString &name)
{
    if(!m_children)
        return 0;
    return m_children->Get(name);
}

NLActor * NLActor::GetChild(int index)
{
    if(!m_children)
        return 0;
    return m_children->Get(index);
}

NLComponent * NLActor::GetComponent(const QString &name)
{
    if(!m_components)
        return 0;
    return m_components->Get(name);
}

NLComponent * NLActor::GetComponent(int index)
{
    if(!m_components)
        return 0;
    return m_components->Get(index);
}

void NLActor::SetRenderable(NLRenderable *renderable)
{
    if(m_renderable != renderable)
    {
        if(m_renderable)
            m_renderable->SetActor(0);
        m_renderable = renderable;
        if(m_renderable)
        {
            if(!m_renderable->IsInited())
                m_renderable->Init();
            m_renderable->SetActor(this);
        }
        emit propertyChanged("renderable", QVariant::fromValue<NLRenderable *>(m_renderable));
    }
}

void NLActor::Reset()
{
    m_position = NL::Init_Position;
    m_rotation = NL::Init_Rotation;
    m_scale = NL::Init_Scale;
    m_up = NL::Init_Up_y;
    UpdateMatrix();
    UpdateDirection();
    emit positionChanged(m_position);
    emit rotationChanged(m_rotation);
    emit scaleChanged(m_scale);
    emit propertyChanged("position", QVariant::fromValue<NLVector3>(m_position));
    emit propertyChanged("rotation",  QVariant::fromValue<NLVector3>(m_rotation));
    emit propertyChanged("scale",   QVariant::fromValue<NLVector3>(m_scale));
    if(m_scripts)
        m_scripts->Reset();
    if(m_components)
        m_components->Reset();
    if(m_children)
        m_children->Reset();
    NLObject::Reset();
}

void NLActor::SetPosition(const NLVector3 &v)
{
    if(vector3_equals(&m_position, &v))
        return;
    m_position = v;
    UpdateMatrix();
    emit positionChanged(m_position);
    emit propertyChanged("position", QVariant::fromValue<NLVector3>(m_position));
}

void NLActor::SetRotation(const NLVector3 &v)
{
    if(vector3_equals(&m_rotation, &v))
        return;
    m_rotation = v;
    UpdateMatrix();
    UpdateDirection();
    emit rotationChanged(m_rotation);
    emit propertyChanged("rotation", QVariant::fromValue<NLVector3>(m_rotation));
}

void NLActor::SetScale(const NLVector3 &v)
{
    if(vector3_equals(&m_scale, &v))
        return;
    m_scale = v;
    UpdateMatrix();
    emit scaleChanged(m_scale);
    emit propertyChanged("scale", QVariant::fromValue<NLVector3>(m_scale));
}

NLActor & NLActor::Move(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return *this;

    NLVector3 pos = m_position;
    vector3_moveve(&pos, &m_right, VECTOR3_X(unit));
    vector3_moveve(&pos, &m_up, VECTOR3_Y(unit));
    vector3_moveve(&pos, &m_direction, VECTOR3_Z(unit));
    SetPosition(pos);
    return *this;
}

NLActor & NLActor::MoveOriginal(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return *this;

    NLVector3 pos = m_position;
    const NLVector3 &right = NL::Init_Right;
    const NLVector3 &up = NL::Init_Up_y;
    const NLVector3 &direction = NL::Init_Direction_y;
    vector3_moveve(&pos, &right, VECTOR3_X(unit));
    vector3_moveve(&pos, &up, VECTOR3_Y(unit));
    vector3_moveve(&pos, &direction, VECTOR3_Z(unit));
    SetPosition(pos);
    return *this;
}

NLActor & NLActor::MoveDirectionOriginal(float len, const NLVector3 &dir)
{
    if(vector3_iszero(&dir) || len == 0.0)
        return *this;

    NLVector3 pos = m_position;
    vector3_moveve(&pos, &dir, len);
    SetPosition(pos);
    return *this;
}

NLActor & NLActor::MoveDirection(float len, const NLVector3 &dir)
{
    if(vector3_iszero(&dir) || len == 0.0)
        return *this;

    NLVector3 pos = m_position;
    NLVector3 nd;
    Mesa_glTransform_row(VECTOR3_V(nd), VECTOR3_V(dir), &m_normalMatrix);
    vector3_moveve(&pos, &nd, len);
    SetPosition(pos);
    return *this;
}

NLActor & NLActor::Turn(const NLVector3 &v)
{
    if(vector3_iszero(&v))
        return *this;

    NLVector3 rot = VECTOR3(
                NL::clamp_angle(VECTOR3_X(m_rotation) + VECTOR3_X(v)),
                NL::clamp_angle(VECTOR3_Y(m_rotation) + VECTOR3_Y(v)),
                NL::clamp_angle(VECTOR3_Z(m_rotation) + VECTOR3_Z(v))
                );
    SetRotation(rot);
    return *this;
}

NLActor & NLActor::Zoom(const NLVector3 &v)
{
    //vector3_multiplyve(&m_scale, &v);
    vector3_addve(&m_scale, &v);
    UpdateMatrix();
    emit scaleChanged(m_scale);
    return *this;
}

void NLActor::UpdateMatrix()
{
    UpdateLocalMatrix();
    UpdateGlobalMatrix();
}

void NLActor::UpdateLocalMatrix()
{
    Mesa_glLoadIdentity(&m_matrix);

    Mesa_glTranslate(&m_matrix,
                VECTOR3_X(m_position),
                VECTOR3_Y(m_position),
                VECTOR3_Z(m_position)
                );

    Mesa_glRotate(&m_matrix, VECTOR3_X(m_rotation), 1, 0, 0);
    {
        Mesa_glRotate(&m_matrix, VECTOR3_Z(m_rotation), 0, 0, 1); // roll
        Mesa_glRotate(&m_matrix, VECTOR3_Y(m_rotation), 0, 1, 0);
    }

    Mesa_glScale(&m_matrix, VECTOR3_X(m_scale), VECTOR3_Y(m_scale), VECTOR3_Z(m_scale));

    UpdateNormalMatrix();
}

void NLActor::UpdateGlobalMatrix()
{
    Mesa_glLoadIdentity(&m_globalMatrix);
    NLActor *actor = ParentActor();
    if(actor)
    {
        Mesa_glLoadMatrix(&m_globalMatrix, GL_MATRIXV_M(actor->GlobalMatrix()));
    }
    Mesa_glMultMatrix(&m_globalMatrix, GL_MATRIX_M(m_matrix));
    UpdateChildrenMatrix();
}

void NLActor::UpdateNormalMatrix()
{
    NL::cale_normal_matrix(m_normalMatrix, m_matrix);
}

void NLActor::UpdateChildrenMatrix()
{
    if(!m_children)
        return;
    Q_FOREACH(NLObject *obj, m_children->ObjectList())
    {
        (static_cast<NLActor *>(obj))->UpdateGlobalMatrix();
    }
}

void NLActor::UpdateDirection()
{
    UpdateUp(); // virtual, constructor call it

    static const float v[] = {0, 0, -1};
    Mesa_glTransform_row(VECTOR3_V(m_direction), v, &m_normalMatrix);

    vector3_crossv(&m_right, &m_direction, &m_up);
    vector3_normalizev(&m_right);
}

void NLActor::UpdateUp()
{
    static const float up[] = {0, 1, 0};
    Mesa_glTransform_row(VECTOR3_V(m_up), up, &m_normalMatrix);
}

void NLActor::InitProperty()
{
    NLObject::InitProperty();
    NLVector3 v = GetInitProperty_T<NLVector3>("position", NL::Init_Position);
    SetPosition(v);
    v = GetInitProperty_T<NLVector3>("scale", NL::Init_Scale);
    SetScale(v);
    v = GetInitProperty_T<NLVector3>("rotation", NL::Init_Rotation);
    //VECTOR3_X(v) = GetProperty_T<float>("pitch", 0);
    //VECTOR3_Y(v) = GetProperty_T<float>("yaw", 0);
    //VECTOR3_Z(v) = GetProperty_T<float>("roll", 0);
    SetRotation(v);
}

void NLActor::SetUp(const NLVector3 &up)
{
    if(!vector3_equals(&m_up, &up))
    {
        m_up = up;
        //UpdateMatrix();
        UpdateDirection();
    }
}

void NLActor::SetDirection(const NLVector3 &dir)
{
    if(!vector3_equals(&m_direction, &dir))
    {
        m_direction = dir;
        //UpdateMatrix();
        UpdateDirection();
    }
}

void NLActor::SetRight(const NLVector3 &r)
{
    if(!vector3_equals(&m_right, &r))
    {
        m_right = r;
        //UpdateMatrix();
        UpdateDirection();
    }
}

int NLActor::ChildrenCount() const
{
    if(m_children)
        return m_children->Count();
    return 0;
}

int NLActor::ComponentCount() const
{
    if(m_components)
        return m_components->Count();
    return 0;
}

int NLActor::ChildrenTotalCount() const
{
    if(m_children)
        return m_children->TotalCount();
    return 0;
}

bool NLActor::AddScript(NLScript *item)
{
    if(!item)
        return false;
    if(!m_scripts)
    {
        m_scripts = new NLScriptContainer(this);
        m_scripts->SetScene(Scene());
    }
    bool res = m_scripts->Add(item);
    emit scriptChanged(item);
    return res;
}

bool NLActor::RemoveScript(NLScript *item)
{
    if(!item)
        return false;
    if(!m_scripts)
        return false;
    bool res = m_scripts->Remove(item);
    if(res)
    {
        delete item;
        //emit scriptChanged();
    }
    return res;
}

bool NLActor::RemoveScript(int index)
{
    if(!m_scripts)
        return false;
    NLScript *script = GetScript(index);
    return RemoveScript(script);
}

bool NLActor::RemoveScript(const QString &name)
{
    if(!m_scripts)
        return false;
    NLScript *script = GetScript(name);
    return RemoveScript(script);
}

NLScript * NLActor::GetScript(const QString &name)
{
    if(!m_scripts)
        return 0;
    return m_scripts->Get(name);
}

NLScript * NLActor::GetScript(int index)
{
    if(!m_scripts)
        return 0;
    return m_scripts->Get(index);
}

int NLActor::ScriptCount() const
{
    if(m_scripts)
        return m_scripts->Count();
    return 0;
}

NLActor * NLActor::CreateChild(const NLProperties &props)
{
    NLActor *actor = new NLActor(props);
    AddChild(actor);
    return actor;
}

NLComponent * NLActor::CreateComponent(const NLProperties &props)
{
    NLComponent *comp = new NLComponent(props);
    AddComponent(comp);
    return comp;
}

NLScript * NLActor::CreateScript(const NLProperties &props)
{
    NLScript *script = new NLScript(props);
    AddScript(script);
    return script;
}
