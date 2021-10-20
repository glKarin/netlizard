#include "roleviewer.h"

#include <QDebug>
#include <QPair>
#include <QVariant>

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QGroupBox>
#include <QSlider>

#include "rolescene.h"
#include "netlizard.h"
#include "qdef.h"

static const QPair<int, QString> Types[] = {
    QPair<int, QString>(NL_SHADOW_OF_EGYPT_3D, "3D Shadows of Egypt"),
    QPair<int, QString>(NL_CLONE_3D, "3D Clone"),
};

static const QPair<int, QString> Anims[NL_FRAME_ANIMATION_TOTAL] = {
    QPair<int, QString>(NL_FRAME_ANIMATION_IDLE, "Idle"),
    QPair<int, QString>(NL_FRAME_ANIMATION_WALK, "Walk"),
    QPair<int, QString>(NL_FRAME_ANIMATION_RUN, "Run"),
    QPair<int, QString>(NL_FRAME_ANIMATION_FIGHTING_1, "Fighting 1"),
    QPair<int, QString>(NL_FRAME_ANIMATION_FIGHTING_2, "Fighting 2"),
    QPair<int, QString>(NL_FRAME_ANIMATION_ATTACK_1, "Attack 1"),
    QPair<int, QString>(NL_FRAME_ANIMATION_ATTACK_2, "Attack 2"),
    QPair<int, QString>(NL_FRAME_ANIMATION_DEAD_1, "Dead 1"),
    QPair<int, QString>(NL_FRAME_ANIMATION_DEAD_2, "Dead 2"),
};

RoleViewer::RoleViewer(QWidget *parent) :
    BaseViewer(parent),
    m_roleScene(0),
    m_gameComboBox(0),
    m_fileChooser(0),
    m_resourceDirChooser(0),
    m_indexSpinBox(0),
    m_colorChooser(0),
    m_openObjButton(0),
    m_openResourcePathButton(0),
    m_animComboBox(0),
    m_frameSlider(0)
{
    setObjectName("RoleViewer");
    Init();
}

RoleViewer::~RoleViewer()
{
}

void RoleViewer::Init()
{
    QPushButton *button;
    m_gameComboBox = new QComboBox;
    m_roleScene = new RoleScene;
    QHBoxLayout *toolLayout = ToolLayout();
    m_indexSpinBox = new QSpinBox;
    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QGroupBox *widget = new QGroupBox;
    m_animComboBox = new QComboBox;
    m_frameSlider = new QSlider;

    for(int i = 0; i < NL_FRAME_ANIMATION_TOTAL; i++)
    {
        const QPair<int, QString> &p = Anims[i];
        m_animComboBox->addItem(p.second, QVariant(p.first));
    }
    widget->setMaximumWidth(240);
    vLayout->addWidget(new QLabel("Animation"));
    vLayout->addWidget(m_animComboBox);
    vLayout->addSpacing(1);
    vLayout->addWidget(new QLabel("Frame"));
    m_frameSlider->setMinimum(0);
    m_frameSlider->setMaximum(10);
    m_frameSlider->setOrientation(Qt::Horizontal);
    m_frameSlider->setTickPosition(QSlider::TicksRight);
    //m_frameSlider->setMaximumWidth(200);
    vLayout->addWidget(m_frameSlider);
    vLayout->addStretch(1);
    widget->setLayout(vLayout);
    layout->addWidget(widget);
    //m_roleScene->setMinimumWidth(320);
    layout->addWidget(m_roleScene, 1);

    for(int i = 0; i < 2; i++)
    {
        const QPair<int, QString> &p = Types[i];
        m_gameComboBox->addItem(p.second, QVariant(p.first));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_indexSpinBox->setMinimum(-1);
    m_indexSpinBox->setValue(-1);
    m_openObjButton = new QPushButton;
    connect(m_openObjButton, SIGNAL(clicked()), this, SLOT(OpenObjFileChooser()));
    m_openObjButton->setText("un file");
    toolLayout->addWidget(m_openObjButton);
    toolLayout->addStretch();

    m_openResourcePathButton = new QPushButton;
    connect(m_openResourcePathButton, SIGNAL(clicked()), this, SLOT(OpenResourceDirChooser()));
    m_openResourcePathButton->setText("Resource path");
    toolLayout->addWidget(m_openResourcePathButton);
    toolLayout->addStretch();

    toolLayout->addWidget(new QLabel("Game: "));
    toolLayout->addWidget(m_gameComboBox);
    toolLayout->addStretch();
    toolLayout->addWidget(new QLabel("Index: "));
    toolLayout->addWidget(m_indexSpinBox);

    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenBackgroundColorChooser()));
    button->setText("Color");
    toolLayout->addWidget(button);

    toolLayout->addStretch();
    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    button->setText("Load");
    toolLayout->addWidget(button);

    connect(m_gameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));
    connect(m_animComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetAnim(int)));
    connect(m_frameSlider, SIGNAL(sliderMoved(int)), this, SLOT(SetFrame(int)));

    CentralWidget()->setLayout(layout);
    SetTitle("NETLizard 3D FPS role viewer");
}

void RoleViewer::OpenBackgroundColorChooser()
{
    if(!m_colorChooser)
    {
        m_colorChooser = new QColorDialog(this);
        connect(m_colorChooser, SIGNAL(colorSelected(const QColor &)), this, SLOT(SetBackgroundColor(const QColor &)));
    }
    m_colorChooser->setCurrentColor(m_roleScene->ClearColor());

    m_colorChooser->exec();
}

void RoleViewer::OpenObjFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        m_fileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetObjFile(const QString &)));
    }

    m_fileChooser->exec();
}

void RoleViewer::OpenResourceDirChooser()
{
    if(!m_resourceDirChooser)
    {
        m_resourceDirChooser = new QFileDialog(this);
        m_resourceDirChooser->setFileMode(QFileDialog::Directory);
        connect(m_resourceDirChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetResourceDirPath(const QString &)));
    }

    m_resourceDirChooser->exec();
}

void RoleViewer::SetObjFile(const QString &file)
{
    if(m_objPath != file)
    {
        m_objPath = file;
        m_openObjButton->setToolTip(m_objPath);
        //m_resourceDirPath.clear();
    }
}

void RoleViewer::SetResourceDirPath(const QString &file)
{
    if(m_resourceDirPath != file)
    {
        m_resourceDirPath = file;
        m_openResourcePathButton->setToolTip(m_resourceDirPath);
    }
}

void RoleViewer::SetBackgroundColor(const QColor &color)
{
    m_roleScene->SetClearColor(color);
}

void RoleViewer::OnTypeCurrentIndexChanged(int index)
{
    //m_levelSpinBox->setEnabled(index == 2);
}

void RoleViewer::SetAnim(int index)
{
    m_roleScene->SetAnim(index);
    m_frameSlider->setValue(0);
    int count = m_roleScene->CurrentAnimationFrames();
    if(count > 0)
        m_frameSlider->setMaximum(count - 1);
    else
        m_frameSlider->setMaximum(0);
}

void RoleViewer::SetFrame(int index)
{
    m_roleScene->SetFrame(index);
}

bool RoleViewer::OpenFile()
{
    if(m_objPath.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose obj file and resource path!");
        return false;
    }
    m_animComboBox->setCurrentIndex(0);
    m_frameSlider->setMaximum(0);
    m_frameSlider->setValue(0);
    m_roleScene->Reset();
    int selectedIndex = m_gameComboBox->currentIndex();
    int game = Types[selectedIndex].first;
    int index = m_indexSpinBox->value();
    if(m_resourceDirPath.isEmpty())
    {
        QFileInfo info(m_objPath);
        SetResourceDirPath(info.absolutePath());
    }
    if(index == -1)
    {
        QFileInfo info(m_objPath);
        QRegExp regexp("un(\\d+)");
        int pos = regexp.indexIn(info.fileName());
        if(pos != -1)
        {
            index = regexp.cap(1).toInt();
            m_indexSpinBox->setValue(index);
        }
    }
    bool res = false;
    switch(game)
    {
    case NL_SHADOW_OF_EGYPT_3D:
    //case NL_CLONE_3D:
        res = m_roleScene->LoadFile(m_objPath, m_resourceDirPath, game, index);
    break;
    default:
        QMessageBox::warning(this, "Error", "Unsupport 3D game!");
        break;
    }
    SetTitleLabel(QString("%1(index-%2)  obj: %3, resource directory: %4 -> %5").arg(Types[selectedIndex].second).arg(index).arg(m_objPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_roleScene->setFocus();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load 3D game map file fail!");
    }
    return res;
}
