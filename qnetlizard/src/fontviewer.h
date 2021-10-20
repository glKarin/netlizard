#ifndef _KARIN_FONTVIEWER_H
#define _KARIN_FONTVIEWER_H

#include "baseviewer.h"

class QTextEdit;
class FontScene;
class QPushButton;
class QFileDialog;
class QColorDialog;

class FontViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit FontViewer(QWidget *parent = 0);
    virtual ~FontViewer();
    
signals:
    
public slots:

private Q_SLOTS:
    bool RenderString();
    bool LoadFont();
    void OpenCfFileChooser();
    void OpenFntFileChooser();
    void OpenBackgroundColorChooser();
    void SetCfFile(const QString &file);
    void SetFntFile(const QString &file);
    void SetBackgroundColor(const QColor &color);

private:
    void Init();
    void Reset();

private:
    QTextEdit *m_textInput;
    FontScene *m_fontScene;
    QPushButton *m_renderButton;
    QFileDialog *m_cfFileChooser;
    QFileDialog *m_fntFileChooser;
    QColorDialog *m_colorChooser;
    QPushButton *m_openCfButton;
    QPushButton *m_openFntButton;
    QString m_cfFile;
    QString m_fntFile;

    Q_DISABLE_COPY(FontViewer)
};

#endif // _KARIN_FONTVIEWER_H
