#include "nlsceneorthocamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

#define NLSCENEORTHOCAMERA_DEFAULT_FACTORY 1
#define NLSCENEORTHOCAMERA_DEFAULT_VALUE(x) ((x) * NLSCENEORTHOCAMERA_DEFAULT_FACTORY)

#define NLSCENEORTHOCAMERA_DEFAULT_LEFT NLSCENEORTHOCAMERA_DEFAULT_VALUE(-1)
#define NLSCENEORTHOCAMERA_DEFAULT_RIGHT NLSCENEORTHOCAMERA_DEFAULT_VALUE(1)
#define NLSCENEORTHOCAMERA_DEFAULT_BOTTOM NLSCENEORTHOCAMERA_DEFAULT_VALUE(-1)
#define NLSCENEORTHOCAMERA_DEFAULT_TOP NLSCENEORTHOCAMERA_DEFAULT_VALUE(1)
#define NLSCENEORTHOCAMERA_DEFAULT_Z_NEAR NLSCENEORTHOCAMERA_DEFAULT_VALUE(-1)
#define NLSCENEORTHOCAMERA_DEFAULT_Z_FAR NLSCENEORTHOCAMERA_DEFAULT_VALUE(1)

NLSceneOrthoCamera::NLSceneOrthoCamera(NLScene *scene)
    : NLSceneCameraBase(scene),
      //m_align(Qt::AlignLeft | Qt::AlignBottom),
      m_align(Qt::AlignCenter),
      m_left(NLSCENEORTHOCAMERA_DEFAULT_LEFT),
      m_right(NLSCENEORTHOCAMERA_DEFAULT_RIGHT),
      m_bottom(NLSCENEORTHOCAMERA_DEFAULT_BOTTOM),
      m_top(NLSCENEORTHOCAMERA_DEFAULT_TOP),
      m_zNear(NLSCENEORTHOCAMERA_DEFAULT_Z_NEAR),
      m_zFar(NLSCENEORTHOCAMERA_DEFAULT_Z_FAR)
{
    UpdateMatrix();
}

NLSceneOrthoCamera::~NLSceneOrthoCamera()
{

}

void NLSceneOrthoCamera::Update(float width, float height)
{
    GLfloat ortho[4] = {0};
    if(m_align & Qt::AlignLeft)
    {
        ortho[1] = width;
    }
    else
    {
        ortho[0] = -width / 2;
        ortho[1] = width / 2;
    }

    if(m_align & Qt::AlignBottom)
    {
        ortho[3] = height;
    }
    else if(m_align & Qt::AlignTop)
    {
        ortho[2] = height;
        ortho[3] = 0;
    }
    else
    {
        ortho[2] = -height / 2;
        ortho[3] = height / 2;
    }
    Set(ortho[0], ortho[1], ortho[2], ortho[3], m_zNear, m_zFar);
}

void NLSceneOrthoCamera::SetLeft(float left)
{
    if(m_left != left)
    {
        m_left = left;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::SetRight(float right)
{
    if(m_right != right)
    {
        m_right = right;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::SetTop(float top)
{
    if(m_top != top)
    {
        m_top = top;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::SetBottom(float bottom)
{
    if(m_bottom != bottom)
    {
        m_bottom = bottom;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::SetZNear(float near)
{
    if(m_zNear != near)
    {
        m_zNear = near;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::SetZFar(float far)
{
    if(m_zFar != far)
    {
        m_zFar = far;
        UpdateMatrix();
    }
}

void NLSceneOrthoCamera::Set2D()
{
    bool b = false;
    if(m_zNear != -1)
    {
        m_zNear = -1;
        b = true;
    }
    if(m_zFar != 1)
    {
        m_zFar = 1;
        b = true;
    }
    if(b)
        UpdateMatrix();
}

void NLSceneOrthoCamera::Set(float left, float right, float bottom, float top, float near, float far)
{
    bool b = false;
    if(m_left != left)
    {
        m_left = left;
        b = true;
    }
    if(m_right != right)
    {
        m_right = right;
        b = true;
    }
    if(m_bottom != bottom)
    {
        m_bottom = bottom;
        b = true;
    }
    if(m_top != top)
    {
        m_top = top;
        b = true;
    }
    if(m_zNear != near)
    {
        m_zNear = near;
        b = true;
    }
    if(m_zFar != far)
    {
        m_zFar = far;
        b = true;
    }
    if(b)
        UpdateMatrix();
}

void NLSceneOrthoCamera::Set2D(float left, float right, float bottom, float top)
{
    Set(left, right, bottom, top);
}

void NLSceneOrthoCamera::Projection()
{
    //glOrtho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
    //glMultMatrixf(GL_MATRIXV_M(ProjectionMatrix()));
    NLSceneCameraBase::Projection();
}

void NLSceneOrthoCamera::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_glOrtho(mat, m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
}

void NLSceneOrthoCamera::SetAlignment(Qt::Alignment align)
{
    Qt::Alignment a = align;
    a = a & (~Qt::AlignRight);
    if(m_align != a)
    {
        m_align = (Qt::Alignment)a;
    }
}

void NLSceneOrthoCamera::Reset()
{
    NLSceneCameraBase::Reset();
    m_align = Qt::AlignCenter;
    m_left = NLSCENEORTHOCAMERA_DEFAULT_LEFT;
    m_right = NLSCENEORTHOCAMERA_DEFAULT_RIGHT;
    m_bottom = NLSCENEORTHOCAMERA_DEFAULT_BOTTOM;
    m_top = NLSCENEORTHOCAMERA_DEFAULT_TOP;
    m_zNear = NLSCENEORTHOCAMERA_DEFAULT_Z_NEAR;
    m_zFar = NLSCENEORTHOCAMERA_DEFAULT_Z_FAR;
    UpdateMatrix();
}
