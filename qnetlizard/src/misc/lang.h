#ifndef _KARIN_LANG_H
#define _KARIN_LANG_H

#include <QObject>
#include <QHash>

#include "qdef.h"

typedef QHash<QString, QString> ContextLangDict;
typedef QHash<QString, ContextLangDict> LangDict;

class Lang : public QObject
{
    Q_OBJECT
public:
    virtual ~Lang();
    SINGLE_INSTANCE_DEF(Lang)
    static const LangDict & GetLangDict();
    QString Tr(const QString &name, const QString &context = QString(), const QString &def = QString()) const;
    QString L(const QString &name, const QString &context = QString(), const QString &def = QString()) const { return Tr(name, context, def); }
    ContextLangDict operator[](const QString &context);
    QString operator()(const QString &context, const QString &name)  const { return Tr(name, context, name); }
    
signals:
    
public slots:
    bool Load(const QString &name = QString());

private:
    static void ClearLangDict();
    explicit Lang(QObject *parent = 0);
    static bool LoadLang(LangDict &dict, const QString &name = QString());

private:
    LangDict m_dict;

    static LangDict _langDict;
    
    Q_DISABLE_COPY(Lang)
};

class LangHelper
{
public:
    explicit LangHelper(const QString &context = QString())
        : m_lang(SINGLE_INSTANCE_OBJ(Lang)),
        m_context(context)
    {}
    virtual ~LangHelper() { m_lang = 0; }
    void SetContext(const QString &context) { if(m_context != context) m_context = context; }
    QString Context() const { return m_context; }
    QString operator[](const QString &name) const {return m_lang->operator()(m_context, name); }
    QString operator()(const QString &name, const QString &def = QString()) const {return m_lang->Tr(name, m_context, def); }
    QString Tr(const QString &name, const QString &def = QString()) const {return m_lang->Tr(name, m_context, def); }

private:
    Lang *m_lang;
    QString m_context;
};

#endif // _KARIN_LANG_H
