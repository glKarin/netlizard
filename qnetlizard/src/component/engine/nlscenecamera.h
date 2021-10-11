#ifndef _KARIN_NLSCENECAMERA_H
#define _KARIN_NLSCENECAMERA_H

#include "nlactor.h"

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
    void SetModelViewMatrix(const NLMatrix4 *mat);
    void Reset();
    virtual void Update(float width, float height) = 0;
    const NLMatrix4 * ModelViewMatrix() const;
    const NLMatrix4 * ProjectionMatrix() const;
    const NLMatrix4 * ModelViewProjectionMatrix() const;

protected:
    virtual void Projection() = 0;
    virtual void ModelView();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat) = 0;
    void UpdateMatrix();

private:
    NLMatrix4 m_modelviewMatrix;
    NLMatrix4 m_projectionMatrix;
    NLMatrix4 m_mvpMatrix;
    NLScene *m_scene;

    Q_DISABLE_COPY(NLSceneCamera)
};

#endif // _KARIN_NLSCENECAMERA_H
