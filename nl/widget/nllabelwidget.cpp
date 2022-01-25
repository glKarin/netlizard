#include "nllabelwidget.h"

#include <QFontMetrics>
#include <QDebug>

#include "engine/nldbg.h"
#include "utils/nlguiutility.h"

NLLabelWidget::NLLabelWidget(QWidget *parent)
    : QLabel(parent),
      m_elideMode(Qt::ElideNone)
{
    setObjectName("NLLabelWidget");
}

NLLabelWidget::NLLabelWidget(const QString &content, QWidget *parent)
    : QLabel(content, parent),
      m_elideMode(Qt::ElideNone),
      m_text(content)
{
    setObjectName("NLLabelWidget");
}

NLLabelWidget::~NLLabelWidget()
{
    NLDEBUG_DESTROY_Q;
}

void NLLabelWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QLabel::mouseDoubleClickEvent(event);
    emit dblClicked();
}

void NLLabelWidget::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    UpdateText();
}

void NLLabelWidget::SetElideMode(Qt::TextElideMode e)
{
    if(m_elideMode != e)
    {
        m_elideMode = e;
        UpdateText();
    }
}

bool NLLabelWidget::TextElideEnabled() const
{
    return m_elideMode != Qt::ElideNone
            && !hasScaledContents()
            && !wordWrap()
            //&& (minimumWidth() == maximumWidth())
            ;
}

void NLLabelWidget::setText(const QString &text)
{
    if(m_text != text)
    {
        m_text = text;
        UpdateText();
        emit textChanged(m_text);
    }
}

void NLLabelWidget::UpdateText()
{
    if(TextElideEnabled())
    {
        int w = CaleWidth();
        QString text = fontMetrics().elidedText(m_text, m_elideMode, w);
        QLabel::setText(text);
    }
    else
        QLabel::setText(m_text);
    setToolTip(m_text);
}

int NLLabelWidget::CaleWidth() const
{
    int w = maximumWidth();
    QMargins margins = contentsMargins();
    w -= (margins.left() + margins.right());
    return w;
}
