#include "textviewer.h"

#include <QDebug>

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTextBrowser>

#include "utils/nlioutility.h"

#include "netlizard.h"
#include "qdef.h"

TextViewer::TextViewer(QWidget *parent) :
    BaseViewer(parent),
    m_textBrowser(0),
    m_fileChooser(0),
    m_saveButton(0),
    m_saveChooser(0)
{
    setObjectName("TextViewer");
    Init();
}

TextViewer::~TextViewer()
{
}

void TextViewer::Init()
{
    QPushButton *button;
    //QHBoxLayout *toolLayout = ToolLayout();
    m_textBrowser = new QTextBrowser;
    SetTitleLabelVisible(false);

    button = new QPushButton;
    button->setText(tr("Open file"));
    button->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool();
    AddTool(button);
    m_saveButton = new QPushButton;
    m_saveButton->setText(tr("Save data"));
    m_saveButton->setShortcut(QKeySequence::fromString("ctrl+s"));
    m_saveButton->setEnabled(false);
    AddTool(m_saveButton);
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(OpenSaveChooser()));

    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));

    m_textBrowser->setAcceptRichText(false);
    SetCentralWidget(m_textBrowser);

    SetTitle(tr("NETLizard text resource viewer"));
}

void TextViewer::OpenFileChooser()
{
    if(!m_fileChooser)
    {
        m_fileChooser = new QFileDialog(this);
        connect(m_fileChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(OpenFile(const QString &)));
    }

    m_fileChooser->exec();
}

bool TextViewer::OpenFile(const QString &file)
{
    NLint len;

    Reset();
    QByteArray ba = file.toLocal8Bit();
    char *data = nlReadAndHandleTextFile(ba.data(), &len);
    if(!data)
    {
        QMessageBox::warning(this, tr("Error"), tr("Load NETLizard text fail!"));
        return false;
    }

    SetTitleLabel(QString(tr("%1 - Length: %2")).arg(file).arg(len));
    m_textBrowser->setPlainText(QString::fromLocal8Bit(data));
    m_data = QByteArray(data, len);
    m_saveButton->setEnabled(true);

    return true;
}

void TextViewer::OpenSaveChooser()
{
    if(m_data.isEmpty())
        return;

    if(!m_saveChooser)
    {
        m_saveChooser = new QFileDialog(this);
        m_saveChooser->setFileMode(QFileDialog::AnyFile);
        m_saveChooser->setAcceptMode(QFileDialog::AcceptSave);
        QStringList filters;
        filters << "Text files (*.txt)"
                << "Any files (*)";
        m_saveChooser->setNameFilters(filters);
        m_saveChooser->setDefaultSuffix("txt");
        connect(m_saveChooser, SIGNAL(fileSelected(const QString &)), this, SLOT(SaveData(const QString &)));
    }

    m_saveChooser->exec();
}

void TextViewer::Reset()
{
    BaseViewer::Reset();
    m_saveButton->setEnabled(false);
    m_textBrowser->clear();
    m_data.clear();
}

bool TextViewer::SaveData(const QString &file)
{
    if(m_data.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("No data!"));
        return false;
    }
    bool res = NLIOUtility::file_put_contents(file, m_data.constData(), (uint)m_data.length());
    if(res)
        QMessageBox::information(this, tr("Success"), tr("File path is ") + file);
    else
        QMessageBox::warning(this, tr("Error"), tr("Save data fail!"));
    return res;
}
