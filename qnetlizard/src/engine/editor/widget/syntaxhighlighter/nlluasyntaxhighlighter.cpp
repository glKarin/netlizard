#include "nlluasyntaxhighlighter.h"

NLLuaSyntaxHighlighter::NLLuaSyntaxHighlighter(QObject *parent)
    : NLGeneralSyntaxHighlighter(parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
    Init();
}

NLLuaSyntaxHighlighter::NLLuaSyntaxHighlighter(QTextDocument *parent)
    : NLGeneralSyntaxHighlighter(parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
    Init();
}

void NLLuaSyntaxHighlighter::Init()
{
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::darkMagenta);
        QString pattern = "\\b(if|else|for|break|do|elseif|end|or|repeat|then|until|while|goto|return)\\b";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::red);
        QString pattern = "\\b(false|true|nil)\\b";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::blue);
        QString pattern = "\\b(function)\\b";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::cyan);
        QString pattern = "\\b(local)\\b";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::yellow);
        QString pattern = "\\b(or|not|and|in)\\b";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        //format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::green);
        QString pattern = "(?!\\\\)\".*(?!\\\\)\"";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        //format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::green);
        QString pattern = "(?!\\\\)'.*(?!\\\\)'";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        //format.setFontWeight(QFont::Bold);
        format.setFontItalic(true);
        format.setForeground(Qt::green);
        QString pattern = "--.*$";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
    {
        QTextCharFormat format;
        //format.setFontWeight(QFont::Bold);
        format.setFontItalic(true);
        format.setForeground(Qt::green);
        QString pattern = "--\\[=*\\[.*\\]=*\\]";
        AddSyntaxConfig(NLGeneralSyntaxHighlighter::SyntaxConfig(pattern, format));
    }
}
