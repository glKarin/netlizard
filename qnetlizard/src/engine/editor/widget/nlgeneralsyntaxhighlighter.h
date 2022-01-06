#ifndef _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H
#define _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>

class NLGeneralSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    struct SyntaxConfig
    {
        QRegExp pattern;
        QTextCharFormat format;
        SyntaxConfig() {}
        SyntaxConfig(const QRegExp &pattern, const QTextCharFormat &format)
            : pattern(pattern),
              format(format)
        {}
        SyntaxConfig(const QString &pattern, const QTextCharFormat &format)
            : pattern(QRegExp(pattern)),
              format(format)
        {}
        bool IsValid() const { return pattern.isValid() && format.isValid(); }
        operator bool() const { return IsValid(); }
        int Highlighting(NLGeneralSyntaxHighlighter *hl, const QString &text);
        friend bool operator==(const SyntaxConfig &a, const SyntaxConfig &b) { return a.pattern == b.pattern && a.format == b.format; }
        friend bool operator!=(const SyntaxConfig &a, const SyntaxConfig &b) { return !operator==(a, b); }
    };
    typedef QHash<QString, SyntaxConfig> SyntaxConfigMap;

public:
    explicit NLGeneralSyntaxHighlighter(QObject *parent);
    explicit NLGeneralSyntaxHighlighter(QTextDocument *parent);
    virtual ~NLGeneralSyntaxHighlighter();
    SyntaxConfigMap SyntaxConfigs() const { return m_syntaxConfigs; }
    void SetSyntaxConfigs(const SyntaxConfigMap &conf);
    void AddSyntaxConfig(const SyntaxConfig &conf);
    void SetSyntaxConfig(const SyntaxConfig &conf);
    void RemoveSyntaxConfig(const QString &pattern);
    void Clear();
    NLGeneralSyntaxHighlighter & operator<<(const SyntaxConfig &conf) { AddSyntaxConfig(conf); return *this; }
    NLGeneralSyntaxHighlighter & operator+(const SyntaxConfig &conf) { AddSyntaxConfig(conf); return *this; }
    NLGeneralSyntaxHighlighter & operator-(const QString &pattern) { RemoveSyntaxConfig(pattern); return *this; }

protected:
    virtual void highlightBlock(const QString &text);
    virtual void GeneralHighlighting(const QString &text);

private:
    void SetFormat(int start, int count, const QTextCharFormat &format) { setFormat(start, count, format); }

private:
    SyntaxConfigMap m_syntaxConfigs;

    friend class SyntaxConfig;
};

#endif // _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H
