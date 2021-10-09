#include "mapviewer.h"

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

#include "mapwidget.h"
#include "netlizard.h"
#include "qdef.h"

static const QPair<int, QString> Types[] = {
    QPair<int, QString>(NL_CONTR_TERRORISM_3D, "3D Contr Terrorism"),
    QPair<int, QString>(NL_ARMY_RANGER_3D, "3D Army Ranger"),
    QPair<int, QString>(NL_CONTR_TERRORISM_3D_EPISODE_2, "3D Contr Terrorism : Episode-2"),
    QPair<int, QString>(NL_SHADOW_OF_EGYPT_3D, "3D Shadows of Egypt"),
    QPair<int, QString>(NL_CLONE_3D, "3D Clone"),
    QPair<int, QString>(NL_CONTR_TERRORISM_3D_EPISODE_3, "3D Contr Terrorism : Episode-3"),
};

MapViewer::MapViewer(QWidget *parent) :
    BaseViewer(parent),
    m_mapWidget(0),
    m_gameComboBox(0),
    m_fileChooser(0),
    m_resourceDirChooser(0),
    m_levelSpinBox(0),
    m_openLvlButton(0),
    m_openResourcePathButton(0)
{
    setObjectName("MapViewer");
    Init();
}

MapViewer::~MapViewer()
{
}

void MapViewer::Init()
{
    QPushButton *button;
    m_gameComboBox = new QComboBox;
    m_mapWidget = new MapWidget;
    QHBoxLayout *toolLayout = ToolLayout();
    m_levelSpinBox = new QSpinBox;

    for(int i = 0; i < 6; i++)
    {
        const QPair<int, QString> &p = Types[i];
        m_gameComboBox->addItem(p.second, QVariant(p.first));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_levelSpinBox->setMinimum(-1);
    m_levelSpinBox->setValue(-1);
    m_openLvlButton = new QPushButton;
    connect(m_openLvlButton, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    m_openLvlButton->setText("lvl/dm file");
    toolLayout->addWidget(m_openLvlButton);
    toolLayout->addStretch();

    m_openResourcePathButton = new QPushButton;
    connect(m_openResourcePathButton, SIGNAL(clicked()), this, SLOT(OpenResourceDirChooser()));
    m_openResourcePathButton->setText("resource path");
    toolLayout->addWidget(m_openResourcePathButton);
    toolLayout->addStretch();

    toolLayout->addWidget(new QLabel("Game: "));
    toolLayout->addWidget(m_gameComboBox);
    toolLayout->addStretch();
    toolLayout->addWidget(new QLabel("Level: "));
    toolLayout->addWidget(m_levelSpinBox);

    toolLayout->addStretch();
    button = new QPushButton;
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    button->setText("Load");
    toolLayout->addWidget(button);

    connect(m_gameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));

    SetCentralWidget(m_mapWidget);
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

bool MapViewer::OpenFile()
{
    if(m_lvlPath.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose lvl file and resource path!");
        return false;
    }
    m_mapWidget->Reset();
    int selectedIndex = m_gameComboBox->currentIndex();
    int game = Types[selectedIndex].first;
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
        res = m_mapWidget->LoadFile(m_lvlPath, m_resourceDirPath, game, level);
    break;
    default:
        QMessageBox::warning(this, "Error", "Unsupport 3D game!");
        break;
    }
    SetTitleLabel(QString("%1(level-%2)  lvl: %3, resource directory: %4 -> %5").arg(Types[selectedIndex].second).arg(level).arg(m_lvlPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_mapWidget->setFocus();
        //m_mapWidget->SetHideMouse(true);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load 3D game map file fail!");
    }
    return res;
}
