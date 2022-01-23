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
    m_autoscanLevel(false),
    m_autoscanLevelCheckBox(0),
    m_autoscanResource(false),
    m_autoscanResourceCheckBox(0)
{
    setObjectName("MapViewer");
    Init();
    Settings *setting = SINGLE_INSTANCE_OBJ(Settings);
    SetAutoscanLevel(setting->GetSetting<bool>("APP/autoscan_level_and_index", false));
    SetAutoscanResourcePath(setting->GetSetting<bool>("APP/autoscan_resource_path", false));
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
    m_levelSpinBox = new QComboBox;
    m_autoscanLevelCheckBox = new QCheckBox(tr("Autoscan"));
    m_autoscanResourceCheckBox = new QCheckBox(tr("Autoscan"));
    SetTitleLabelVisible(false);

    for(int i = 0; i <= NL_CONTR_TERRORISM_3D_EPISODE_3; i++)
    {
        m_gameComboBox->addItem(nlGet3DGameName(static_cast<NETLizard_Game>(i)), QVariant(i));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_openLvlButton = new QPushButton;
    connect(m_openLvlButton, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    m_openLvlButton->setText(tr("lvl/dm/track file"));
    m_openLvlButton->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(m_openLvlButton);
    AddTool();

    m_openResourcePathButton = new QPushButton;
    connect(m_openResourcePathButton, SIGNAL(clicked()), this, SLOT(OpenResourceDirChooser()));
    m_openResourcePathButton->setText(tr("Resource path"));
    m_openResourcePathButton->setShortcut(QKeySequence::fromString("ctrl+r"));
    AddTool(m_openResourcePathButton);
    AddTool(m_autoscanResourceCheckBox);
    connect(m_autoscanResourceCheckBox, SIGNAL(clicked(bool)), this, SLOT(SetAutoscanResourcePath(bool)));
    AddTool();

    SetGameLevels(0);
    AddTool(new QLabel(tr("Game: ")));
    AddTool(m_gameComboBox);
    AddTool();
    AddTool(new QLabel(tr("Level: ")));
    AddTool(m_levelSpinBox);
    AddTool(m_autoscanLevelCheckBox);
    connect(m_autoscanLevelCheckBox, SIGNAL(clicked(bool)), this, SLOT(SetAutoscanLevel(bool)));

    AddTool();
    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    button->setText(tr("Load"));
    button->setShortcut(QKeySequence::fromString("ctrl+o"));
    AddTool(button);

    connect(m_gameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetGameLevels(int)));

    connect(m_mapScene, SIGNAL(currentSceneChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentCollisionItemChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentViewItemChanged(int)), this, SLOT(UpdateSceneInfo()));
    connect(m_mapScene, SIGNAL(currentViewSceneChanged(int)), this, SLOT(UpdateSceneInfo()));

    m_mapScene->setMinimumSize(128, 128);
    SetCentralWidget(m_mapScene);
    SetTitle(tr("NETLizard 3D map model viewer"));
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

    if(!m_resourceDirPath.isEmpty())
        m_resourceDirChooser->setDirectory(m_resourceDirPath);
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

void MapViewer::SetGameLevels(int index)
{
    m_levelSpinBox->clear();
    m_levelSpinBox->addItem("<-- " + tr("Autoscan") + " -->", -1);
    int selectedIndex = m_gameComboBox->itemData(index).toInt();
    NETLizard_Game game = static_cast<NETLizard_Game>(selectedIndex + NL_RACING_EVOLUTION_3D);
    NLint start = 0;
    NLint count = 0;
    NLboolean res = nlGet3DGameLevelRange(game, &start, &count);
    if(!res)
        return;
    for(int i = 0; i < count; i++)
    {
        const int index = i + start;
        const char *name = nlGet3DGameLevelName(game, index);
        if(name)
            m_levelSpinBox->addItem(QString("%1.%2").arg(index).arg(name), index);
    }
}

void MapViewer::Reset()
{
    BaseViewer::Reset();
    m_mapScene->Reset();
    if(m_autoscanLevel)
        m_levelSpinBox->setCurrentIndex(0);
    if(m_autoscanResource)
        m_resourceDirPath.clear();
}

bool MapViewer::OpenFile()
{
    if(m_lvlPath.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose lvl/dm/track file and resource path!"));
        return false;
    }
    Reset();

    int selectedIndex = m_gameComboBox->currentIndex();
    int game = selectedIndex + NL_RACING_EVOLUTION_3D;
    int level = m_levelSpinBox->itemData(m_levelSpinBox->currentIndex()).toInt();
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
            m_levelSpinBox->setCurrentIndex(m_levelSpinBox->findData(level));
        }
        // Egypt 3D survive mode: dm1.png
        regexp.setPattern("dm(\\d+)");
        pos = regexp.indexIn(info.fileName());
        if(pos != -1)
        {
            level = regexp.cap(1).toInt();
            m_levelSpinBox->setCurrentIndex(17 + m_levelSpinBox->findData(level));
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
        QMessageBox::warning(this, tr("Error"), tr("Unsupport NETLizard 3D game!"));
        break;
    }
    SetTitleLabel(QString(tr("%1(level-%2) object: %3, resource directory: %4 -> %5")).arg(nlGet3DGameName(static_cast<NETLizard_Game>(game))).arg(level).arg(m_lvlPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_mapScene->setFocus();
        //m_mapScene->SetHideMouse(true);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Load NETLizard 3D game map model file fail!"));
    }
    return res;
}

void MapViewer::UpdateSceneInfo()
{
    QString str = QString(tr("Collision(Scene: %1, Item: %2) View(Scene: %3, Item: %4)"))
            .arg(m_mapScene->CurrentScene())
            .arg(m_mapScene->CurrentCollisionItem())
            .arg(m_mapScene->CurrentViewScene())
            .arg(m_mapScene->CurrentViewItem())
            ;
    SetStatusText(str);
}

void MapViewer::SetAutoscanLevel(bool b)
{
    if(m_autoscanLevel != b)
    {
        m_autoscanLevel = b;
        if(m_autoscanLevel)
            m_levelSpinBox->setCurrentIndex(0);
        m_levelSpinBox->setEnabled(!m_autoscanLevel);
        if(m_autoscanLevelCheckBox->isChecked() != m_autoscanLevel)
            m_autoscanLevelCheckBox->setChecked(m_autoscanLevel);
    }
}

void MapViewer::SetAutoscanResourcePath(bool b)
{
    if(m_autoscanResource != b)
    {
        m_autoscanResource = b;
        if(m_autoscanResource)
            m_resourceDirPath.clear();
        m_openResourcePathButton->setEnabled(!m_autoscanResource);
        if(m_autoscanResourceCheckBox->isChecked() != m_autoscanResource)
            m_autoscanResourceCheckBox->setChecked(m_autoscanResource);
    }
}
