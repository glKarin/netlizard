#include "baseviewer.h"

#include <QDebug>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "qdef.h"

BaseViewer::BaseViewer(QWidget *parent) :
    QWidget(parent),
    m_mainLayout(0),
    m_toolLayout(0),
    m_centralWidget(0),
    m_titleLabel(0)
{
    setObjectName("BaseViewer");
    Init();
}

BaseViewer::~BaseViewer()
{
    DEBUG_DESTROY_Q;
}

QString BaseViewer::Title() const
{
    return m_title;
}

void BaseViewer::SetTitle(const QString &str)
{
    if(m_title != str)
    {
        m_title = str;
        emit titleChanged(m_title);
    }
}

QString BaseViewer::StatusText() const
{
    return m_statusText;
}

void BaseViewer::SetStatusText(const QString &str)
{
    if(m_statusText != str)
    {
        m_statusText = str;
        emit statusTextChanged(m_statusText);
    }
}

void BaseViewer::SetTitleLabel(const QString &str)
{
    m_titleLabel->setText(str);
}

QWidget * BaseViewer::CentralWidget()
{
    return m_centralWidget;
}

void BaseViewer::SetCentralWidget(QWidget *widget)
{
    if(m_centralWidget)
    {
        m_mainLayout->removeWidget(m_centralWidget);
        m_centralWidget = 0;
    }
    if(widget)
    {
        m_centralWidget = widget;
        m_mainLayout->insertWidget(1, m_centralWidget);
    }
}

QHBoxLayout * BaseViewer::ToolLayout()
{
    return m_toolLayout;
}

QLabel * BaseViewer::TitleLabel()
{
    return m_titleLabel;
}

void BaseViewer::Reset()
{
    //SetTitle("");
    SetStatusText("");
    SetTitleLabel();
}

void BaseViewer::Init()
{
    m_toolLayout = new QHBoxLayout;
    m_mainLayout = new QVBoxLayout;
    m_titleLabel = new QLabel;
    m_centralWidget = new QWidget;

    m_titleLabel->setContentsMargins(0, 0, 0, 0);
    m_titleLabel->setFixedHeight(24);
    m_titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_titleLabel->setWordWrap(true);

    m_toolLayout->setMargin(0);

    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_centralWidget);
    m_mainLayout->addLayout(m_toolLayout);

    setLayout(m_mainLayout);
    setWindowTitle(m_title);
    resize(480, 360);
}
