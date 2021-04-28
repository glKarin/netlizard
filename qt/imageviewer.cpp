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
    m_indexSpinBox(0)
{
    Init();
}

ImageViewer::~ImageViewer()
{

}

void ImageViewer::Init()
{
    QLayout *layout = new QVBoxLayout(this);
    QPushButton *button = new QPushButton(this);
    m_typeComboBox = new QComboBox(this);
    m_imageWidget = new ImageWidget(this);
    QLayout *toolLayout = new QHBoxLayout(this);
    m_indexSpinBox = new QSpinBox(this);

    for(int i = 0; i < 4; i++)
    {
        const QPair<QString, QString> &p = Types[i];
        m_typeComboBox->addItem(p.second, QVariant(p.first));
    }
    m_indexSpinBox->setMinimum(0);
    m_indexSpinBox->setEnabled(false);
    button->setText("file");
    toolLayout->addWidget(button);
    toolLayout->addWidget(m_typeComboBox);
    toolLayout->addWidget(m_indexSpinBox);

    layout->addWidget(m_imageWidget);
    layout->addItem(toolLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));
    connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeCurrentIndexChanged(int)));

    setLayout(layout);
    setWindowTitle("NETLizard图片资源浏览");
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
    int selectedIndex = m_typeComboBox->currentIndex();
    QString type = Types[selectedIndex].first;
    if(type == "png")
    {
        NLint len;
        char *data = nlHandlePNG_File2Memory(file.toStdString().c_str(), &len);
        if(!data)
        {
            QMessageBox::warning(this, "错误", "加载NETLizard加密png文件失败!");
            return;
        }

        bool res = m_imageWidget->LoadImage((uchar *)data, len);
        if(!res)
        {
            QMessageBox::warning(this, "错误", "加载图像数据失败!");
            return;
        }
        free(data);
    }
    else if(type == "texture_v2")
    {
        NETLizard_Texture tex;
        NLboolean ok = nlReadTextureV2_File(file.toStdString().c_str(), &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "错误", "加载NETLizard v2纹理文件失败!");
            return;
        }

        NLuchar *data = nlMakeOpenGLTextureDataRGB(&tex, NULL);
        bool res = m_imageWidget->LoadImage((uchar *)data, tex.width, tex.height, QImage::Format_RGB888);
        if(!res)
        {
            QMessageBox::warning(this, "错误", "加载图像数据失败!");
            return;
        }
        delete_NETLizard_Texture(&tex);
        free(data);
    }
    else if(type == "texture_v3")
    {
        int index = m_indexSpinBox->value();
        NETLizard_Texture tex;
        NLboolean ok = nlReadTextureV3_File(file.toStdString().c_str(), index, &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "错误", "加载NETLizard v3纹理文件失败!");
            return;
        }

        NLuchar *data = nlMakeOpenGLTextureDataRGB(&tex, NULL);
        delete_NETLizard_Texture(&tex);
        bool res = m_imageWidget->LoadImage(data, tex.width, tex.height, QImage::Format_RGB888);
        if(!res)
        {
            free(data);
            QMessageBox::warning(this, "错误", "加载图像数据失败!");
            return;
        }
    }
    else if(type == "texture_v3_compress")
    {
        NETLizard_Texture tex;
        NLboolean ok = nlReadCompressTextureV3_File(file.toStdString().c_str(), &tex);
        if(!ok)
        {
            QMessageBox::warning(this, "错误", "加载NETLizard v3 compress纹理文件失败!");
            return;
        }

        NLuchar *data = nlMakeOpenGLTextureDataRGB(&tex, NULL);
        delete_NETLizard_Texture(&tex);
        bool res = m_imageWidget->LoadImage(data, tex.width, tex.height, QImage::Format_RGB888);
        if(!res)
        {
            free(data);
            QMessageBox::warning(this, "错误", "加载图像数据失败!");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "错误", "不支持的资源类型!");
    }
}
