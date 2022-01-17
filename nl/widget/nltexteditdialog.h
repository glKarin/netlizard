#ifndef _KARIN_NLTEXTEDITDIALOG_H
#define _KARIN_NLTEXTEDITDIALOG_H

#include <QDialog>

class QPushButton;
class QPlainTextEdit;
class QActionGroup;
class NLGeneralSyntaxHighlighter;

class NLTextEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NLTextEditDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
    explicit NLTextEditDialog(const QString &contents, QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~NLTextEditDialog();
    QString Text() const { return m_text; }
    QString EditText() const;
    bool IsEditing() const { return m_edited; }
    static QString Edit(const QString &text = QString(), const QString &syntax = QString(), QWidget *parent = 0, Qt::WindowFlags f = 0);
    QString Exec(const QString &text);
    QString Exec();
    QPlainTextEdit * TextEdit() { return m_textEdit; }

public Q_SLOTS:
    void SetText(const QString &text);
    virtual void accept();
    virtual void reject();
    void SetSyntax(const QString &type);

Q_SIGNALS:
    void textChanged(const QString &text);
    void textEdited(const QString &text);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void Init();
    void FinishEdit();
    void StartEdit();

private Q_SLOTS:
    void Save();
    bool SaveToFile();
    bool LoadFromFile();
    void OnTextChanged();
    void SetSyntaxHighlighter(QAction *action);
    void SetSyntaxHighlighter(const QString &type = QString());
    void SetHighlighterColorScheme(QAction *action);

private:
    QPlainTextEdit *m_textEdit;
    QPushButton *m_saveButton;
    QActionGroup *m_hlGroup;
    QActionGroup *m_colorSchemeGroup;
    NLGeneralSyntaxHighlighter *m_hl;
    QString m_text;
    bool m_edited;

    static QSize _lastSize;

    Q_DISABLE_COPY(NLTextEditDialog)
};

#endif // _KARIN_NLTEXTEDITDIALOG_H
