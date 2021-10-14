#ifndef _KARIN_ITEMSCENE_H
#define _KARIN_ITEMSCENE_H

#include "nlscene.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer;

NLSCENE(ItemScene)
class ItemScene : public NLScene
{
    Q_OBJECT
public:
    ItemScene(QWidget *parent = 0);
    virtual ~ItemScene();
    bool IsValid() const;
    const GL_NETLizard_3D_Model * Model() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int index);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

private:
    GL_NETLizard_3D_Model *m_model;
    NETLizardMapModelRenderer *m_renderer;

    Q_DISABLE_COPY(ItemScene)
};

#endif // _KARIN_ITEMSCENE_H
