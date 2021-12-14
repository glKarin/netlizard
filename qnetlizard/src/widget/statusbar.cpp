#include "statusbar.h"

#include <QDebug>

#include <QLabel>
#include <QTimer>

#include "qdef.h"

StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent),
    m_timer(0)
{
    setObjectName("StatusBar");
}

StatusBar::~StatusBar()
{
    DEBUG_DESTROY_Q;
}

void StatusBar::ShowPermanentMessage(const QString &str)
{
    if(m_permanentLabel != str)
    {
        m_permanentLabel = str;
        ShowText();
        emit permanentMessageChanged(m_permanentLabel);
    }
}

void StatusBar::ClearPermanentMessage()
{
    m_permanentLabel.clear();
}

QString StatusBar::CurrentPermanentMessage() const
{
    return m_permanentLabel;
}

void StatusBar::Reset(bool visible)
{
    clearMessage();
    ClearPermanentMessage();
    ClearTemporaryMessage();
    if(visible)
        setVisible(false);
}

void StatusBar::ShowText()
{
    QString str;
    bool hasTemp = !m_temporaryLabel.isEmpty();
    if(hasTemp)
        str += m_temporaryLabel;
    if(!m_permanentLabel.isEmpty())
    {
        if(hasTemp)
            str += " | ";
        str += m_permanentLabel;
    }
    if(!str.isEmpty())
    {
        if(str == currentMessage())
            clearMessage();
        showMessage(str);
    }
    else
    {
        clearMessage();
    }
}

void StatusBar::ShowTemporaryMessage(const QString &str, int timeout)
{
    if(m_temporaryLabel != str)
    {
        m_temporaryLabel = str;
        ShowText();
        emit permanentMessageChanged(m_temporaryLabel);
    }
    if(timeout > 0)
    {
        if(!m_timer)
        {
            m_timer = new QTimer(this);
            m_timer->setSingleShot(true);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimeOut()));
        }
        else
        {
            if(m_timer->isActive())
                m_timer->stop();
        }
        m_timer->start(timeout);
    }
}

void StatusBar::OnTimeOut()
{
    if(!m_temporaryLabel.isEmpty())
    {
        m_temporaryLabel.clear();
        ShowText();
        emit permanentMessageChanged(m_temporaryLabel);
    }
}

void StatusBar::ClearTemporaryMessage()
{
    m_temporaryLabel.clear();
}

QString StatusBar::CurrentTemporaryMessage() const
{
    return m_temporaryLabel;
}
