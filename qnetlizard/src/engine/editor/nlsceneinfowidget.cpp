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

#include "nlscene.h"
#include "nlscenecamera.h"
#include "qdef.h"

#define TEXT_FORMAT 1 // 1 2
#if(TEXT_FORMAT == 2)

#define _ENDL "<br/>"
#define SETTEXT setHtml
#define GETTEXT toHtml
#define RICH_TEXT true

#else

#define _ENDL "\n"
#ifdef _B
#undef _B
#define _B(x) x
#endif
#define SETTEXT setPlainText
#define GETTEXT toPlainText
#define RICH_TEXT false

#endif

NLSceneInfoWidget::NLSceneInfoWidget(QWidget *parent) :
    QTabWidget(parent),
    m_scene(0),
    m_baseInfo(0),
    m_cameraInfo(0),
    m_cameraMatrix(0),
    m_matrixList(0),
    m_settingGroupBox(0),
    m_fpsSpinBox(0),
    m_updateSpinBox(0),
    m_colorButton(0)
{
    setObjectName("NLSceneInfoWidget");
    Init();
}

NLSceneInfoWidget::~NLSceneInfoWidget()
{
    DEBUG_DESTROY_Q
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
    m_settingGroupBox = new QGroupBox(tr("General"), this);
    QFormLayout *settingLayout = new QFormLayout;
    m_fpsSpinBox = new QSpinBox;
    m_updateSpinBox = new QSpinBox;
    m_colorButton = new QPushButton(tr("Choose color"));

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

    m_fpsSpinBox->setMinimum(0);
    m_updateSpinBox->setMinimum(0);
    m_fpsSpinBox->setMaximum(300);
    m_updateSpinBox->setMaximum(1000);
    m_fpsSpinBox->setSuffix(tr("ms"));
    m_updateSpinBox->setSuffix(tr("ms"));
    connect(m_colorButton, SIGNAL(clicked()), this, SLOT(ChooseClearColor()));
    settingLayout->addRow(tr("FPS"), m_fpsSpinBox);
    settingLayout->addRow(tr("Update interval"), m_updateSpinBox);
    settingLayout->addRow(tr("Clear color"), m_colorButton);
    settingLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    m_settingGroupBox->setLayout(settingLayout);

    addTab(m_baseInfo, tr("Base"));
    addTab(cameraTab, tr("Camera"));
    addTab(m_settingGroupBox, tr("Setting"));

    setCurrentWidget(m_baseInfo);
}

#define _DBG_VEC3(label, v) QString(_B(label + ": ")"[%1, %2, %3]").arg(VECTOR3_X(v), 0, 'f', 6).arg(VECTOR3_Y(v), 0, 'f', 6).arg(VECTOR3_Z(v), 0, 'f', 6)
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
         << _B(tr("FPS: ")) + QString::number(m_scene->CurrentFPS()) + " (" + QString(_B(tr("Limit: "))) + QString::number(m_scene->FPS()) + ")"
         << _B(tr("Actor count: ")) + tr("root") + "(" + QString::number(m_scene->ActorCount()) + ") / " + tr("total") + "(" + QString::number(m_scene->ActorTotalCount()) + ")"
               << _B(tr("Update delta: ")) + QString::number(m_scene->CurrendDelta())
               << _B(tr("Update time: ")) + QDateTime::fromMSecsSinceEpoch(m_scene->UpdateTime()).toString("yyyy-MM-dd HH:mm:ss zzz")
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
        list << _B(tr("Vector3: "));
        list << _DBG_VEC3(tr("Position"), v);
        v = camera->Rotation();
        list << _DBG_VEC3(tr("Rotation"), v);
//        v = camera->Scale();
//        list << _DBG_VEC3("Scale", v);

        list << "";

        list << _B(tr("Normal: "));
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
        for(uint i = 0; i < countof(mats); i++)
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
            m_settingGroupBox->setEnabled(true);
            m_fpsSpinBox->setValue(m_scene->FPS());
            m_updateSpinBox->setValue(m_scene->UpdateInterval());
            QString c(m_scene->ClearColor().name().toUpper());
            m_colorButton->setText(c);
            m_colorButton->setProperty("color", c);
            m_colorButton->setStyleSheet(QString("QPushButton { color: %1; }").arg(c));
            connect(m_fpsSpinBox, SIGNAL(valueChanged(int)), m_scene, SLOT(SetFPS(float)));
            connect(m_updateSpinBox, SIGNAL(valueChanged(int)), m_scene, SLOT(SetUpdateInterval(int)));
            connect(m_scene, SIGNAL(updated(float)), this, SLOT(UpdateSceneInfo()));
            connect(m_scene, SIGNAL(destroyed()), this, SLOT(Reset()));
        }
        UpdateSceneInfo();
    }
}

void NLSceneInfoWidget::ChooseClearColor()
{
    if(!m_scene)
        return;
    QPushButton *w = static_cast<QPushButton *>(sender());
    QVariant va = w->property("color");
    QColor color = QColorDialog::getColor(QColor(va.toString()), w);
    QString c(color.name().toUpper());
    w->setText(c);
    w->setProperty("color", c);
    w->setStyleSheet(QString("QPushButton { color: %1; }").arg(c));
    m_scene->SetClearColor(color);
}

void NLSceneInfoWidget::Reset()
{
    if(m_scene)
        m_scene->disconnect(this);
    m_scene = 0;
    m_fpsSpinBox->disconnect();
    m_updateSpinBox->disconnect();
    m_fpsSpinBox->setValue(0);
    m_updateSpinBox->setValue(0);
    QString c("#000000");
    m_colorButton->setText(tr("Choose color"));
    m_colorButton->setProperty("color", c);
    m_colorButton->setStyleSheet(QString("QPushButton { color: %1; }").arg(c));
    m_settingGroupBox->setEnabled(false);
    m_matrixList->setEnabled(false);
    UpdateSceneInfo();
}
