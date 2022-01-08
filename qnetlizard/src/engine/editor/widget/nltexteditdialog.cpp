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
#include <QMenuBar>
#include <QMenu>
#include <QWidgetAction>

#include "qdef.h"
#include "nlluasyntaxhighlighter.h"

QSize NLTextEditDialog::_lastSize;

NLTextEditDialog::NLTextEditDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      m_textEdit(0),
      m_saveButton(0),
      m_hlGroup(0),
      m_colorSchemeGroup(0),
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
      m_hlGroup(0),
      m_colorSchemeGroup(0),
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
    m_hlGroup = new QActionGroup(this);
    m_colorSchemeGroup = new QActionGroup(this);
    //QToolBar *toolbar = new QToolBar;
    QMenuBar *toolbar = new QMenuBar(this);

    QMenu *menu = toolbar->addMenu(tr("File"));
    menu->addAction(tr("Save"), this, SLOT(Save()))->setShortcut(QKeySequence("ctrl+s"));
    menu->addSeparator();
    menu->addAction(tr("Save to file"), this, SLOT(SaveToFile()))->setShortcut(QKeySequence("ctrl+w"));
    menu->addAction(tr("Load from file"), this, SLOT(LoadFromFile()))->setShortcut(QKeySequence("ctrl+f"));

    menu = toolbar->addMenu(tr("Setting"));
    QMenu *subMenu = menu->addMenu(tr("Syntax"));
    const QPair<QString, QString> HLs[] = {
        qMakePair<QString, QString>(tr("None"), QString()),
        qMakePair<QString, QString>("Lua", NLLuaSyntaxHighlighter::SyntaxName),
    };
    for(uint i = 0; i < countof(HLs); i++)
    {
        const QPair<QString, QString> &p = HLs[i];
        QAction *action = new QAction(p.first, this);
        action->setData(p.second);
        action->setCheckable(true);
        subMenu->addAction(action);
        m_hlGroup->addAction(action);
        if(i == 0)
        {
            action->setChecked(true);
            subMenu->addSeparator();
        }
    }
    m_hlGroup->setExclusive(true);
    connect(m_hlGroup, SIGNAL(triggered(QAction *)), this, SLOT(SetSyntaxHighlighter(QAction *)));

    subMenu = menu->addMenu(tr("Color scheme"));
    const QPair<QString, int> CSs[] = {
        qMakePair<QString, int>("QtCreator", NLGeneralSyntaxHighlighter::SyntaxColorScheme::ColorScheme_QtCreator),
        qMakePair<QString, int>("Vim(dark)", NLGeneralSyntaxHighlighter::SyntaxColorScheme::ColorScheme_Vim),
    };
    for(uint i = 0; i < countof(CSs); i++)
    {
        const QPair<QString, int> &p = CSs[i];
        QAction *action = new QAction(p.first, this);
        action->setData(p.second);
        action->setCheckable(true);
        subMenu->addAction(action);
        m_colorSchemeGroup->addAction(action);
        if(i == 0)
        {
            action->setChecked(true);
        }
    }
    m_colorSchemeGroup->setExclusive(true);
    m_colorSchemeGroup->setEnabled(false);
    connect(m_colorSchemeGroup, SIGNAL(triggered(QAction *)), this, SLOT(SetHighlighterColorScheme(QAction *)));

    m_saveButton->setEnabled(false);
    QMargins margins = toolsLayout->contentsMargins();
    margins.setBottom(0);
    margins.setTop(0);
    toolsLayout->setContentsMargins(margins);
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
    action->setShortcut(QKeySequence("ctrl+d"));
    connect(action, SIGNAL(triggered()), m_textEdit, SLOT(clear()));
    m_textEdit->addAction(action);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("Edit"));

    if(!_lastSize.isEmpty())
        resize(_lastSize);
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
    if(!m_edited && m_text != m_textEdit->toPlainText())
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
    editor.SetSyntax(syntax);
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

void NLTextEditDialog::SetHighlighterColorScheme(QAction *action)
{
    if(!m_hl)
        return;
    NLGeneralSyntaxHighlighter::SyntaxColorScheme::ColorScheme_e e = static_cast<NLGeneralSyntaxHighlighter::SyntaxColorScheme::ColorScheme_e>(action->data().toInt());
    if(m_hl->ColorScheme().SchemeType() == e)
        return;
    m_hl->SetColorScheme(e);
}

void NLTextEditDialog::SetSyntaxHighlighter(QAction *action)
{
    SetSyntaxHighlighter(action->data().toString());
}

void NLTextEditDialog::SetSyntaxHighlighter(const QString &type)
{
    const QString t(type.toLower());
    m_colorSchemeGroup->setEnabled(!t.isEmpty());
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

void NLTextEditDialog::SetSyntax(const QString &type)
{
    QList<QAction *> actions = m_hlGroup->actions();
    Q_FOREACH(QAction *action, actions)
    {
        if(action->data().toString() == type)
        {
            action->trigger();
            return;
        }
    }
}

void NLTextEditDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    _lastSize = event->size();
}
