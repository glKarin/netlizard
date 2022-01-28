#ifndef _KARIN_NETLIZARDSPRITEENDERER_H
#define _KARIN_NETLIZARDSPRITEENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_Sprite;

NLRENDERER(NETLizardSpriteRenderer)
class NETLizardSpriteRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(int index READ Index WRITE SetIndex FINAL)
    Q_PROPERTY(QVariant sprite READ SpritePtr FINAL)
public:
    explicit NETLizardSpriteRenderer(NLActor *actor = 0);
    virtual ~NETLizardSpriteRenderer();
    struct _GL_NETLizard_Sprite * Sprite() { return m_sprite; }
    void SetSprite(struct _GL_NETLizard_Sprite *sprite);
    void SetIndex(int i);
    int Index() const { return m_index; }
    QVariant SpritePtr() const;

protected:
    virtual void Render();
    virtual void Destroy();

private:
    void RenderSprite();

private:
    struct _GL_NETLizard_Sprite *m_sprite;
    int m_index;

    Q_DISABLE_COPY(NETLizardSpriteRenderer)

};

#endif // _KARIN_NETLIZARDSPRITEENDERER_H
