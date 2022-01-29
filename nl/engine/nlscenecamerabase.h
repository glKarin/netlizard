#ifndef _KARIN_NLSCENECAMERABASE_H
#define _KARIN_NLSCENECAMERABASE_H

#include <QtGlobal>

#include "common/nlinc.h"
#include "common/nlvec.h"
#include "common/nlmatrix.h"

class QVariant;
class NLScene;
class NLActor;
class NLActorContainer;

enum GL_matrix_e
{
    ModelviewMatrix = 0,
    ProjectionMatrix,
    ModelviewProjectionMatrix,
    NormalMatrix,
    ViewMatrix,
    TotalMatrix
};

class NLLIB_EXPORT NLSceneCameraBase
{
public:
    enum SceneCamera_Type
    {
        Type_Perspective = 1,
        Type_Ortho = 2
    };
    typedef void (*NLSceneCameraRenderFunc)(const float view_mat[16], const float proj_mat[16], const float mvp_mat[16]);

    class NLLIB_EXPORT NLSceneCameraChangedNotify
    {
        public:
        enum Notify_Type
        {
            Notify_PropertyChanged = 1,
            Notify_ValueChanged = 2
        };

    public:
            explicit NLSceneCameraChangedNotify(int type);
            virtual ~NLSceneCameraChangedNotify();
            virtual void PropertyChanged(const QString &name, const QVariant &value) = 0;
            virtual void ValueChanged(const QString &name, const QVariant &value) = 0;

    protected:
        void SetType(int type);
        int Type() const { return m_type; }

        private:
            int m_type;

       friend class NLSceneCameraBase;
    };

public:
    NLSceneCameraBase(NLScene *widget = 0);
    virtual ~NLSceneCameraBase();
    NLScene * Scene() { return m_scene; }
    void SetScene(NLScene *scene);
    void Render();
    void Render(NLScene *scene);
    void Render(NLActor *actor);
    void Render(NLActorContainer *actors);
    void Render(NLSceneCameraRenderFunc func);
    void SetGlobalMatrix(const NLMatrix4 *mat);
    virtual void Reset();
    virtual void Update(float width, float height) = 0;
    const NLMatrix4 * ViewMatrix() const { return &m_viewMatrix; }
    const NLMatrix4 * ProjectionMatrix() const { return &m_projectionMatrix; }
    const NLMatrix4 * NormalMatrix() const { return &m_normalMatrix; }
    const NLMatrix4 * GlobalMatrix() const { return &m_globalMatrix; }
    const NLMatrix4 * ViewProjectionMatrix() const { return &m_mvpMatrix; }
    const NLMatrix4 * InitialFixedViewMatrix() const { return &m_initialFixedViewMatrix; }
    void SetPosition(const NLVector3 &v);
    void SetRotation(const NLVector3 &v);
    void SetScale(const NLVector3 &v);
    NLVector3 Position() const { return m_position; }
    NLVector3 Rotation() const { return m_rotation; }
    NLVector3 Scale() const { return m_scale; }
    NLVector3 Direction() const { return m_direction; }
    NLVector3 Up() const { return m_up; }
    NLVector3 Right_XPositive() const { return m_right_x_positive; }
    NLSceneCameraBase * Move(const NLVector3 &v);
    NLSceneCameraBase * Turn(const NLVector3 &v);
    NLSceneCameraBase * Zoom(const NLVector3 &v);
    bool ZIsUp() const;
    void SetZIsUp(bool b);
    void SetEnabled(bool b);
    bool IsEnabled() const { return m_enabled; }
    void SetChangedNotifyFunc(NLSceneCameraChangedNotify *func);

protected:
    virtual void Projection();
    virtual void View();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);
    void UpdateMatrix();
    void PropertyChanged(const QString &name, const QVariant &value);
    void ValueChanged(const QString &name, const QVariant &value);

private:
    typedef struct GL_matrix_status_s {
        GLint mode;
        GLfloat modelview_matrix[16];
        GLfloat projection_matrix[16];
    } GL_matrix_status;
    GL_matrix_status BeginRender();
    void EndRender(const GL_matrix_status &status);

private:
    void UpdateViewMatrix();
    void UpdateMvpMatrix();
    void UpdateNormalMatrix();
    void UpdateDirection();

private:
    NLScene *m_scene;
    bool m_zIsUp;
    bool m_enabled;
    NLVector3 m_position;
    NLVector3 m_rotation;
    NLVector3 m_scale;
    NLVector3 m_direction;
    NLVector3 m_up;
    NLVector3 m_right_x_positive;
    NLMatrix4 m_viewMatrix;
    NLMatrix4 m_projectionMatrix;
    NLMatrix4 m_mvpMatrix;
    NLMatrix4 m_normalMatrix; // normal
    NLMatrix4 m_globalMatrix;
    NLMatrix4 m_initialFixedViewMatrix;
    NLSceneCameraChangedNotify *m_notifyFunc;

    Q_DISABLE_COPY(NLSceneCameraBase)
};

#endif // _KARIN_NLSCENECAMERABASE_H
