#include "indexviewer.h"

#include <QDebug>

#include <QScrollArea>
#include <QScrollBar>
#include <QAction>
#include <QVBoxLayout>
#include <QColor>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QTimer>
#include <QLinearGradient>
#include <QTimeLine>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include "netlizard.h"
#include "flowlayout.h"
#include "qdef.h"
#include "utils/nlmath.h"
#include "lang.h"

#define CELL_SIZE 128
#define CELL_SPACING 10

#define MIN_ALPHA 0.2
#define ROT_SPEED 1

static MenuItem LoadMenuItem(const QDomElement &category)
{
    MenuItem sc;
    if(category.tagName() != "item")
    {
        return sc;
    }

    sc.label = category.attribute("name");
    sc.data = category.attribute("alias");
    sc.description = category.attribute("description");
    sc.show = category.attribute("show");
    sc.shortcut = category.attribute("shortcut");

    QDomNodeList items = category.childNodes();
    for(int j = 0; j < items.size(); j++)
    {
        QDomNode node = items.at(j);
        if(!node.isElement())
            continue;
        QDomElement item = node.toElement();
        if(item.tagName() == "item")
        {
            MenuItem si = LoadMenuItem(item);
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


IndexGroupBox::IndexGroupBox(QWidget *parent)
    : QGroupBox(parent),
      m_timer(0),
      m_angle(0),
      m_speed(ROT_SPEED),
      m_alphaFactory(0)
{
    setObjectName("IndexGroupBox");
    Init();
}

IndexGroupBox::IndexGroupBox(const QString &title, QWidget *parent)
    : QGroupBox(title, parent),
      m_timer(0),
      m_angle(0),
      m_speed(ROT_SPEED),
      m_alphaFactory(0)
{
    setObjectName("IndexGroupBox");
    Init();
}

IndexGroupBox::~IndexGroupBox()
{
    DEBUG_DESTROY_Q
}

void IndexGroupBox::Init()
{
    m_timer = new QTimeLine(10000, this);
    m_timer->setUpdateInterval(100);
    m_timer->setLoopCount(1);
    m_timer->setCurveShape(QTimeLine::LinearCurve);
    connect(m_timer, SIGNAL(valueChanged(qreal)), this, SLOT(Update(qreal)));
    connect(m_timer, SIGNAL(finished()), this, SLOT(OnFinished()));
    m_timer->start();
    m_rect.setSize(size());
    CalePainterPath();
}

void IndexGroupBox::OnFinished()
{
    if(m_timer->direction() == QTimeLine::Backward)
    {
        m_timer->setDirection(QTimeLine::Forward);
    }
    else
    {
        m_timer->setDirection(QTimeLine::Backward);
    }
    m_timer->start();
}

void IndexGroupBox::Update(qreal value)
{
    QTimeLine::Direction d = m_timer->direction();
    m_alphaFactory = (1.0 - MIN_ALPHA) * value;
    m_angle = NL::clamp_angle(360 * value* m_speed);
    if(d == QTimeLine::Backward)
    {
        //m_alphaFactory *= 0.9;
        m_angle = 360 - m_angle;
    }
    m_alphaFactory += MIN_ALPHA;
    repaint();
}

void IndexGroupBox::ResetPaintSize(int value)
{
//    QScrollBar *bar = static_cast<QScrollArea *>(parentWidget()->parentWidget())->verticalScrollBar();
//    int document_length = bar->maximum() - bar->minimum() + bar->pageStep();
    m_rect.setTop(value);
    m_rect.setHeight(parentWidget()->height());
    repaint();
}

void IndexGroupBox::CalePainterPath()
{
    const int R = GetRadius();
    const QRectF Rect(-R, -R, R * 2, R * 2);
    const QRectF CenterRect(-R * 0.309, -R * 0.309, R * 0.618, R * 0.618);
    const QRectF EmptyRect(-R / 8, -R / 8, R / 4, R / 4);

    m_diskPath &= QPainterPath();
    m_diskPath.arcTo(Rect, 90, 360);

    m_diskCenterPath &= QPainterPath();
    m_diskCenterPath.arcTo(CenterRect, 90, 360);
    m_diskPath -= m_diskCenterPath;

    QPainterPath emptyPath;
    emptyPath.arcTo(EmptyRect, 90, 360);
    m_diskCenterPath -= emptyPath;
}

void IndexGroupBox::ResetPaintSize(const QSize &size)
{
    if(m_rect.size() != size)
    {
        QScrollBar *bar = static_cast<QScrollArea *>(parentWidget()->parentWidget())->verticalScrollBar();
        m_rect.setTop(bar->value());
        m_rect.setSize(size);
        CalePainterPath();
        repaint();
    }
}

void IndexGroupBox::paintEvent(QPaintEvent *event)
{
    QGroupBox::paintEvent(event);
    QPainter p(this);
    const int W = m_rect.width();
    const int H = m_rect.height();
    const int R = GetRadius();
//    const int X = W / 2 - R;
//    const int Y = H / 2 - R;
    const QPoint Center(W / 2, m_rect.top() + H / 2);
    p.translate(Center);
    p.rotate(-m_angle);
    p.save();
    {
        QLinearGradient gradient(QPoint(-R, 0), QPoint(R, 0));
        gradient.setColorAt(0.0, QColor::fromRgbF(1.0f, 0.0f, 0.0f, 0.8f * m_alphaFactory));
        gradient.setColorAt(0.5, QColor::fromRgbF(0.5f, 0.0f, 0.5f, 0.618f * m_alphaFactory));
        gradient.setColorAt(1.0, QColor::fromRgbF(0.0f, 0.0f, 1.0f, 0.8f * m_alphaFactory));
        QBrush brush(gradient);
        brush.setStyle(Qt::LinearGradientPattern);
        p.fillPath(m_diskPath, brush);

        QBrush transBrush(QColor::fromRgbF(0.5, 0.5, 0.5, 0.1));
        p.fillPath(m_diskCenterPath, transBrush);

        QFont font = p.font();
        font.setBold(true);
        font.setPixelSize(R);
        p.setFont(font);
        QPen pen = p.pen();

        pen.setColor(QColor::fromRgbF(0.0f, 0.0f, 1.0f, 0.618f * m_alphaFactory));
        p.setPen(pen);
        p.drawText(-R, -R, R, R * 2, Qt::AlignCenter, "C");

        p.rotate(-180);
        pen.setColor(QColor::fromRgbF(1.0f, 0.0f, 0.0f, 0.618f * m_alphaFactory));
        p.setPen(pen);
        p.drawText(/*0, -R, R, R * 2*/-R, -R, R, R * 2, Qt::AlignCenter, "V");
        p.rotate(180);
    }
    p.restore();
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

const MenuItemList & IndexViewer::ActionMap()
{
     static MenuItemList _map;
     if(_map.isEmpty())
     {
        LoadMenus(_map);
     }
     return _map;
}

void IndexViewer::Init()
{
    QScrollArea *root = new QScrollArea;
    IndexGroupBox *container = new IndexGroupBox(root);
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
    connect(root->verticalScrollBar(), SIGNAL(valueChanged(int)), container, SLOT(ResetPaintSize(int)));

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
    QSize size = container->size();
    size.setWidth(size.width() - container->verticalScrollBar()->width());
    static_cast<IndexGroupBox *>(container->widget())->ResetPaintSize(size);
}

void IndexViewer::Layout()
{
    QScrollArea *container = static_cast<QScrollArea *>(CentralWidget());

    QStringList actions;
    actions << "resource"
               << "3d"
                  ;

    const LangHelper lang("MENU");
    const QString buttonStyle("QPushButton { border: 1px solid #8f8f91; font-size: 16px; font-weight: bold; color: #FFFFFF; background-color: %1 }");
    const MenuItemList &Map = ActionMap();
    Q_FOREACH(const QString &name, actions)
    {
        Q_FOREACH(const MenuItem &item, Map)
        {
            if(item.data.toString() == name)
            {
                Q_FOREACH(const MenuItem &a, item.items)
                {
                    if(a.IsMenu())
                        continue;
                    HomeCell *cell = new HomeCell(lang[a.label], a.data);
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
        Q_FOREACH(const MenuItem &item, Map)
        {
            if(item.data.toString() == name)
            {
                if(item.IsItem())
                {
                    cell = new HomeCell(lang[item.label], item.data);
                    cell->setFixedSize(48, 48);
                    cell->setStyleSheet(toolsButtonStyle);
                    connect(cell, SIGNAL(actionTrigger(QAction *)), this, SIGNAL(openViewer(QAction *)));
                    vLayout->addWidget(cell);
                }
                else
                {
                    Q_FOREACH(const MenuItem &a, item.items)
                    {
                        if(a.IsMenu())
                            continue;
                        cell = new HomeCell(lang[a.label], a.data);
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
#undef CRAND

bool IndexViewer::LoadMenus(MenuItemList &map)
{
    const QString Menu(":/MENU");

    QFile f(Menu);
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
        MenuItem sc = LoadMenuItem(category);
        if(sc.IsValid())
            map.push_back(sc);
    }
    return true;
}
