#include "itemwidget.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"

ItemWidget::ItemWidget(QWidget *parent)
    : NLScene(parent),
      m_model(0)
{
    setObjectName("ItemWidget");
    SetEnableDefaultWheelHandler(false);
}

ItemWidget::~ItemWidget()
{
    DEBUG_DESTROY_Q;
}

void ItemWidget::Init()
{
    NLScene::Init();
}

void ItemWidget::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
}

void ItemWidget::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
        NETLizard_RenderGL3DModel(m_model);
    }
    glPopMatrix();

    glFlush();
}

void ItemWidget::Deinit()
{
    NLScene::Deinit();
    Reset();
}

bool ItemWidget::LoadFile(const QString &file, const QString &resourcePath, int game, int index)
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
        b = NETLizard_ReadGLCT3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
        b = NETLizard_ReadGLCT3DEp2ItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_ARMY_RANGER_3D:
        b = NETLizard_ReadGLSpecnaz3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_SHADOW_OF_EGYPT_3D:
        b = NETLizard_ReadGLEgypt3DItemModelFile(path, index, resc_path, m_model);
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

    return true;
}

void ItemWidget::Reset()
{
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }
    vector3_s startPos = {{0, 120, 1000}};
    vector3_s startRotate = {{0.0, 0, 0}};
    initcam(&m_cam, &startPos, &startRotate);
    SetHideMouse(false);
}

bool ItemWidget::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * ItemWidget::Model() const
{
    return m_model;
}
