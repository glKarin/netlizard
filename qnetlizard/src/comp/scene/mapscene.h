#ifndef _KARIN_MAPSCENE_H
#define _KARIN_MAPSCENE_H

#include "nlscene.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer;
class NETLizardTextureRenderer;
class NETLizardItemModelRenderer;
class NLSceneOrthoCamera;
class NLScenePerspectiveCamera;
class SimpleControlComponent;
class SimpleCameraActor;
class NETLizardShadowModelRenderer;

NLSCENE(MapScene)
class MapScene : public NLScene
{
    Q_OBJECT
public:
    explicit MapScene(QWidget *parent = 0);
    virtual ~MapScene();
    bool IsValid() const;
    const GL_NETLizard_3D_Model * Model() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int level);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);
    void SetNoclip(bool b);

    private:
    void ConvToAlgoVector3(vector3_t &v);
    void ConvToRenderVector3(vector3_t &v);

private:
    GL_NETLizard_3D_Model *m_model;
    SimpleCameraActor *m_mainCameraActor;
    NLActor *m_mapActor;
    NLActor *m_shadowActor;
    NLActor *m_skyActor;
    NLActor *m_sky3DActor;
    SimpleCameraActor *m_sky3DCameraActor;
    NETLizardMapModelRenderer *m_renderer;
    NETLizardShadowModelRenderer *m_shadowRenderer;
    NETLizardTextureRenderer *m_skyRenderer;
    NETLizardItemModelRenderer *m_sky3DRenderer;
    NLSceneOrthoCamera *m_skyCamera;
    NLScenePerspectiveCamera *m_sky3DCamera;
    SimpleControlComponent *m_control;
    bool m_noclip;

    Q_DISABLE_COPY(MapScene)
};

#endif // _KARIN_MAPSCENE_H
