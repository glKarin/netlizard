#ifndef _KARIN_NLSCENECAMERA_H
#define _KARIN_NLSCENECAMERA_H

#include "nldef.h"

class NLScene;

enum GL_matrix_e
{
    ModelviewMatrix = 0,
    ProjectionMatrix,
    ModelviewProjectionMatrix,
    NormalMatrix,
    ViewMatrix,
    TotalMatrix
};

class NLSceneCamera
{
public:
    enum SceneCamera_Type
    {
        Type_Perspective = 1,
        Type_Ortho = 2
    };

public:
    NLSceneCamera(NLScene *widget = 0);
    virtual ~NLSceneCamera();
    NLScene * Scene();
    void SetScene(NLScene *scene);
    virtual void Render();
    void SetGlobalMatrix(const NLMatrix4 *mat);
    void Reset();
    virtual void Update(float width, float height) = 0;
    const NLMatrix4 * ViewMatrix() const;
    const NLMatrix4 * ProjectionMatrix() const;
    const NLMatrix4 * NormalMatrix() const;
    const NLMatrix4 * GlobalMatrix() const;
    const NLMatrix4 * ViewProjectionMatrix() const;
    void SetPosition(const NLVector3 &v);
    void SetRotation(const NLVector3 &v);
    void SetScale(const NLVector3 &v);
    NLVector3 Position() const;
    NLVector3 Rotation() const;
    NLVector3 Scale() const;
    NLVector3 Direction() const;
    NLVector3 Up() const;
    NLSceneCamera * Move(const NLVector3 &v);
    NLSceneCamera * Turn(const NLVector3 &v);
    NLSceneCamera * Zoom(const NLVector3 &v);
    void SetZIsUp(bool b);

protected:
    virtual void Projection() = 0;
    virtual void View();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);
    void UpdateMatrix();

private:
    void UpdateViewMatrix();
    void UpdateMvpMatrix();
    void UpdateNormalMatrix();
    void UpdateDirection();

private:
    NLScene *m_scene;
    bool m_zIsUp;
    NLVector3 m_position;
    NLVector3 m_rotation;
    NLVector3 m_scale;
    NLVector3 m_direction;
    NLVector3 m_up;
    NLVector3 m_right;
    NLMatrix4 m_viewMatrix;
    NLMatrix4 m_projectionMatrix;
    NLMatrix4 m_mvpMatrix;
    NLMatrix4 m_normalMatrix; // normal
    NLMatrix4 m_globalMatrix;

    Q_DISABLE_COPY(NLSceneCamera)
};

#endif // _KARIN_NLSCENECAMERA_H
