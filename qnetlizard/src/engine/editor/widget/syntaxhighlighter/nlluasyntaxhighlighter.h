#ifndef _KARIN_NLLUASYNTAXHIGHLIGHTER_H
#define _KARIN_NLLUASYNTAXHIGHLIGHTER_H

#include "nlgeneralsyntaxhighlighter.h"

class NLLuaSyntaxHighlighter : public NLGeneralSyntaxHighlighter
{
public:
    explicit NLLuaSyntaxHighlighter(QObject *parent);
    explicit NLLuaSyntaxHighlighter(QTextDocument *parent);

private:
    void Init();
};

#endif // _KARIN_NLLUASYNTAXHIGHLIGHTER_H
