#ifndef STRINGVIEWER_H
#define STRINGVIEWER_H

#include <QWidget>

class QLabel;
class QTextEdit;

class StringViewer : public QWidget
{
    Q_OBJECT
public:
    explicit StringViewer(QWidget *parent = 0);
    ~StringViewer();
    
signals:
    
public slots:

private:
    void Init();

private Q_SLOTS:
    bool DecodeString();
    bool EncodeString();

private:
    QLabel *m_encodeLabel;
    QLabel *m_decodeLabel;
    QTextEdit *m_encodeInput;
    QTextEdit *m_decodeInput;
    
};

#endif // STRINGVIEWER_H
