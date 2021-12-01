#ifndef _KARIN_SCENEINFOWIDGET_H
#define _KARIN_SCENEINFOWIDGET_H

#include <QDockWidget>

class NLSceneInfoWidget;
class NLScene;
class NLActor;

class SceneInfoWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SceneInfoWidget(QWidget *parent = 0);
    virtual ~SceneInfoWidget();
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    
public slots:
    virtual void Reset();
    
protected Q_SLOTS:

private:
    void Init();

private:
    NLScene *m_scene;
    NLSceneInfoWidget *m_infoWidget;

    Q_DISABLE_COPY(SceneInfoWidget)
};

#endif // _KARIN_SCENEINFOWIDGET_H
