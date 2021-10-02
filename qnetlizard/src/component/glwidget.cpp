#include "glwidget.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include <QMouseEvent>

#include "lib/vector3.h"
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

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_movesens(1800),
    m_turnsens(18),
    m_pressed(false),
    m_loop(false),
    m_delta(0.0f),
    m_hideMouse(false),
    m_enableDefaultKeyHandler(true),
    m_enableDefaultMouseHandler(true),
    m_enableDefaultWheelHandler(false)
{
    newcam(&m_cam);

    m_clearColor = QColor::fromRgbF(1.0, 1.0, 1.0);

    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_AllocGLMatrix(m_matrix + i);
        Mesa_glLoadIdentity(m_matrix + i);
    }

    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
}

GLWidget::~GLWidget()
{
    Deinit();
}

void GLWidget::Deinit()
{
    RunLoop(false);
    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_FreeGLMatrix(m_matrix + i);
    }
}

void GLWidget::Init()
{
    setFocusPolicy(Qt::StrongFocus);
    RunLoop(true);
}

void GLWidget::initializeGL()
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

void GLWidget::paintGL()
{
    QGLWidget::paintGL();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void GLWidget::SetMVP(const GLmatrix *mat)
{
    Mesa_glLoadIdentity(m_matrix + ModelviewMatrix);
    Mesa_glLoadMatrix(m_matrix + ModelviewMatrix, m_matrix[ViewMatrix].m);

    if(mat)
    {
        Mesa_glMultMatrix(m_matrix + ModelviewMatrix, mat->m);
    }

    Mesa_glLoadMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ProjectionMatrix].m);
    Mesa_glMultMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ModelviewMatrix].m);

// not use OpenGL matrix stack, like glPushMatrix()/glPopMatrix for GLSL render
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_matrix[ModelviewMatrix].m);
}

void GLWidget::SetCamera()
{
    Mesa_glLoadIdentity(m_matrix + ViewMatrix);
    camtrans_gl(m_matrix + ViewMatrix, &m_cam);
#if 0
    glRotatef(VECTOR3_X(m_cam.rotation), 1, 0, 0);
    glRotatef(VECTOR3_Y(m_cam.rotation), 0, 1, 0);
    glTranslatef(
                -VECTOR3_X(m_cam.position),
                -VECTOR3_Y(m_cam.position),
                -VECTOR3_Z(m_cam.position)
                );
#endif
}

void GLWidget::Render2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    Mesa_gluOrtho2D(m_matrix + ProjectionMatrix, left, right, bottom, top);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
}

void GLWidget::Render3D(GLfloat fovy, GLfloat w, GLfloat h, GLfloat n, GLfloat f)
{
    GLfloat th;

    th = h != 0.0 ? h : 1;
    Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    Mesa_gluPerspective(m_matrix + ProjectionMatrix, fovy, w / th, n, f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    m_pressed = true;
    if(!m_enableDefaultMouseHandler)
    {
        QGLWidget::mousePressEvent(event);
        return;
    }
    if(!m_hideMouse)
    {
        if(event->modifiers() & Qt::ControlModifier)
            QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        event->accept();
    }

    QGLWidget::mousePressEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(!m_enableDefaultMouseHandler)
    {
        QGLWidget::mouseMoveEvent(event);
        return;
    }
    if(m_pressed || m_hideMouse)
    {
        if((event->modifiers() & Qt::ControlModifier) == 0)
        {
            QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
            QPoint delta = (QCursor::pos() - center) * m_turnsens * m_delta;
            if(!delta.isNull())
            {
                vector3_s v = VECTOR3(delta.y(), delta.x(), 0);
                camrot(&m_cam, &v);
                //updateGL();
            }
            QCursor::setPos(center);
        }
    }
    QGLWidget::mouseMoveEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    QGLWidget::mouseReleaseEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(!keyev(event, true))
        QGLWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(!keyev(event, false))
        QGLWidget::keyReleaseEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(!m_enableDefaultWheelHandler)
    {
        QGLWidget::wheelEvent(event);
        return;
    }
#define MIN_SCALE 0.1
    if(event->orientation() == Qt::Vertical)
    {
        int d = event->delta();
        float delta = d * m_delta * 0.1;
        if(d < 0 && (VECTOR3_X(m_cam.scale) * VECTOR3_Y(m_cam.scale) < 1.0))
            delta /= 4;
        qDebug() << delta;
        VECTOR3_X(m_cam.scale) += delta;
        if(VECTOR3_X(m_cam.scale) < MIN_SCALE)
            VECTOR3_X(m_cam.scale) = MIN_SCALE;
        VECTOR3_Y(m_cam.scale) += delta;
        if(VECTOR3_Y(m_cam.scale) < MIN_SCALE)
            VECTOR3_Y(m_cam.scale) = MIN_SCALE;
    }
    QGLWidget::wheelEvent(event);
#undef MIN_SCALE
}

void GLWidget::Transform()
{
    if(m_direction[Direction_Forward] || m_direction[Direction_Backward]
            || m_direction[Direction_Left] || m_direction[Direction_Right]
            || m_direction[Direction_Down] || m_direction[Direction_Up]
            )
    {
        vector3_s unit = VECTOR3(0, 0, 0);

        float movesens = m_movesens * m_delta;

        if(m_direction[Direction_Forward])
            VECTOR3_Z(unit) = movesens;
        else if(m_direction[Direction_Backward])
            VECTOR3_Z(unit) = -movesens;

        if(m_direction[Direction_Left])
            VECTOR3_X(unit) = -movesens;
        else if(m_direction[Direction_Right])
            VECTOR3_X(unit) = movesens;

        if(m_direction[Direction_Down])
            VECTOR3_Y(unit) = -movesens;
        else if(m_direction[Direction_Up])
            VECTOR3_Y(unit) = movesens;

        cammove(&m_cam, &unit);
    }

    if(m_rotation[Rotation_Up] || m_rotation[Rotation_Down]
            || m_rotation[Rotation_Left] || m_rotation[Rotation_Right]
            )
    {
        float turnsens = 10 * m_turnsens * m_delta;
        vector3_s unit = VECTOR3(0, 0, 0);

        if(m_rotation[Rotation_Up])
            VECTOR3_X(unit) = -turnsens;
        else if(m_rotation[Rotation_Down])
            VECTOR3_X(unit) = turnsens;

        if(m_rotation[Rotation_Left])
            VECTOR3_Y(unit) = -turnsens;
        else if(m_rotation[Rotation_Right])
            VECTOR3_Y(unit) = turnsens;

        camrot(&m_cam, &unit);
    }
}

bool GLWidget::keyev(QKeyEvent *event, bool pressed)
{
    int i, j;
    bool r;

    r = false;

    if(!m_enableDefaultKeyHandler)
        return r;
    if(event->modifiers() == Qt::NoModifier)
    {
        i = -1;
        j = -1;
        switch(event->key())
        {
        case Qt::Key_W:
            i = Direction_Forward;
            break;
        case Qt::Key_S:
            i = Direction_Backward;
            break;
        case Qt::Key_A:
            i = Direction_Left;
            break;
        case Qt::Key_D:
            i = Direction_Right;
            break;
        case Qt::Key_Q:
            i = Direction_Down;
            break;
        case Qt::Key_E:
            i = Direction_Up;
            break;

        case Qt::Key_Up:
            j = Rotation_Up;
            break;
        case Qt::Key_Down:
            j = Rotation_Down;
            break;
        case Qt::Key_Left:
            j = Rotation_Left;
            break;
        case Qt::Key_Right:
            j = Rotation_Right;
            break;

        case Qt::Key_C:
            if(pressed)
            {
                if(glIsEnabled(GL_CULL_FACE))
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);
                updateGL();
            }
            break;
        case Qt::Key_L:
            if(pressed)
            {
                RunLoop(!m_loop);
            }
            break;
        case Qt::Key_M:
            if(pressed)
            {
                SetHideMouse(!m_hideMouse);
            }
            break;
        case Qt::Key_Escape:
            if(pressed)
            {
                SetHideMouse(false);
            }
            break;
        default:
            break;
        }
        if(i != -1)
        {
            m_direction[i] = pressed;
            //Transform();
            r = true;
        }
        if(j != -1)
        {
            m_rotation[j] = pressed;
            //Transform();
            r = true;
        }
        goto __Exit;
    }
    else if(event->modifiers() & Qt::ControlModifier)
    {
        goto __Exit;
    }
    else
        goto __Exit;

__Exit:
    return r;
}

void GLWidget::SetHideMouse(bool b)
{
    if(m_hideMouse != b)
    {
        m_hideMouse = b;
        this->setCursor(m_hideMouse ? Qt::BlankCursor : Qt::ArrowCursor);
        setMouseTracking(m_hideMouse);
        if(m_hideMouse)
            QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    }
}

void GLWidget::RunLoop(bool b)
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

void GLWidget::ExecLoop()
{
    if(m_loop)
        QTimer::singleShot(0, this, SLOT(IdleTimer_slot()));
}

void GLWidget::IdleTimer_slot()
{
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    qint64 delta = ts - m_lastTime;
    m_delta = delta / 1000.0f;
    m_lastTime = ts;
    updateGL();
    Update(m_delta);
    ExecLoop();
}

void GLWidget::resizeEvent(QResizeEvent *event)
{
    QGLWidget::resizeEvent(event);
    emit sizeChanged(event->size());
}

void GLWidget::SetClearColor(const QColor &color)
{
    if(m_clearColor != color)
    {
        m_clearColor = color;
        qglClearColor(m_clearColor);
        updateGL();
    }
}

QColor GLWidget::ClearColor() const
{
    return m_clearColor;
}

void GLWidget::SetEnableDefaultKeyHandler(bool b)
{
    if(m_enableDefaultKeyHandler != b)
        m_enableDefaultKeyHandler = b;
}

void GLWidget::SetEnableDefaultMouseHandler(bool b)
{
    if(m_enableDefaultMouseHandler != b)
        m_enableDefaultMouseHandler = b;
}

void GLWidget::SetEnableDefaultWheelHandler(bool b)
{
    if(m_enableDefaultWheelHandler != b)
        m_enableDefaultWheelHandler = b;
}

void GLWidget::Reset()
{
    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
}

void GLWidget::Update(float f)
{
    Transform();
}
