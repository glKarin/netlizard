#include "nltexteditdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>

#include "qdef.h"

NLTextEditDialog::NLTextEditDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_textEdit(0),
      m_saveButton(0),
      m_edited(false)
{
    setObjectName("NLTextEditDialog");
    Init();
}

NLTextEditDialog::NLTextEditDialog(const QString &contents, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_textEdit(0),
      m_saveButton(0),
      m_text(contents),
      m_edited(false)
{
    setObjectName("NLTextEditDialog");
    Init();
}

NLTextEditDialog::~NLTextEditDialog()
{
    DEBUG_DESTROY_Q
}

void NLTextEditDialog::Init()
{
    m_textEdit = new QTextEdit(m_text);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *toolsLayout = new QHBoxLayout;
    m_saveButton = new QPushButton(tr("Save"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    m_textEdit->setAcceptRichText(false);
    m_saveButton->setEnabled(false);
    toolsLayout->addStretch(1);
    toolsLayout->addWidget(m_saveButton);
    toolsLayout->addWidget(cancelButton);
    mainLayout->addWidget(m_textEdit, 1);
    mainLayout->addLayout(toolsLayout);

    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    StartEdit();

    QAction *action = new QAction(m_textEdit);
    action->setShortcut(QKeySequence("ctrl+s"));
    connect(action, SIGNAL(triggered()), this, SLOT(Save()));
    m_textEdit->addAction(action);
    action = new QAction(m_textEdit);
    action->setShortcut(QKeySequence("ctrl+d"));
    connect(action, SIGNAL(triggered()), m_textEdit, SLOT(clear()));
    m_textEdit->addAction(action);

    setLayout(mainLayout);
    setWindowTitle(tr("Edit"));
}

void NLTextEditDialog::Save()
{
    QString str(m_textEdit->toPlainText());
    if(m_text != str)
    {
        m_text = str;
        emit textChanged(m_text);
        emit textEdited(m_text);
    }
    FinishEdit();
}

QString NLTextEditDialog::EditText() const
{
    return m_textEdit->toPlainText();
}

void NLTextEditDialog::OnTextChanged()
{
    if(!m_edited)
    {
        m_edited = true;
        m_saveButton->setEnabled(true);
        setWindowTitle("*" + tr("Edit"));
    }
}

void NLTextEditDialog::accept()
{
    Save();
    QDialog::accept();
}

void NLTextEditDialog::StartEdit()
{
    connect(m_textEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
}

void NLTextEditDialog::FinishEdit()
{
    if(m_edited)
    {
        m_edited = false;
        m_saveButton->setEnabled(false);
        setWindowTitle(tr("Edit"));
    }
}

void NLTextEditDialog::reject()
{
    FinishEdit();
    m_textEdit->setPlainText(m_text);
    QDialog::reject();
}

QString NLTextEditDialog::Edit(const QString &text, QWidget *parent, Qt::WindowFlags f)
{
    NLTextEditDialog editor(text, parent, f);
    return editor.Exec();
}

QString NLTextEditDialog::Exec(const QString &text)
{
    SetText(text);
    return Exec();
}

QString NLTextEditDialog::Exec()
{
    int result = exec();
    if(result == QDialog::Accepted)
        return Text();
    return QString();
}

void NLTextEditDialog::SetText(const QString &text)
{
    if(m_text != text)
    {
        FinishEdit();
        m_text = text;
        m_textEdit->disconnect(this);
        m_textEdit->setPlainText(m_text);
        StartEdit();
        emit textChanged(m_text);
    }
}
