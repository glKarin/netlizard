#ifndef _KARIN_ITEMVIEWER_H
#define _KARIN_ITEMVIEWER_H

#include "baseviewer.h"

class ImageWidget;
class QComboBox;
class QFileDialog;
class QSpinBox;
class QString;
class ItemWidget;
class QColorDialog;
class QPushButton;

class ItemViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit ItemViewer(QWidget *parent = 0);
    virtual ~ItemViewer();

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

private:
    ItemWidget *m_itemWidget;
    QComboBox *m_gameComboBox;
    QFileDialog *m_fileChooser;
    QFileDialog *m_resourceDirChooser;
    QSpinBox *m_indexSpinBox;
    QColorDialog *m_colorChooser;
    QPushButton *m_openObjButton;
    QPushButton *m_openResourcePathButton;
    QString m_objPath;
    QString m_resourceDirPath;


    Q_DISABLE_COPY(ItemViewer)
};

#endif // _KARIN_ITEMVIEWER_H
