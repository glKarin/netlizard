#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>

class ImageWidget : public QWidget
{
public:
    ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

    bool LoadImage(const QString &file);
    bool LoadImage(const uchar *data, int len);
    bool LoadImage(uchar *data, int width, int height, QImage::Format format);

protected:
    void paintEvent(QPaintEvent * event);

private:
    void SetData(uchar *data);

private:
    QImage *m_image;
    uchar *m_data;
};

#endif // IMAGEWIDGET_H
