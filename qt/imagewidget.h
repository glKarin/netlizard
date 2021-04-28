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
    bool LoadImage(uchar *data, int width, int height); // RGBA

protected:
    void paintEvent(QPaintEvent * event);

private:
    void SetData(uchar *data);
    void ConvertRGBA32toARGB32(uchar *data, int width, int height);

private:
    QImage *m_image;
    uchar *m_data;
};

#endif // IMAGEWIDGET_H
