#include "nltexteditwidget.h"

#include <QTextBlock>
#include <QDebug>
#include "engine/nldbg.h"
#include "utils/nlguiutility.h"

#define NLTEXTEDITWIDGET_DECL(X, P, currentFont_func, ctor2_P_func, ctor2_setText_func) \
X::X(QWidget *widget) \
    : P(widget), \
      m_maxHeight(-1) \
{ \
    setObjectName(#X); \
    Init(); \
} \
X::X(const QString &text, QWidget *widget) \
    : ctor2_P_func(text, widget), \
      m_maxHeight(-1) \
{ \
    setObjectName(#X); \
    Init(); \
    ctor2_setText_func(text); \
} \
X::~X() \
{ \
    NLDEBUG_DESTROY_Q \
} \
void X::SetMaxHeight(int height) \
{ \
    if(m_maxHeight != height) \
    { \
        m_maxHeight = height; \
        if(m_maxHeight < 0) \
        { \
            NLGUIUtility::UnsetFixedHeight(this); \
            disconnect(this, SLOT(OnContentsChanged())); \
        } \
        else \
        { \
            OnContentsChanged(); \
            connect(document(), SIGNAL(contentsChanged()), this, SLOT(OnContentsChanged())); \
        } \
    } \
} \
int X::MinHeight() const \
{ \
    QFontMetrics fm(currentFont_func()); \
    return fm.lineSpacing() + fm.height(); \
} \
void X::Init() \
{ \
} \
void X::OnContentsChanged() \
{ \
    if(m_maxHeight < 0) \
        return; \
    QSizeF s = ContentsHeight(); \
    const int minH = MinHeight(); \
    int maxH = m_maxHeight < minH ? minH : m_maxHeight; \
    int h = qMax(maxH, (int)s.height()); \
    m_size.setHeight(h); \
    if(height() != m_size.height()) \
        setFixedHeight(m_size.height()); \
} \
void X::showEvent(QShowEvent *event) \
{ \
    P::showEvent(event); \
    adjustSize(); \
    OnContentsChanged(); \
}

NLTEXTEDITWIDGET_DECL(NLTextEditWidget, QTextEdit, currentFont, QTextEdit, (void))
NLTEXTEDITWIDGET_DECL(NLPlainTextEditWidget, QPlainTextEdit, currentCharFormat().font, QPlainTextEdit, (void))
#define TEXTBROWSER_CTOR2_P(text, widget) QTextBrowser(widget)
NLTEXTEDITWIDGET_DECL(NLTextBrowserWidget, QTextBrowser, currentFont, TEXTBROWSER_CTOR2_P, setText)

QSizeF NLTextEditWidget::ContentsHeight() const
{
    const QTextDocument *doc = document();
    return doc->size();
}

QSizeF NLPlainTextEditWidget::ContentsHeight() const
{
    const QTextDocument *doc = document();
    QFontMetrics fm(currentCharFormat().font());
    QSizeF s = doc->size();
    s.setHeight(s.rheight() * (fm.lineSpacing() + fm.leading()));
    return s;
}

QSizeF NLTextBrowserWidget::ContentsHeight() const
{
    const QTextDocument *doc = document();
    return doc->size();
}
