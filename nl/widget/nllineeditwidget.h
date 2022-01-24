#ifndef _KARIN_NLLINEEDITWIDGET_H
#define _KARIN_NLLINEEDITWIDGET_H

#include <QWidget>

class QPushButton;
class NLLineEditWidgetLabel;

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

    friend class NLLineEditWidgetLabel;

    Q_DISABLE_COPY(NLLineEditWidget)
};

#endif // _KARIN_NLLINEEDITWIDGET_H
