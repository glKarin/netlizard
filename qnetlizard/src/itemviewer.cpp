#include "itemviewer.h"

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

#include "itemscene.h"
#include "netlizard.h"
#include "qdef.h"

ItemViewer::ItemViewer(QWidget *parent) :
    BaseViewer(parent),
    m_itemScene(0),
    m_gameComboBox(0),
    m_fileChooser(0),
    m_resourceDirChooser(0),
    m_indexSpinBox(0),
    m_colorChooser(0),
    m_openObjButton(0),
    m_openResourcePathButton(0)
{
    setObjectName("ItemViewer");
    Init();
}

ItemViewer::~ItemViewer()
{
}

void ItemViewer::Init()
{
    QPushButton *button;
    m_gameComboBox = new QComboBox;
    m_itemScene = new ItemScene;
    QHBoxLayout *toolLayout = ToolLayout();
    m_indexSpinBox = new QSpinBox;

    for(int i = 0; i <= NL_CONTR_TERRORISM_3D_EPISODE_3; i++)
    {
        m_gameComboBox->addItem(nlGet3DGameName(static_cast<NETLizard_Game>(i)), QVariant(i));
    }
    m_gameComboBox->setMaximumWidth(180);
    m_indexSpinBox->setMinimum(-1);
    m_indexSpinBox->setValue(-1);
    m_openObjButton = new QPushButton;
    connect(m_openObjButton, SIGNAL(clicked()), this, SLOT(OpenObjFileChooser()));
    m_openObjButton->setText("obj/o/car file");
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

    SetCentralWidget(m_itemScene);
    SetTitle("NETLizard 3D FPS item viewer");
}

void ItemViewer::OpenBackgroundColorChooser()
{
    if(!m_colorChooser)
    {
        m_colorChooser = new QColorDialog(this);
        connect(m_colorChooser, SIGNAL(colorSelected(const QColor &)), this, SLOT(SetBackgroundColor(const QColor &)));
    }
    m_colorChooser->setCurrentColor(m_itemScene->ClearColor());

    m_colorChooser->exec();
}

void ItemViewer::OpenObjFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        m_fileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetObjFile(const QString &)));
    }

    m_fileChooser->exec();
}

void ItemViewer::OpenResourceDirChooser()
{
    if(!m_resourceDirChooser)
    {
        m_resourceDirChooser = new QFileDialog(this);
        m_resourceDirChooser->setFileMode(QFileDialog::Directory);
        connect(m_resourceDirChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetResourceDirPath(const QString &)));
    }

    m_resourceDirChooser->exec();
}

void ItemViewer::SetObjFile(const QString &file)
{
    if(m_objPath != file)
    {
        m_objPath = file;
        m_openObjButton->setToolTip(m_objPath);
        //m_resourceDirPath.clear();
    }
}

void ItemViewer::SetResourceDirPath(const QString &file)
{
    if(m_resourceDirPath != file)
    {
        m_resourceDirPath = file;
        m_openResourcePathButton->setToolTip(m_resourceDirPath);
    }
}

void ItemViewer::SetBackgroundColor(const QColor &color)
{
    m_itemScene->SetClearColor(color);
}

void ItemViewer::OnTypeCurrentIndexChanged(int index)
{
    //m_levelSpinBox->setEnabled(index == 2);
}

bool ItemViewer::OpenFile()
{
    if(m_objPath.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose obj file and resource path!");
        return false;
    }
    m_itemScene->Reset();
    int selectedIndex = m_gameComboBox->currentIndex();
    int game = selectedIndex + NL_RACING_EVOLUTION_3D;
    int index = m_indexSpinBox->value();
    if(m_resourceDirPath.isEmpty())
    {
        QFileInfo info(m_objPath);
        SetResourceDirPath(info.absolutePath());
    }
    if(index == -1)
    {
        QFileInfo info(m_objPath);
        QRegExp regexp("ob?j?(\\d+)");
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
        case NL_CONTR_TERRORISM_3D:
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
    case NL_ARMY_RANGER_3D:
    case NL_SHADOW_OF_EGYPT_3D:
    //case NL_CLONE_3D:
    case NL_CONTR_TERRORISM_3D_EPISODE_3:
    case NL_RACING_EVOLUTION_3D:
        res = m_itemScene->LoadFile(m_objPath, m_resourceDirPath, game, index);
    break;
    default:
        QMessageBox::warning(this, "Error", "Unsupport 3D game!");
        break;
    }
    SetTitleLabel(QString("%1(index-%2)  obj: %3, resource directory: %4 -> %5").arg(nlGet3DGameName(static_cast<NETLizard_Game>(game))).arg(index).arg(m_objPath).arg(m_resourceDirPath).arg(res ? "Success" : "Fail"));
    if(res)
    {
        m_itemScene->setFocus();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load 3D game map file fail!");
    }
    return res;
}
