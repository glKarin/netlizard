#include "fontviewer.h"

#include <QDebug>

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QColorDialog>

#include "gl/nl_gl.h"
#include "fontscene.h"
#include "qdef.h"

FontViewer::FontViewer(QWidget *parent) :
    BaseViewer(parent),
    m_textInput(0),
    m_fontScene(0),
    m_renderButton(0),
    m_cfFileChooser(0),
    m_fntFileChooser(0),
    m_colorChooser(0),
    m_openCfButton(0),
    m_openFntButton(0)
{
    setObjectName("FontViewer");
    Init();
}

FontViewer::~FontViewer()
{
    m_fontScene->Reset();
}

void FontViewer::Init()
{
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *button;
    QHBoxLayout *toolLayout = ToolLayout();
    QVBoxLayout *vLayout;
    SetTitleLabelVisible(false);

    vLayout = new QVBoxLayout;
    vLayout->addStretch();
    m_renderButton = new QPushButton;
    m_renderButton->setText(">>");
    m_renderButton->setShortcut(QKeySequence::fromString("ctrl+d"));
    m_renderButton->setEnabled(false);
    m_renderButton->setMaximumWidth(32);
    connect(m_renderButton, SIGNAL(clicked()), this, SLOT(RenderString()));
    vLayout->addWidget(m_renderButton);
    vLayout->addStretch();

    m_textInput = new QPlainTextEdit;
    m_textInput->setEnabled(false);
    layout->addWidget(m_textInput);
    layout->addLayout(vLayout);
    m_fontScene = new FontScene;
    //m_fontScene->setMinimumWidth(320);
    layout->addWidget(m_fontScene, 1);

    m_openCfButton = new QPushButton;
    m_openCfButton->setText(tr("config file(cf.png)"));
    m_openCfButton->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(m_openCfButton);
    connect(m_openCfButton, SIGNAL(clicked()), this, SLOT(OpenCfFileChooser()));
    m_openFntButton = new QPushButton(this);
    m_openFntButton->setText(tr("image file(fnt.png)"));
    m_openFntButton->setShortcut(QKeySequence::fromString("ctrl+r"));
    connect(m_openFntButton, SIGNAL(clicked()), this, SLOT(OpenFntFileChooser()));
    AddTool(m_openFntButton);
    toolLayout->addStretch();
    button = new QPushButton;
    button->setText(tr("Background color"));
    connect(button, SIGNAL(clicked()), this, SLOT(OpenBackgroundColorChooser()));
    AddTool(button);
    button = new QPushButton;
    button->setText(tr("Load"));
    button->setShortcut(QKeySequence::fromString("ctrl+o"));
    connect(button, SIGNAL(clicked()), this, SLOT(LoadFont()));
    AddTool(button);

    m_fontScene->setMinimumWidth(72);
    connect(m_fontScene, SIGNAL(propertyChanged(const QString &, const QVariant &, int)), this, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
    CentralWidget()->setLayout(layout);
    SetTitle(tr("NETLizard font resource viewer"));
}

void FontViewer::OpenBackgroundColorChooser()
{
    if(!m_colorChooser)
    {
        m_colorChooser = new QColorDialog(this);
        connect(m_colorChooser, SIGNAL(colorSelected(const QColor &)), this, SLOT(SetBackgroundColor(const QColor &)));
    }
    m_colorChooser->setCurrentColor(m_fontScene->ClearColor());

    m_colorChooser->exec();
}

void FontViewer::OpenCfFileChooser()
{
    if(!m_cfFileChooser)
    {
        m_cfFileChooser = new QFileDialog(this);
        m_cfFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_cfFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetCfFile(const QString &)));
    }

    m_cfFileChooser->exec();
}

void FontViewer::OpenFntFileChooser()
{
    if(!m_fntFileChooser)
    {
        m_fntFileChooser = new QFileDialog(this);
        m_fntFileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fntFileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SetFntFile(const QString &)));
    }

    m_fntFileChooser->exec();
}

void FontViewer::SetBackgroundColor(const QColor &color)
{
    m_fontScene->SetClearColor(color);
}

void FontViewer::SetCfFile(const QString &file)
{
    if(m_cfFile != file)
    {
        m_cfFile = file;
        m_openCfButton->setToolTip(m_cfFile);
    }
}

void FontViewer::SetFntFile(const QString &file)
{
    if(m_fntFile != file)
    {
        m_fntFile = file;
        m_openFntButton->setToolTip(m_fntFile);
    }
}

bool FontViewer::LoadFont()
{
    if(m_cfFile.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose font config file and texture file!"));
        return false;
    }
    Reset();
    if(m_fntFile.isEmpty())
    {
        QFileInfo info(m_cfFile);
        SetFntFile(info.absolutePath() + "/fnt.png");
    }
    bool res = m_fontScene->LoadFile(m_cfFile, m_fntFile);
    if(res)
    {
        m_renderButton->setEnabled(true);
        m_textInput->setEnabled(true);
        SetTitleLabel(QString(tr("config file: %1, texture file: %2 -> character count: %3")).arg(m_cfFile).arg(m_fntFile).arg(m_fontScene->Font()->char_count));
        QMessageBox::warning(this, tr("Success"), tr("Load NETLizard font success!"));
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Load NETLizard font fail!"));
    }
    return res;
}

bool FontViewer::RenderString()
{
    if(!m_fontScene->IsValid())
        return false;
    QString str = m_textInput->toPlainText();
    if(str.isEmpty())
        return false;
    m_fontScene->SetText(str);
    return true;
}

void FontViewer::Reset()
{
    BaseViewer::Reset();
    m_fontScene->Reset();
    m_renderButton->setEnabled(false);
    m_textInput->setEnabled(false);
}

NLScene * FontViewer::Scene()
{
    return m_fontScene;
}

void FontViewer::OnPropertyChanged(const QString &name, const QVariant &value, int type)
{
    if(name == "text" && type == 0 && m_textInput->isEnabled())
    {
        QString old = m_textInput->toPlainText();
        QString n = value.toString();
        if(old != n)
            m_textInput->setPlainText(n);
    }
}
