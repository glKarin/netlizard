#ifndef _KARIN_ITEMSCENE_H
#define _KARIN_ITEMSCENE_H

#include "engine/nlscene.h"

struct _GL_NETLizard_3D_Model;
class NETLizardMapModelRenderer;
class SimpleControlComponent;

NLSCENE(ItemScene)
class ItemScene : public NLScene
{
    Q_OBJECT
    Q_PROPERTY(QVariant model READ ModelPtr FINAL)
public:
    explicit ItemScene(QWidget *parent = 0);
    virtual ~ItemScene();
    bool IsValid() const { return m_model != 0; }
    const struct _GL_NETLizard_3D_Model * Model() const { return m_model; }
    QVariant ModelPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_3D_Model, m_model)); }

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int index);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);

private:
    struct _GL_NETLizard_3D_Model *m_model;
    NETLizardMapModelRenderer *m_renderer;
    SimpleControlComponent *m_control;

    Q_DISABLE_COPY(ItemScene)
};

#endif // _KARIN_ITEMSCENE_H
