#include "nllineeditwidget.h"

#include <QFontMetrics>
#include <QDebug>

#include "common/nldbg.h"
#include "utils/nlguiutility.h"

#define SINGLE_CHAR "C"

NLLineEditWidget::NLLineEditWidget(QWidget *parent)
    : QLineEdit(parent),
      m_autoSize(false),
      m_dblEdit(false),
      m_horizontalPaddings(4)
{
    setObjectName("NLLineEditWidget");
}

NLLineEditWidget::NLLineEditWidget(const QString &content, QWidget *parent)
    : QLineEdit(content, parent),
      m_autoSize(false),
      m_dblEdit(false),
      m_horizontalPaddings(4)
{
    setObjectName("NLLineEditWidget");
}

NLLineEditWidget::~NLLineEditWidget()
{
    NLDEBUG_DESTROY_Q;
}

void NLLineEditWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_dblEdit && isReadOnly())
    {
        setReadOnly(false);
    }
    else
        QLineEdit::mouseDoubleClickEvent(event);
    emit dblClicked();
}

void NLLineEditWidget::SetAutoSize(bool b)
{
    if(m_autoSize != b)
    {
        m_autoSize = b;
        if(m_autoSize)
        {
            connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(OnTextChanged(const QString &)));
            setFixedSize(sizeHint());
        }
        else
        {
            disconnect(this, SLOT(OnTextChanged(const QString &)));
            NLGUIUtility::UnsetFixedSize(this);
        }
    }
}

void NLLineEditWidget::SetDoubleClickEdit(bool b)
{
    if(m_dblEdit != b)
    {
        m_dblEdit = b;
        setReadOnly(m_dblEdit);
        if(m_dblEdit)
        {
            connect(this, SIGNAL(editingFinished()), this, SLOT(FinishEdit()));
            connect(this, SIGNAL(returnPressed()), this, SLOT(FinishEdit()));
        }
        else
        {
            disconnect(this, SLOT(FinishEdit()));
            disconnect(this, SLOT(FinishEdit()));
        }
    }
}

void NLLineEditWidget::FinishEdit()
{
    if(m_dblEdit)
        setReadOnly(true);
}

void NLLineEditWidget::FixSize(QSize &size) const
{
    QMargins margins = contentsMargins();
    size.rwidth() += margins.left() + margins.right();
    size.rheight() += margins.top() + margins.bottom();
    margins = textMargins();
    size.rwidth() += margins.left() + margins.right();
    size.rheight() += margins.top() + margins.bottom();
}

void NLLineEditWidget::OnTextChanged(const QString &text)
{
    if(m_autoSize)
    {
        QSize size = CaleTextSize(text.isEmpty() ? SINGLE_CHAR : text);
        FixSize(size);
        setFixedSize(size);
    }
}

QSize NLLineEditWidget::CaleTextSize(const QString &str) const
{
    QFontMetrics fm(fontMetrics());
    QSize size = fm.size(Qt::TextSingleLine, str);
    size.rheight() += fm.leading() * 4;
    size.rwidth() += fm.leading() * m_horizontalPaddings;
    return size;
}

void NLLineEditWidget::SetHorizontalPaddings(int padding)
{
    if(m_horizontalPaddings != padding)
    {
        m_horizontalPaddings = padding;
        if(m_autoSize)
        {
            setFixedSize(sizeHint());
        }
    }
}

QSize NLLineEditWidget::sizeHint() const
{
    if(m_autoSize)
    {
        QString str(text());
        QSize size = CaleTextSize(str.isEmpty() ? SINGLE_CHAR : str);
        FixSize(size);
        return size;
    }
    return QLineEdit::sizeHint();
}

QSize NLLineEditWidget::minimumSizeHint() const
{
    if(m_autoSize)
    {
        QSize size = CaleTextSize(SINGLE_CHAR);
        FixSize(size);
        return size;
    }
    return QLineEdit::minimumSizeHint();
}
