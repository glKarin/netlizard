#include "imagewidget.h"

#include <QtGui/QtGui>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent),
      m_image(0)
{
}

ImageWidget::~ImageWidget()
{
    if(m_image)
        delete m_image;
}

bool ImageWidget::LoadImage(const QString &file)
{
    if(!m_image)
        m_image = new QImage;
    SetData(0);
    return m_image->load(file);
}

bool ImageWidget::LoadImage(const uchar *data, int len)
{
    if(!m_image)
        m_image = new QImage;
    SetData(0);
    return m_image->loadFromData(data, len);
}

bool ImageWidget::LoadImage(uchar *data, int width, int height, QImage::Format format)
{
    if(m_image)
    {
        delete m_image;
        m_image = 0;
    }
    SetData(0);
    m_image = new QImage(data, width, height, format);
    bool res = !m_image->isNull();
    if(res)
        SetData(data);
    return res;
}

void ImageWidget::SetData(uchar *data)
{
    if(m_data != data)
    {
        free(m_data);
        m_data = 0;
    }
    m_data = 0;
}

void ImageWidget::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);

    if(m_image)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *m_image);
    }
}
