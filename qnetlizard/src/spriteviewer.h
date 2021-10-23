#ifndef _KARIN_SPRITEVIEWER_H
#define _KARIN_SPRITEVIEWER_H

#include "baseviewer.h"

class SpriteScene;
class QListWidget;
class QFileDialog;
class QColorDialog;
class QPushButton;
class QListWidgetItem;

class SpriteViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit SpriteViewer(QWidget *parent = 0);
    virtual ~SpriteViewer();
    
signals:
    
public slots:

private Q_SLOTS:
    bool RenderIndex(QListWidgetItem *item);
    bool LoadSprite();
    void OpenCfFileChooser();
    void OpenFntFileChooser();
    void OpenBackgroundColorChooser();
    void SetCuFile(const QString &file);
    void SetCFile(const QString &file);
    void SetBackgroundColor(const QColor &color);

private:
    void Init();
    void Reset();

private:
    SpriteScene *m_spriteScene;
    QListWidget *m_indexList;
    QFileDialog *m_cuFileChooser;
    QFileDialog *m_cFileChooser;
    QColorDialog *m_colorChooser;
    QPushButton *m_openCfButton;
    QPushButton *m_openFntButton;
    QString m_cuFile;
    QString m_cFile;

    Q_DISABLE_COPY(SpriteViewer)
};

#endif // _KARIN_SPRITEVIEWER_H
