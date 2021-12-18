#include "animationviewer.h"

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
#include <QCheckBox>
#include <QToolButton>
#include <QToolBar>

#include "animationscene.h"
#include "netlizard.h"
#include "qdef.h"
#include "settings.h"

AnimationViewer::AnimationViewer(QWidget *parent) :
    BaseViewer(parent),
    m_animationScene(0),
    m_gameComboBox(0),
    m_fileChooser(0),
    m_resourceDirChooser(0),
    m_indexSpinBox(0),
    m_colorChooser(0),
    m_openObjButton(0),
    m_openResourcePathButton(0),
    m_animComboBox(0),
    m_frameSlider(0),
    m_playButton(0),
    m_animFPSSpinBox(0),
    m_playSeqCheckBox(0),
    m_toolbar(0),
    m_autoscan(false),
    m_autoscanCheckBox(0)
{
    setObjectName("AnimationViewer");
    Init();
    SetAutoscan(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<bool>("APP/autoscan_level_and_index", false));
}

AnimationViewer::~AnimationViewer()
{
}

void AnimationViewer::Init()
{
    QPushButton *button;
    m_gameComboBox = new QComboBox;
    m_animationScene = new AnimationScene;
    //QHBoxLayout *toolLayout = ToolLayout();
    m_indexSpinBox = new QSpinBox;
    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QGroupBox *widget = new QGroupBox;
    m_animComboBox = new QComboBox;
    m_frameSlider = new QSlider;
    m_playButton = new QToolButton;
    m_animFPSSpinBox = new QSpinBox;
    m_playSeqCheckBox = new QCheckBox(tr("Invert"));
    m_toolbar = new QToolBar;
    QToolButton *nextFrame = new QToolButton;
    QToolButton *prevFrame = new QToolButton;
    m_autoscanCheckBox = new QCheckBox(tr("Autoscan"));
    SetTitleLabelVisible(false);

    for(int i = NL_FRAME_ANIMATION_IDLE; i < NL_FRAME_ANIMATION_TOTAL; i++)
    {
        m_animComboBox->addItem(nlGet3DModelFrameAnimationName(static_cast<NETLizard_3D_Animation_Type>(i)), QVariant(i));
    }
    widget->setTitle(tr("Operation"));
    widget->setMinimumWidth(240);
    widget->setMaximumWidth(360);
    vLayout->addWidget(new QLabel(tr("Animation: ")));
    vLayout->addWidget(m_animComboBox);
    vLayout->addSpacing(1);
    vLayout->addWidget(new QLabel(tr("Frame: ")));
    m_frameSlider->setMinimum(0);
    m_frameSlider->setMaximum(0);
    m_frameSlider->setOrientation(Qt::Horizontal);
    m_frameSlider->setTickPosition(QSlider::TicksBelow);
    vLayout->addWidget(m_frameSlider);
    vLayout->addSpacing(1);
    vLayout->addWidget(new QLabel(tr("FPS: ")));
    m_animFPSSpinBox->setMinimum(0);
    m_animFPSSpinBox->setMaximum(60);
    m_animFPSSpinBox->setValue(m_animationScene->AnimFPS());
    vLayout->addWidget(m_animFPSSpinBox);
    vLayout->addSpacing(1);
    vLayout->addWidget(new QLabel(tr("Play sequence: ")));
    vLayout->addWidget(m_playSeqCheckBox);
    vLayout->addSpacing(1);
    m_playButton->setText(tr("Play"));
    m_playButton->setCheckable(true);
    m_toolbar->addWidget(m_playButton);
    prevFrame->setArrowType(Qt::LeftArrow);
    m_toolbar->addWidget(prevFrame);
    nextFrame->setArrowType(Qt::RightArrow);
    m_toolbar->addWidget(nextFrame);
    m_toolbar->setEnabled(false);
    vLayout->addWidget(m_toolbar);
    vLayout->addStretch(1);

    widget->setLayout(vLayout);
    layout->addWidget(widget);
    //m_animationScene->setMinimumWidth(320);
    layout->addWidget(m_animationScene, 1);

    const NETLizard_Game Games[] = {
        NL_SHADOW_OF_EGYPT_3D,
        NL_CLONE_3D
    };
    for(uint i = 0; i < countof(Games); i++)
    {
        m_gameComboBox->addItem(nlGet3DGameName(Games[i]), QVariant(Games[i]));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_indexSpinBox->setMinimum(-1);
    m_indexSpinBox->setValue(-1);
    m_openObjButton = new QPushButton;
    connect(m_openObjButton, SIGNAL(clicked()), this, SLOT(OpenObjFileChooser()));
    m_openObjButton->setText(tr("un file"));
    m_openObjButton->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(m_openObjButton);
    AddTool();

    m_openResourcePathButton = new QPushButton;
    connect(m_openResourcePathButton, SIGNAL(clicked()), this, SLOT(OpenResourceDirChooser()));
    m_openResourcePathButton->setText(tr("Resource path"));
    m_openResourcePathButton->setShortcut(QKeySequence::fromString("ctrl+r"));
    AddTool(m_openResourcePathButton);
    AddTool();

    AddTool(new QLabel(tr("Game: ")));
    AddTool(m_gameComboBox);
    AddTool();
    AddTool(new QLabel(tr("Index: ")));
    AddTool(m_indexSpinBox);
    AddTool(m_autoscanCheckBox);
    connect(m_autoscanCheckBox, SIGNAL(clicked(bool)), this, SLOT(SetAutoscan(bool)));

    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenBackgroundColorChooser()));
    button->setText(tr("Color"));
    AddTool(button);

    AddTool();
    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    button->setText(tr("Load"));
    button->setShortcut(QKeySequence::fromString("ctrl+o"));
    AddTool(button);

    connect(m_animComboBox, SIGNAL(currentIndexChanged(int)), m_animationScene, SLOT(SetAnim(int)));
    connect(m_frameSlider, SIGNAL(sliderMoved(int)), m_animationScene, SLOT(SetFrame(int)));
    connect(m_animFPSSpinBox, SIGNAL(valueChanged(int)), m_animationScene, SLOT(SetAnimFPS(int)));
    connect(m_animationScene, SIGNAL(frameChanged(int)), m_frameSlider, SLOT(setValue(int)));
    connect(m_animationScene, SIGNAL(animChanged(int)), this, SLOT(OnAnimChanged(int)));
    connect(m_playSeqCheckBox, SIGNAL(clicked(bool)), m_animationScene, SLOT(SetPlaySequence(bool)));
    connect(m_playButton, SIGNAL(clicked(bool)), m_animationScene, SLOT(SetPlaying(bool)));
    connect(nextFrame, SIGNAL(clicked()), this, SLOT(NextFrame()));
    connect(prevFrame, SIGNAL(clicked()), this, SLOT(PrevFrame()));
    connect(m_animationScene, SIGNAL(playing()), this, SLOT(OnPlaying()));
    connect(m_animationScene, SIGNAL(stopped()), this, SLOT(OnStopped()));

    CentralWidget()->setLayout(layout);
    SetTitle(tr("NETLizard 3D animation model viewer"));
}

void AnimationViewer::OpenBackgroundColorChooser()
{
    if(!m_colorChooser)
    {
        m_colorChooser = new QColorDialog(this);
        connect(m_colorChooser, SIGNAL(colorSelected(const QColor &)), this, SLOT(SetBackgroundColor(const QColor &)));
    }
    m_colorChooser->setCurrentColor(m_animationScene->ClearColor());

    m_colorChooser->exec();
}

void AnimationViewer::OpenObjFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        m_fileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetObjFile(const QString &)));
    }

    m_fileChooser->exec();
}

void AnimationViewer::OpenResourceDirChooser()
{
    if(!m_resourceDirChooser)
    {
        m_resourceDirChooser = new QFileDialog(this);
        m_resourceDirChooser->setFileMode(QFileDialog::Directory);
        connect(m_resourceDirChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetResourceDirPath(const QString &)));
    }

    m_resourceDirChooser->exec();
}

void AnimationViewer::SetObjFile(const QString &file)
{
    if(m_objPath != file)
    {
        m_objPath = file;
        m_openObjButton->setToolTip(m_objPath);
        //m_resourceDirPath.clear();
    }
}

void AnimationViewer::NextFrame()
{
    m_animationScene->Stop();
    m_animationScene->NextFrame();
}

void AnimationViewer::PrevFrame()
{
    m_animationScene->Stop();
    m_animationScene->PrevFrame();
}

void AnimationViewer::SetResourceDirPath(const QString &file)
{
    if(m_resourceDirPath != file)
    {
        m_resourceDirPath = file;
        m_openResourcePathButton->setToolTip(m_resourceDirPath);
    }
}

void AnimationViewer::SetBackgroundColor(const QColor &color)
{
    m_animationScene->SetClearColor(color);
}

void AnimationViewer::OnAnimChanged(int index)
{
    m_frameSlider->setValue(0);
    const NETLizard_3D_Frame_Animation *config = m_animationScene->CurrentAnimation();
    if(config)
    {
        SetStatusText(QString("Animation: %1, frame range: %2 - %3, frame count: %4").arg(nlGet3DModelFrameAnimationName(config->type)).arg(config->begin_frame).arg(config->end_frame).arg(config->count));
        m_frameSlider->setMaximum(config->count - 1);
    }
    else
    {
        SetStatusText("Invalid frame animation!");
        m_frameSlider->setMaximum(0);
    }
}

void AnimationViewer::UpdatePlayState(bool b)
{
    if(b)
    {
        m_playButton->setText(tr("Stop"));
        m_playButton->setChecked(true);
    }
    else
    {
        m_playButton->setText(tr("Play"));
        m_playButton->setChecked(false);
    }
}

void AnimationViewer::OnPlaying()
{
    UpdatePlayState(true);
}

void AnimationViewer::OnStopped()
{
    UpdatePlayState(false);
}

void AnimationViewer::Reset()
{
    BaseViewer::Reset();
    OnStopped();
    m_animationScene->Reset();
    m_animComboBox->setCurrentIndex(0);
    m_frameSlider->setMaximum(0);
    m_frameSlider->setValue(0);
    m_toolbar->setEnabled(false);
    if(m_autoscan)
        m_indexSpinBox->setValue(-1);
}

bool AnimationViewer::OpenFile()
{
    if(m_objPath.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose obj file and resource path!");
        return false;
    }
    Reset();

    const NETLizard_Game Games[] = {
        NL_SHADOW_OF_EGYPT_3D,
        NL_CLONE_3D
    };
    int selectedIndex = m_gameComboBox->currentIndex();
    int game = Games[selectedIndex];
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
    case NL_CLONE_3D:
        res = m_animationScene->LoadFile(m_objPath, m_resourceDirPath, game, index);
    break;
    default:
        QMessageBox::warning(this, "Error", "Unsupport 3D game!");
        break;
    }
    SetTitleLabel(QString("%1(index-%2)  obj: %3, resource directory: %4 -> %5").arg(nlGet3DGameName(static_cast<NETLizard_Game>(game))).arg(index).arg(m_objPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_toolbar->setEnabled(true);
        m_animationScene->setFocus();
        m_animationScene->SetAnim(0);
        m_animationScene->Play();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load 3D game animation file fail!");
    }
    return res;
}

NLScene * AnimationViewer::Scene()
{
    return m_animationScene;
}

void AnimationViewer::SetAutoscan(bool b)
{
    if(m_autoscan != b)
    {
        m_autoscan = b;
        if(m_autoscan)
            m_indexSpinBox->setValue(-1);
        m_indexSpinBox->setEnabled(!m_autoscan);
        if(m_autoscanCheckBox->isChecked() != m_autoscan)
            m_autoscanCheckBox->setChecked(m_autoscan);
    }
}
