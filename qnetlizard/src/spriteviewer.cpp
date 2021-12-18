#include "spriteviewer.h"

#include <QDebug>

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QColorDialog>

#include "spritescene.h"
#include "qdef.h"

SpriteViewer::SpriteViewer(QWidget *parent) :
    BaseViewer(parent),
    m_spriteScene(0),
    m_indexList(0),
    m_cuFileChooser(0),
    m_uFileChooser(0),
    m_colorChooser(0),
    m_openCuButton(0),
    m_openUButton(0)
{
    setObjectName("SpriteViewer");
    Init();
}

SpriteViewer::~SpriteViewer()
{
    m_spriteScene->Reset();
}

void SpriteViewer::Init()
{
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *button;
    //QHBoxLayout *toolLayout = ToolLayout();
    SetTitleLabelVisible(false);

    m_indexList = new QListWidget;
    layout->addWidget(m_indexList);
    m_spriteScene = new SpriteScene;
    layout->addWidget(m_spriteScene, 1);

    // cu - u0
    // ccc - ccu
    m_openCuButton = new QPushButton;
    m_openCuButton->setText(tr("config file(cu|ccc.png)"));
    m_openCuButton->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(m_openCuButton);
    connect(m_openCuButton, SIGNAL(clicked()), this, SLOT(openCuFileChooser()));
    m_openUButton = new QPushButton(this);
    m_openUButton->setText(tr("image file(u0|ccu.png)"));
    m_openUButton->setShortcut(QKeySequence::fromString("ctrl+r"));
    connect(m_openUButton, SIGNAL(clicked()), this, SLOT(OpenUFileChooser()));
    AddTool(m_openUButton);
    AddTool();
    button = new QPushButton;
    button->setText(tr("Background color"));
    connect(button, SIGNAL(clicked()), this, SLOT(OpenBackgroundColorChooser()));
    AddTool(button);
    button = new QPushButton;
    button->setText(tr("Load"));
    button->setShortcut(QKeySequence::fromString("ctrl+o"));
    connect(button, SIGNAL(clicked()), this, SLOT(LoadSprite()));
    AddTool(button);
    connect(m_indexList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(RenderIndex(QListWidgetItem *)));

    CentralWidget()->setLayout(layout);
    SetTitle(tr("NETLizard sprite resource viewer"));
}

void SpriteViewer::OpenBackgroundColorChooser()
{
    if(!m_colorChooser)
    {
        m_colorChooser = new QColorDialog(this);
        connect(m_colorChooser, SIGNAL(colorSelected(const QColor &)), this, SLOT(SetBackgroundColor(const QColor &)));
    }
    m_colorChooser->setCurrentColor(m_spriteScene->ClearColor());

    m_colorChooser->exec();
}

void SpriteViewer::openCuFileChooser()
{
    if(!m_cuFileChooser)
    {
        m_cuFileChooser = new QFileDialog(this);
        m_cuFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_cuFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetCuFile(const QString &)));
    }

    m_cuFileChooser->exec();
}

void SpriteViewer::OpenUFileChooser()
{
    if(!m_uFileChooser)
    {
        m_uFileChooser = new QFileDialog(this);
        m_uFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_uFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetCFile(const QString &)));
    }

    m_uFileChooser->exec();
}

void SpriteViewer::SetBackgroundColor(const QColor &color)
{
    m_spriteScene->SetClearColor(color);
}

void SpriteViewer::SetCuFile(const QString &file)
{
    if(m_cuFile != file)
    {
        m_cuFile = file;
        m_openCuButton->setToolTip(m_cuFile);
    }
}

void SpriteViewer::SetCFile(const QString &file)
{
    if(m_uFile != file)
    {
        m_uFile = file;
        m_openUButton->setToolTip(m_uFile);
    }
}

bool SpriteViewer::LoadSprite()
{
    if(m_cuFile.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Choose sprite config file and texture file!");
        return false;
    }
    Reset();
    if(m_uFile.isEmpty())
    {
        QFileInfo info(m_cuFile);
        SetCFile(info.absolutePath() + "/cuu.png");
    }
    bool res = m_spriteScene->LoadFile(m_cuFile, m_uFile);
    if(res)
    {
        const int Co = m_spriteScene->Count();
        for(int i = 0; i < Co; i++)
            m_indexList->addItem(QString::number(i));
        SetTitleLabel(QString("config file: %1, texture file: %2 -> sprite count: %3").arg(m_cuFile).arg(m_uFile).arg(m_spriteScene->Sprite()->sprite_count));
    }
    else
    {
        QMessageBox::warning(this, "Error", "Load sprite fail!");
    }
    return res;
}

bool SpriteViewer::RenderIndex(QListWidgetItem *item)
{
    if(!m_spriteScene->IsValid())
        return false;
    const int Index = item->text().toInt();
    if(Index < 0)
        return false;
    m_spriteScene->SetIndex(Index);
    return true;
}

void SpriteViewer::Reset()
{
    BaseViewer::Reset();
    m_spriteScene->Reset();
    m_indexList->clear();
}

NLScene * SpriteViewer::Scene()
{
    return m_spriteScene;
}
