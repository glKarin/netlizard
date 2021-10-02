#include "MapWidget.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"

MapWidget::MapWidget(QWidget *parent)
    : GLWidget(parent),
      m_model(0)
{
    setObjectName("MapWidget");
    SetEnableDefaultWheelHandler(false);
}

MapWidget::~MapWidget()
{
    DEBUG_DESTROY_Q;
}

void MapWidget::Init()
{
    GLWidget::Init();
}

void MapWidget::Update(float delta)
{
    //qDebug() << delta;
    GLWidget::Update(delta);
}

void MapWidget::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderSky();
    //Render3D(45, width(), height(), 0.01, 999999);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width() / height(), 0.01, 999999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    {
        glRotatef(VECTOR3_X(m_cam.rotation), 1, 0, 0);
        glRotatef(VECTOR3_Y(m_cam.rotation), 0, 1, 0);
        glTranslatef(
                    -VECTOR3_X(m_cam.position),
                    -VECTOR3_Y(m_cam.position),
                    -VECTOR3_Z(m_cam.position)
                    );
        glRotatef(-90, 1, 0, 0);
//        gluLookAt(
//                    VECTOR3_X(m_cam.position),
//                    VECTOR3_Y(m_cam.position),
//                    VECTOR3_Z(m_cam.position),
//                    VECTOR3_X(forward),
//                    VECTOR3_Y(forward),
//                    VECTOR3_Z(forward),
//                    VECTOR3_X(m_cam.up),
//                    VECTOR3_Y(m_cam.up),
//                    VECTOR3_Z(m_cam.up)
//                    );
        NETLizard_RenderGL3DModel(m_model);
    }
    glPopMatrix();

    glFlush();
}

void MapWidget::Deinit()
{
    GLWidget::Deinit();
    Reset();
}

bool MapWidget::LoadFile(const QString &file, const QString &resourcePath, int game, int level)
{
    Reset();
    QByteArray ba1 = file.toLocal8Bit();
    const char *path = ba1.data();
    QByteArray ba2 = resourcePath.toLocal8Bit();
    const char *resc_path = ba2.data();
    m_model = (GL_NETLizard_3D_Model *)malloc(sizeof(GL_NETLizard_3D_Model));
    memset(m_model, 0, sizeof(GL_NETLizard_3D_Model));
    GLboolean b = GL_FALSE;
    qDebug() << "Load game: " << path << resc_path;
    switch(game)
    {
    case NL_CONTR_TERRORISM_3D:
        b = NETLizard_ReadGLCT3DMapModelFile(path, level, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
        b = NETLizard_ReadGLCT3DEp2MapModelFile(path, level, resc_path, m_model);
        break;
    case NL_ARMY_RANGER_3D:
        b = NETLizard_ReadGLSpecnaz3DMapModelFile(path, level, resc_path, m_model);
        break;
    default:
        qDebug() << "Unsupport game";
        break;
    }
    qDebug() << b;
    if(!b)
    {
        free(m_model);
        m_model = 0;
        return false;
    }

    vector3_s startPos = {{m_model->start_pos[0], m_model->start_pos[2], -m_model->start_pos[1]}};
    vector3_s startRotate = {{m_model->start_angle[0] + 90.0, m_model->start_angle[1] - 180.0, 0}};
    initcam(&m_cam, &startPos, &startRotate);
    return true;
}

void MapWidget::Reset()
{
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }
    SetHideMouse(false);
}

void MapWidget::RenderSky()
{
    if(!m_model)
        return;
    if(!m_model->bg_tex)
        return;

    GLfloat w = width();
    GLfloat h = height();
    const GLfloat _Vertex[] = {
        0.0, 0.0,
        w, 0.0,
        0.0, h,
        w, h
    };
    const GLfloat _Texcoord[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, _Vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, _Texcoord);
    glBindTexture(GL_TEXTURE_2D, m_model->bg_tex->texid);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_DEPTH_TEST);
}


bool MapWidget::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * MapWidget::Model() const
{
    return m_model;
}
