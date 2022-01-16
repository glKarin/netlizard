#ifndef _KARIN_NLTEXTEDITWIDGET_H
#define _KARIN_NLTEXTEDITWIDGET_H

#include <QWidget>
#include <QLineEdit>

class QPushButton;

class NLTextEditWidgetLabel : public QLineEdit
{
    Q_OBJECT

public:
    NLTextEditWidgetLabel(QWidget *parent = 0)
        : QLineEdit(parent) { setObjectName("NLTextEditWidgetLabel"); }
    NLTextEditWidgetLabel(const QString &contents, QWidget *parent = 0)
        : QLineEdit(contents, parent) { setObjectName("NLTextEditWidgetLabel"); }
    virtual ~NLTextEditWidgetLabel();

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

class NLTextEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLTextEditWidget(QWidget *widget = 0);
    virtual ~NLTextEditWidget();
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
    NLTextEditWidgetLabel *m_textLabel;
    QPushButton *m_editButton;
    QString m_syntax;

    Q_DISABLE_COPY(NLTextEditWidget)
};

#endif // _KARIN_NLTEXTEDITWIDGET_H
