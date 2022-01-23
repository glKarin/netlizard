#ifndef _KARIN_STRINGVIEWER_H
#define _KARIN_STRINGVIEWER_H

#include "baseviewer.h"

class QLabel;
class QPlainTextEdit;
class QPushButton;

class StringViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit StringViewer(QWidget *parent = 0);
    virtual ~StringViewer();
    
signals:
    
public slots:

private:
    void Init();
    void Reset(int mask = 1 | 2);

private Q_SLOTS:
    bool DecodeString();
    bool EncodeString();
    void ClearInput();
    void OnTextChanged();

private:
    QLabel *m_encodeLabel;
    QLabel *m_decodeLabel;
    QPlainTextEdit *m_encodeInput;
    QPlainTextEdit *m_decodeInput;
    QPushButton *m_encodeClearButton;
    QPushButton *m_decodeClearButton;
    QPushButton *m_encodeButton;
    QPushButton *m_decodeButton;

    Q_DISABLE_COPY(StringViewer)
};

#endif // _KARIN_STRINGVIEWER_H
