#include "nlluasyntaxhighlighter.h"

const QString NLLuaSyntaxHighlighter::SyntaxName("lua");

NLLuaSyntaxHighlighter::NLLuaSyntaxHighlighter(QObject *parent)
    : NLGeneralSyntaxHighlighter(NLLuaSyntaxHighlighter::SyntaxName, parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
    InitSyntaxConfigs();
}

NLLuaSyntaxHighlighter::NLLuaSyntaxHighlighter(QTextDocument *parent)
    : NLGeneralSyntaxHighlighter(NLLuaSyntaxHighlighter::SyntaxName, parent)
{
    setObjectName("NLGeneralSyntaxHighlighter");
    InitSyntaxConfigs();
}

void NLLuaSyntaxHighlighter::InitSyntaxConfigs()
{
    SetSyntaxConfig("\\b(if|else|for|break|do|elseif|end|or|repeat|then|until|while|goto|return|or|not|and|in)\\b", SyntaxConfig::Syntax_Keyword);
    SetSyntaxConfig("\\b(false|true|nil)\\b", SyntaxConfig::Syntax_Constant);
    SetSyntaxConfig("\\b(function|local)\\b", SyntaxConfig::Syntax_Type);
    SetSyntaxConfig("::\\b[a-zA-Z0-9_]+\\b::", SyntaxConfig::Syntax_Label);
    SetSyntaxConfig("\\b-?\\d+f?\\b", SyntaxConfig::Syntax_Number); // -1f
    SetSyntaxConfig("\\b-?\\d*\\.\\d*f?\\b", SyntaxConfig::Syntax_Number); // -1.1f
    SetSyntaxConfig(QRegExp("-?\\d+e-?\\d+f?\\b", Qt::CaseInsensitive), SyntaxConfig::Syntax_Number); // -1e-1f

    SetSyntaxConfig("\".*\"", SyntaxConfig::Syntax_String);
    SetSyntaxConfig("'.*'", SyntaxConfig::Syntax_String);

    SetSyntaxConfig("--.*$", SyntaxConfig::Syntax_Comment);
}
