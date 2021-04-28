#include "imageviewer.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "imagewidget.h"
#include "netlizard.h"

static const QPair<QString, QString> Types[] = {
    QPair<QString, QString>("png", "PNG"),
    QPair<QString, QString>("texture_v2", "Texture v2"),
    QPair<QString, QString>("texture_v3", "Texture v3"),
    QPair<QString, QString>("texture_v3_compress", "Texture v3 compress"),
};

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    m_imageWidget(0),
    m_typeComboBox(0),
    m_fileChooser(0),
    m_indexSpinBox(0),
    m_titleLabel(0)
{
    Init();
}

ImageViewer::~ImageViewer()
{

}

void ImageViewer::Init()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button = new QPushButton(this);
    m_typeComboBox = new QComboBox(this);
    m_imageWidget = new ImageWidget(this);
    QHBoxLayout *toolLayout = new QHBoxLayout(this);
    m_indexSpinBox = new QSpinBox(this);
    m_titleLabel = new QLabel(this);

    for(int i = 0; i < 4; i++)
    {
        const QPair<QString, QString> &p = Types[i];
        m_typeComboBox->addItem(p.second, QVariant(p.first));
    }
    m_indexSpinBox->setMinimum(-1);
    m_indexSpinBox->setEnabled(false);
    m_indexSpinBox->setValue(-1);
    button->setText("Open file");
    toolLayout->addWidget(button);
    toolLayout->addStretch();
    toolLayout->addWidget(new QLabel("Type: ", this));
    toolLayout->addWidget(m_typeComboBox);
    toolLayout->addWidget(new QLabel("Index: ", this));
    toolLayout->addWidget(m_indexSpinBox);

    m_titleLabel->setFixedHeight(18);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_imageWidget);
    layout->addItem(toolLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));

    setLayout(layout);
    setWindowTitle("NETLizard image/texture resource viewer");
    resize(480, 360);
}

void ImageViewer::OpenFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(OpenFile(const QString &)));
    }

    m_fileChooser->exec();
}

void ImageViewer::OnTypeCurrentIndexChanged(int index)
{
    m_indexSpinBox->setEnabled(index == 2);
}

void ImageViewer::OpenFile(const QString &file)
{
    m_titleLabel->setText("");
    int selectedIndex = m_typeComboBox->currentIndex();
    QString type = Types[selectedIndex].first;

    if(type == "png")
    {
        NLint len;
        char *data = nlHandlePNG_File2Memory(file.toStdString().c_str(), &len);
        if(!data)
        {
            QMessageBox::warning(this, "Error", "Load NETLizard png image fail!");
            return;
        }

        bool res = m_imageWidget->LoadImage((uchar *)data, len);
        if(!res)
        {
            QMessageBox::warning(this, "Error", "Load image data fail!");
            return;
        }
        free(data);
        m_titleLabel->setText(QString("PNG image: %1").arg(len));
    }
    else if(type == "texture_v2")
    {
        NETLizard_Texture tex;
        NLboolean ok = nlReadTextureV2_File(file.toStdString().c_str(), &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "Error", "Load NETLizard v2 texture fail!");
            return;
        }

        m_titleLabel->setText(QString("Texture v2: %1x%2(%3)").arg(tex.width).arg(tex.height).arg(tex.format == NL_RGB ? "RGB" : "RGBA"));
        NLuchar *data = nlMakeOpenGLTextureData(&tex, NULL);
        bool res = tex.format == NL_RGB ? m_imageWidget->LoadImage((uchar *)data, tex.width, tex.height, QImage::Format_RGB888) : m_imageWidget->LoadImage((uchar *)data, tex.width, tex.height);
        delete_NETLizard_Texture(&tex);
        if(!res)
        {
            QMessageBox::warning(this, "Error", "Load image data fail!");
            return;
        }
    }
    else if(type == "texture_v3")
    {
        int index = m_indexSpinBox->value();
        NETLizard_Texture tex;
        NLboolean ok = nlReadTextureV3_File(file.toStdString().c_str(), index, &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "Error", "Load NETLizard v3 texture image fail!");
            return;
        }

        m_titleLabel->setText(QString("Texture v3: %1x%2(%3)").arg(tex.width).arg(tex.height).arg(tex.format == NL_RGB ? "RGB" : "RGBA"));
        NLuchar *data = nlMakeOpenGLTextureData(&tex, NULL);
        bool res = tex.format == NL_RGB ? m_imageWidget->LoadImage((uchar *)data, tex.width, tex.height, QImage::Format_RGB888) : m_imageWidget->LoadImage((uchar *)data, tex.width, tex.height);
        delete_NETLizard_Texture(&tex);
        if(!res)
        {
            free(data);
            QMessageBox::warning(this, "Error", "Load image data fail!");
            return;
        }
    }
    else if(type == "texture_v3_compress") // RGBA
    {
        NETLizard_Texture tex;
        NLboolean ok = nlReadCompressTextureV3_File(file.toStdString().c_str(), &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "Error", "Load NETLizard v3 compress texture fail!");
            return;
        }

        m_titleLabel->setText(QString("Texture v3 compress: %1x%2(RGBA)").arg(tex.width).arg(tex.height));
        NLuchar *data = nlMakeOpenGLTextureDataRGBA(&tex, NULL);
        delete_NETLizard_Texture(&tex);
        bool res = m_imageWidget->LoadImage(data, tex.width, tex.height);
        if(!res)
        {
            free(data);
            QMessageBox::warning(this, "Error", "Load image data fail!");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unsupport resource type!");
    }
}
