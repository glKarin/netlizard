#include "indexviewer.h"

#include <QDebug>

#include <QScrollArea>
#include <QAction>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QColor>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include "utils/ioutility.h"

#include "netlizard.h"
#include "qdef.h"

#define CELL_SIZE 128
#define CELL_SPACING 10

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

QAction * HomeCell::Action()
{
    return m_action;
}

void HomeCell::TriggerAction()
{
    emit actionTrigger(m_action);
}




IndexViewer::IndexViewer(QWidget *parent) :
    BaseViewer(parent),
    m_layout(0),
    m_tools(0),
    m_inited(false)
{
    setObjectName("IndexViewer");
    Init();
}

IndexViewer::~IndexViewer()
{
}

void IndexViewer::Init()
{
    QScrollArea *root = new QScrollArea;
    m_layout = new QGridLayout;
    QGroupBox *container = new QGroupBox;

    m_list
            << HomeCellItem("&Text viewer", "text_viewer")
               << HomeCellItem("&String viewer", "string_viewer")
                  << HomeCellItem("&Image viewer", "image_viewer")
                     << HomeCellItem("&Font viewer", "font_viewer")
                        << HomeCellItem("&Map viewer", "map_viewer")
                           << HomeCellItem("&Item viewer", "item_viewer")
                              << HomeCellItem("&Animation viewer", "animation_viewer")
                                 << HomeCellItem("&Sprite viewer", "sprite_viewer")
                ;

    //container->setMinimumWidth(CELL_SIZE);
    container->setTitle("NETLizard");
    m_layout->setSpacing(10);
    container->setLayout(m_layout);

    root->setWidget(container);
    root->setWidgetResizable(true);
    root->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SetCentralWidget(root);

    Layout();

    SetTitleLabel("Home");
    SetTitle("NETLizard resource viewer");
}

void IndexViewer::resizeEvent(QResizeEvent *event)
{
    BaseViewer::resizeEvent(event);
    Layout();
}

void IndexViewer::Layout()
{
    QScrollArea *container = static_cast<QScrollArea *>(CentralWidget());
    int w = container->width();
    const int Col = qMax((w - 80) / (CELL_SIZE + m_layout->spacing()), 1);
    int r = 0;
    int c = 0;
    if(!m_inited)
    {
        m_inited = true;
        const QString buttonStyle("QPushButton { border: 1px solid #8f8f91; font-size: 16px; font-weight: bold; color: #FFFFFF; background-color: %1 }");
        Q_FOREACH(const HomeCellItem &item, m_list)
        {
            HomeCell *cell = new HomeCell(item.label, item.data);
            connect(cell, SIGNAL(actionTrigger(QAction*)), this, SIGNAL(openViewer(QAction*)));
            cell->setStyleSheet(buttonStyle.arg(RandomColor()));
            m_layout->addWidget(cell, r, c++);
            if(c >= Col)
            {
                r++;
                c = 0;
            }
        }

        for(int i = 0; i < m_layout->columnCount(); i++)
        {
            m_layout->setColumnStretch(i, 0);
            m_layout->setColumnMinimumWidth(i, CELL_SIZE);
        }
        for(int i = 0; i < m_layout->rowCount(); i++)
        {
            m_layout->setRowStretch(i, 0);
            m_layout->setRowMinimumHeight(i, CELL_SIZE);
        }

        const QString toolsButtonStyle("QPushButton { border: 1px solid #8f8f91; border-radius: 24px }");
        HomeCell *cell;
        QVBoxLayout *vLayout = new QVBoxLayout;
        HomeCellItemList list;
        list << HomeCellItem("&Setting", "setting")
                   << HomeCellItem("&Help", "help")
                      << HomeCellItem("&About", "about")
                         << HomeCellItem("&Exit", "exit")
                    ;
        Q_FOREACH(const HomeCellItem &item, list)
        {
            cell = new HomeCell(item.label, item.data);
            cell->setFixedSize(48, 48);
            cell->setStyleSheet(toolsButtonStyle);
            connect(cell, SIGNAL(actionTrigger(QAction*)), this, SIGNAL(openViewer(QAction*)));
            vLayout->addWidget(cell);
        }
        vLayout->addStretch();
        m_tools = new QWidget(container);
        m_tools->setFixedWidth(64);
        m_tools->setLayout(vLayout);
    }
    else
    {
        QList<QLayoutItem *> list;
        while(!m_layout->isEmpty())
        {
            QLayoutItem *item = m_layout->takeAt(0);
            list.push_back(item);
        }
        Q_FOREACH(QLayoutItem *item, list)
        {
            m_layout->addItem(item, r, c++);
            if(c >= Col)
            {
                r++;
                c = 0;
            }
        }
    }
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
