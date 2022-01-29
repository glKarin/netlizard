#ifndef _KARIN_NLSCENETREEWIDGET_H
#define _KARIN_NLSCENETREEWIDGET_H

#include <QTreeWidget>

#include "engine/nldef.h"

class QTreeWidgetItem;
class QMenu;
class NLScene;
class NLActor;

class NLLIB_EXPORT NLSceneTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit NLSceneTreeWidget(QWidget *widget = 0);
    virtual ~NLSceneTreeWidget();
    NLScene * Scene() { return m_scene; }
    void SetScene(NLScene *scene);
    void Reset();

Q_SIGNALS:
    void actorSelected(NLActor *actor);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

private Q_SLOTS:
    void OnItemClicked(QTreeWidgetItem *item, int i);
    void OnActorChanged();
    void OnSceneActorChanged(NLActor *actor = 0);

private:
    void Init();
    void AddActorNode(NLActor *actor, QTreeWidgetItem *parent = 0);
    int ShowMenu(const QPoint &pos, QTreeWidgetItem *item = 0);
    void UpdateTreeData();

private:
    NLScene *m_scene;
    QPoint m_dragStartPosition;

    Q_DISABLE_COPY(NLSceneTreeWidget)
};

#endif // _KARIN_NLSCENETREEWIDGET_H
