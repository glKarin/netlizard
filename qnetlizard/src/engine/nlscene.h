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

protected:
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

    Q_DISABLE_COPY(NLScene)
};

#endif // _KARIN_NLSCENE_H
