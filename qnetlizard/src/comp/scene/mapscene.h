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
    bool IsValid() const { return m_model != 0; }
    const GL_NETLizard_3D_Model * Model() const { return m_model; }

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int level);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);
    void SetNoclip(int b);
    void SetFog(bool b);
    void SetSingleScene(bool b);

    private:
    void ConvToAlgoVector3(vector3_t &v);
    void ConvToRenderVector3(vector3_t &v);
    bool CollisionItem(int item);

private:
    typedef QHash<NLint, const NETLizard_Level_Teleport *> MapTeleportMap;
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
    int m_noclip;
    bool m_fog;
    bool m_singleScene;
    MapTeleportMap m_teleport;

    Q_DISABLE_COPY(MapScene)
};

#endif // _KARIN_MAPSCENE_H
