#ifndef _KARIN_NETLIZARDSPRITEENDERER_H
#define _KARIN_NETLIZARDSPRITEENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

NLRENDERER(NETLizardSpriteRenderer)
class NETLizardSpriteRenderer : public NLRenderable
{
public:
    explicit NETLizardSpriteRenderer(NLActor *actor = 0);
    virtual ~NETLizardSpriteRenderer();
    GL_NETLizard_Sprite * Sprite();
    void SetSprite(GL_NETLizard_Sprite *sprite);
    void SetIndex(int i);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderSprite();

private:
    GL_NETLizard_Sprite *m_sprite;
    int m_index;

    Q_DISABLE_COPY(NETLizardSpriteRenderer)

};

#endif // _KARIN_NETLIZARDSPRITEENDERER_H
