#ifndef _KARIN_NLEDITWIDGET_H
#define _KARIN_NLEDITWIDGET_H

#include <QWidget>

class QPushButton;
class NLLineEditWidget;

class NLEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLEditWidget(QWidget *widget = 0);
    virtual ~NLEditWidget();
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
    NLLineEditWidget *m_textLabel;
    QPushButton *m_editButton;
    QString m_syntax;

    Q_DISABLE_COPY(NLEditWidget)
};

#endif // _KARIN_NLEDITWIDGET_H
