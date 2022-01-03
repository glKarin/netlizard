#include "nlscenetreewidget.h"

#include <QDebug>

#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"
#include "nlactorcontainer.h"

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
    DEBUG_DESTROY_Q
}

void NLSceneTreeWidget::Init()
{
    setHeaderLabel(tr("No scene"));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(OnItemClicked(QTreeWidgetItem *, int)));
    //setMinimumWidth(128);
}

void NLSceneTreeWidget::Reset()
{
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
        UpdateTreeData();
    }
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
