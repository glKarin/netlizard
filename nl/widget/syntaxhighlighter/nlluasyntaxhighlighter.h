#ifndef _KARIN_NLLUASYNTAXHIGHLIGHTER_H
#define _KARIN_NLLUASYNTAXHIGHLIGHTER_H

#include "nlgeneralsyntaxhighlighter.h"

class NLLuaSyntaxHighlighter : public NLGeneralSyntaxHighlighter
{
public:
    explicit NLLuaSyntaxHighlighter(QObject *parent);
    explicit NLLuaSyntaxHighlighter(QTextDocument *parent);
    static const QString SyntaxName;

protected:
    void InitSyntaxConfigs();
};

#endif // _KARIN_NLLUASYNTAXHIGHLIGHTER_H
