#ifndef _KARIN_NLSCENE_H
#define _KARIN_NLSCENE_H

#include <QGLWidget>

#include "nlactorcontainer.h"

class QColor;
class NLSceneCamera;

class NLScene : public QGLWidget
{
    Q_OBJECT
public:
    explicit NLScene(QWidget *parent = 0);
    virtual ~NLScene();
    QColor ClearColor() const;
    void Render();
    void MoveCursorToCenter();
    void SetCursorVisible(bool b);
    void GrabMouseCursor(bool b);
    bool IsGrabMouseCursor() const;
    bool IsCursorVisible() const;
    bool IsLoop() const;
    QPoint MousePointerPosition() const;
    float CurrendDelta() const;
    qint64 UpdateTime() const;
    float FPS() const;
    void SetFPS(float fps);
    float CurrentFPS() const;
    
signals:
    void sizeChanged(const QSize &s);
    
public slots:
    void RunLoop(bool b);
    void SetClearColor(const QColor &color);
    void Reset();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void Init();
    virtual void Deinit();
    void SetCurrentCamera(NLSceneCamera *camera);
    NLSceneCamera * CurrentCamera();
    bool MousePressed() const;
    void AddActor(NLActor *actor);
    void RemoveActor(NLActor *actor);
    void RemoveActor(int index);
    NLActor * GetActor(int index);
    template <class T>
    T * GetActor_T(int index);
    NLActor * operator[](int index);
    NLScene & operator<<(NLActor *actor);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual void Update(float f);

protected Q_SLOTS:

private Q_SLOTS:
    virtual void IdleTimer_slot();

private:
    void ExecLoop();
    void UpdateCurrentFPS(qint64 delta);
    void SetupOpenGL();

private:
    QColor m_clearColor;
    bool m_pressed;
    bool m_loop;
    float m_delta;
    QPoint m_lastPos;
    qint64 m_lastTime;
    NLActorContainer m_actors;
    NLSceneCamera *m_currentCamera;
    bool m_cursorVisible;
    bool m_grabMouse;
    float m_fps;
    qint64 m_updateGLInterval;
    qint64 m_updateGLLastTime;
    float m_currentFps;

    Q_DISABLE_COPY(NLScene)
};

template <class T>
T * NLScene::GetActor_T(int index)
{
    NLActor *obj = GetActor(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

#endif // _KARIN_NLSCENE_H
