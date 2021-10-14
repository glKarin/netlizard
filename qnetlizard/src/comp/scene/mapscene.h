#ifndef _KARIN_MAPSCENE_H
#define _KARIN_MAPSCENE_H

#include "nlscene.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer;

NLSCENE(MapScene)
class MapScene : public NLScene
{
    Q_OBJECT
public:
    MapScene(QWidget *parent = 0);
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

private:
    GL_NETLizard_3D_Model *m_model;
    NETLizardMapModelRenderer *m_renderer;

    Q_DISABLE_COPY(MapScene)
};

#endif // _KARIN_MAPSCENE_H
