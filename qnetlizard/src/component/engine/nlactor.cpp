#include "nlactor.h"

#include <QDebug>

#include <math.h>

#include "qdef.h"
#include "nlrenderable.h"
#include "nlactorcontainer.h"
#include "nlcomponentcontainer.h"
#include "nlcomponent.h"

#define DM(m_modelviewMatrix)  \
qDebug()\
         <<    m_modelviewMatrix.m[0]\
            <<    m_modelviewMatrix.m[1]\
               <<    m_modelviewMatrix.m[2]\
                  <<    m_modelviewMatrix.m[3]\
                     << "|"\
                     <<    m_modelviewMatrix.m[4]\
                        <<    m_modelviewMatrix.m[5]\
                           <<    m_modelviewMatrix.m[6]\
                              <<    m_modelviewMatrix.m[7]\
                                 << "|"\
                                 <<    m_modelviewMatrix.m[8]\
                                    <<    m_modelviewMatrix.m[9]\
                                       <<    m_modelviewMatrix.m[10]\
                                          <<    m_modelviewMatrix.m[11]\
                                             << "|"\
                                             <<    m_modelviewMatrix.m[12]\
                                                <<    m_modelviewMatrix.m[13]\
                                                   <<    m_modelviewMatrix.m[14]\
                                                      <<    m_modelviewMatrix.m[15]\
                                                         << "\n"\
    ;

#define ator(a) ((double)(a) / 180.0 * M_PI)
#define rtoa(r) ((double)(r) / M_PI * 180.0)

static const NLVector3 InitUp_z = VECTOR3(0, 0, 1);
static const NLVector3 InitUp_y = VECTOR3(0, 0, -1);
static const NLVector3 InitDirection_z = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_y = VECTOR3(0, 1, 0);
static const NLVector3 InitLeft = VECTOR3(-1, 0, 0);
static const NLVector3 InitPosition = VECTOR3(0, 0, 0);
static const NLVector3 InitRotation = VECTOR3(-90, 0, 0);
static const NLVector3 InitScale = VECTOR3(1, 1, 1);

NLActor::NLActor(NLActor *parent) :
    NLObject(parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_zIsUp(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLActor::NLActor(const NLPropperties &prop, NLActor *parent) :
    NLObject(prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_zIsUp(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLActor::NLActor(NLScene *scene, NLActor *parent) :
    NLObject(scene, parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_zIsUp(false)
{
    Construct();
}

NLActor::NLActor(NLScene *scene, const NLPropperties &prop, NLActor *parent) :
    NLObject(scene, prop, parent),
    m_renderable(0),
    m_components(0),
    m_children(0),
    m_zIsUp(false)
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
    m_zIsUp = true;
    setObjectName("NLActor");
    SetType(NLObject::Type_Actor);
    Mesa_AllocGLMatrix(&m_matrix);
    Mesa_AllocGLMatrix(&m_globalMatrix);
    Mesa_AllocGLMatrix(&m_normalMatrix);
    m_position = InitPosition;
    m_rotation = InitRotation;
    m_scale = InitScale;
    m_up = InitUp_z;
    UpdateMatrix();
    UpdateDirection();
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
    {
        m_renderable->DeinitRender();
        delete m_renderable;
        m_renderable = 0;
    }
    if(m_components)
    {
        m_components->Destroy();
        m_components->deleteLater();
        m_components = 0;
    }
    NLObject::Destroy();
}

bool NLActor::keyev(int key, bool pressed, int modifier)
{
    if(m_components)
        return m_components->KeyEventHandler(key, pressed, modifier);
    return false;
}

bool NLActor::mouseev(int mouse, bool pressed, int x, int y, int modifier)
{
    if(m_components)
        return m_components->MouseEventHandler(mouse, pressed, x, y, modifier);
    return false;
}

bool NLActor::motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    if(m_components)
        return m_components->MouseMotionHandler(mouse, pressed, x, y, oldx, oldy, modifier);
    return false;
}

bool NLActor::wheelev(int orientation, int delta, int x, int y, int modifier)
{
    if(m_components)
        return m_components->WheelHandler(orientation, delta, x, y, modifier);
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
//    if(m_renderable)
//        m_renderable->setParent(this);
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
    m_up = InitUp_z;
    UpdateMatrix();
    UpdateDirection();
    emit positionChanged(m_position);
    emit rotationChanged(m_rotation);
    emit scaleChanged(m_scale);
}

void NLActor::SetPosition(const vector3_s &v)
{
    if(vector3_equals(&m_position, &v))
        return;
    VECTOR3_X(m_position) = VECTOR3_X(v);
    VECTOR3_Y(m_position) = VECTOR3_Y(v);
    VECTOR3_Z(m_position) = VECTOR3_Z(v);
    UpdateMatrix();
    emit positionChanged(m_position);
}

void NLActor::SetRotation(const vector3_s &v)
{
    if(vector3_equals(&m_rotation, &v))
        return;
    VECTOR3_X(m_rotation) = VECTOR3_X(v);
    VECTOR3_Y(m_rotation) = VECTOR3_Y(v);
    VECTOR3_Z(m_rotation) = VECTOR3_Z(v);
    UpdateMatrix();
    UpdateDirection();
    emit rotationChanged(m_rotation);
}

void NLActor::SetScale(const vector3_s &v)
{
    if(vector3_equals(&m_scale, &v))
        return;
    VECTOR3_X(m_scale) = VECTOR3_X(v);
    VECTOR3_Y(m_scale) = VECTOR3_Y(v);
    VECTOR3_Z(m_scale) = VECTOR3_Z(v);
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

NLActor * NLActor::Move(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return this;

    NLVector3 v;
    v = vector3_scale(&m_left, VECTOR3_X(unit));
    vector3_addv_self(&m_position, &v);

    v = vector3_scale(&m_up, VECTOR3_Y(unit));
    vector3_addv_self(&m_position, &v);

    v = vector3_scale(&m_direction, VECTOR3_Z(unit));
    vector3_addv_self(&m_position, &v);
    UpdateMatrix();
    emit positionChanged(m_position);
    return this;
}

NLActor * NLActor::Turn(const NLVector3 &v)
{
    if(vector3_iszero(&v))
        return this;
    VECTOR3_X(m_rotation) = ClampAngle(VECTOR3_X(m_rotation) + VECTOR3_X(v));
    VECTOR3_Y(m_rotation) = ClampAngle(VECTOR3_Y(m_rotation) + VECTOR3_Y(v));
    VECTOR3_Z(m_rotation) = ClampAngle(VECTOR3_Z(m_rotation) + VECTOR3_Z(v));
    UpdateMatrix();
    UpdateDirection();
    emit rotationChanged(m_rotation);
    return this;
}

NLActor * NLActor::Zoom(const NLVector3 &v)
{
    vector3_multiplyv_self(&m_scale, &v);
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
    if(m_zIsUp)
        Mesa_glRotate(&m_matrix, 90, 1, 0, 0); // z_is_up

    Mesa_glScale(&m_matrix, VECTOR3_X(m_scale), VECTOR3_Y(m_scale), VECTOR3_Z(m_scale));

    Mesa_glRotate(&m_matrix, VECTOR3_X(m_rotation), 1, 0, 0);
    if(m_zIsUp)
    {
        Mesa_glRotate(&m_matrix, VECTOR3_Z(m_rotation), 0, -1, 0); // roll
        Mesa_glRotate(&m_matrix, VECTOR3_Y(m_rotation), 0, 0, 1); // z_is_up
    }
    else
    {
        Mesa_glRotate(&m_matrix, VECTOR3_Z(m_rotation), 0, 0, 1); // roll
        Mesa_glRotate(&m_matrix, VECTOR3_Y(m_rotation), 0, 1, 0);
    }

    Mesa_glTranslate(&m_matrix,
                -VECTOR3_X(m_position),
                -VECTOR3_Y(m_position),
                -VECTOR3_Z(m_position)
                );

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
    float m[16] = {
        GL_MATRIX_M(m_matrix)[0], GL_MATRIX_M(m_matrix)[1], GL_MATRIX_M(m_matrix)[2], 0.0,
        GL_MATRIX_M(m_matrix)[4], GL_MATRIX_M(m_matrix)[5], GL_MATRIX_M(m_matrix)[6], 0.0,
        GL_MATRIX_M(m_matrix)[8], GL_MATRIX_M(m_matrix)[9], GL_MATRIX_M(m_matrix)[10], 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    Mesa_glLoadIdentity(&m_normalMatrix);
    Mesa_NormalMatrix(&m_normalMatrix, m);
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
//    float xrad, yrad;
//    float x, y, z;
//    float xz;

//    xrad = ator(VECTOR3_X(m_rotation));
//    yrad = ator(VECTOR3_Y(m_rotation));
//    y = sin(xrad);
//    xz = cos(xrad);
//    x = -sin(yrad) * xz;
//    z = cos(yrad) * xz;
//    VECTOR3_X(m_direction) = -x;
//    VECTOR3_Y(m_direction) = -y;
//    VECTOR3_Z(m_direction) = -z;
//    vector3_normalizev(&m_direction);

    float v[] = {0, 0, -1};
    Mesa_glTransform(VECTOR3_V(m_direction), v, &m_normalMatrix);

    vector3_crossv(&m_left, &m_direction, &m_up);
//    qDebug() << "ANGLE: " << QString::number(m_rotation.v[0]) << QString::number(m_rotation.v[1]) <<  QString::number(m_rotation.v[2]);
//    qDebug() << "left: " << QString::number(m_left.v[0]) << QString::number(m_left.v[1]) <<  QString::number(m_left.v[2]);
//    qDebug() << "up: " << QString::number(m_up.v[0]) << QString::number(m_up.v[1]) <<  QString::number(m_up.v[2]);
//    qDebug() << "dir: " << QString::number(m_direction.v[0]) << QString::number(m_direction.v[1]) <<  QString::number(m_direction.v[2]);
//    qDebug() << "\n";
}

float NLActor::ClampAngle(float angle)
{
    int i = (int)angle;
    float f = angle - i;
    float r = 0.0;
    if(angle > 360)
    {
        r = i % 360 + f;
    }
    else if(angle < 0)
    {
        r = 360 - fabs(i % 360 + f);
    }
    else
        r = angle;
    if(r == 360.0)
        r = 0.0;
    qDebug() <<angle <<r;
    return r;
}
