#include "nlgeneralsyntaxhighlighter.h"

#include <QDebug>

#include "qdef.h"

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
