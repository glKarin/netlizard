#include "nlscenetreewidget.h"

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QApplication>

#include "common/nldef.h"
#include "common/nldbg.h"
#include "engine/nlscene.h"
#include "engine/nlactor.h"
#include "engine/nlcomponent.h"
#include "engine/nlscript.h"
#include "engine/nlactorcontainer.h"
#include "engine/nlrigidbody.h"
#include "widget/nlpropformgroupwidget.h"

NLSceneTreeWidget::NLSceneTreeWidget(QWidget *widget)
    : QTreeWidget(widget),
      m_scene(0)
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
        return;

    const int Count = m_scene->ActorCount();
    for(int i = 0; i < Count; i++)
    {
        NLActor *a = m_scene->GetActor(i);
        AddActorNode(a);
    }
    setHeaderLabel(m_scene->ClassName() + "::" + m_scene->objectName() + "(" + NLPOINTER_STR(m_scene) + ")");
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
    subItem->setToolTip(0, actor->ClassName() + "::" + actor->objectName() + "(" + actor->Name() + " " + QString().sprintf("%p", actor) + ")");
    subItem->setData(0, Qt::UserRole, QVariant::fromValue<NLActor *>(actor));
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
    NLActor *obj = item->data(0, Qt::UserRole).value<NLActor *>();
    if(obj)
        emit actorSelected(obj);
}

#define ACTION_INVALID -1
#define ACTION_ADD 1
#define ACTION_ADD_CHILD 2
#define ACTION_REMOVE 3
#define ACTION_ADD_COMPONENT 4
#define ACTION_ADD_SCRIPT 5
#define ACTION_EXPAND_ALL 6
#define ACTION_COLLASPSE_ALL 7
int NLSceneTreeWidget::ShowMenu(const QPoint &pos, QTreeWidgetItem *item)
{
    QScopedPointer<QMenu> m_menu(new QMenu(this));
    if(item)
    {
        m_menu->addAction(tr("Add child"))->setData(ACTION_ADD_CHILD);
        m_menu->addAction(tr("Remove actor"))->setData(ACTION_REMOVE);
        m_menu->addSeparator();
        m_menu->addAction(tr("Add component"))->setData(ACTION_ADD_COMPONENT);
        m_menu->addAction(tr("Add script"))->setData(ACTION_ADD_SCRIPT);
    }
    else
    {
        m_menu->addAction(tr("Add actor"))->setData(ACTION_ADD);
    }
    m_menu->addSeparator();
    m_menu->addAction(tr("Expand all"))->setData(ACTION_EXPAND_ALL);
    m_menu->addAction(tr("Collaspse all"))->setData(ACTION_COLLASPSE_ALL);
    QAction *action = m_menu->exec(pos);
    if(action)
        return action->data().toInt();
    return ACTION_INVALID;
}

void NLSceneTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(!m_scene)
    {
        QTreeWidget::contextMenuEvent(event);
        return;
    }
    QTreeWidgetItem *item = itemAt(event->pos());
    int action = ShowMenu(event->globalPos(), item);
    if(action < 0)
    {
        QTreeWidget::contextMenuEvent(event);
        return;
    }
    if(action == ACTION_COLLASPSE_ALL)
    {
        collapseAll();
    }
    else if(action == ACTION_EXPAND_ALL)
    {
        expandAll();
    }
    else
    {
        if(item)
        {
            NLActor *actor = item->data(0, Qt::UserRole).value<NLActor *>();
            if(action == ACTION_ADD_CHILD)
            {
                actor->CreateChild(NLProperties("name", "new_NLActor"));
            }
            else if(action == ACTION_REMOVE)
            {
                delete actor;
            }
            else if(action == ACTION_ADD_COMPONENT)
            {
                actor->CreateComponent(NLProperties("name", "new_NLComponent"));
            }
            else if(action == ACTION_ADD_SCRIPT)
            {
                actor->CreateScript(NLProperties("name", "new_NLScript"));
            }
            else
            {
                QTreeWidget::contextMenuEvent(event);
            }
        }
        else
        {
            if(action == ACTION_ADD)
            {
                m_scene->CreateActor(NLProperties("name", "new_NLActor"));
            }
            else
            {
                QTreeWidget::contextMenuEvent(event);
            }
        }
    }
}

void NLSceneTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    if(!m_scene)
        return;
    if (event->button() == Qt::LeftButton)
    {
        QTreeWidgetItem *item = itemAt(event->pos());
        if(!item)
            return;
        m_dragStartPosition = event->pos();
    }
}

void NLSceneTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTreeWidget::mouseMoveEvent(event);
    if(!m_scene)
        return;

    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QTreeWidgetItem *item = itemAt(m_dragStartPosition);
    if(!item)
        return;
    NLActor *actor = item->data(0, Qt::UserRole).value<NLActor *>();
    QVariant value = NLinstanceofv(actor, NLRigidbody) ? QVariant::fromValue<NLRigidbody *>(static_cast<NLRigidbody *>(actor)) : QVariant::fromValue<NLActor *>(actor);

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setProperty(NLFormGroupWidget::FormFieldDragDropDataKey, value);
    mimeData->setData(NLFormGroupWidget::FormFieldDragDropMIME, QByteArray());
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
    Q_UNUSED(dropAction);
    //delete drag;
}
