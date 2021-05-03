#include "textviewer.h"

#include <QtGui/QtGui>

#include "netlizard.h"
#include "qdef.h"

TextViewer::TextViewer(QWidget *parent) :
    QWidget(parent),
    m_fileChooser(0),
    m_textBrowser(0),
    m_titleLabel(0)
{
    setObjectName("TextViewer");
    Init();
}

TextViewer::~TextViewer()
{
    DEBUG_DESTROY_Q;
}

void TextViewer::Init()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button = new QPushButton(this);
    m_textBrowser = new QTextBrowser(this);
    QHBoxLayout *toolLayout = new QHBoxLayout(this);
    m_titleLabel = new QLabel(this);

    button->setText("Open file");
    toolLayout->addStretch();
    toolLayout->addWidget(button);

    m_titleLabel->setFixedHeight(18);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_textBrowser);
    layout->addLayout(toolLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(OpenFileChooser()));

    setLayout(layout);
    setWindowTitle("NETLizard text resource viewer");
    resize(480, 360);
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

    m_titleLabel->setText("");
    char *data = nlHandleText_File2Memory(file.toStdString().c_str(), &len);
    if(!data)
    {
        QMessageBox::warning(this, "Error", "Load NETLizard text fail!");
        return false;
    }

    m_titleLabel->setText(QString("Length: %1").arg(len));
    m_textBrowser->setText(data);
    free(data);

    return true;
}
