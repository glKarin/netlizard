#include "imageviewer.h"

#include <QVariant>
#include <QDebug>

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>

#include "utils/ioutility.h"

#include "imagescene.h"
#include "gl/nl_gl.h"
#include "netlizard.h"
#include "qdef.h"

static const QPair<int, QString> Types[] = {
    QPair<int, QString>(NL_TEXTURE_UNKNOWN, "Raw data(exam jpg, jpeg)"),
    QPair<int, QString>(NL_TEXTURE_NORMAL_PNG, "PNG(stable png file)"),
    QPair<int, QString>(NL_TEXTURE_ENCODE_PNG, "Encode PNG(encode png file)"),
    QPair<int, QString>(NL_TEXTURE_3D_ENGINE_V2, "Texture v2(3D CT, 3D Spacnaz, 3D CT2, 3D CT3)"),
    QPair<int, QString>(NL_TEXTURE_3D_ENGINE_V3, "Texture v3(3D Egypt, 3D Clone)"),
    QPair<int, QString>(NL_TEXTURE_3D_ENGINE_V3_COMPRESS, "Texture v3 compress(3D Egypt, 3D Clone)"),
};

static const QPair<Qt::Alignment, QString> Aligns[] = {
    QPair<Qt::Alignment, QString>(Qt::AlignLeft | Qt::AlignTop, "Left-Top"),
    QPair<Qt::Alignment, QString>(Qt::AlignCenter, "Center"),
    QPair<Qt::Alignment, QString>(Qt::AlignLeft | Qt::AlignBottom, "Left-Bottom"),
    QPair<Qt::Alignment, QString>(Qt::AlignTop | Qt::AlignHCenter, "Center-Top"),
    QPair<Qt::Alignment, QString>(Qt::AlignBottom | Qt::AlignHCenter, "Center-Bottom"),
    QPair<Qt::Alignment, QString>(Qt::AlignLeft | Qt::AlignVCenter, "Left-Center"),
};

ImageViewer::ImageViewer(QWidget *parent) :
    BaseViewer(parent),
    m_imageScene(0),
    m_typeComboBox(0),
    m_fileChooser(0),
    m_indexSpinBox(0),
    m_saveButton(0),
    m_saveChooser(0)
{
    setObjectName("ImageViewer");
    Init();
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::Init()
{
    QPushButton *button;
    m_typeComboBox = new QComboBox;
    m_imageScene = new ImageScene;
    //QHBoxLayout *toolLayout = ToolLayout();
    m_indexSpinBox = new QSpinBox;
    m_alignComboBox = new QComboBox;
    SetTitleLabelVisible(false);

    for(int i = 0; i < 6; i++)
    {
        const QPair<int, QString> &p = Types[i];
        m_typeComboBox->addItem(p.second, QVariant(p.first));
    }
    m_typeComboBox->setMaximumWidth(180);
    Qt::Alignment align = m_imageScene->Alignment();
    for(int i = 0; i < 6; i++)
    {
        const QPair<Qt::Alignment, QString> &p = Aligns[i];
        m_alignComboBox->addItem(p.second, QVariant(p.first));
        if(align == p.first)
            m_alignComboBox->setCurrentIndex(i);
    }
    m_alignComboBox->setMaximumWidth(100);
    m_indexSpinBox->setMinimum(-1);
    m_indexSpinBox->setEnabled(false);
    m_indexSpinBox->setValue(-1);
    AddTool(new QLabel("Type: "));
    AddTool(m_typeComboBox);
    button = new QPushButton;
    button->setText("Open file");
    button->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool(button);
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    m_saveButton = new QPushButton;
    m_saveButton->setText("Save data");
    m_saveButton->setShortcut(QKeySequence::fromString("ctrl+s"));
    m_saveButton->setEnabled(false);
    AddTool(m_saveButton);
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(OpenSaveChooser()));
    AddTool();
    AddTool(new QLabel("Index: "));
    AddTool(m_indexSpinBox);
    AddTool(new QLabel("Align: "));
    AddTool(m_alignComboBox);

    connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));
    connect(m_alignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAlignCurrentIndexChanged(int)));

    SetCentralWidget(m_imageScene);
    SetTitle("NETLizard image/texture resource viewer");
}

void ImageViewer::OpenFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        m_fileChooser->setFileMode(QFileDialog::ExistingFile);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(OpenFile(const QString &)));
    }

    m_fileChooser->exec();
}

void ImageViewer::OpenSaveChooser()
{
    if(!m_imageScene->IsValid())
        return;

    if(!m_saveChooser)
    {
        m_saveChooser = new QFileDialog(this);
        m_saveChooser->setFileMode(QFileDialog::AnyFile);
        m_saveChooser->setAcceptMode(QFileDialog::AcceptSave);
        connect(m_saveChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SaveData(const QString &)));
    }

    QString ext = m_imageScene->GetSaveTextureSuffix();
    if(!ext.isEmpty())
    {
        m_saveChooser->setNameFilter("*." + ext);
        m_saveChooser->setDefaultSuffix(ext);
    }

    m_saveChooser->exec();
}

void ImageViewer::OnTypeCurrentIndexChanged(int index)
{
    m_indexSpinBox->setEnabled(index == 2);
}

void ImageViewer::OnAlignCurrentIndexChanged(int index)
{
    Qt::Alignment align = Aligns[index].first;
    m_imageScene->SetAlignment(align);
}

bool ImageViewer::OpenFile(const QString &file)
{
    Reset();
    int selectedIndex = m_typeComboBox->currentIndex();
    int type = Types[selectedIndex].first;
    bool res = m_imageScene->LoadFile(file, type, m_indexSpinBox->value());
    if(!res)
    {
        QMessageBox::warning(this, "Error", "Load image/texture file fail!");
        return false;
    }
    m_saveButton->setEnabled(true);
    const texture_s *tex = m_imageScene->Texture();
    SetStatusText(QString("%1: size %2 x %3, format %4").arg(Types[selectedIndex].second).arg(tex->width).arg(tex->height).arg(tex->format == GL_RGB ? "RGB" : "RGBA"));

    return true;
}

void ImageViewer::Reset()
{
    BaseViewer::Reset();
    m_saveButton->setEnabled(false);
    m_imageScene->Reset();
}

bool ImageViewer::SaveData(const QString &file)
{
    if(!m_imageScene->IsValid())
    {
        QMessageBox::warning(this, "Error", "No data!");
        return false;
    }

    bool res = m_imageScene->SaveData(file);
    if(res)
        QMessageBox::information(this, "Success", "File path is " + file);
    else
        QMessageBox::warning(this, "Error", "Save data fail!");
    return res;
}
