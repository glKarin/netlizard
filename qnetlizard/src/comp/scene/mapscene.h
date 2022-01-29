#ifndef _KARIN_MAPSCENE_H
#define _KARIN_MAPSCENE_H

#include <QHash>

#include "engine/nlscene.h"
#include "common/nlvec.h"

struct _GL_NETLizard_3D_Model;
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
    Q_PROPERTY(int noclip READ Noclip WRITE SetNoclip FINAL)
    Q_PROPERTY(bool fog READ Fog WRITE SetFog FINAL)
    Q_PROPERTY(int singleScene READ SingleScene WRITE SetSingleScene FINAL)
    Q_PROPERTY(bool cull READ Cull WRITE SetCull FINAL)
    Q_PROPERTY(int shadowMethod READ ShadowMethod WRITE SetShadowMethod FINAL)
    Q_PROPERTY(int shadowObject READ ShadowObject WRITE SetShadowObject FINAL)
    Q_PROPERTY(int renderDebug READ RenderDebug WRITE SetRenderDebug FINAL)
    Q_PROPERTY(int currentScene READ CurrentScene FINAL)
    Q_PROPERTY(int currentViewItem READ CurrentViewItem FINAL)
    Q_PROPERTY(int currentCollisionItem READ CurrentCollisionItem FINAL)
    Q_PROPERTY(int currentViewScene READ CurrentViewScene FINAL)
    Q_PROPERTY(QVariant model READ ModelPtr FINAL)
public:
    explicit MapScene(QWidget *parent = 0);
    virtual ~MapScene();
    bool IsValid() const { return m_model != 0; }
    const struct _GL_NETLizard_3D_Model * Model() const { return m_model; }
    QVariant ModelPtr() const;
    int CurrentScene() const { return m_currentScene; }
    int CurrentViewItem() const { return m_currentViewItem; }
    int CurrentCollisionItem() const { return m_currentCollisionItem; }
    int CurrentViewScene() const { return m_currentViewScene; }
    int Noclip() const { return m_noclip; }
    bool Fog() const { return m_fog; }
    int SingleScene() const { return m_singleScene; }
    int ShadowMethod() const { return m_shadowMethod; }
    bool Cull() const { return m_cull; }
    int ShadowObject() const { return m_shadowObject; }
    int RenderDebug() const { return m_renderDebug; }

    Q_SIGNALS:
    void currentSceneChanged(int scene);
    void currentViewSceneChanged(int scene);
    void currentCollisionItemChanged(int item);
    void currentViewItemChanged(int item);

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int level);
    void Reset();
    void SetNoclip(int b);
    void SetFog(bool b);
    void SetSingleScene(int b);
    void SetShadowMethod(int i);
    void SetCull(bool b);
    void SetShadowObject(int i);
    void SetRenderDebug(int i);

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int mouse, bool pressed, int x, int y, int modifier);

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);

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
    void UpdateSkybox();

private:
    struct _GL_NETLizard_3D_Model *m_model;
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
    QHash<int, int> m_skyboxs;
    int m_shadowMethod;
    int m_shadowObject;
    int m_renderDebug;
    bool m_cull;

    Q_DISABLE_COPY(MapScene)
};

#endif // _KARIN_MAPSCENE_H
