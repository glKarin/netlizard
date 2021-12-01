#include "logdialog.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QScrollBar>
#include <QPushButton>

#include "logoutput.h"
#include "qdef.h"

QString LogDialog::_logText;

LogDialog::LogDialog(QWidget *parent) :
    QDockWidget(parent),
    m_log(0),
    m_textViewer(0)
{
    setObjectName("LogDialog");
    m_log = LogOutput::Instance();
    Init();
}

LogDialog::~LogDialog()
{
    m_log->Finish();
    DEBUG_DESTROY_Q
}

void LogDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *toolLayout = new QHBoxLayout;
    m_textViewer = new QTextBrowser;
    QPushButton *button = new QPushButton;
    QWidget *root = new QWidget;

    m_textViewer->setAcceptRichText(true);
    layout->addWidget(m_textViewer, 1);
    button->setText("Clear");
    toolLayout->addStretch();
    toolLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(ClearLog()));
    layout->addLayout(toolLayout);

    connect(m_log, SIGNAL(outputLog(int, const QString &)), this, SLOT(PushLog(int, const QString &)));

    root->setLayout(layout);

    m_textViewer->setHtml(LogDialog::_logText);

    setWidget(root);

    setWindowTitle("Log output");
}

void LogDialog::PushLog(int type, const QString &str)
{
    QString color("#000000");
    if(type == 1)
        color = "yellow";
    else if(type != 0)
        color = "#ff0000";

    QString text(QString("<font color='%1'>%2</font>").arg(color).arg(str));
    LogDialog::_logText.append(text);
    m_textViewer->append(text);
    m_textViewer->verticalScrollBar()->setValue(m_textViewer->verticalScrollBar()->maximum());
    m_textViewer->horizontalScrollBar()->setValue(0);
}

void LogDialog::ResetPosAndSize()
{
    QWidget *p = parentWidget();
    if(!p)
        return;
    QRect rect = p->frameGeometry();
    move(rect.x() + rect.width(), rect.y());
    setFixedHeight(p->height());
    m_textViewer->verticalScrollBar()->setValue(m_textViewer->verticalScrollBar()->maximum());
    m_textViewer->horizontalScrollBar()->setValue(0);
}

void LogDialog::ClearLog()
{
    m_textViewer->clear();
    LogDialog::_logText.clear();
    m_log->Clear();
}

void LogDialog::hideEvent(QHideEvent *e)
{
    QDockWidget::hideEvent(e);
#ifdef _DEV_TEST
    m_log->Finish();
#endif
}

void LogDialog::showEvent(QShowEvent *e)
{
    QDockWidget::showEvent(e);
#ifdef _DEV_TEST
    m_log->Start();
#endif
}
