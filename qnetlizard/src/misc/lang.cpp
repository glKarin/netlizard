#include "lang.h"

#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QLocale>

//#define LANG_DICT_AUTOLOAD
//#define LANG_DICT_ALLOW_EMPTY_STRING

static ContextLangDict LoadLangContext(const QDomElement &context)
{
    ContextLangDict sc;
    if(context.tagName() != "context")
    {
        return sc;
    }

    QDomNodeList items = context.childNodes();
    for(int j = 0; j < items.size(); j++)
    {
        QDomNode node = items.at(j);
        if(!node.isElement())
            continue;
        QDomElement item = node.toElement();
        if(item.tagName() != "translation")
            continue;
        QString name = item.attribute("source");
        if(name.isEmpty())
            continue;
        QString text("");
        if(item.hasChildNodes())
        {
            QDomNodeList textNodes = node.childNodes();
            for(int k = 0; k < textNodes.size(); k++)
            {
                QDomNode textNode = textNodes.at(k);
                if(!textNode.isCDATASection())
                    continue;
                QDomCDATASection cdata = textNode.toCDATASection();
                text.append(cdata.data());
            }
        }
        else
        {
            text = item.attribute("translation");
        }

        sc.insert(name, text);
    }

    return sc;
}

Lang::Lang(QObject *parent) :
    QObject(parent)
{
    setObjectName("Lang");
}

Lang::~Lang()
{
    ClearLangDict();
    DEBUG_DESTROY_Q
}


ContextLangDict Lang::operator[](const QString &context)
{
    if(!_langDict.contains(context))
        return ContextLangDict();
    return _langDict[context];
}

SINGLE_INSTANCE_DECL(Lang)

LangDict Lang::_langDict;

const LangDict & Lang::GetLangDict()
{
#ifdef LANG_DICT_AUTOLOAD
    if(_langDict.isEmpty())
    {
        LoadLang(_langDict);
    }
#endif
    return _langDict;
}

void Lang::ClearLangDict()
{
    _langDict.clear();
}

QString Lang::Tr(const QString &name, const QString &context, const QString &def) const
{
    if(!_langDict.contains(context))
        return def;
    const ContextLangDict &dict = _langDict[context];
    if(!dict.contains(name))
        return def;
    const QString &res = dict[name];
#ifndef LANG_DICT_ALLOW_EMPTY_STRING
    if(res.isEmpty())
        return def;
#endif
    return res;
}

bool Lang::Load(const QString &name)
{
    ClearLangDict();
    return LoadLang(_langDict, name);
}

bool Lang::LoadLang(LangDict &map, const QString &name)
{
    const QString Locale = name.isEmpty() ? QLocale::system().name() : name;
    QStringList chooses;
    chooses << "./i18n/lang." + Locale + ".xml"
               << "./i18n/lang.xml"
               << ":/LANG"
                  ;
    QFile f;
    bool has = false;
    Q_FOREACH(const QString &str, chooses)
    {
        f.setFileName(str);
        if(f.exists())
        {
            has = true;
            break;
        }
    }

    if(!has)
        return false;
    QXmlSimpleReader reader;
    QXmlInputSource source(&f);
    QDomDocument doc;
    if(!doc.setContent(&source, &reader))
        return false;

    QDomElement lang = doc.documentElement();
    if (lang.tagName() != "lang")
        return false;

    QDomNodeList contexts = lang.childNodes();
    for(int i = 0; i < contexts.size(); i++)
    {
        QDomNode node = contexts.at(i);
        if(!node.isElement())
            continue;
        QDomElement context = node.toElement();
        if(context.tagName() != "context")
            continue;
        QString name = context.attribute("name");
        ContextLangDict sc = LoadLangContext(context);
        if(!sc.isEmpty())
            map.insert(name, sc);
    }
    return true;
}
