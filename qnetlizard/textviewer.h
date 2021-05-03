#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QWidget>

class QLabel;
class QTextBrowser;
class QFileDialog;

class TextViewer : public QWidget
{
    Q_OBJECT
public:
    explicit TextViewer(QWidget *parent = 0);
    ~TextViewer();
    
signals:

public slots:

private:
    void Init();

private Q_SLOTS:
    void OpenFileChooser();
    bool OpenFile(const QString &file);
    
private:
    QTextBrowser *m_textBrowser;
    QFileDialog *m_fileChooser;
    QLabel *m_titleLabel;
    
};

#endif // TEXTVIEWER_H
