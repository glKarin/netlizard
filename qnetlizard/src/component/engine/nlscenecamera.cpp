#include "nlscenecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <math.h>

#include "nlscene.h"
#include "nlfuncs.h"

static const NLVector3 InitUp_z = VECTOR3(0, 0, 1);
static const NLVector3 InitUp_y = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_z = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_y = VECTOR3(0, 0, -1);
static const NLVector3 InitLeft = VECTOR3(-1, 0, 0);
static const NLVector3 InitPosition = VECTOR3(0, 0, 0);
static const NLVector3 InitRotation = VECTOR3(/*-9*/0, 0, 0);
static const NLVector3 InitScale = VECTOR3(1, 1, 1);

NLSceneCamera::NLSceneCamera(NLScene *widget)
    : m_scene(widget),
      m_zIsUp(false)
{
    Mesa_AllocGLMatrix(&m_viewMatrix);
    Mesa_AllocGLMatrix(&m_normalMatrix);
    Mesa_AllocGLMatrix(&m_projectionMatrix);
    Mesa_AllocGLMatrix(&m_globalMatrix);
    Mesa_AllocGLMatrix(&m_mvpMatrix);
    m_position = InitPosition;
    m_rotation = InitRotation;
    m_scale = InitScale;
    m_up = m_zIsUp ? InitUp_z : InitUp_y;
    UpdateMatrix();
    UpdateDirection();
}

NLSceneCamera::~NLSceneCamera()
{
    m_scene = 0;
    Mesa_FreeGLMatrix(&m_normalMatrix);
    Mesa_FreeGLMatrix(&m_viewMatrix);
    Mesa_FreeGLMatrix(&m_projectionMatrix);
    Mesa_FreeGLMatrix(&m_globalMatrix);
    Mesa_FreeGLMatrix(&m_mvpMatrix);
}

void NLSceneCamera::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(mat);
}

void NLSceneCamera::UpdateMvpMatrix()
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

void NLSceneCamera::SetPosition(const vector3_s &v)
{
    if(vector3_equals(&m_position, &v))
        return;
    VECTOR3_X(m_position) = VECTOR3_X(v);
    VECTOR3_Y(m_position) = VECTOR3_Y(v);
    VECTOR3_Z(m_position) = VECTOR3_Z(v);
    UpdateMatrix();
}

void NLSceneCamera::SetRotation(const vector3_s &v)
{
    if(vector3_equals(&m_rotation, &v))
        return;
    VECTOR3_X(m_rotation) = VECTOR3_X(v);
    VECTOR3_Y(m_rotation) = VECTOR3_Y(v);
    VECTOR3_Z(m_rotation) = VECTOR3_Z(v);
    UpdateMatrix();
    UpdateDirection();
}

void NLSceneCamera::SetScale(const vector3_s &v)
{
    if(vector3_equals(&m_scale, &v))
        return;
    VECTOR3_X(m_scale) = VECTOR3_X(v);
    VECTOR3_Y(m_scale) = VECTOR3_Y(v);
    VECTOR3_Z(m_scale) = VECTOR3_Z(v);
    UpdateMatrix();
}

NLVector3 NLSceneCamera::Position() const
{
    return m_position;
}

NLVector3 NLSceneCamera::Rotation() const
{
    return m_rotation;
}

NLVector3 NLSceneCamera::Scale() const
{
    return m_scale;
}

NLVector3 NLSceneCamera::Direction() const
{
    return m_direction;
}

NLVector3 NLSceneCamera::Up() const
{
    return m_up;
}

NLSceneCamera * NLSceneCamera::Move(const NLVector3 &unit)
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
    //qDebug() << "position: " << m_position.v[0] << m_position.v[1] << m_position.v[2];
    return this;
}

NLSceneCamera * NLSceneCamera::Turn(const NLVector3 &v)
{
    if(vector3_iszero(&v))
        return this;
    VECTOR3_X(m_rotation) = NL::clamp_angle(VECTOR3_X(m_rotation) + VECTOR3_X(v));
    VECTOR3_Y(m_rotation) = NL::clamp_angle(VECTOR3_Y(m_rotation) + VECTOR3_Y(v));
    VECTOR3_Z(m_rotation) = NL::clamp_angle(VECTOR3_Z(m_rotation) + VECTOR3_Z(v));
    UpdateMatrix();
    UpdateDirection();
    return this;
}

NLSceneCamera * NLSceneCamera::Zoom(const NLVector3 &v)
{
    //vector3_multiplyv_self(&m_scale, &v);
    vector3_addv_self(&m_scale, &v);
    UpdateMatrix();
    return this;
}

const NLMatrix4 * NLSceneCamera::ViewMatrix() const
{
    return &m_viewMatrix;
}

const NLMatrix4 * NLSceneCamera::ProjectionMatrix() const
{
    return &m_projectionMatrix;
}

const NLMatrix4 * NLSceneCamera::ViewProjectionMatrix() const
{
    return &m_mvpMatrix;
}

const NLMatrix4 * NLSceneCamera::NormalMatrix() const
{
    return &m_normalMatrix;
}

const NLMatrix4 * NLSceneCamera::GlobalMatrix() const
{
    return &m_globalMatrix;
}

void NLSceneCamera::View()
{
    glLoadIdentity();
    // glRotatef(90, 1, 0, 0); // z_is_up
    glLoadMatrixf(GL_MATRIX_M(m_globalMatrix));
    glMultMatrixf(GL_MATRIX_M(m_viewMatrix));
}

void NLSceneCamera::Render()
{
    GLint mode;
    GLfloat mm[16];
    GLfloat pm[16];
    glGetIntegerv(GL_MATRIX_MODE, &mode);
    glGetFloatv(GL_MODELVIEW_MATRIX, mm);
    glGetFloatv(GL_PROJECTION_MATRIX, pm);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Projection();

    glMatrixMode(GL_MODELVIEW);
    View();

    glPushMatrix();
    {
        m_scene->Render();
    }
    glPopMatrix();

    glLoadMatrixf(mm);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(pm);
    glMatrixMode(mode);
}

NLScene * NLSceneCamera::Scene()
{
    return m_scene;
}

void NLSceneCamera::SetScene(NLScene *scene)
{
    m_scene = scene;
}

void NLSceneCamera::Reset()
{
    SetGlobalMatrix(0);
    m_position = InitPosition;
    m_rotation = InitRotation;
    m_scale = InitScale;
    m_up = InitUp_z;
    UpdateMatrix();
    UpdateDirection();
}

void NLSceneCamera::SetGlobalMatrix(const NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(&m_globalMatrix);
    if(mat)
        Mesa_glLoadMatrix(&m_globalMatrix, GL_MATRIXV_M(mat));
}

void NLSceneCamera::UpdateMatrix()
{
    UpdateViewMatrix();
    UpdateMvpMatrix();
}

void NLSceneCamera::UpdateViewMatrix()
{
    Mesa_glLoadIdentity(&m_viewMatrix);

#if 0
    if(m_zIsUp)
        Mesa_glRotate(&m_viewMatrix, -90, 1, 0, 0); // z_is_up
#endif

    Mesa_glRotate(&m_viewMatrix, VECTOR3_X(m_rotation), 1, 0, 0);
#if 0
    if(m_zIsUp)
    {
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Z(m_rotation), 0, 1, 0); // roll
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Y(m_rotation), 0, 0, 1); // z_is_up
    }
    else
#endif
    {
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Z(m_rotation), 0, 0, 1); // roll
        Mesa_glRotate(&m_viewMatrix, VECTOR3_Y(m_rotation), 0, 1, 0);
    }

    Mesa_glTranslate(&m_viewMatrix,
                -VECTOR3_X(m_position),
                -VECTOR3_Y(m_position),
                -VECTOR3_Z(m_position)
                );

    if(m_zIsUp)
        Mesa_glRotate(&m_viewMatrix, -90, 1, 0, 0); // z_is_up

    //Mesa_glScale(&m_viewMatrix, VECTOR3_X(m_scale), VECTOR3_Y(m_scale), VECTOR3_Z(m_scale));

    UpdateNormalMatrix();
}

void NLSceneCamera::UpdateNormalMatrix()
{
    NL::cale_normal_matrix(m_normalMatrix, m_viewMatrix);
}

void NLSceneCamera::UpdateDirection()
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

void NLSceneCamera::SetZIsUp(bool b)
{
    if(m_zIsUp != b)
    {
        m_zIsUp = b;
        m_up = m_zIsUp ? InitUp_z : InitUp_y;
        UpdateMatrix();
        UpdateDirection();
    }
}

