#ifndef _KARIN_GLWIDGET_H
#define _KARIN_GLWIDGET_H

#include "lib/mesa_gl_math.h"
#include "lib/camera.h"

#include <QGLWidget>

class QColor;

enum GL_matrix_e
{
    ModelviewMatrix = 0,
    ProjectionMatrix,
    ModelviewProjectionMatrix,
    NormalMatrix,
    ViewMatrix,
    TotalMatrix
};

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~GLWidget();
    void SetHideMouse(bool b);
    QColor ClearColor() const;
    
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
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    void SetCamera();
    void SetMVP(const GLmatrix *mat);
    void Render2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
    void Render3D(GLfloat fovy, GLfloat w, GLfloat h, GLfloat near, GLfloat far);
    virtual void Update(float f);
    void SetEnableDefaultKeyHandler(bool b);
    void SetEnableDefaultMouseHandler(bool b);
    void SetEnableDefaultWheelHandler(bool b);

protected Q_SLOTS:

private Q_SLOTS:
    virtual void IdleTimer_slot();
    void Transform();

private:
    bool keyev(QKeyEvent *event, bool pressed);
    void ExecLoop();

protected:
    QColor m_clearColor;
    GLmatrix m_matrix[TotalMatrix];
    camera_s m_cam;
    GLfloat m_movesens;
    GLfloat m_turnsens;
    bool m_pressed;
    bool m_loop;
    float m_delta;
    bool m_hideMouse;
    bool m_enableDefaultKeyHandler;
    bool m_enableDefaultMouseHandler;
    bool m_enableDefaultWheelHandler;

    qint64 m_lastTime;

private:
    bool m_direction[Direction_Total];
    bool m_rotation[Rotation_Total];

    static const int M_Timer_Interval = 100;
    static const int M_Move_Unit = 10;
    
};

#endif // KARIN_GLWIDGET_H
