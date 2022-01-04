#ifndef _KARIN_NLTEXTEDITDIALOG_H
#define _KARIN_NLTEXTEDITDIALOG_H

#include <QDialog>

class QPushButton;
class QTextEdit;

class NLTextEditDialog : public QDialog
{
    Q_OBJECT

public:
    NLTextEditDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
    NLTextEditDialog(const QString &contents, QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~NLTextEditDialog();
    QString Text() const { return m_text; }
    QString EditText() const;
    bool IsEditing() const { return m_edited; }
    static QString Edit(const QString &text = QString(), QWidget *parent = 0, Qt::WindowFlags f = 0);
    QString Exec(const QString &text);
    QString Exec();
    QTextEdit * TextEdit() { return m_textEdit; }

public Q_SLOTS:
    void SetText(const QString &text);
    virtual void accept();
    virtual void reject();

Q_SIGNALS:
    void textChanged(const QString &text);
    void textEdited(const QString &text);

private:
    void Init();
    void FinishEdit();
    void StartEdit();

private Q_SLOTS:
    void Save();
    void OnTextChanged();

private:
    QTextEdit *m_textEdit;
    QPushButton *m_saveButton;
    QString m_text;
    bool m_edited;

    Q_DISABLE_COPY(NLTextEditDialog)
};

#endif // _KARIN_NLTEXTEDITDIALOG_H
