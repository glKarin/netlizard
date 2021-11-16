#ifndef _KARIN_NLSCENETREEWIDGET_H
#define _KARIN_NLSCENETREEWIDGET_H

#include <QTreeWidget>

#include "nldef.h"

class QTreeWidgetItem;
class NLScene;
class NLActor;

class NLSceneTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit NLSceneTreeWidget(QWidget *widget = 0);
    virtual ~NLSceneTreeWidget();
    NLScene * Scene();
    void SetScene(NLScene *scene);
    void Reset();

Q_SIGNALS:
    void actorSelected(NLActor *actor);

protected:
    void UpdateTreeData();

private Q_SLOTS:
    void OnItemClicked(QTreeWidgetItem *item, int i);

private:
    void Init();
    void AddActorNode(NLActor *actor, QTreeWidgetItem *parent = 0);

private:
    NLScene *m_scene;

    Q_DISABLE_COPY(NLSceneTreeWidget)
};

#endif // _KARIN_NLSCENETREEWIDGET_H
