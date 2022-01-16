#ifndef _KARIN_NLSCENE_H
#define _KARIN_NLSCENE_H

#include <QGLWidget>

#include "nlactorcontainer.h"
#include "trait/nlpropertytrait.h"

#define NL_MAX_KEY (Qt::Key_AsciiTilde + (Qt::Key_Direction_R - Qt::Key_Shift) + 1)
#define NL_MAX_BUTTON 3

class QColor;
class NLSceneCamera;

class NLLIB_EXPORT NLScene : public QGLWidget
{
    Q_OBJECT

    Q_PROPERTY(float fps READ FPS WRITE SetFPS)
    Q_PROPERTY(int updateInterval READ UpdateInterval WRITE SetUpdateInterval)
    Q_PROPERTY(QColor clearColor READ ClearColor WRITE SetClearColor)
    Q_PROPERTY(bool grabMouse READ IsGrabMouseCursor WRITE GrabMouseCursor)

public:
    explicit NLScene(QWidget *parent = 0);
    virtual ~NLScene();
    QColor ClearColor() const { return m_clearColor; }
    void Render();
    void MoveCursorToCenter();
    void SetCursorVisible(bool b);
    void GrabMouseCursor(bool b);
    bool IsGrabMouseCursor() const { return m_grabMouse; }
    bool IsCursorVisible() const { return m_cursorVisible; }
    bool IsLoop() const { return m_loop; }
    QPoint MousePointerPosition() const { return m_lastPos; }
    float CurrendDelta() const { return m_delta; }
    qint64 UpdateTime() const { return m_lastTime; }
    float FPS() const { return m_fps; }
    float CurrentFPS() const { return m_currentFps; }
    int ActorCount() const { return m_actors.Count(); }
    int UpdateInterval() const { return m_updateInterval; }
    bool KeyState(int key);
    bool MouseState(int button);
    int ActorTotalCount() const { return m_actors.TotalCount(); }
    bool KeyPressed(int key);
    bool MousePressed(int button);
    void SetCurrentCamera(NLSceneCamera *camera);

    NLSceneCamera * CurrentCamera() { return m_currentCamera; }
    NLActor * GetActor(int index) { return m_actors.Get(index); }
    NLActor * GetActor(const QString &name) { return m_actors.Get(name); }
    template <class T>
    T * GetActor_T(int index);
    template <class T>
    T * GetActor_T(const QString &name);
    NLActor * operator[](int index) { return GetActor(index); }
    NLActor * operator[](const QString &name) { return GetActor(name); }
    NLScene & operator<<(NLActor *actor) { AddActor(actor); return *this; }
    NLScene & operator+(NLActor *actor) { AddActor(actor); return *this; }
    NLScene & operator-(NLActor *actor) { RemoveActor(actor); return *this; }
    NLScene & operator-(int index) { RemoveActor(index); return *this; }
    NLScene & operator-(const QString &name) { RemoveActor(name); return *this; }
    void AddActor(NLActor *actor);
    void RemoveActor(NLActor *actor);
    void RemoveActor(int index) { m_actors.Remove(index); }
    void RemoveActor(const QString &name) { m_actors.Remove(name); }
    NLActor * CreateActor(const NLProperties &props = NLProperties());
    NLPROPERTY_DEF_TRAIT
    
signals:
    void sizeChanged(const QSize &s);
    void updated(float delta);
    void rendered(float fps);
    void actorChanged(NLActor *actor = 0);
    void propertyChanged(const QString &name, const QVariant &value = QVariant(), int action = 0);
    
public slots:
    void RunLoop(bool b);
    void SetClearColor(const QColor &color);
    void Reset();
    void SetFPS(float fps);
    void SetUpdateInterval(int ui);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void Init();
    virtual void Deinit();
    bool MousePressed() const { return m_pressed; }

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual void Update(float f);
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool MouseMotionHandler(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier);

private Q_SLOTS:
    virtual void IdleTimer_slot();
    void OnObjectChanged(NLObject *object);

private:
    void ExecLoop();
    void UpdateCurrentFPS(qint64 delta);
    void SetupOpenGL();
    void SetKeyState(int key, bool pressed);
    void SetButtonState(int button, bool pressed);
    int QKeyToIndex(int key);
    int QMouseToIndex(int button);

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
    int m_updateInterval;
    bool m_keyState[NL_MAX_KEY];
    bool m_mouseState[NL_MAX_BUTTON];
    bool m_keyPressed[NL_MAX_KEY];
    bool m_mousePressed[NL_MAX_BUTTON];

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

template <class T>
T * NLScene::GetActor_T(const QString &name)
{
    NLActor *obj = GetActor(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

NLPROPERTY_TEMPLATE_DECL_TRAIT(NLScene, propertyChanged)

#endif // _KARIN_NLSCENE_H
