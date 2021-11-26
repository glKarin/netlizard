#ifndef _KARIN_NLNLSceneInfoWidget_H
#define _KARIN_NLNLSceneInfoWidget_H

#include <QTabWidget>

class NLScene;
class QTextBrowser;

class NLSceneInfoWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit NLSceneInfoWidget(QWidget *parent = 0);
    virtual ~NLSceneInfoWidget();
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    
signals:
    
public slots:
    void Reset();
    void UpdateSceneInfo();

private:
    void Init();

private:
    NLScene *m_scene;
    QTextBrowser *m_baseInfo;
    QTextBrowser *m_cameraInfo;

    Q_DISABLE_COPY(NLSceneInfoWidget)
    
};

#endif // _KARIN_NLNLSceneInfoWidget_H
