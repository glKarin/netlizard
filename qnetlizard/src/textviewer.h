#ifndef _KARIN_TEXTVIEWER_H
#define _KARIN_TEXTVIEWER_H

#include "baseviewer.h"

class QTextBrowser;
class QFileDialog;
class QPushButton;

class TextViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit TextViewer(QWidget *parent = 0);
    virtual ~TextViewer();
    
signals:

public slots:

private:
    void Init();
    void Reset();
    void SetData(char *data, int len);
    void UnsetData();

private Q_SLOTS:
    void OpenFileChooser();
    void OpenSaveChooser();
    bool SaveData(const QString &file);
    bool OpenFile(const QString &file);
    
private:
    QTextBrowser *m_textBrowser;
    QFileDialog *m_fileChooser;
    QPushButton *m_saveButton;
    QFileDialog *m_saveChooser;
    struct {
        char *data;
        int len;
    } m_data;
    
    Q_DISABLE_COPY(TextViewer)
};

#endif // _KARIN_TEXTVIEWER_H
