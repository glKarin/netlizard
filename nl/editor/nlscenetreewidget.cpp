#include "nlscenetreewidget.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

#include "engine/nldbg.h"
#include "engine/nlscene.h"
#include "engine/nlactor.h"
#include "engine/nlcomponent.h"
#include "engine/nlscript.h"
#include "engine/nlactorcontainer.h"

NLSceneTreeWidget::NLSceneTreeWidget(QWidget *widget)
    : QTreeWidget(widget),
      m_scene(0),
      m_menu(0)
{
    setObjectName("NLSceneTreeWidget");
    Init();
}

NLSceneTreeWidget::~NLSceneTreeWidget()
{
    m_scene = 0;
    NLDEBUG_DESTROY_Q
}

void NLSceneTreeWidget::Init()
{
    setHeaderLabel(tr("No scene"));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(OnItemClicked(QTreeWidgetItem *, int)));
    //setMinimumWidth(128);
}

void NLSceneTreeWidget::Reset()
{
    if(m_scene)
        m_scene->disconnect(this);
    m_scene = 0;
    clear();
    setHeaderLabel(tr("No scene"));
}

void NLSceneTreeWidget::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        Reset();
        m_scene = scene;
        if(m_scene)
            connect(m_scene, SIGNAL(actorChanged(NLActor *)), this, SLOT(OnSceneActorChanged()));
        UpdateTreeData();
    }
}

void NLSceneTreeWidget::OnSceneActorChanged(NLActor *actor)
{
    Q_UNUSED(actor);
    clear();
    UpdateTreeData();
}

void NLSceneTreeWidget::UpdateTreeData()
{
    if(!m_scene)
    {
        return;
    }

    const int Count = m_scene->ActorCount();
    for(int i = 0; i < Count; i++)
    {
        NLActor *a = m_scene->GetActor(i);
        AddActorNode(a);
    }
    setHeaderLabel(m_scene->objectName());
    expandAll();
}

void NLSceneTreeWidget::OnActorChanged()
{
    clear();
    UpdateTreeData();
}

void NLSceneTreeWidget::AddActorNode(NLActor *actor, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *subItem = new QTreeWidgetItem;
    subItem->setText(0, actor->objectName() + "(" + actor->ClassName() + ")");
    subItem->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<QObject *>(actor)));
    if(parent)
        parent->addChild(subItem);
    else
        addTopLevelItem(subItem);

    actor->disconnect(this);
    connect(actor, SIGNAL(childChanged()), this, SLOT(OnActorChanged()));

    const int Count = actor->ChildrenCount();
    if(Count == 0)
        return;
    for(int i = 0; i < Count; i++)
    {
        NLActor *a = actor->GetChild(i);
        AddActorNode(a, subItem);
    }
}

void NLSceneTreeWidget::OnItemClicked(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    QObject *obj = item->data(0, Qt::UserRole).value<QObject *>();
    if(obj)
        emit actorSelected(static_cast<NLActor *>(obj));
}

#define ACTION_INVALID -1
#define ACTION_ADD 1
#define ACTION_REMOVE 2
#define ACTION_ADD_COMPONENT 3
#define ACTION_ADD_SCRIPT 4
int NLSceneTreeWidget::ShowMenu(const QPoint &pos)
{
    if(!m_menu)
    {
        m_menu = new QMenu(this);
        m_menu->addAction(tr("Add child"))->setData(ACTION_ADD);
        m_menu->addAction(tr("Remove actor"))->setData(ACTION_REMOVE);
        m_menu->addSeparator();
        m_menu->addAction(tr("Add component"))->setData(ACTION_ADD_COMPONENT);
        m_menu->addAction(tr("Add script"))->setData(ACTION_ADD_SCRIPT);
    }
    QAction *action = m_menu->exec(pos);
    if(action)
        return action->data().toInt();
    return ACTION_INVALID;
}

void NLSceneTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());
    if(!item)
    {
        QTreeWidget::contextMenuEvent(event);
        return;
    }
    int action = ShowMenu(event->globalPos());
    if(action < 0)
    {
        QTreeWidget::contextMenuEvent(event);
        return;
    }
    QObject *data = item->data(0, Qt::UserRole).value<QObject *>();
    NLActor *actor = static_cast<NLActor *>(data);
    if(action == ACTION_ADD)
    {
        actor->CreateChild()->setObjectName("new_NLActor");
    }
    else if(action == ACTION_REMOVE)
    {
        delete actor;
    }
    else if(action == ACTION_ADD_COMPONENT)
    {
        actor->CreateComponent()->setObjectName("new_NLComponent");
    }
    else if(action == ACTION_ADD_SCRIPT)
    {
        actor->CreateScript()->setObjectName("new_NLScript");
    }
    else
    {
        QTreeWidget::contextMenuEvent(event);
    }
}
