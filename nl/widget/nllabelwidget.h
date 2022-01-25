#ifndef _KARIN_NLLABELWIDGET_H
#define _KARIN_NLLABELWIDGET_H

#include <QLabel>

class NLLabelWidget : public QLabel
{
    Q_OBJECT
public:
    explicit NLLabelWidget(QWidget *parent = 0);
    explicit NLLabelWidget(const QString &content, QWidget *parent = 0);
    virtual ~NLLabelWidget();
    Qt::TextElideMode ElideMode() const { return m_elideMode; }
    void SetElideMode(Qt::TextElideMode e);
    void setText(const QString &text);
    QString text() const { return m_text; }
    bool TextElideEnabled() const;

Q_SIGNALS:
    void dblClicked();
    void textChanged(const QString &text);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    void UpdateText();
    int CaleWidth() const;

private:
    Qt::TextElideMode m_elideMode;
    QString m_text;

    Q_DISABLE_COPY(NLLabelWidget)
};

#endif // _KARIN_NLLABELWIDGET_H
