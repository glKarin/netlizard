#include "nlgeneralsyntaxhighlighter.h"

#include <QDebug>

#include "qdef.h"

NLGeneralSyntaxHighlighter::SyntaxColorScheme::SyntaxColorScheme()
{
    SYNTAX_COLOR(Normal) = QColor::fromRgb(0, 0, 0);
    SYNTAX_COLOR(Keyword) = QColor::fromRgb(128, 128, 0);
    SYNTAX_COLOR(Number) = QColor::fromRgb(0, 0, 128);
    SYNTAX_COLOR(String) = QColor::fromRgb(0, 128, 0);
    SYNTAX_COLOR(Type) = QColor::fromRgb(128, 0, 128);
    SYNTAX_COLOR(Keyword) = QColor::fromRgb(128, 128, 0);
    SYNTAX_COLOR(Constant) = QColor::fromRgb(128, 128, 0);
    SYNTAX_COLOR(Label) = QColor::fromRgb(128, 0, 0);
    SYNTAX_COLOR(Comment) = QColor::fromRgb(0, 128, 0);
}

QTextCharFormat NLGeneralSyntaxHighlighter::SyntaxConfig::GenFormat(Syntax_e e, const QColor &color)
{
    QTextCharFormat format;
    switch(e)
    {
    case Syntax_Keyword:
        format.setFontWeight(QFont::Bold);
        break;
    case Syntax_Number:
        break;
    case Syntax_String:
        break;
    case Syntax_Type:
        format.setFontWeight(QFont::Bold);
        break;
    case Syntax_Constant:
        format.setFontWeight(QFont::Bold);
        break;
    case Syntax_Label:
        break;
    case Syntax_Comment:
        break;
    case Syntax_Normal:
    default:
        break;
    }
    format.setForeground(color);
    return format;
}

int NLGeneralSyntaxHighlighter::SyntaxConfig::Highlighting(NLGeneralSyntaxHighlighter *hl, const QString &text)
{
    if(!IsValid())
        return -1;
    int c = 0;
    int index = text.indexOf(pattern);
    while(index >= 0)
    {
        int length = pattern.matchedLength();
        hl->setFormat(index, length, format);
        index = text.indexOf(pattern, index + length);
    }
    return c;
}

NLGeneralSyntaxHighlighter::NLGeneralSyntaxHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
}

NLGeneralSyntaxHighlighter::NLGeneralSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
}

NLGeneralSyntaxHighlighter::NLGeneralSyntaxHighlighter(const QString &name, QObject *parent)
    : QSyntaxHighlighter(parent),
      m_name(name)
{
    setObjectName("NLGeneralSyntaxHighlighter");
}

NLGeneralSyntaxHighlighter::NLGeneralSyntaxHighlighter(const QString &name, QTextDocument *parent)
    : QSyntaxHighlighter(parent),
      m_name(name)
{
    setObjectName("NLGeneralSyntaxHighlighter");
}

NLGeneralSyntaxHighlighter::~NLGeneralSyntaxHighlighter()
{
    DEBUG_DESTROY_Q
}

void NLGeneralSyntaxHighlighter::highlightBlock(const QString &text)
{
    GeneralHighlighting(text);
}

void NLGeneralSyntaxHighlighter::GeneralHighlighting(const QString &text)
{
    Q_FOREACH(const QString &name, m_syntaxConfigs.SequenceKeys())
    {
        if(m_syntaxConfigs[name])
            m_syntaxConfigs[name].Highlighting(this, text);
    }
}

void NLGeneralSyntaxHighlighter::SetSyntaxConfigs(const NLGeneralSyntaxHighlighter::SyntaxConfigMap &conf)
{
    if(m_syntaxConfigs != conf)
    {
        bool c = !m_syntaxConfigs.isEmpty();
        m_syntaxConfigs.clear();
        Q_FOREACH(const QString &name, conf.SequenceKeys())
        {
            if(!conf[name])
                continue;
            m_syntaxConfigs[name] = conf[name];
            c = true;
        }
        if(c)
            rehighlight();
    }
}

void NLGeneralSyntaxHighlighter::AddSyntaxConfig(const NLGeneralSyntaxHighlighter::SyntaxConfig &conf)
{
    if(!conf)
        return;
    QString pattern(conf.pattern.pattern());
    if(!m_syntaxConfigs.contains(pattern))
    {
        m_syntaxConfigs.insert(pattern, conf);
        rehighlight();
    }
}

void NLGeneralSyntaxHighlighter::SetSyntaxConfig(const NLGeneralSyntaxHighlighter::SyntaxConfig &conf)
{
    if(!conf)
        return;
    if(m_syntaxConfigs[conf.pattern.pattern()] != conf)
    {
        m_syntaxConfigs[conf.pattern.pattern()] = conf;
        rehighlight();
    }
}

void NLGeneralSyntaxHighlighter::AddSyntaxConfig(const QString &pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::Syntax_e e)
{
    SyntaxConfig config(pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::GenFormat(e, m_syntaxColorScheme[e]));
    AddSyntaxConfig(config);
}

void NLGeneralSyntaxHighlighter::SetSyntaxConfig(const QString &pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::Syntax_e e)
{
    SyntaxConfig config(pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::GenFormat(e, m_syntaxColorScheme[e]));
    SetSyntaxConfig(config);
}

void NLGeneralSyntaxHighlighter::AddSyntaxConfig(const QRegExp &pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::Syntax_e e)
{
    SyntaxConfig config(pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::GenFormat(e, m_syntaxColorScheme[e]));
    AddSyntaxConfig(config);
}

void NLGeneralSyntaxHighlighter::SetSyntaxConfig(const QRegExp &pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::Syntax_e e)
{
    SyntaxConfig config(pattern, NLGeneralSyntaxHighlighter::SyntaxConfig::GenFormat(e, m_syntaxColorScheme[e]));
    SetSyntaxConfig(config);
}

void NLGeneralSyntaxHighlighter::RemoveSyntaxConfig(const QString &pattern)
{
    if(m_syntaxConfigs.remove(pattern))
        rehighlight();
}

void NLGeneralSyntaxHighlighter::Clear()
{
    if(!m_syntaxConfigs.isEmpty())
    {
        m_syntaxConfigs.clear();
        rehighlight();
    }
}

void NLGeneralSyntaxHighlighter::SetColorScheme(const SyntaxColorScheme &scheme)
{
    if(m_syntaxColorScheme != scheme)
    {
        m_syntaxColorScheme = scheme;
        InitSyntaxConfigs();
        rehighlight();
    }
}
