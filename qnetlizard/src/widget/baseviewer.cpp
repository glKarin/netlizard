#include "baseviewer.h"

#include <QDebug>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>

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

void BaseViewer::SetTitle(const QString &str)
{
    if(m_title != str)
    {
        m_title = str;
        emit titleChanged(m_title);
    }
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

void BaseViewer::SetupToolBar(QToolBar *toolbar)
{
    Q_FOREACH(QWidget *w, m_tools)
    {
        if(w)
            toolbar->addWidget(w);
        else
            toolbar->addSeparator();
    }
}

void BaseViewer::SetupToolBar(QHBoxLayout *toolbar)
{
    Q_FOREACH(QWidget *w, m_tools)
    {
        if(w)
            toolbar->addWidget(w);
        else
            toolbar->addStretch();
    }
}
