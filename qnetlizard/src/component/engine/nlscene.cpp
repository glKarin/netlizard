#include "nlscene.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include <QMouseEvent>

#include "lib/vector3.h"
#include "nlscenecamera.h"
#include "qdef.h"

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
    m_cursorVisible(true),
    m_grabMouse(false)
{
    setObjectName("NLScene");
    m_clearColor = QColor::fromRgbF(1.0, 1.0, 1.0);
    m_actors.SetScene(this);
}

NLScene::~NLScene()
{
    DEBUG_DESTROY_Q;
    Deinit();
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
    QGLWidget::makeCurrent();

    printfgl();

    //glClearColor(1, 1, 1, 1);
    qglClearColor(m_clearColor);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glStencilMask(~0);

    glAlphaFunc(GL_GREATER, 0.01f);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_TEXTURE_2D);

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
    m_lastPos.setX(x);
    m_lastPos.setY(y);
    if(m_grabMouse)
        MoveCursorToCenter();
    if(res)
        event->accept();
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
    bool res = m_actors.WheelEventHandler(event->orientation(), event->delta(), event->x(), event->y(), event->modifiers());
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
        QTimer::singleShot(0, this, SLOT(IdleTimer_slot()));
}

void NLScene::IdleTimer_slot()
{
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    qint64 delta = ts - m_lastTime;
    m_delta = delta / 1000.0f;
    m_lastTime = ts;
    updateGL();
    Update(m_delta);
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
