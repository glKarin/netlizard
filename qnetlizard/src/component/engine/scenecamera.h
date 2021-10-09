#ifndef _KARIN_SCENECAMERA_H
#define _KARIN_SCENECAMERA_H

#include "lib/camera.h"
#include "lib/vector3.h"

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

class SceneCamera
{
public:
    enum SceneCamera_Type
    {
        Type_Perspective = 1,
        Type_Ortho = 2
    };

public:
    SceneCamera(NLScene *widget = 0);
    virtual ~SceneCamera();
    NLScene * Scene();
    void SetScene(NLScene *scene);
    virtual void Render();
    void Move(const vector3_s *v);
    void Turn(const vector3_s *v);
    void Zoom(const vector3_s *v);
    void Reset();
    void SetPosition(const vector3_s *v);
    void SetRotation(const vector3_s *v);
    virtual void Update(float width, float height) = 0;

protected:
    virtual void Projection() = 0;
    virtual void ModelView();

private:
    GLmatrix m_matrix[TotalMatrix];
    camera_s m_cam;
    NLScene *m_scene;
};

#endif // _KARIN_SCENECAMERA_H
