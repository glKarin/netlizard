#include "nlscene.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QWaitCondition>
#include <QMutex>

#include <QMouseEvent>

#include "nlscenecamera.h"
#include "qdef.h"

#define LOOP_INTERVAL 10 // 0

inline void printfgl()
{
    qDebug() << "------------------------------ OpenGL ------------------------------";
    qDebug() << "Renderer: " << (const char *)glGetString(GL_RENDERER);
    qDebug() << "Vendor: " << (const char *)glGetString(GL_VENDOR);
    qDebug() << "Version: " << (const char *)glGetString(GL_VERSION);
    //qDebug() << "Shading Language Version: " << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    qDebug() << "Extension: " << (const char *)glGetString(GL_EXTENSIONS);
    qDebug() << "--------------------------------------------------------------------";
}

NLScene::NLScene(QWidget *parent) :
    QGLWidget(parent),
    m_pressed(false),
    m_loop(false),
    m_delta(0.0f),
    m_currentCamera(0),
    m_cursorVisible(true),
    m_grabMouse(false),
    m_fps(0),
    m_updateGLInterval(0),
    m_updateGLLastTime(0),
    m_currentFps(0)
{
    setObjectName("NLScene");
    SetupOpenGL();
    m_actors.SetScene(this);
    m_actors.setObjectName("SceneRootActorContainer");
    m_clearColor = QColor::fromRgbF(
                //1.0, 1.0, 1.0
                0.0, 0.0, 0.0
                );
}

NLScene::~NLScene()
{
    DEBUG_DESTROY_Q;
    Deinit();
}

void NLScene::SetupOpenGL()
{
    return;
    QGLFormat qglFormat = format();
    qglFormat.setVersion(2, 1);
    qglFormat.setStencil(true);
    qglFormat.setProfile(QGLFormat::CompatibilityProfile);
    setFormat(qglFormat);
}

void NLScene::Deinit()
{
    RunLoop(false);
    m_actors.Destroy();
}

void NLScene::Init()
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    m_actors.Init();
    RunLoop(true);
}

void NLScene::initializeGL()
{
    printfgl();

    glColor4f(0, 0, 0, 1); // black
    qglClearColor(m_clearColor);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearStencil(0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glStencilMask(~0);

    glAlphaFunc(GL_GREATER, 0.01f);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    Init();
}

void NLScene::paintGL()
{
    if(!m_currentCamera)
        return;
    m_currentCamera->Render();
    //QGLWidget::paintGL();
}

void NLScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void NLScene::mousePressEvent(QMouseEvent* event)
{
    m_pressed = true;
    int x = event->x();
    int y = event->y();
    m_lastPos.setX(x);
    m_lastPos.setY(y);

    bool res = m_actors.MouseEventHandler(event->button(), true, x, y, event->modifiers());
    if(res)
        event->accept();

    QGLWidget::mousePressEvent(event);
}

void NLScene::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    int x = event->x();
    int y = event->y();
    m_lastPos.setX(x);
    m_lastPos.setY(y);

    bool res = m_actors.MouseEventHandler(event->button(), false, x, y, event->modifiers());
    if(res)
        event->accept();
    QGLWidget::mouseReleaseEvent(event);
}

void NLScene::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->x();
    int y = event->y();
    bool res = m_actors.MouseMotionHandler(event->button(), m_pressed, x, y, m_lastPos.x(), m_lastPos.y(), event->modifiers());
    if(res)
        event->accept();
    m_lastPos.setX(x);
    m_lastPos.setY(y);
    if(m_grabMouse)
        MoveCursorToCenter();
    QGLWidget::mouseMoveEvent(event);
}

void NLScene::keyPressEvent(QKeyEvent *event)
{
    bool res = m_actors.KeyEventHandler(event->key(), true, event->modifiers());
    if(res)
        event->accept();
    QGLWidget::keyPressEvent(event);
}

void NLScene::keyReleaseEvent(QKeyEvent *event)
{
    bool res = m_actors.KeyEventHandler(event->key(), false, event->modifiers());
    if(res)
        event->accept();
   QGLWidget::keyReleaseEvent(event);
}

void NLScene::wheelEvent(QWheelEvent *event)
{
    bool res = m_actors.WheelEventHandler(event->buttons(), event->orientation(), event->delta(), event->x(), event->y(), event->modifiers());
    if(res)
        event->accept();
   QGLWidget::wheelEvent(event);
}

void NLScene::MoveCursorToCenter()
{
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    QPoint pos = mapFromGlobal(QCursor::pos());
    m_lastPos.setX(pos.x());
    m_lastPos.setY(pos.y());
}

void NLScene::SetCursorVisible(bool b)
{
    if(m_cursorVisible != b)
    {
        m_cursorVisible = b;
        setCursor(m_cursorVisible ? Qt::ArrowCursor : Qt::BlankCursor);
        //setMouseTracking(!m_cursorVisible);
    }
}

void NLScene::GrabMouseCursor(bool b)
{
    if(m_grabMouse != b)
    {
        m_grabMouse = b;
        SetCursorVisible(!b);
        if(m_grabMouse)
            MoveCursorToCenter();
    }
}

bool NLScene::IsGrabMouseCursor() const
{
    return m_grabMouse;
}

bool NLScene::IsCursorVisible() const
{
    return m_cursorVisible;
}

void NLScene::RunLoop(bool b)
{
    if(m_loop == b)
        return;
    m_delta = 0.0f;
    m_loop = b;
    if(b)
    {
        m_lastTime = QDateTime::currentMSecsSinceEpoch();
        ExecLoop();
    }
}

void NLScene::ExecLoop()
{
    if(m_loop)
        QTimer::singleShot(LOOP_INTERVAL, this, SLOT(IdleTimer_slot()));
}

void NLScene::IdleTimer_slot()
{
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    qint64 delta = ts - m_lastTime;
    m_delta = delta / 1000.0f;
    m_lastTime = ts;
    Update(m_delta);

    if(m_updateGLInterval > 0)
    {
        qint64 fd = m_lastTime - m_updateGLLastTime;
        if(fd >= m_updateGLInterval)
        {
            m_updateGLLastTime = m_lastTime;
            updateGL();
            UpdateCurrentFPS(fd);
        }
    }
    else
    {
        updateGL();
        UpdateCurrentFPS(ts);
    }

    ExecLoop();
}

void NLScene::resizeEvent(QResizeEvent *event)
{
    QGLWidget::resizeEvent(event);
    emit sizeChanged(event->size());
}

void NLScene::SetClearColor(const QColor &color)
{
    if(m_clearColor != color)
    {
        m_clearColor = color;
        qglClearColor(m_clearColor);
        updateGL();
    }
}

QColor NLScene::ClearColor() const
{
    return m_clearColor;
}

void NLScene::Reset()
{
    m_actors.Reset();
}

void NLScene::Update(float f)
{
    m_actors.Update(f);
}

void NLScene::Render()
{
    m_actors.Render();
}

void NLScene::SetCurrentCamera(NLSceneCamera *camera)
{
    m_currentCamera = camera;
    if(m_currentCamera)
        m_currentCamera->SetScene(this);
}

NLSceneCamera * NLScene::CurrentCamera()
{
    return m_currentCamera;
}

bool NLScene::MousePressed() const
{
    return m_pressed;
}

void NLScene::AddActor(NLActor *actor)
{
    if(!actor)
        return;
    m_actors.Add(actor);
}

NLScene & NLScene::operator<<(NLActor *actor)
{
    AddActor(actor);
    return *this;
}

void NLScene::RemoveActor(NLActor *actor)
{
    if(!actor)
        return;
    m_actors.Remove(actor);
}

void NLScene::RemoveActor(int index)
{
    m_actors.Remove(index);
}

NLActor * NLScene::GetActor(int index)
{
    return m_actors.Get(index);
}

NLActor * NLScene::operator[](int index)
{
    return GetActor(index);
}

bool NLScene::IsLoop() const
{
    return m_loop;
}

QPoint NLScene::MousePointerPosition() const
{
    return m_lastPos;
}

float NLScene::CurrendDelta() const
{
    return m_delta;
}

qint64 NLScene::UpdateTime() const
{
    return m_lastTime;
}

float NLScene::FPS() const
{
    return m_fps;
}

void NLScene::SetFPS(float fps)
{
    if(m_fps != fps)
    {
        m_fps = fps;
        if(m_fps <= 0)
            m_updateGLInterval = 0;
        else
        {
            m_updateGLInterval = qRound(1000.0 / (float)m_fps);
            qDebug() << "SetFPS" << m_fps << "UpdateGLInterval" << m_updateGLInterval;
        }
    }
}

float NLScene::CurrentFPS() const
{
    return m_currentFps;
}

void NLScene::UpdateCurrentFPS(qint64 delta)
{
    m_currentFps = qRound(1000.0 / (float)delta);
    //qDebug() << m_currentFps;
}
