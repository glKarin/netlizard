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
    m_cFileChooser(0),
    m_colorChooser(0),
    m_openCfButton(0),
    m_openFntButton(0)
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
    QHBoxLayout *toolLayout = ToolLayout();

    m_indexList = new QListWidget;
    layout->addWidget(m_indexList);
    m_spriteScene = new SpriteScene;
    //m_spriteScene->setMinimumWidth(320);
    layout->addWidget(m_spriteScene, 1);

    m_openCfButton = new QPushButton;
    m_openCfButton->setText("config file(cu.png)");
    toolLayout->addWidget(m_openCfButton);
    connect(m_openCfButton, SIGNAL(clicked()), this, SLOT(OpenCfFileChooser()));
    m_openFntButton = new QPushButton(this);
    m_openFntButton->setText("image file(cxx.png)");
    connect(m_openFntButton, SIGNAL(clicked()), this, SLOT(OpenFntFileChooser()));
    toolLayout->addWidget(m_openFntButton);
    toolLayout->addStretch();
    button = new QPushButton;
    button->setText("background color");
    connect(button, SIGNAL(clicked()), this, SLOT(OpenBackgroundColorChooser()));
    toolLayout->addWidget(button);
    button = new QPushButton;
    button->setText("Load");
    connect(button, SIGNAL(clicked()), this, SLOT(LoadSprite()));
    toolLayout->addWidget(button);
    connect(m_indexList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(RenderIndex(QListWidgetItem *)));

    CentralWidget()->setLayout(layout);
    SetTitle("NETLizard sprite viewer");
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

void SpriteViewer::OpenCfFileChooser()
{
    if(!m_cuFileChooser)
    {
        m_cuFileChooser = new QFileDialog(this);
        m_cuFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_cuFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetCuFile(const QString &)));
    }

    m_cuFileChooser->exec();
}

void SpriteViewer::OpenFntFileChooser()
{
    if(!m_cFileChooser)
    {
        m_cFileChooser = new QFileDialog(this);
        m_cFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_cFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetCFile(const QString &)));
    }

    m_cFileChooser->exec();
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
        m_openCfButton->setToolTip(m_cuFile);
    }
}

void SpriteViewer::SetCFile(const QString &file)
{
    if(m_cFile != file)
    {
        m_cFile = file;
        m_openFntButton->setToolTip(m_cFile);
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
    if(m_cFile.isEmpty())
    {
        QFileInfo info(m_cuFile);
        SetCFile(info.absolutePath() + "/cuu.png");
    }
    bool res = m_spriteScene->LoadFile(m_cuFile, m_cFile);
    if(res)
    {
        const int Co = m_spriteScene->Count();
        for(int i = 0; i < Co; i++)
            m_indexList->addItem(QString::number(i));
        SetTitleLabel(QString("config file: %1, texture file: %2").arg(m_cuFile).arg(m_cFile));
        QMessageBox::warning(this, "Success", "Load sprite success!");
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
