#ifndef _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H
#define _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "nlsequencemap.h"

#define SYNTAX_COLOR(e) Color(NLGeneralSyntaxHighlighter::SyntaxConfig::Syntax_##e)

class NLGeneralSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    struct SyntaxConfig
    {
        enum Syntax_e{
            Syntax_Normal = 0,
            Syntax_Keyword = 1,
            Syntax_Number = 2,
            Syntax_String = 3,
            Syntax_Type = 4,
            Syntax_Constant = 5,
            Syntax_Label = 6,
            Syntax_Comment = 7
        };
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
        {
            this->pattern.setMinimal(true);
            this->pattern.setPatternSyntax(QRegExp::RegExp2);
        }
        bool IsValid() const { return pattern.isValid() && format.isValid(); }
        operator bool() const { return IsValid(); }
        int Highlighting(NLGeneralSyntaxHighlighter *hl, const QString &text);
        friend bool operator==(const SyntaxConfig &a, const SyntaxConfig &b) { return a.pattern == b.pattern && a.format == b.format; }
        friend bool operator!=(const SyntaxConfig &a, const SyntaxConfig &b) { return !operator==(a, b); }
        static QTextCharFormat GenFormat(Syntax_e e, const QColor &color = QColor());
    };
    typedef NLSequenceHash<QString, SyntaxConfig> SyntaxConfigMap;

    class SyntaxColorScheme
    {
    public:
        enum ColorScheme_e{
            ColorScheme_User = 0,
            ColorScheme_QtCreator = 1,
            ColorScheme_Vim = 2
        };
        typedef QHash<SyntaxConfig::Syntax_e, QColor> ColorSchemeMap;
    public:
        SyntaxColorScheme(ColorScheme_e e = ColorScheme_QtCreator);
        QColor & Color(SyntaxConfig::Syntax_e e) { return m_colorScheme[e]; }
        void SetColor(SyntaxConfig::Syntax_e e, const QColor &c) { m_colorScheme[e] = c; }
        QColor & operator[](SyntaxConfig::Syntax_e e) { return m_colorScheme[e]; }
        QColor operator[](SyntaxConfig::Syntax_e e) const { return m_colorScheme.value(e); }
        friend bool operator==(const SyntaxColorScheme &a, const SyntaxColorScheme &b) { return a.m_colorScheme == b.m_colorScheme; }
        friend bool operator!=(const SyntaxColorScheme &a, const SyntaxColorScheme &b) { return a.m_colorScheme != b.m_colorScheme; }
        QString Name() const;
        void SetColorScheme(ColorScheme_e e);
        void SetColorScheme(const ColorSchemeMap &cs);
        void Clear();
        ColorScheme_e SchemeType() const { return m_schemeType; }

    private:
        ColorSchemeMap m_colorScheme;
        ColorScheme_e m_schemeType;
    };

public:
    explicit NLGeneralSyntaxHighlighter(QObject *parent);
    explicit NLGeneralSyntaxHighlighter(QTextDocument *parent);
    virtual ~NLGeneralSyntaxHighlighter();
    SyntaxConfigMap SyntaxConfigs() const { return m_syntaxConfigs; }
    void SetSyntaxConfigs(const SyntaxConfigMap &conf);
    void AddSyntaxConfig(const SyntaxConfig &conf);
    void SetSyntaxConfig(const SyntaxConfig &conf);
    void RemoveSyntaxConfig(const QString &pattern);
    void AddSyntaxConfig(const QString &pattern, SyntaxConfig::Syntax_e e);
    void SetSyntaxConfig(const QString &pattern, SyntaxConfig::Syntax_e e);
    void AddSyntaxConfig(const QRegExp &pattern, SyntaxConfig::Syntax_e e);
    void SetSyntaxConfig(const QRegExp &pattern, SyntaxConfig::Syntax_e e);
    void Clear();
    void SetColorScheme(SyntaxColorScheme::ColorScheme_e e);
    void SetColorScheme(const SyntaxColorScheme &scheme);
    SyntaxColorScheme ColorScheme() const { return m_syntaxColorScheme; }
    NLGeneralSyntaxHighlighter & operator<<(const SyntaxConfig &conf) { AddSyntaxConfig(conf); return *this; }
    NLGeneralSyntaxHighlighter & operator+(const SyntaxConfig &conf) { AddSyntaxConfig(conf); return *this; }
    NLGeneralSyntaxHighlighter & operator-(const QString &pattern) { RemoveSyntaxConfig(pattern); return *this; }
    QString Name() const { return m_name; }

protected:
    explicit NLGeneralSyntaxHighlighter(const QString &name, QObject *parent);
    explicit NLGeneralSyntaxHighlighter(const QString &name, QTextDocument *parent);
    virtual void highlightBlock(const QString &text);
    virtual void GeneralHighlighting(const QString &text);
    virtual void InitSyntaxConfigs() {}

private:
    void SetFormat(int start, int count, const QTextCharFormat &format) { setFormat(start, count, format); }

private:
    SyntaxConfigMap m_syntaxConfigs;
    SyntaxColorScheme m_syntaxColorScheme;
    QString m_name;

    friend class SyntaxConfig;
};

#endif // _KARIN_NLGENERALSYNTAXHIGHLIGHTER_H
