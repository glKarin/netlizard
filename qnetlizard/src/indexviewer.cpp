#include "indexviewer.h"

#include <QDebug>

#include <QScrollArea>
#include <QAction>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QColor>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include "utils/ioutility.h"

#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "netlizard.h"
#include "flowlayout.h"
#include "qdef.h"

#define CELL_SIZE 128
#define CELL_SPACING 10

static HomeCellItem LoadMenuItem(const QDomElement &category)
{
    HomeCellItem sc;
    if(category.tagName() != "item")
    {
        return sc;
    }

    sc.label = category.attribute("name");
    sc.data = category.attribute("alias");
    sc.description = category.attribute("description");
    sc.show = category.attribute("show");

    QDomNodeList items = category.childNodes();
    for(int j = 0; j < items.size(); j++)
    {
        QDomNode node = items.at(j);
        if(!node.isElement())
            continue;
        QDomElement item = node.toElement();
        if(item.tagName() == "item")
        {
            HomeCellItem si = LoadMenuItem(item);
            if(si.IsValid())
                sc << si;
        }
    }

    return sc;
}


HomeCell::HomeCell(const QString &text, const QVariant &data, QWidget *parent)
    : QPushButton(text, parent),
    m_action(new QAction(this))
{
    setObjectName("Cell");
    setText(text);
    m_action->setData(data);
    connect(this, SIGNAL(clicked()), SLOT(TriggerAction()));
    setFixedSize(CELL_SIZE, CELL_SIZE);
}

HomeCell::~HomeCell()
{
    DEBUG_DESTROY_Q;
}

void HomeCell::TriggerAction()
{
    emit actionTrigger(m_action);
}




IndexViewer::IndexViewer(QWidget *parent) :
    BaseViewer(parent),
    m_layout(0),
    m_tools(0)
{
    setObjectName("IndexViewer");
    Init();
}

IndexViewer::~IndexViewer()
{
}

const HomeCellItemList & IndexViewer::ActionMap()
{
     static HomeCellItemList _map;
     if(_map.isEmpty())
     {
        LoadMenus(_map);
     }
     return _map;
}

void IndexViewer::Init()
{
    QScrollArea *root = new QScrollArea;
    QGroupBox *container = new QGroupBox;
    m_layout = new FlowLayout(container, CELL_SPACING, CELL_SPACING);
    SetTitleLabelVisible(false);

    //container->setMinimumWidth(CELL_SIZE);
    container->setTitle("NETLizard");
    m_layout->setSpacing(10);
    container->setLayout(m_layout);

    root->setWidget(container);
    root->setWidgetResizable(true);
    root->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SetCentralWidget(root);

    Layout();

    SetTitleLabel(tr("Home"));
    SetTitle(tr("NETLizard resource viewer"));
}

void IndexViewer::resizeEvent(QResizeEvent *event)
{
    BaseViewer::resizeEvent(event);

    QScrollArea *container = static_cast<QScrollArea *>(CentralWidget());
    m_tools->move(container->width() - 80, 0);
    m_tools->setFixedHeight(container->height());
}

void IndexViewer::Layout()
{
    QScrollArea *container = static_cast<QScrollArea *>(CentralWidget());

    QStringList actions;
    actions << "resource"
               << "3d"
                  ;

    const QString buttonStyle("QPushButton { border: 1px solid #8f8f91; font-size: 16px; font-weight: bold; color: #FFFFFF; background-color: %1 }");
    const HomeCellItemList &Map = ActionMap();
    Q_FOREACH(const QString &name, actions)
    {
        Q_FOREACH(const HomeCellItem &item, Map)
        {
            if(item.data.toString() == name)
            {
                Q_FOREACH(const HomeCellItem &a, item.items)
                {
                    if(a.IsMenu())
                        continue;
                    HomeCell *cell = new HomeCell(a.label, a.data);
                    connect(cell, SIGNAL(actionTrigger(QAction *)), this, SIGNAL(openViewer(QAction *)));
                    cell->setStyleSheet(buttonStyle.arg(RandomColor()));
                    m_layout->addWidget(cell);
                }
                break;
            }
        }
    }

    actions.clear();
    actions << "others"
               << "exit"
                  ;
    const QString toolsButtonStyle("QPushButton { border: 1px solid #8f8f91; border-radius: 24px }");
    HomeCell *cell;
    QVBoxLayout *vLayout = new QVBoxLayout;
    Q_FOREACH(const QString &name, actions)
    {
        Q_FOREACH(const HomeCellItem &item, Map)
        {
            if(item.data.toString() == name)
            {
                if(item.IsItem())
                {
                    cell = new HomeCell(item.label, item.data);
                    cell->setFixedSize(48, 48);
                    cell->setStyleSheet(toolsButtonStyle);
                    connect(cell, SIGNAL(actionTrigger(QAction *)), this, SIGNAL(openViewer(QAction *)));
                    vLayout->addWidget(cell);
                }
                else
                {
                    Q_FOREACH(const HomeCellItem &a, item.items)
                    {
                        if(a.IsMenu())
                            continue;
                        cell = new HomeCell(a.label, a.data);
                        cell->setFixedSize(48, 48);
                        cell->setStyleSheet(toolsButtonStyle);
                        connect(cell, SIGNAL(actionTrigger(QAction *)), this, SIGNAL(openViewer(QAction *)));
                        vLayout->addWidget(cell);
                    }
                }
                break;
            }
        }
    }
    vLayout->addStretch();
    m_tools = new QWidget(container);
    m_tools->setFixedWidth(64);
    m_tools->setLayout(vLayout);
    m_tools->move(container->width() - 80, 0);
    m_tools->setFixedHeight(container->height());
}

#define CRAND (qrand() % 255)
QString IndexViewer::RandomColor() const
{
    const int Limit = std::sqrt(250 * 250 * 3);
    uint r = 0;
    uint g = 0;
    uint b = 0;
    static bool first = true;
    do
    {
        qsrand(first ? std::time(0) : qrand() * qMax(1u, b));
        r = CRAND;
        qsrand(qrand() * qMax(1u, r));
        g = CRAND;
        qsrand(qrand() * qMax(1u, g));
        b = CRAND;
        QColor color(r, g, b);
        first = false;
        return color.name();
    } while(std::sqrt(r * r + g * g + b * b) >= Limit);
}

bool IndexViewer::LoadMenus(HomeCellItemList &map)
{
    const QString Help(":/MENU");

    QFile f(Help);
    QXmlSimpleReader reader;
    QXmlInputSource source(&f);
    QDomDocument doc;
    if(!doc.setContent(&source, &reader))
        return false;

    QDomElement menu = doc.documentElement();
    if (menu.tagName() != "menu")
        return false;

    QDomNodeList categorys = menu.childNodes();
    for(int i = 0; i < categorys.size(); i++)
    {
        QDomNode node = categorys.at(i);
        if(!node.isElement())
            continue;
        QDomElement category = node.toElement();
        if(category.tagName() != "item")
            continue;
        HomeCellItem sc = LoadMenuItem(category);
        if(sc.IsValid())
            map.push_back(sc);
    }
    return true;
}

