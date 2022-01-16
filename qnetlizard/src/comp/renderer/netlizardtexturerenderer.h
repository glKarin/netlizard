#ifndef _KARIN_NETLIZARDTEXTURERENDERER_H
#define _KARIN_NETLIZARDTEXTURERENDERER_H

#include "engine/nlrenderable.h"

struct _texture_s;

NLRENDERER(NETLizardTextureRenderer)
class NETLizardTextureRenderer : public NLRenderable
{
public:
    explicit NETLizardTextureRenderer(NLActor *actor = 0);
    virtual ~NETLizardTextureRenderer();
    struct _texture_s * Texture() { return m_tex; }
    void SetTexture(struct _texture_s *tex);
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const { return m_align; }

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    Qt::Alignment m_align;
    struct _texture_s *m_tex;

    Q_DISABLE_COPY(NETLizardTextureRenderer)
    
};

#endif // _KARIN_NETLIZARDTEXTURERENDERER_H
