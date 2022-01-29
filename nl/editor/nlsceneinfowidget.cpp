#include "nlsceneinfowidget.h"

#include <QDebug>
#include <QDateTime>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QScrollBar>
#include <QPushButton>
#include <QTextBrowser>
#include <QStackedWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QFormLayout>
#include <QColorDialog>
#include <QScrollArea>

#include "utils/nlfuncs.h"
#include "engine/nlscene.h"
#include "engine/nlscenecamera.h"
#include "common/nldef.h"
#include "common/nldbg.h"

#define TEXT_FORMAT 1 // 1 2
#if(TEXT_FORMAT == 2)

#define _ENDL "<br/>"
#define SETTEXT setHtml
#define GETTEXT toHtml
#define RICH_TEXT true

#else

#define _ENDL "\n"
#define SETTEXT setPlainText
#define GETTEXT toPlainText
#define RICH_TEXT false

#endif

NLScenePropFormGroupWidget::NLScenePropFormGroupWidget(QWidget *widget)
    : NLPropFormGroupWidget(widget)
{
    setObjectName("NLScenePropFormGroupWidget");
}

NLScenePropFormGroupWidget::NLScenePropFormGroupWidget(const QString &title, QWidget *widget)
    : NLPropFormGroupWidget(title, widget)
{
    setObjectName("NLScenePropFormGroupWidget");
}

NLScenePropFormGroupWidget::~NLScenePropFormGroupWidget()
{

}

void NLScenePropFormGroupWidget::SetObject(QObject *obj)
{
    NLPropFormGroupWidget::SetObject(obj);
    if(obj)
    {
        connect(obj, SIGNAL(destroyed()), this, SLOT(Reset()));
    }
}

QVariant NLScenePropFormGroupWidget::GetObjectProperty(QObject *obj, const QString &name)
{
    return static_cast<NLScene *>(obj)->GetProperty(name);
}

void NLScenePropFormGroupWidget::SetObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<NLScene *>(obj)->SetProperty(name, value);
}

void NLScenePropFormGroupWidget::CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<NLScene *>(obj)->CoverProperty(name, value);
}

NLPropertyInfoList NLScenePropFormGroupWidget::GetPropertyInfoList(QObject *obj)
{
    return NL::scene_propertics(static_cast<NLScene *>(obj));
}

void NLScenePropFormGroupWidget::OnPropertyChanged(const QString &name, const QVariant &value, int action)
{
    if(Object() != sender())
        return;
    NotifyPropertyChanged(name, value);
}



NLSceneInfoWidget::NLSceneInfoWidget(QWidget *parent) :
    QTabWidget(parent),
    m_scene(0),
    m_baseInfo(0),
    m_cameraInfo(0),
    m_cameraMatrix(0),
    m_matrixList(0),
    m_settingGroupBox(0)
{
    setObjectName("NLSceneInfoWidget");
    Init();
}

NLSceneInfoWidget::~NLSceneInfoWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLSceneInfoWidget::Init()
{
    m_baseInfo = new QTextBrowser(this);
    QTabWidget *cameraTab = new QTabWidget(this);
    m_cameraInfo = new QTextBrowser(cameraTab);
    m_cameraMatrix = new QStackedWidget;
    QWidget *matrixWidget = new QWidget(cameraTab);
    m_matrixList = new QComboBox;
    QVBoxLayout *matrixLayout = new QVBoxLayout;
    m_settingGroupBox = new NLScenePropFormGroupWidget(tr("General"), this);
    QScrollArea *scrollArea = new QScrollArea(this);

    m_baseInfo->setAcceptRichText(RICH_TEXT);
    m_cameraInfo->setAcceptRichText(RICH_TEXT);

    const QString types[] = {
        tr("View matrix"),
        tr("Projection matrix"),
        tr("View-projection matrix"),
        tr("Normal matrix(transposed)"),
        tr("Initial fixed view matrix"),
    };
    for(int i = 0; i < 5; i++)
    {
        m_matrixList->addItem(types[i], i);
        QTextBrowser *tb = new QTextBrowser(m_cameraMatrix);
        tb->SETTEXT(tr("Invalid matrix!"));
        m_cameraMatrix->addWidget(tb);
    }
    matrixLayout->addWidget(m_matrixList);
    matrixLayout->addWidget(m_cameraMatrix);
    matrixWidget->setLayout(matrixLayout);
    cameraTab->setTabShape(QTabWidget::QTabWidget::Triangular);
    cameraTab->setTabPosition(QTabWidget::South);

    connect(m_matrixList, SIGNAL(currentIndexChanged(int)), m_cameraMatrix, SLOT(setCurrentIndex(int)));

    cameraTab->addTab(m_cameraInfo, tr("Base"));
    cameraTab->addTab(matrixWidget, tr("Matrix"));

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_settingGroupBox);

    addTab(m_baseInfo, tr("Base"));
    addTab(cameraTab, tr("Camera"));
    addTab(scrollArea, tr("Setting"));

    setCurrentWidget(m_baseInfo);
}

#define _DBG_VEC3(label, v) QString(label + ": ""[%1, %2, %3]").arg(VECTOR3_X(v), 0, 'f', 6).arg(VECTOR3_Y(v), 0, 'f', 6).arg(VECTOR3_Z(v), 0, 'f', 6)
#define _DBG_MAT4(v) QString("|%1, %2, %3, %4|"_ENDL"|%5, %6, %7, %8|"_ENDL"|%9, %10, %11, %12|"_ENDL"|%13, %14, %15, %16|")\
        .arg(GL_MATRIXV_M_CR4(v, 1, 1), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 1), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 1), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 4, 1), 12, 'f', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 2), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 2), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 2), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 4, 2), 12, 'f', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 3), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 3), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 3), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 4, 3), 12, 'f', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 4), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 4), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 4), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 4, 4), 12, 'f', 6)
#define _DBG_MAT3(v) QString("|%1, %2, %3|"_ENDL"|%4, %5, %6|"_ENDL"|%7, %8, %9|")\
        .arg(GL_MATRIXV_M_CR4(v, 1, 1), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 1), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 1), 12, 'f', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 2), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 2), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 2), 12, 'f', 6)\
        .arg(GL_MATRIXV_M_CR4(v, 1, 3), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 2, 3), 12, 'f', 6).arg(GL_MATRIXV_M_CR4(v, 3, 3), 12, 'f', 6)
void NLSceneInfoWidget::UpdateSceneInfo()
{
    if(!m_scene)
    {
        m_baseInfo->SETTEXT(tr("No active scene!"));
        m_cameraInfo->SETTEXT(tr("No active camera!"));
        for(int i = 0; i < m_cameraMatrix->count(); i++)
        {
            static_cast<QTextBrowser *>(m_cameraMatrix->widget(i))->SETTEXT(tr("Invalid matrix!"));
        }
        return;
    }

    QStringList list;
    list
         << tr("FPS: ") + QString::number(m_scene->CurrentFPS()) + " (" + QString(tr("Limit: ")) + QString::number(m_scene->FPS()) + ")"
         << tr("Actor count: ") + tr("root") + "(" + QString::number(m_scene->ActorCount()) + ") / " + tr("total") + "(" + QString::number(m_scene->ActorTotalCount()) + ")"
               << tr("Update delta: ") + QString::number(m_scene->CurrendDelta())
               << tr("Update time: ") + QDateTime::fromMSecsSinceEpoch(m_scene->UpdateTime()).toString("yyyy-MM-dd HH:mm:ss zzz")
               << tr("Update render delta: ") + QString::number(m_scene->CurrendRenderDelta())
               << tr("Update render time: ") + QDateTime::fromMSecsSinceEpoch(m_scene->UpdateRenderTime()).toString("yyyy-MM-dd HH:mm:ss zzz")
               ;
    m_baseInfo->SETTEXT(list.join(_ENDL));

    NLSceneCamera *camera = m_scene->CurrentCamera();
    if(!camera)
    {
        m_cameraInfo->SETTEXT(tr("No active camera!"));
        for(int i = 0; i < m_cameraMatrix->count(); i++)
        {
            static_cast<QTextBrowser *>(m_cameraMatrix->widget(i))->SETTEXT(tr("Invalid matrix!"));
        }
    }
    else
    {
        list.clear();
        NLVector3 v = camera->Position();
        list << tr("Vector3: ");
        list << _DBG_VEC3(tr("Position"), v);
        v = camera->Rotation();
        list << _DBG_VEC3(tr("Rotation"), v);
//        v = camera->Scale();
//        list << _DBG_VEC3("Scale", v);

        list << "";

        list << tr("Normal: ");
        v = camera->Direction();
        list << _DBG_VEC3(tr("Direction"), v);
        v = camera->Up();
        list << _DBG_VEC3(tr("Up"), v);
        v = camera->Right_XPositive();
        list << _DBG_VEC3(tr("Right"), v);
        const QString cameraInfoText(list.join(_ENDL));
        if(cameraInfoText != m_cameraInfo->GETTEXT())
            m_cameraInfo->SETTEXT(cameraInfoText);

        const GLmatrix *mats[] = {
            camera->ViewMatrix(),
            camera->ProjectionMatrix(),
            camera->ViewProjectionMatrix(),
            camera->NormalMatrix(),
            camera->InitialFixedViewMatrix(),
        };
        for(uint i = 0; i < nlcountof(mats); i++)
        {
            const NLMatrix4 *m = mats[i];
            const QString mstr(_DBG_MAT4(m));
            QTextBrowser *textBrowser = static_cast<QTextBrowser *>(m_cameraMatrix->widget(i));
            if(mstr == textBrowser->GETTEXT())
                continue;
            //QTextCursor cursor = textBrowser->textCursor();
            textBrowser->SETTEXT(mstr);
            //textBrowser->setTextCursor(cursor);
        }
    }

}

void NLSceneInfoWidget::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        Reset();
        m_scene = scene;
        if(m_scene)
        {
            m_matrixList->setEnabled(true);
            connect(m_scene, SIGNAL(updated(float)), this, SLOT(UpdateSceneInfo()));
            connect(m_scene, SIGNAL(destroyed()), this, SLOT(Reset()));
            m_settingGroupBox->SetObject(m_scene);
            connect(m_scene, SIGNAL(propertyChanged(const QString &, const QVariant &)), this, SLOT(OnPropertyChanged(const QString &, const QVariant &)));
        }
        UpdateSceneInfo();
    }
}

void NLSceneInfoWidget::Reset()
{
    if(m_scene)
        m_scene->disconnect(this);
    m_scene = 0;
    m_settingGroupBox->SetObject(0);
    m_matrixList->setEnabled(false);
    UpdateSceneInfo();
}
