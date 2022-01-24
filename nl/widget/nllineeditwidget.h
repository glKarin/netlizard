#ifndef _KARIN_NLLINEEDITWIDGET_H
#define _KARIN_NLLINEEDITWIDGET_H

#include <QLineEdit>

class NLLineEditWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit NLLineEditWidget(QWidget *parent = 0);
    explicit NLLineEditWidget(const QString &content, QWidget *parent = 0);
    virtual ~NLLineEditWidget();
    bool IsAutoSize() const { return m_autoSize; }
    void SetAutoSize(bool b);
    bool IsDoubleClickEdit() const { return m_dblEdit; }
    void SetDoubleClickEdit(bool b);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    void SetHorizontalPaddings(int padding);

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    void FixSize(QSize &size) const;
    QSize CaleTextSize(const QString &str) const;

private Q_SLOTS:
    void OnTextChanged(const QString &text);
    void FinishEdit();

private:
    bool m_autoSize;
    bool m_dblEdit;
    int m_horizontalPaddings;

    Q_DISABLE_COPY(NLLineEditWidget)
};

#endif // _KARIN_NLLINEEDITWIDGET_H
