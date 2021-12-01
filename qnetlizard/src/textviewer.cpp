#include "textviewer.h"

#include <QDebug>

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTextBrowser>

#include "utils/ioutility.h"

#include "netlizard.h"
#include "qdef.h"

TextViewer::TextViewer(QWidget *parent) :
    BaseViewer(parent),
    m_textBrowser(0),
    m_fileChooser(0),
    m_saveButton(0),
    m_saveChooser(0)
{
    memset(&m_data, 0, sizeof(m_data));
    setObjectName("TextViewer");
    Init();
}

TextViewer::~TextViewer()
{
    UnsetData();
}

void TextViewer::Init()
{
    QPushButton *button;
    //QHBoxLayout *toolLayout = ToolLayout();
    m_textBrowser = new QTextBrowser;
    SetTitleLabelVisible(false);

    button = new QPushButton;
    button->setText("Open file");
    button->setShortcut(QKeySequence::fromString("ctrl+f"));
    AddTool();
    AddTool(button);
    m_saveButton = new QPushButton;
    m_saveButton->setText("Save data");
    m_saveButton->setShortcut(QKeySequence::fromString("ctrl+s"));
    m_saveButton->setEnabled(false);
    AddTool(m_saveButton);
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(OpenSaveChooser()));

    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));

    SetCentralWidget(m_textBrowser);

    SetTitle("NETLizard text resource viewer");
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
        QMessageBox::warning(this, "Error", "Load NETLizard text fail!");
        return false;
    }

    SetStatusText(QString("%1 - Length: %2").arg(file).arg(len));
    m_textBrowser->setText(QString::fromLocal8Bit(data));
    SetData(data, len);
    m_saveButton->setEnabled(true);

    return true;
}

void TextViewer::OpenSaveChooser()
{
    if(!m_data.data)
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
    m_textBrowser->setText("");
    UnsetData();
}

void TextViewer::UnsetData()
{
    free(m_data.data);
    memset(&m_data, 0, sizeof(m_data));
}

void TextViewer::SetData(char *data, int len)
{
    UnsetData();
    if(data)
    {
        m_data.data = data;
        m_data.len = len;
    }
}

bool TextViewer::SaveData(const QString &file)
{
    if(!m_data.data)
    {
        QMessageBox::warning(this, "Error", "No data!");
        return false;
    }
    bool res = IOUtility::file_put_contents(file, m_data.data, (quint64)m_data.len);
    if(res)
        QMessageBox::information(this, "Success", "File path is " + file);
    else
        QMessageBox::warning(this, "Error", "Save data fail!");
    return res;
}
