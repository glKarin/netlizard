#ifndef _KARIN_NETLIZARDTEXTURERENDERER_H
#define _KARIN_NETLIZARDTEXTURERENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

class NETLizardTextureRenderer : public NLRenderable
{
public:
    explicit NETLizardTextureRenderer();
    virtual ~NETLizardTextureRenderer();
    texture_s * Texture();
    void SetTexture(texture_s *tex);
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const;

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    Qt::Alignment m_align;
    texture_s *m_tex;

    Q_DISABLE_COPY(NETLizardTextureRenderer)
    
};

#endif // _KARIN_NETLIZARDTEXTURERENDERER_H
