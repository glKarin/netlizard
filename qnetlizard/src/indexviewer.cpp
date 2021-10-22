#include "indexviewer.h"

#include <QDebug>

#include <QScrollArea>
#include <QAction>
#include <QGridLayout>
#include <QGroupBox>

#include <cmath>

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
    m_layout(0)
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

    container->setTitle("Operation");
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
    int w = CentralWidget()->width();
    const int Col = qMax(w / CELL_SIZE, 1);
    const int Row = (int)std::ceil((float)m_list.size() / (float)Col);
    int r = 0;
    int c = 0;
    while(!m_layout->isEmpty())
    {
        QLayoutItem *item = m_layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    Q_FOREACH(const HomeCellItem &item, m_list)
    {
        HomeCell *cell = new HomeCell(item.label, item.data);
        connect(cell, SIGNAL(actionTrigger(QAction*)), this, SIGNAL(openViewer(QAction*)));
        m_layout->addWidget(cell, r, c++);
        if(c >= Col)
        {
            r++;
            c = 0;
        }
    }

    for(int i = 0; i < m_layout->columnCount(); i++)
    {
        //m_layout->setColumnStretch(i, CELL_SPACING);
        m_layout->setColumnMinimumWidth(i, CELL_SIZE);
    }
    for(int i = 0; i < m_layout->rowCount(); i++)
    {
        //m_layout->setRowStretch(i, CELL_SPACING);
        m_layout->setRowMinimumHeight(i, CELL_SIZE);
    }
}
