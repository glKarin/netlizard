#include "nlscenecamerabase.h"

#include <QDebug>

#include <GL/gl.h>
#include <math.h>

#include "nlscene.h"
#include "nlactor.h"
#include "nlactorcontainer.h"
#include "nlmath.h"
#include "qdef.h"

NLSceneCameraBase::NLSceneCameraChangedNotify::NLSceneCameraChangedNotify(int type)
    : m_type(type)
{

}

NLSceneCameraBase::NLSceneCameraChangedNotify::~NLSceneCameraChangedNotify()
{
    DEBUG_DESTROY(NLSceneCameraChangedNotify)
}

void NLSceneCameraBase::NLSceneCameraChangedNotify::SetType(int type)
{
    if(m_type != type)
        m_type = type;
}

NLSceneCameraBase::NLSceneCameraBase(NLScene *widget)
    : m_scene(widget),
      m_zIsUp(false),
      m_enabled(true),
      m_notifyFunc(0)
{
    Mesa_AllocGLMatrix(&m_viewMatrix);
    Mesa_AllocGLMatrix(&m_normalMatrix);
    Mesa_AllocGLMatrix(&m_projectionMatrix);
    Mesa_AllocGLMatrix(&m_globalMatrix);
    Mesa_AllocGLMatrix(&m_mvpMatrix);
    Mesa_AllocGLMatrix(&m_initialFixedViewMatrix);
    m_position = NL::Init_Position;
    m_rotation = NL::Init_Rotation;
    m_scale = NL::Init_Scale;
    m_up = m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y;
    if(m_zIsUp)
        Mesa_glRotate(&m_initialFixedViewMatrix, -90, 1, 0, 0);
    UpdateMatrix();
    UpdateDirection();
}

NLSceneCameraBase::~NLSceneCameraBase()
{
    m_scene = 0;
    PTR_DELETE(NLSceneCameraBase::NLSceneCameraChangedNotify, m_notifyFunc)
    Mesa_FreeGLMatrix(&m_normalMatrix);
    Mesa_FreeGLMatrix(&m_viewMatrix);
    Mesa_FreeGLMatrix(&m_projectionMatrix);
    Mesa_FreeGLMatrix(&m_globalMatrix);
    Mesa_FreeGLMatrix(&m_mvpMatrix);
    Mesa_FreeGLMatrix(&m_initialFixedViewMatrix);
}

void NLSceneCameraBase::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(mat);
}

void NLSceneCameraBase::UpdateMvpMatrix()
{
    UpdateProjectionMatrix(&m_projectionMatrix);
    Mesa_glLoadMatrix(&m_mvpMatrix, GL_MATRIX_M(m_projectionMatrix));
    Mesa_glMultMatrix(&m_mvpMatrix, GL_MATRIX_M(m_globalMatrix));
    Mesa_glMultMatrix(&m_mvpMatrix, GL_MATRIX_M(m_viewMatrix));
#if 0
    Mesa_glLoadIdentity(m_matrix + ViewMatrix);
    Mesa_glLoadMatrix(m_matrix + ViewMatrix, m_matrix[ViewMatrix].m);

    if(mat)
    {
        Mesa_glMultMatrix(m_matrix + ViewMatrix, mat->m);
    }

    Mesa_glLoadMatrix(m_matrix + ViewProjectionMatrix, m_matrix[ProjectionMatrix].m);
    Mesa_glMultMatrix(m_matrix + ViewProjectionMatrix, m_matrix[ViewMatrix].m);
#endif
}

void NLSceneCameraBase::Projection()
{
    glMultMatrixf(GL_MATRIX_M(m_projectionMatrix));
}

void NLSceneCameraBase::SetPosition(const NLVector3 &v)
{
    if(vector3_equals(&m_position, &v))
        return;
    m_position = v;
    UpdateMatrix();
    ValueChanged("position", NLProperty::fromValue<NLVector3>(m_position));
}

void NLSceneCameraBase::SetRotation(const NLVector3 &v)
{
    if(vector3_equals(&m_rotation, &v))
        return;
    m_rotation = v;
    UpdateMatrix();
    UpdateDirection();
    ValueChanged("rotation", NLProperty::fromValue<NLVector3>(m_rotation));
}

void NLSceneCameraBase::SetScale(const NLVector3 &v)
{
    if(vector3_equals(&m_scale, &v))
        return;
    m_scale = v;
    UpdateMatrix();
    ValueChanged("scale", NLProperty::fromValue<NLVector3>(m_scale));
}

NLSceneCameraBase * NLSceneCameraBase::Move(const NLVector3 &unit)
{
    if(vector3_iszero(&unit))
        return this;

    NLVector3 pos = m_position;
    vector3_moveve(&pos, &m_right_x_positive, VECTOR3_X(unit));
    vector3_moveve(&pos, &m_up, VECTOR3_Y(unit));
    vector3_moveve(&pos, &m_direction, VECTOR3_Z(unit));

    SetPosition(pos);
    return this;
}

NLSceneCameraBase * NLSceneCameraBase::Turn(const NLVector3 &v)
{
    if(vector3_iszero(&v))
        return this;

    NLVector3 rot = m_rotation;
    VECTOR3_X(rot) = NL::clamp_angle(VECTOR3_X(rot) + VECTOR3_X(v));
    VECTOR3_Y(rot) = NL::clamp_angle(VECTOR3_Y(rot) + VECTOR3_Y(v));
    VECTOR3_Z(rot) = NL::clamp_angle(VECTOR3_Z(rot) + VECTOR3_Z(v));
    SetRotation(rot);
    return this;
}

NLSceneCameraBase * NLSceneCameraBase::Zoom(const NLVector3 &v)
{
    //vector3_multiplyve(&m_scale, &v);
    vector3_addve(&m_scale, &v);
    UpdateMatrix();
    ValueChanged("scale", NLProperty::fromValue<NLVector3>(m_scale));
    return this;
}

void NLSceneCameraBase::View()
{
    glLoadIdentity();
    glLoadMatrixf(GL_MATRIX_M(m_globalMatrix));
    glMultMatrixf(GL_MATRIX_M(m_viewMatrix));
}

NLSceneCameraBase::GL_matrix_status NLSceneCameraBase::BeginRender()
{
    NLSceneCameraBase::GL_matrix_status status;
    glGetIntegerv(GL_MATRIX_MODE, &status.mode);
    glGetFloatv(GL_MODELVIEW_MATRIX, status.modelview_matrix);
    glGetFloatv(GL_PROJECTION_MATRIX, status.projection_matrix);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Projection();

    glMatrixMode(GL_MODELVIEW);
    View();

    glPushMatrix();
    return status;
}

void NLSceneCameraBase::EndRender(const NLSceneCameraBase::GL_matrix_status &status)
{
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(status.modelview_matrix);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(status.projection_matrix);
    glMatrixMode(status.mode);
}

#define CAMERA_RENDER(x) \
{ \
    NLSceneCameraBase::GL_matrix_status status = BeginRender(); \
    { \
    x; \
    } \
    EndRender(status); \
}

void NLSceneCameraBase::Render()
{
    if(!m_enabled)
        return;
    if(!m_scene)
        return;
    CAMERA_RENDER(m_scene->Render())
}

void NLSceneCameraBase::Render(NLScene *scene)
{
    if(!m_enabled)
        return;
    if(!scene)
        return;
    CAMERA_RENDER(scene->Render())
}

void NLSceneCameraBase::Render(NLActor *actor)
{
    if(!m_enabled)
        return;
    if(!actor)
        return;
    CAMERA_RENDER(actor->Render())
}

void NLSceneCameraBase::Render(NLActorContainer *actors)
{
    if(!m_enabled)
        return;
    if(!actors)
        return;
    CAMERA_RENDER(actors->Render())
}

void NLSceneCameraBase::Render(NLSceneCameraBase::NLSceneCameraRenderFunc func)
{
    if(!m_enabled)
        return;
    if(!func)
        return;
    CAMERA_RENDER(func(GL_MATRIX_M(m_viewMatrix), GL_MATRIX_M(m_projectionMatrix), GL_MATRIX_M(m_mvpMatrix)))
}

void NLSceneCameraBase::SetScene(NLScene *scene)
{
    if(m_scene != scene)
        m_scene = scene;
}

void NLSceneCameraBase::Reset()
{
    SetGlobalMatrix(0);
    m_position = NL::Init_Position;
    ValueChanged("position", NLProperty::fromValue<NLVector3>(m_position));
    m_rotation = NL::Init_Rotation;
    ValueChanged("rotation", NLProperty::fromValue<NLVector3>(m_rotation));
    m_scale = NL::Init_Scale;
    ValueChanged("scale", NLProperty::fromValue<NLVector3>(m_scale));
    m_up = m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y;
    if(m_zIsUp)
    {
        Mesa_glLoadIdentity(&m_initialFixedViewMatrix);
        Mesa_glRotate(&m_initialFixedViewMatrix, -90, 1, 0, 0);
    }
    UpdateMatrix();
    UpdateDirection();
}

void NLSceneCameraBase::SetGlobalMatrix(const NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(&m_globalMatrix);
    if(mat)
        Mesa_glLoadMatrix(&m_globalMatrix, GL_MATRIXV_M(mat));
}

void NLSceneCameraBase::UpdateMatrix()
{
    //NLDEBUG_VECTOR3(m_position);
    UpdateViewMatrix();
    UpdateMvpMatrix();
}

void NLSceneCameraBase::UpdateViewMatrix()
{
    Mesa_glLoadIdentity(&m_viewMatrix);

    Mesa_glRotate(&m_viewMatrix, VECTOR3_X(m_rotation), 1, 0, 0);
    {
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Z(m_rotation), 0, 0, 1); // roll
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Y(m_rotation), 0, 1, 0);
    }

    Mesa_glTranslate(&m_viewMatrix,
                -VECTOR3_X(m_position),
                -VECTOR3_Y(m_position),
                -VECTOR3_Z(m_position)
                );

#if 0
    if(m_zIsUp)
        Mesa_glRotate(&m_viewMatrix, -90, 1, 0, 0); // z_is_up
#else
        Mesa_glMultMatrix(&m_viewMatrix, GL_MATRIX_M(m_initialFixedViewMatrix));
#endif

    //Mesa_glScale(&m_viewMatrix, VECTOR3_X(m_scale), VECTOR3_Y(m_scale), VECTOR3_Z(m_scale));

    UpdateNormalMatrix();
}

void NLSceneCameraBase::UpdateNormalMatrix()
{
    NL::cale_normal_matrix(m_normalMatrix, m_viewMatrix);
}

void NLSceneCameraBase::UpdateDirection()
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

    static const float v[] = {0, 0, -1};
    Mesa_glTransform_row(VECTOR3_V(m_direction), v, &m_normalMatrix);

    vector3_crossv(&m_right_x_positive, &m_direction, &m_up);
    vector3_normalizev(&m_right_x_positive);
}

void NLSceneCameraBase::SetZIsUp(bool b)
{
    if(m_zIsUp != b)
    {
        m_zIsUp = b;
        m_up = m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y;

        Mesa_glLoadIdentity(&m_initialFixedViewMatrix);
        if(m_zIsUp)
            Mesa_glRotate(&m_initialFixedViewMatrix, -90, 1, 0, 0); // z_is_up

        UpdateMatrix();
        UpdateDirection();
        PropertyChanged("zIsUp", NLProperty::fromValue(m_zIsUp));
    }
}

bool NLSceneCameraBase::ZIsUp() const
{
    return m_zIsUp;
}

void NLSceneCameraBase::SetEnabled(bool b)
{
    if(m_enabled != b)
    {
        m_enabled = b;
        PropertyChanged("enabled", NLProperty::fromValue(m_enabled));
    }
}

void NLSceneCameraBase::SetChangedNotifyFunc(NLSceneCameraBase::NLSceneCameraChangedNotify *func)
{
    PTR_DELETE(NLSceneCameraBase::NLSceneCameraChangedNotify, m_notifyFunc)
    if(m_notifyFunc != func)
        m_notifyFunc = func;
}

void NLSceneCameraBase::PropertyChanged(const QString &name, const NLProperty &value)
{
    if(m_notifyFunc && (m_notifyFunc->m_type & NLSceneCameraBase::NLSceneCameraChangedNotify::Notify_PropertyChanged))
        m_notifyFunc->PropertyChanged(name, value);
}

void NLSceneCameraBase::ValueChanged(const QString &name, const NLProperty &value)
{
    if(m_notifyFunc && (m_notifyFunc->m_type & NLSceneCameraBase::NLSceneCameraChangedNotify::Notify_ValueChanged))
        m_notifyFunc->ValueChanged(name, value);
}

