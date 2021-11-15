#ifndef _KARIN_SCENETREEINFOWIDGET_H
#define _KARIN_SCENETREEINFOWIDGET_H

#include <QDockWidget>

class NLSceneInfoWidget;
class NLSceneTreeWidget;
class NLScene;
class NLActor;

class SceneTreeInfoWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SceneTreeInfoWidget(QWidget *parent = 0);
    virtual ~SceneTreeInfoWidget();
    void SetScene(NLScene *scene);
    NLScene * Scene();
    
signals:
    
public slots:
    virtual void Reset();
    
protected Q_SLOTS:

private:
    void Init();

private:
    NLScene *m_scene;
    NLSceneInfoWidget *m_infoWidget;
    NLSceneTreeWidget *m_treeWidget;

    Q_DISABLE_COPY(SceneTreeInfoWidget)
};

#endif // _KARIN_SCENETREEINFOWIDGET_H
