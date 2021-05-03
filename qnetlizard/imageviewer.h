#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>

class ImageWidget;
class QComboBox;
class QFileDialog;
class QSpinBox;
class QLabel;

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    
signals:
    
public slots:

private:
    void Init();

private Q_SLOTS:
    void OpenFileChooser();
    void OpenFile(const QString &file);
    void OnTypeCurrentIndexChanged(int index);

private:
    ImageWidget *m_imageWidget;
    QComboBox *m_typeComboBox;
    QFileDialog *m_fileChooser;
    QSpinBox *m_indexSpinBox;
    QLabel *m_titleLabel;
};

#endif // IMAGEVIEWER_H
