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
class NETLizardMapModelDebugRenderer;
class MapEventHandlerComponent;

NLSCENE(MapScene)
class MapScene : public NLScene
{
    Q_OBJECT
public:
    explicit MapScene(QWidget *parent = 0);
    virtual ~MapScene();
    bool IsValid() const { return m_model != 0; }
    const GL_NETLizard_3D_Model * Model() const { return m_model; }
    int CurrentScene() const { return m_currentScene; }
    int CurrentViewItem() const { return m_currentViewItem; }
    int CurrentCollisionItem() const { return m_currentCollisionItem; }
    int CurrentViewScene() const { return m_currentViewScene; }

    Q_SIGNALS:
    void currentSceneChanged(int scene);
    void currentViewSceneChanged(int scene);
    void currentCollisionItemChanged(int item);
    void currentViewItemChanged(int item);

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int level);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int mouse, bool pressed, int x, int y, int modifier);

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);
    void SetNoclip(int b);
    void SetFog(bool b);
    void SetSingleScene(int b);

    private:
    void ConvToAlgoVector3(vector3_t &v);
    void ConvToRenderVector3(vector3_t &v);
    void SetCurrentScene(int scene);
    void SetCurrentViewScene(int scene);
    void SetCurrentViewItem(int item);
    void SetCurrentCollisionItem(int item);
    bool RayIntersect();
    void UpdateCullRenderScene();
    bool CollisionTesting(const vector3_t &oldPos);

private:
    GL_NETLizard_3D_Model *m_model;
    SimpleCameraActor *m_mainCameraActor;
    NLActor *m_mapActor;
    NLActor *m_shadowActor;
    NLActor *m_debugActor;
    NLActor *m_skyActor;
    NLActor *m_sky3DActor;
    SimpleCameraActor *m_sky3DCameraActor;
    NETLizardMapModelRenderer *m_renderer;
    NETLizardShadowModelRenderer *m_shadowRenderer;
    NETLizardMapModelDebugRenderer *m_debugRenderer;
    NETLizardTextureRenderer *m_skyRenderer;
    NETLizardItemModelRenderer *m_sky3DRenderer;
    NLSceneOrthoCamera *m_skyCamera;
    NLScenePerspectiveCamera *m_sky3DCamera;
    SimpleControlComponent *m_control;
    MapEventHandlerComponent *m_eventHandler;
    int m_noclip;
    bool m_fog;
    int m_singleScene;
    int m_currentScene;
    int m_currentCollisionItem;
    int m_currentViewItem;
    int m_currentViewScene;

    Q_DISABLE_COPY(MapScene)
};

#endif // _KARIN_MAPSCENE_H
