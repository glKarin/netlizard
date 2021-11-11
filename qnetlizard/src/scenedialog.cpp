#include "scenedialog.h"

#include <QDebug>
#include <QDateTime>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QScrollBar>
#include <QPushButton>

#include "nlscene.h"
#include "nlscenecamera.h"
#include "qdef.h"

#define TEXT_FORMAT 1 // 1 2
#if(TEXT_FORMAT == 2)

#define _ENDL "<br/>"
#define SETTEXT setHtml
#define RICH_TEXT true

#else

#define _ENDL "\n"
#ifdef _B
#undef _B
#define _B(x) x
#endif
#define SETTEXT setText
#define RICH_TEXT false

#endif

SceneDialog::SceneDialog(QWidget *parent) :
    QDialog(parent),
    m_scene(0),
    m_textViewer(0)
{
    setObjectName("SceneDialog");
    Init();
}

SceneDialog::~SceneDialog()
{
    DEBUG_DESTROY_Q
}

void SceneDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    //QHBoxLayout *toolLayout = new QHBoxLayout;
    m_textViewer = new QTextBrowser;
    //QPushButton *button = new QPushButton;

    m_textViewer->setAcceptRichText(RICH_TEXT);
    layout->addWidget(m_textViewer, 1);
    //button->setText("Clear");
    //toolLayout->addStretch();
    //toolLayout->addWidget(button);
    //layout->addLayout(toolLayout);

    setLayout(layout);

    setWindowTitle("Scene infomation");
}

void SceneDialog::ResetPosAndSize()
{
    QWidget *p = parentWidget();
    if(!p)
        return;
    QRect rect = p->frameGeometry();
    move(rect.x() + rect.width(), rect.y());
    resize(p->height(), p->height());
    //m_textViewer->verticalScrollBar()->setValue(0);
    m_textViewer->horizontalScrollBar()->setValue(0);
}

#define _DBG_VEC3(label, v) QString(_B(" "label": ")"[%1, %2, %3]").arg(VECTOR3_X(v), 0, 'g', 6).arg(VECTOR3_Y(v), 0, 'g', 6).arg(VECTOR3_Z(v), 0, 'g', 6)
#define _DBG_MAT4(label, v) QString(_B(" "label": ")"["_ENDL"| %1, %2, %3, %4 |"_ENDL"| %5, %6, %7, %8 |"_ENDL"| %9, %10, %11, %12 |"_ENDL"| %13, %14, %15, %16 |   ]")\
        .arg(GL_MATRIXV_M_CR4(v, 1, 1), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 1), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 1), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 4, 1), 12, 'g', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 2), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 2), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 2), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 4, 2), 12, 'g', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 3), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 3), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 3), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 4, 3), 12, 'g', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 4), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 4), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 4), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 4, 4), 12, 'g', 6)
#define _DBG_MAT3(label, v) QString(_B(" "label": ")"["_ENDL"| %1, %2, %3 |"_ENDL"| %4, %5, %6 |"_ENDL"| %7, %8, %9 |   ]")\
        .arg(GL_MATRIXV_M_CR4(v, 1, 1), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 1), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 1), 12, 'g', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 2), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 2), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 2), 12, 'g', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 3), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 2, 3), 12, 'g', 6).arg(GL_MATRIXV_M_CR4(v, 3, 3), 12, 'g', 6)
void SceneDialog::UpdateSceneInfo()
{
    if(!m_scene)
    {
        m_textViewer->SETTEXT("No active scene!");
        return;
    }

    QStringList list;
    list << _B("Scene infomation: ")
         << _B(" FPS: ") + QString::number(m_scene->CurrentFPS()) + " (" + QString(_B("Limit: ")) + QString::number(m_scene->FPS()) + ")"
         << _B(" Actor count: ") + QString::number(m_scene->ActorCount())
               << _B(" Update delta: ") + QString::number(m_scene->CurrendDelta())
               << _B(" Update time: ") + QDateTime::fromMSecsSinceEpoch(m_scene->UpdateTime()).toString("yyyy-MM-dd HH:mm:ss zzz")
               ;

    list << "";
    list << _B("Camera infomation: ");
    NLSceneCamera *camera = m_scene->CurrentCamera();
    if(!camera)
    {
        list << "No active camera";
    }
    else
    {
        NLVector3 v = camera->Position();
        list << _B("vector3");
        list << _DBG_VEC3("Position", v);
        v = camera->Rotation();
        list << _DBG_VEC3("Rotation", v);
//        v = camera->Scale();
//        list << _DBG_VEC3("Scale", v);

        list << _B("normal");
        v = camera->Direction();
        list << _DBG_VEC3("Direction", v);
        v = camera->Up();
        list << _DBG_VEC3("Up", v);
        v = camera->Right();
        list << _DBG_VEC3("Right", v);

        const NLMatrix4 *m = camera->ViewMatrix();
        list << _B("matrix");
        list << _DBG_MAT4("View matrix", m);
        m = camera->ProjectionMatrix();
        list << _DBG_MAT4("Projection matrix", m);
        m = camera->ViewProjectionMatrix();
        list << _DBG_MAT4("View-projection matrix", m);
        m = camera->NormalMatrix();
        list << _DBG_MAT3("Normal matrix", m);
        m = camera->RenderMatrix();
        list << _DBG_MAT3("Render matrix", m);
    }

    m_textViewer->SETTEXT(list.join(_ENDL));
}

void SceneDialog::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        disconnect();
        m_scene = scene;
        if(m_scene)
        {
            connect(m_scene, SIGNAL(updated(float)), this, SLOT(UpdateSceneInfo()));
            connect(m_scene, SIGNAL(destroyed()), this, SLOT(Reset()));
        }
        UpdateSceneInfo();
    }
}

NLScene * SceneDialog::Scene()
{
    return m_scene;
}

void SceneDialog::Reset()
{
    disconnect();
    m_scene = 0;
    UpdateSceneInfo();
}