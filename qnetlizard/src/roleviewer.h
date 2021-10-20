#ifndef _KARIN_ROLEVIEWER_H
#define _KARIN_ROLEVIEWER_H

#include "baseviewer.h"

class QComboBox;
class QFileDialog;
class QSpinBox;
class QString;
class RoleScene;
class QColorDialog;
class QPushButton;
class QSlider;

class RoleViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit RoleViewer(QWidget *parent = 0);
    virtual ~RoleViewer();

signals:

public slots:

private:
    void Init();

private Q_SLOTS:
    void OpenObjFileChooser();
    void OpenResourceDirChooser();
    void OpenBackgroundColorChooser();
    bool OpenFile();
    void OnTypeCurrentIndexChanged(int index);
    void SetObjFile(const QString &file);
    void SetResourceDirPath(const QString &file);
    void SetBackgroundColor(const QColor &color);
    void SetAnim(int anim);
    void SetFrame(int frame);

private:
    RoleScene *m_roleScene;
    QComboBox *m_gameComboBox;
    QFileDialog *m_fileChooser;
    QFileDialog *m_resourceDirChooser;
    QSpinBox *m_indexSpinBox;
    QColorDialog *m_colorChooser;
    QPushButton *m_openObjButton;
    QPushButton *m_openResourcePathButton;
    QComboBox *m_animComboBox;
    QSlider *m_frameSlider;
    QString m_objPath;
    QString m_resourceDirPath;

    Q_DISABLE_COPY(RoleViewer)
};

#endif // _KARIN_ROLEVIEWER_H
