#include "mapviewer.h"

#include <QDebug>
#include <QPair>
#include <QVariant>

#include <QLabel>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

#include "mapscene.h"
#include "netlizard.h"
#include "qdef.h"
#include "settings.h"

MapViewer::MapViewer(QWidget *parent) :
    BaseViewer(parent),
    m_mapScene(0),
    m_gameComboBox(0),
    m_fileChooser(0),
    m_resourceDirChooser(0),
    m_levelSpinBox(0),
    m_openLvlButton(0),
    m_openResourcePathButton(0),
    m_autoscan(false),
    m_autoscanCheckBox(0)
{
    setObjectName("MapViewer");
    Init();
    SetAutoscan(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<bool>("APP/autoscan_level_and_index", false));
}

MapViewer::~MapViewer()
{
}

void MapViewer::Init()
{
    QPushButton *button;
    m_gameComboBox = new QComboBox;
    m_mapScene = new MapScene;
    //QHBoxLayout *toolLayout = ToolLayout();
    m_levelSpinBox = new QSpinBox;
    m_autoscanCheckBox = new QCheckBox("Autoscan");
    SetTitleLabelVisible(false);

    for(int i = 0; i <= NL_CONTR_TERRORISM_3D_EPISODE_3; i++)
    {
        m_gameComboBox->addItem(nlGet3DGameName(static_cast<NETLizard_Game>(i)), QVariant(i));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_levelSpinBox->setMinimum(-1);
    m_levelSpinBox->setValue(-1);
    m_openLvlButton = new QPushButton;
    connect(m_openLvlButton, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    m_openLvlButton->setText("lvl/dm/track file");
    m_openLvlButton->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(m_openLvlButton);
    AddTool();

    m_openResourcePathButton = new QPushButton;
    connect(m_openResourcePathButton, SIGNAL(clicked()), this, SLOT(OpenResourceDirChooser()));
    m_openResourcePathButton->setText("Resource path");
    m_openResourcePathButton->setShortcut(QKeySequence::fromString("ctrl+r"));
    AddTool(m_openResourcePathButton);
    AddTool();

    AddTool(new QLabel("Game: "));
    AddTool(m_gameComboBox);
    AddTool();
    AddTool(new QLabel("Level: "));
    AddTool(m_levelSpinBox);
    AddTool(m_autoscanCheckBox);
    connect(m_autoscanCheckBox, SIGNAL(clicked(bool)), this, SLOT(SetAutoscan(bool)));

    AddTool();
    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    button->setText("Load");
    button->setShortcut(QKeySequence::fromString("ctrl+o"));
    AddTool(button);

    connect(m_gameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));

    connect(m_mapScene, SIGNAL(currentSceneChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentCollisionItemChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentViewItemChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentViewSceneChanged(int)), this, SLOT(UpdateSceneInfo()));

    SetCentralWidget(m_mapScene);
    SetTitle("NETLizard 3D FPS map viewer");
}

void MapViewer::OpenFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        m_fileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetLvlFile(const QString &)));
    }

    m_fileChooser->exec();
}

void MapViewer::OpenResourceDirChooser()
{
    if(!m_resourceDirChooser)
    {
        m_resourceDirChooser = new QFileDialog(this);
        m_resourceDirChooser->setFileMode(QFileDialog::Directory);
        connect(m_resourceDirChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetResourceDirPath(const QString &)));
    }

    m_resourceDirChooser->exec();
}

void MapViewer::SetLvlFile(const QString &file)
{
    if(m_lvlPath != file)
    {
        m_lvlPath = file;
        m_openLvlButton->setToolTip(m_lvlPath);
        //m_resourceDirPath.clear();
    }
}

void MapViewer::SetResourceDirPath(const QString &file)
{
    if(m_resourceDirPath != file)
    {
        m_resourceDirPath = file;
        m_openResourcePathButton->setToolTip(m_resourceDirPath);
    }
}

void MapViewer::OnTypeCurrentIndexChanged(int index)
{
    //m_levelSpinBox->setEnabled(index == 2);
}

void MapViewer::Reset()
{
    BaseViewer::Reset();
    m_mapScene->Reset();
    if(m_autoscan)
        m_levelSpinBox->setValue(-1);
}

bool MapViewer::OpenFile()
{
    if(m_lvlPath.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose lvl file and resource path!");
        return false;
    }
    Reset();

    int selectedIndex = m_gameComboBox->currentIndex();
    int game = selectedIndex + NL_RACING_EVOLUTION_3D;
    int level = m_levelSpinBox->value();
    if(m_resourceDirPath.isEmpty())
    {
        QFileInfo info(m_lvlPath);
        SetResourceDirPath(info.absolutePath());
    }
    if(level == -1)
    {
        QFileInfo info(m_lvlPath);
        QRegExp regexp("lvl(\\d+)");
        int pos = regexp.indexIn(info.fileName());
        if(pos != -1)
        {
            level = regexp.cap(1).toInt();
            m_levelSpinBox->setValue(level);
        }
        // Egypt 3D survive mode: dm1.png
        regexp.setPattern("dm(\\d+)");
        pos = regexp.indexIn(info.fileName());
        if(pos != -1)
        {
            level = regexp.cap(1).toInt();
            m_levelSpinBox->setValue(level);
        }
    }
    bool res = false;
    switch(game)
    {
        case NL_CONTR_TERRORISM_3D:
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
    case NL_ARMY_RANGER_3D:
    case NL_SHADOW_OF_EGYPT_3D:
    case NL_CLONE_3D:
    case NL_CONTR_TERRORISM_3D_EPISODE_3:
    case NL_RACING_EVOLUTION_3D:
        res = m_mapScene->LoadFile(m_lvlPath, m_resourceDirPath, game, level);
    break;
    default:
        QMessageBox::warning(this, "Error", "Unsupport 3D game!");
        break;
    }
    SetTitleLabel(QString("%1(level-%2)  lvl: %3, resource directory: %4 -> %5").arg(nlGet3DGameName(static_cast<NETLizard_Game>(game))).arg(level).arg(m_lvlPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_mapScene->setFocus();
        //m_mapScene->SetHideMouse(true);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load 3D game map file fail!");
    }
    return res;
}

void MapViewer::UpdateSceneInfo()
{
    QString str = QString("Collision(Scene: %1, Item: %2) View(Scene: %3, Item: %4)")
            .arg(m_mapScene->CurrentScene())
            .arg(m_mapScene->CurrentCollisionItem())
            .arg(m_mapScene->CurrentViewScene())
            .arg(m_mapScene->CurrentViewItem())
            ;
    SetStatusText(str);
}

void MapViewer::SetAutoscan(bool b)
{
    if(m_autoscan != b)
    {
        m_autoscan = b;
        if(m_autoscan)
            m_levelSpinBox->setValue(-1);
        m_levelSpinBox->setEnabled(!m_autoscan);
        if(m_autoscanCheckBox->isChecked() != m_autoscan)
            m_autoscanCheckBox->setChecked(m_autoscan);
    }
}
