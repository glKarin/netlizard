#ifndef _KARIN_IMAGEVIEWER_H
#define _KARIN_IMAGEVIEWER_H

#include "baseviewer.h"

class ImageScene;
class QComboBox;
class QFileDialog;
class QSpinBox;
class QPushButton;

typedef struct NETLizard_Texture_s NETLizard_Texture;

class ImageViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);
    virtual ~ImageViewer();
    
signals:
    
public slots:

private:
    void Init();
    void Reset();

private Q_SLOTS:
    void OpenFileChooser();
    void OpenSaveChooser();
    bool OpenFile(const QString &file);
    bool SaveData(const QString &file);
    void OnTypeCurrentIndexChanged(int index);
    void OnAlignCurrentIndexChanged(int index);

private:
    ImageScene *m_imageScene;
    QComboBox *m_typeComboBox;
    QFileDialog *m_fileChooser;
    QSpinBox *m_indexSpinBox;
    QPushButton *m_saveButton;
    QFileDialog *m_saveChooser;
    QComboBox *m_alignComboBox;

    Q_DISABLE_COPY(ImageViewer)
};

#endif // _KARIN_IMAGEVIEWER_H
