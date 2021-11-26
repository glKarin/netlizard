#ifndef _KARIN_SCENETREEWIDGET_H
#define _KARIN_SCENETREEWIDGET_H

#include <QDockWidget>

class NLSceneTreeWidget;
class NLScene;
class NLActor;

class SceneTreeWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SceneTreeWidget(QWidget *parent = 0);
    virtual ~SceneTreeWidget();
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    
signals:
    void actorSelected(NLActor *a);
    
public slots:
    virtual void Reset();
    
protected Q_SLOTS:

private:
    void Init();

private:
    NLScene *m_scene;
    NLSceneTreeWidget *m_treeWidget;

    Q_DISABLE_COPY(SceneTreeWidget)
};

#endif // _KARIN_SCENETREEWIDGET_H
