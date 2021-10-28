#include "helpdialog.h"

#include <QDebug>

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "qdef.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    m_textViewer(0)
{
    setObjectName("HelpDialog");

    LoadHelp();

    Init();
}

HelpDialog::~HelpDialog()
{
    DEBUG_DESTROY_Q
}

void HelpDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *contentLayout = new QHBoxLayout;
    QListWidget *list = new QListWidget;
    m_textViewer = new QTextBrowser;
    QStringList menuText;

    Q_FOREACH(const HelpDialog::HelpItem &item, m_helpList)
        menuText.append(item.category);

    list->addItems(menuText);
    list->setMaximumWidth(128);
    list->setMinimumWidth(96);
    contentLayout->addWidget(list);
    contentLayout->addWidget(m_textViewer);

    layout->addLayout(contentLayout);

    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetContentText(QListWidgetItem *)));

    setLayout(layout);

    SetContentText(list->item(0));
    list->setCurrentRow(0);

    setWindowTitle("Help");
    resize(480, 360);
}

void HelpDialog::SetContentText(QListWidgetItem *item)
{
    QString label = item->text();
    Q_FOREACH(const HelpDialog::HelpItem &item, m_helpList)
    {
        if(item.category == label)
        {
            //m_textViewer->setText(item.items.join("\n"));
            m_textViewer->setHtml(item.items.join("<br/>"));
            break;
        }
    }
}

int HelpDialog::Show(QWidget *parent)
{
    HelpDialog dialog(parent);
    return dialog.exec();
}

bool HelpDialog::LoadHelp()
{
    const QString Help(":/HELP");

    QFile f(Help);
    QXmlSimpleReader reader;
    QXmlInputSource source(&f);
    QDomDocument doc;
    if(!doc.setContent(&source, &reader))
        return false;

    QDomElement help = doc.documentElement();
    if (help.tagName() != "help")
        return false;

    QDomNodeList categorys = help.childNodes();
    for(int i = 0; i < categorys.size(); i++)
    {
        QDomNode node = categorys.at(i);
        if(!node.isElement())
            continue;
        QDomElement category = node.toElement();
        if(category.tagName() != "category")
            continue;
        QString categroyName = category.attribute("name");
        QStringList list;
        QDomNodeList items = category.childNodes();
        for(int j = 0; j < items.size(); j++)
        {
            QDomNode node = items.at(j);
            if(!node.isElement())
                continue;
            QDomElement item = node.toElement();
            if(item.tagName() != "item")
                continue;
            QDomNodeList texts = item.childNodes();
            for(int k = 0; k < texts.size(); k++)
            {
                QDomNode node = texts.at(k);
                if(!node.isCDATASection())
                    continue;
                QDomCDATASection cdata = node.toCDATASection();
                QString t = cdata.data();
                QString fontWeight = item.attribute("font-weight");
                if(fontWeight == "bold")
                    t = "<b>" + t + "</b>";
                QString fontStyle = item.attribute("font-weight");
                if(fontStyle == "italic")
                    t = "<i>" + t + "</i>";
                QString fontSize = item.attribute("font-size");
                if(!fontSize.isEmpty())
                    t = QString("<font size=\"%1\">").arg(fontSize) + t + "</font>";
                QString color = item.attribute("color");
                if(!color.isEmpty())
                    t = QString("<font color=\"%1\">").arg(color) + t + "</font>";
                list.push_back(t);
            }
        }

        m_helpList.push_back(HelpDialog::HelpItem(categroyName, list));
    }
    return true;
}
