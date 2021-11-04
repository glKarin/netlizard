#include "nlactor.h"

#include <QDebug>

#include <math.h>

#include "qdef.h"
#include "nlrenderable.h"
#include "nlactorcontainer.h"
#include "nlcomponentcontainer.h"
#include "nlcomponent.h"
#include "nlmath.h"

static const NLVector3 InitUp_z = VECTOR3(0, 0, 1);
static const NLVector3 InitUp_y = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_z = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_y = VECTOR3(0, 0, -1);
static const NLVector3 InitRight = VECTOR3(1, 0, 0);
static const NLVector3 InitPosition = VECTOR3(0, 0, 0);
static const NLVector3 InitRotation = VECTOR3(/*-9*/0, 0, 0);
static const NLVector3 InitScale = VECTOR3(1, 1, 1);

NLActor::NLActor(NLActor *parent) :
    NLObject(parent),
    m_renderable(0),
    m_components(0),
    m_children(0)
{
    Construct();
}

NLActor::NLActor(const NLProperties &prop, NLActor *parent) :
    NLObject(prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0)
{
    Construct();
}

NLActor::NLActor(NLScene *scene, NLActor *parent) :
    NLObject(scene, parent),
    m_renderable(0),
    m_components(0),
    m_children(0)
{
    Construct();
}

NLActor::NLActor(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(scene, prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0)
{
    Construct();
}

NLActor::~NLActor()
{
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

    setObjectName("NLActor");
    SetType(NLObject::Type_Actor);
    Mesa_AllocGLMatrix(&m_matrix);
    Mesa_AllocGLMatrix(&m_globalMatrix);
    Mesa_AllocGLMatrix(&m_normalMatrix);
    m_position = InitPosition;
    m_rotation = InitRotation;
    m_scale = InitScale;
    m_up = /*m_zIsUp ? InitUp_z : */InitUp_y;
    UpdateMatrix();
    UpdateDirection();
}

void NLActor::Init()
{
    if(IsInited())
        return;
    if(m_renderable)
        m_renderable->InitRender();
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
    NLObject::Update(delta);
    if(m_components)
        m_components->Update(delta);
    if(m_children)
        m_children->Update(delta);
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

    //DM(m_globalMatrix);
}

void NLActor::Destroy()
{
    if(!IsInited())
        return;
    if(m_renderable)
    {
        m_renderable->DeinitRender();
        delete m_renderable;
        m_renderable = 0;
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
    NLObject::Destroy();
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

void NLActor::SetParentActor(NLActor *actor)
{
    setParent(actor);
    if(actor)
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
    return m_components->Add(item);
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
        item->Destroy();
        delete item;
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

bool NLActor::RemoveComponent(const NLName &name)
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
    return m_children->Add(actor);
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
        actor->Destroy();
        delete actor;
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

bool NLActor::RemoveChild(const NLName &name)
{
    if(!m_children)
        return false;
    NLActor *actor = GetChild(name);
    return RemoveChild(actor);
}

NLActor * NLActor::GetChild(const NLName &name)
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

NLComponent * NLActor::GetComponent(const NLName &name)
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

NLActor * NLActor::operator[](int index)
{
    return GetChild(index);
}

NLActor * NLActor::operator[](const NLName &name)
{
    return GetChild(name);
}

NLActor & operator+(NLActor &actor, NLComponent *item)
{
    actor.AddComponent(item);
    return actor;
}

NLActor & operator-(NLActor &actor, NLComponent *item)
{
    actor.RemoveComponent(item);
    return actor;
}

void NLActor::SetRenderable(NLRenderable *renderable)
{
    if(m_renderable != renderable)
    {
        if(m_renderable)
            m_renderable->SetActor(0);
        m_renderable = renderable;
        if(m_renderable)
            m_renderable->SetActor(this);
    }
}

NLRenderable * NLActor::Renderable()
{
    return m_renderable;
}

void NLActor::Reset()
{
    m_position = InitPosition;
    m_rotation = InitRotation;
    m_scale = InitScale;
    m_up = /*m_zIsUp ? InitUp_z : */InitUp_y;
    UpdateMatrix();
    UpdateDirection();
    emit positionChanged(m_position);
    emit rotationChanged(m_rotation);
    emit scaleChanged(m_scale);
    if(m_components)
        m_components->Reset();
    if(m_children)
        m_children->Reset();
    NLObject::Reset();
}

void NLActor::SetPosition(const vector3_t &v)
{
    if(vector3_equals(&m_position, &v))
        return;
    m_position = v;
    UpdateMatrix();
    emit positionChanged(m_position);
}

void NLActor::SetRotation(const vector3_t &v)
{
    if(vector3_equals(&m_rotation, &v))
        return;
    m_rotation = v;
    UpdateMatrix();
    UpdateDirection();
    emit rotationChanged(m_rotation);
}

void NLActor::SetScale(const vector3_t &v)
{
    if(vector3_equals(&m_scale, &v))
        return;
    m_scale = v;
    UpdateMatrix();
    emit scaleChanged(m_scale);
}

NLVector3 NLActor::Position() const
{
    return m_position;
}

NLVector3 NLActor::Rotation() const
{
    return m_rotation;
}

NLVector3 NLActor::Scale() const
{
    return m_scale;
}

NLVector3 NLActor::Direction() const
{
    return m_direction;
}

NLVector3 NLActor::Up() const
{
    return m_up;
}

NLVector3 NLActor::Right() const
{
    return m_right;
}

NLActor * NLActor::Move(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return this;

    vector3_moveve(&m_position, &m_right, VECTOR3_X(unit));
    vector3_moveve(&m_position, &m_up, VECTOR3_Y(unit));
    vector3_moveve(&m_position, &m_direction, VECTOR3_Z(unit));

    UpdateMatrix();
    emit positionChanged(m_position);
//    NLDEBUG_VECTOR3(m_position);
//    NLDEBUG_VECTOR3(m_direction);
//    NLDEBUG_VECTOR3(m_right);
//    NLDEBUG_VECTOR3(m_up);
//    NLDEBUG_VECTOR3(unit);
    return this;
}

NLActor * NLActor::MoveOriginal(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return this;

    NLVector3 right = InitRight;
    NLVector3 up = InitUp_y;
    NLVector3 direction = InitDirection_y;
    vector3_moveve(&m_position, &right, VECTOR3_X(unit));
    vector3_moveve(&m_position, &up, VECTOR3_Y(unit));
    vector3_moveve(&m_position, &direction, VECTOR3_Z(unit));

    UpdateMatrix();
    emit positionChanged(m_position);
//    NLDEBUG_VECTOR3(m_position);
//    NLDEBUG_VECTOR3(m_direction);
//    NLDEBUG_VECTOR3(m_right);
//    NLDEBUG_VECTOR3(m_up);
//    NLDEBUG_VECTOR3(unit);
    return this;
}

NLActor * NLActor::Turn(const NLVector3 &v)
{
    if(vector3_iszero(&v))
        return this;
    VECTOR3_X(m_rotation) = NL::clamp_angle(VECTOR3_X(m_rotation) + VECTOR3_X(v));
    VECTOR3_Y(m_rotation) = NL::clamp_angle(VECTOR3_Y(m_rotation) + VECTOR3_Y(v));
    VECTOR3_Z(m_rotation) = NL::clamp_angle(VECTOR3_Z(m_rotation) + VECTOR3_Z(v));
    UpdateMatrix();
    UpdateDirection();
    emit rotationChanged(m_rotation);
    return this;
}

NLActor * NLActor::Zoom(const NLVector3 &v)
{
    //vector3_multiplyve(&m_scale, &v);
    vector3_addve(&m_scale, &v);
    UpdateMatrix();
    emit scaleChanged(m_scale);
    return this;
}

const NLMatrix4 * NLActor::LocalMatrix() const
{
    return &m_matrix;
}

const NLMatrix4 * NLActor::GlobalMatrix() const
{
    return &m_globalMatrix;
}

const NLMatrix4 * NLActor::NormalMatrix() const
{
    return &m_normalMatrix;
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

    float v[] = {0, 0, -1};
    Mesa_glTransform_row(VECTOR3_V(m_direction), v, &m_normalMatrix);

    vector3_crossv(&m_right, &m_direction, &m_up);
    vector3_normalizev(&m_right);
}

void NLActor::UpdateUp()
{
    NLVector3 up = /*m_zIsUp ? InitUp_z : */InitUp_y;
    Mesa_glTransform_row(VECTOR3_V(m_up), VECTOR3_V(up), &m_normalMatrix);
}

void NLActor::InitProperty()
{
    NLObject::InitProperty();
    NLVector3 v = VECTOR3(
                GetProperty_T<float>("x", 0),
                GetProperty_T<float>("y", 0),
                GetProperty_T<float>("z", 0)
                );
    SetPosition(v);
    VECTOR3_X(v) = GetProperty_T<float>("scale_x", 1);
    VECTOR3_Y(v) = GetProperty_T<float>("scale_y", 1);
    VECTOR3_Z(v) = GetProperty_T<float>("scale_z", 1);
    SetScale(v);
    VECTOR3_X(v) = GetProperty_T<float>("pitch", 0);
    VECTOR3_Y(v) = GetProperty_T<float>("yaw", 0);
    VECTOR3_Z(v) = GetProperty_T<float>("roll", 0);
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

bool NLActor::HasChildren() const
{
    return ChildrenCount() > 0;
}

bool NLActor::HasComponent() const
{
    return ComponentCount() > 0;
}

bool NLActor::CanRender() const
{
    return m_renderable != 0;
}
