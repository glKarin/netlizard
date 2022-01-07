#include "nltexteditdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>

#include "qdef.h"
#include "nlluasyntaxhighlighter.h"

NLTextEditDialog::NLTextEditDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_textEdit(0),
      m_saveButton(0),
      m_hlComboBox(0),
      m_hl(0),
      m_edited(false)
{
    setObjectName("NLTextEditDialog");
    Init();
}

NLTextEditDialog::NLTextEditDialog(const QString &contents, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_textEdit(0),
      m_saveButton(0),
      m_hlComboBox(0),
      m_hl(0),
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
    m_textEdit = new QPlainTextEdit(m_text);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *toolsLayout = new QHBoxLayout;
    m_saveButton = new QPushButton(tr("Save"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    QToolBar *toolbar = new QToolBar;
    m_hlComboBox = new QComboBox;

    const QPair<QString, QString> HLs[] = {
        qMakePair<QString, QString>(tr("None"), QString()),
        qMakePair<QString, QString>("Lua", NLLuaSyntaxHighlighter::SyntaxName),
    };
    for(uint i = 0; i < countof(HLs); i++)
    {
        const QPair<QString, QString> &p = HLs[i];
        m_hlComboBox->addItem(p.first, p.second);
    }
    connect(m_hlComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetSyntaxHighlighter(int)));

    toolbar->addAction(tr("Save"), this, SLOT(Save()));
    toolbar->addAction(tr("Save to file"), this, SLOT(SaveToFile()))->setShortcut(QKeySequence("ctrl+w"));
    toolbar->addAction(tr("Load from file"), this, SLOT(LoadFromFile()))->setShortcut(QKeySequence("ctrl+f"));
    toolbar->addSeparator();
    toolbar->addWidget(new QLabel(tr("Syntax: ")));
    toolbar->addWidget(m_hlComboBox);
    m_saveButton->setEnabled(false);
    toolsLayout->addStretch(1);
    toolsLayout->addWidget(m_saveButton);
    toolsLayout->addWidget(cancelButton);
    mainLayout->addWidget(toolbar);
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

bool NLTextEditDialog::SaveToFile()
{
    Save();
    QString file = QFileDialog::getSaveFileName(this);
    if(file.isEmpty())
        return false;

    QFile f(file);
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;

    if(!f.open(mode))
    {
        QMessageBox::warning(this, tr("Error"), tr("Can not open file: ") + file);
        return false;
    }
    QTextStream os(&f);
#if 0
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    if(codec)
        os.setCodec(codec);
#endif
    os << m_text;
    f.flush();
    f.close();
    QMessageBox::information(this, tr("Success"), tr("Save text file: ") + file);
    return true;
}

bool NLTextEditDialog::LoadFromFile()
{
    QString file = QFileDialog::getOpenFileName(this);
    if(file.isEmpty())
        return false;

    QFile f(file);
    if(!f.exists())
    {
        QMessageBox::warning(this, tr("Error"), tr("Can not open file: ") + file);
        return false;
    }
    if(!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Error"), tr("Can not read file: ") + file);
        return false;
    }
    QTextStream is(&f);
    QString text = is.readAll();
    f.close();

    FinishEdit();
    m_textEdit->disconnect(this);
    m_textEdit->setPlainText(text);
    if(m_text != text)
    {
        m_text = text;
        emit textChanged(m_text);
    }
    StartEdit();

    QMessageBox::information(this, tr("Success"), tr("Load file finished: ") + file);
    return true;
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

QString NLTextEditDialog::Edit(const QString &text, const QString &syntax, QWidget *parent, Qt::WindowFlags f)
{
    NLTextEditDialog editor(text, parent, f);
    editor.SetSyntaxHighlighter(syntax);
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

void NLTextEditDialog::SetSyntaxHighlighter(int index)
{
    QVariant data = m_hlComboBox->itemData(index);
    SetSyntaxHighlighter(data.toString());
}

void NLTextEditDialog::SetSyntaxHighlighter(const QString &type)
{
    const QString t(type.toLower());
    if(m_hl)
    {
        if(m_hl->Name() == t)
            return;
        m_hl->setDocument(0);
        m_hl->deleteLater();
        m_hl = 0;
    }
    else
    {
        if(t.isEmpty())
            return;
    }

    if(t == NLLuaSyntaxHighlighter::SyntaxName)
        m_hl = new NLLuaSyntaxHighlighter(this);

    if(m_hl)
        m_hl->setDocument(m_textEdit->document());
}
