#ifndef _KARIN_ITEMSCENE_H
#define _KARIN_ITEMSCENE_H

#include "engine/nlscene.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer;
class SimpleControlComponent;

NLSCENE(ItemScene)
class ItemScene : public NLScene
{
    Q_OBJECT
public:
    explicit ItemScene(QWidget *parent = 0);
    virtual ~ItemScene();
    bool IsValid() const { return m_model != 0; }
    const GL_NETLizard_3D_Model * Model() const { return m_model; }

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
    GL_NETLizard_3D_Model *m_model;
    NETLizardMapModelRenderer *m_renderer;
    SimpleControlComponent *m_control;

    Q_DISABLE_COPY(ItemScene)
};

#endif // _KARIN_ITEMSCENE_H
