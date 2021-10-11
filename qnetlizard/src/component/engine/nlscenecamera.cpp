#include "nlscenecamera.h"

#include <QDebug>

#include <GL/gl.h>

#include "nlscene.h"

NLSceneCamera::NLSceneCamera(NLScene *widget)
    : m_scene(widget)
{
    Mesa_AllocGLMatrix(&m_modelviewMatrix);
    Mesa_AllocGLMatrix(&m_projectionMatrix);
    Mesa_AllocGLMatrix(&m_mvpMatrix);
}

NLSceneCamera::~NLSceneCamera()
{
    m_scene = 0;
    Mesa_FreeGLMatrix(&m_modelviewMatrix);
    Mesa_FreeGLMatrix(&m_projectionMatrix);
    Mesa_FreeGLMatrix(&m_mvpMatrix);
}

void NLSceneCamera::SetModelViewMatrix(const NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(&m_modelviewMatrix);
    if(mat)
        Mesa_glLoadMatrix(&m_modelviewMatrix, GL_MATRIXV_M(mat));
#if 0
    qDebug()
             <<    m_modelviewMatrix.m[0]
                <<    m_modelviewMatrix.m[1]
                   <<    m_modelviewMatrix.m[2]
                      <<    m_modelviewMatrix.m[3]
                         << "|"
                         <<    m_modelviewMatrix.m[4]
                            <<    m_modelviewMatrix.m[5]
                               <<    m_modelviewMatrix.m[6]
                                  <<    m_modelviewMatrix.m[7]
                                     << "|"
                                     <<    m_modelviewMatrix.m[8]
                                        <<    m_modelviewMatrix.m[9]
                                           <<    m_modelviewMatrix.m[10]
                                              <<    m_modelviewMatrix.m[11]
                                                 << "|"
                                                 <<    m_modelviewMatrix.m[12]
                                                    <<    m_modelviewMatrix.m[13]
                                                       <<    m_modelviewMatrix.m[14]
                                                          <<    m_modelviewMatrix.m[15]
                                                             << "\n"
            ;
#endif
}

void NLSceneCamera::UpdateMatrix()
{
    UpdateProjectionMatrix(&m_projectionMatrix);
    Mesa_glLoadMatrix(&m_mvpMatrix, GL_MATRIX_M(m_projectionMatrix));
    Mesa_glMultMatrix(&m_mvpMatrix, GL_MATRIX_M(m_modelviewMatrix));
#if 0
    Mesa_glLoadIdentity(m_matrix + ModelviewMatrix);
    Mesa_glLoadMatrix(m_matrix + ModelviewMatrix, m_matrix[ViewMatrix].m);

    if(mat)
    {
        Mesa_glMultMatrix(m_matrix + ModelviewMatrix, mat->m);
    }

    Mesa_glLoadMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ProjectionMatrix].m);
    Mesa_glMultMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ModelviewMatrix].m);
#endif
}

const NLMatrix4 * NLSceneCamera::ModelViewMatrix() const
{
    return &m_modelviewMatrix;
}

const NLMatrix4 * NLSceneCamera::ProjectionMatrix() const
{
    return &m_projectionMatrix;
}

const NLMatrix4 * NLSceneCamera::ModelViewProjectionMatrix() const
{
    return &m_mvpMatrix;
}

void NLSceneCamera::ModelView()
{
    glLoadIdentity();
    // glRotatef(90, 1, 0, 0); // z_is_up
    glLoadMatrixf(GL_MATRIX_M(m_modelviewMatrix));
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
    ModelView();

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
    Mesa_glLoadIdentity(&m_modelviewMatrix);
}
