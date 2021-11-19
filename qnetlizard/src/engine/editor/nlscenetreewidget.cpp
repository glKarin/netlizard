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
    setHeaderLabel("No scene");
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(OnItemClicked(QTreeWidgetItem *, int)));
    //setMinimumWidth(128);
}

void NLSceneTreeWidget::Reset()
{
    m_scene = 0;
    clear();
    setHeaderLabel("No scene");
}

NLScene * NLSceneTreeWidget::Scene()
{
    return m_scene;
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
        QTreeWidgetItem *subItem = new QTreeWidgetItem;
        NLActor *a = m_scene->GetActor(i);
        subItem->setText(0, a->objectName() + "(" + a->Name() + ")");
        subItem->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<QObject *>(a)));
        addTopLevelItem(subItem);

        AddActorNode(a, subItem);
    }
    setHeaderLabel(m_scene->objectName());
    expandAll();
}

void NLSceneTreeWidget::OnActorChanged()
{
    Reset();
    UpdateTreeData();
}

void NLSceneTreeWidget::AddActorNode(NLActor *actor, QTreeWidgetItem *parent)
{
    const int Count = actor->ChildrenCount();
    if(Count == 0)
        return;
    for(int i = 0; i < Count; i++)
    {
        NLActor *a = actor->GetChild(i);

        QTreeWidgetItem *subItem = new QTreeWidgetItem;
        subItem->setText(0, a->objectName() + "(" + a->Name() + ")");
        subItem->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<QObject *>(a)));
        if(parent)
            parent->addChild(subItem);
        else
            addTopLevelItem(subItem);

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
