#include "nlscene.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QWaitCondition>
#include <QMutex>
#include <QMetaObject>
#include <QMetaProperty>

#include <QMouseEvent>

#include "nlscenecamera.h"
#include "nlactor.h"
#include "utils/nlfuncs.h"
#include "common/nldbg.h"

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
    m_renderDelta(0.0f),
    m_currentCamera(0),
    m_cursorVisible(true),
    m_grabMouse(false),
    m_fps(0),
    m_updateGLInterval(0),
    m_updateGLLastTime(0),
    m_currentFps(0),
    m_updateInterval(LOOP_INTERVAL)
{
    setObjectName("NLScene");
    memset(m_keyState, 0, sizeof(m_keyState));
    memset(m_mouseState, 0, sizeof(m_mouseState));
    memset(m_keyPressed, 0, sizeof(m_keyPressed));
    memset(m_mousePressed, 0, sizeof(m_mousePressed));
    SetupOpenGL();
    m_actors.SetScene(this);
    connect(&m_actors, SIGNAL(objectChanged(NLObject *)), this, SLOT(OnObjectChanged(NLObject *)));
    m_actors.setObjectName("SceneRootActorContainer");
    m_clearColor = QColor::fromRgbF(
                //1.0, 1.0, 1.0
                0.0, 0.0, 0.0
                );

    NLProperties props;

    props.Insert("fps", NLProperties("range", NLProperties("min", 0)("max", 300)));
    props.Insert("updateInterval", NLProperties("range", NLProperties("min", 0)("max", 1000)));

    SetPropertyConfig(props);
}

NLScene::~NLScene()
{
    NLDEBUG_DESTROY_Q;
    Deinit();
}

void NLScene::SetupOpenGL()
{
    return;
    QGLFormat qglFormat = format();
    qglFormat.setDepthBufferSize(32);
//    qglFormat.setVersion(2, 1);
//    qglFormat.setStencil(true);
//    qglFormat.setProfile(QGLFormat::CompatibilityProfile);
    setFormat(qglFormat);
}

void NLScene::OnObjectChanged(NLObject *object)
{
    emit actorChanged(static_cast<NLActor *>(object));
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

    //glColor4f(0, 0, 0, 1); // black
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
    const int button = event->button();
    SetButtonState(button, true);
    const int modifiers = event->modifiers();

    bool res = m_actors.MouseEventHandler(button, true, x, y, modifiers);
    res = MouseEventHandler(button, true, x, y, modifiers) || res;
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
    const int button = event->button();
    SetButtonState(button, false);
    const int modifiers = event->modifiers();

    bool res = m_actors.MouseEventHandler(button, false, x, y, modifiers);
    res = MouseEventHandler(button, false, x, y, modifiers) || res;
    if(res)
        event->accept();
    QGLWidget::mouseReleaseEvent(event);
}

void NLScene::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->x();
    int y = event->y();
    const int modifiers = event->modifiers();
    const int button = event->button();
    bool res = m_actors.MouseMotionHandler(button, m_pressed, x, y, m_lastPos.x(), m_lastPos.y(), modifiers);
    res = MouseMotionHandler(button, m_pressed, x, y, m_lastPos.x(), m_lastPos.y(), modifiers) || res;
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
    const int key = event->key();
    if(!event->isAutoRepeat())
        SetKeyState(key, true);
    const int modifiers = event->modifiers();
    bool res = m_actors.KeyEventHandler(key, true, modifiers);
    res = KeyEventHandler(key, true, modifiers) || res;
    if(res)
        event->accept();
    QGLWidget::keyPressEvent(event);
}

void NLScene::keyReleaseEvent(QKeyEvent *event)
{
    const int key = event->key();
    if(!event->isAutoRepeat())
        SetKeyState(key, false);
    const int modifiers = event->modifiers();
    bool res = m_actors.KeyEventHandler(key, false, modifiers);
    res = KeyEventHandler(key, false, modifiers) || res;
    if(res)
        event->accept();
   QGLWidget::keyReleaseEvent(event);
}

void NLScene::wheelEvent(QWheelEvent *event)
{
    const int modifiers = event->modifiers();
    const int buttons = event->buttons();
    bool res = m_actors.WheelEventHandler(buttons, event->orientation(), event->delta(), event->x(), event->y(), modifiers);
    res = WheelEventHandler(buttons, event->orientation(), event->delta(), event->x(), event->y(), modifiers) || res;
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
        emit propertyChanged("grabMouse", m_grabMouse);
    }
}

void NLScene::RunLoop(bool b)
{
    if(m_loop == b)
        return;
    m_delta = 0.0f;
    m_renderDelta = 0.0f;
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
        QTimer::singleShot(m_updateInterval, this, SLOT(IdleTimer_slot()));
}

void NLScene::IdleTimer_slot()
{
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    qint64 delta = ts - m_lastTime;
    m_delta = delta / 1000.0f;
    m_lastTime = ts;
    Update(m_delta);
    emit updated(m_delta);

    if(m_updateGLInterval > 0)
    {
        qint64 fd = m_lastTime - m_updateGLLastTime;
        if(fd >= m_updateGLInterval)
        {
            m_updateGLLastTime = m_lastTime;
            m_renderDelta = fd / 1000.0f;
            updateGL();
            UpdateCurrentFPS(fd);
            emit rendered(m_currentFps);
        }
    }
    else
    {
        updateGL();
        UpdateCurrentFPS(delta);
        emit rendered(m_currentFps);
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
        emit propertyChanged("clearColor", m_clearColor);
    }
}

void NLScene::Reset()
{
    m_actors.Reset();
    memset(m_keyState, 0, sizeof(m_keyState));
    memset(m_mouseState, 0, sizeof(m_mouseState));
    memset(m_keyPressed, 0, sizeof(m_keyPressed));
    memset(m_mousePressed, 0, sizeof(m_mousePressed));
}

void NLScene::Update(float f)
{
    m_actors.Update(f);
    memset(m_keyPressed, 0, sizeof(m_keyPressed));
    memset(m_mousePressed, 0, sizeof(m_mousePressed));
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

void NLScene::AddActor(NLActor *actor)
{
    if(!actor)
        return;
    m_actors.Add(actor);
}

void NLScene::RemoveActor(NLActor *actor)
{
    if(!actor)
        return;
    if(m_actors.Remove(actor))
    {
        delete actor;
    }
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
        emit propertyChanged("fps", m_fps);
    }
}

void NLScene::SetUpdateInterval(int ui)
{
    if(m_updateInterval != ui)
    {
        m_updateInterval = ui;
        if(m_updateInterval < 0)
            m_updateInterval = 0;
        emit propertyChanged("updateInterval", m_updateInterval);
    }
}

void NLScene::UpdateCurrentFPS(qint64 delta)
{
    m_currentFps = qRound(1000.0 / (float)delta);
    //qDebug() << m_currentFps;
}

bool NLScene::KeyEventHandler(int key, bool pressed, int modifier)
{
    Q_UNUSED(key);
    Q_UNUSED(pressed);
    Q_UNUSED(modifier);
    return false;
}

bool NLScene::MouseEventHandler(int mouse, bool pressed, int x, int y, int modifier)
{
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(modifier);
    return false;
}

bool NLScene::MouseMotionHandler(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(oldx);
    Q_UNUSED(oldy);
    Q_UNUSED(modifier);
    return false;
}

bool NLScene::WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    Q_UNUSED(mouse);
    Q_UNUSED(orientation);
    Q_UNUSED(delta);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(modifier);
    return false;
}

int NLScene::QKeyToIndex(int key)
{
    if(key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde)
    {
        return key;
    }
    if(key < Qt::Key_Shift)
    {
        return key - 0x01000000;
    }
    if(key >= Qt::Key_Shift && key <= Qt::Key_Direction_R)
    {
        return Qt::Key_AsciiTilde + (key - Qt::Key_Shift) + 1;
    }
    return -1;
}

int NLScene::QMouseToIndex(int button)
{
    if(button == Qt::LeftButton)
        return 0;
    else if(button == Qt::RightButton)
        return 1;
    else if(button == Qt::MiddleButton)
        return 2;
    return -1;
}

void NLScene::SetKeyState(int key, bool pressed)
{
    int index = QKeyToIndex(key);
    if(index >= 0)
    {
        m_keyState[index] = pressed;
        m_keyPressed[index] = pressed;
    }
}

void NLScene::SetButtonState(int button, bool pressed)
{
    int index = QMouseToIndex(button);
    if(index >= 0)
    {
        m_mouseState[index] = pressed;
        m_mousePressed[index] = pressed;
    }
}

bool NLScene::KeyState(int key)
{
    int index = QKeyToIndex(key);
    return index >= 0 ? m_keyState[index] : false;
}

bool NLScene::MouseState(int button)
{
    int index = QMouseToIndex(button);
    return index >= 0 ? m_mouseState[index] : false;
}

bool NLScene::KeyPressed(int key)
{
    int index = QKeyToIndex(key);
    return index >= 0 ? m_keyPressed[index] : false;
}

bool NLScene::MousePressed(int button)
{
    int index = QMouseToIndex(button);
    return index >= 0 ? m_mousePressed[index] : false;
}

NLActor * NLScene::CreateActor(const NLProperties &props)
{
    NLActor *actor = new NLActor(props);
    AddActor(actor);
    return actor;
}

void NLScene::SetPropertyConfig(const NLProperties &props)
{
    m_propertyConfig = props;
}

QString NLScene::ClassName() const
{
    //return NLScene::staticMetaObject().className();
    return metaObject()->className();
}

NLPROPERTY_DECL_TRAIT(NLScene, NL::property_equals, propertyChanged)
