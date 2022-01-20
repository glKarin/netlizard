#ifndef _KARIN_NLLINEEDITWIDGET_H
#define _KARIN_NLLINEEDITWIDGET_H

#include <QWidget>
#include <QLineEdit>

class QPushButton;

class NLLineEditWidgetLabel : public QLineEdit
{
    Q_OBJECT

public:
    NLLineEditWidgetLabel(QWidget *parent = 0)
        : QLineEdit(parent) { setObjectName("NLLineEditWidgetLabel"); }
    NLLineEditWidgetLabel(const QString &contents, QWidget *parent = 0)
        : QLineEdit(contents, parent) { setObjectName("NLLineEditWidgetLabel"); }
    virtual ~NLLineEditWidgetLabel();

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

class NLLineEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLLineEditWidget(QWidget *widget = 0);
    virtual ~NLLineEditWidget();
    QString Text() const { return m_text; }
    QString Syntax() const { return m_syntax; }
    void SetSyntax(const QString &type);

public Q_SLOTS:
    void SetText(const QString &t);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void textChanged(const QString &text);
    void textEdited(const QString &text);

private Q_SLOTS:
    void OpenTextEditor();
    void SetEditText(const QString &t);

private:
    void Init();
    void UpdateWidget();

private:
    QString m_text;
    NLLineEditWidgetLabel *m_textLabel;
    QPushButton *m_editButton;
    QString m_syntax;

    Q_DISABLE_COPY(NLLineEditWidget)
};

#endif // _KARIN_NLLINEEDITWIDGET_H
