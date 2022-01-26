#ifndef _KARIN_NETLIZARDTEXTURERENDERER_H
#define _KARIN_NETLIZARDTEXTURERENDERER_H

#include "engine/nlrenderable.h"

struct _texture_s;

NLRENDERER(NETLizardTextureRenderer)
class NETLizardTextureRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(int alignment READ Alignmenti WRITE SetAlignmenti FINAL)
    Q_PROPERTY(QVariant texture READ TexturePtr FINAL)
public:
    explicit NETLizardTextureRenderer(NLActor *actor = 0);
    virtual ~NETLizardTextureRenderer();
    struct _texture_s * Texture() { return m_tex; }
    void SetTexture(struct _texture_s *tex);
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const { return m_align; }
    QVariant TexturePtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _texture_s, m_tex)); }
    void SetAlignmenti(int align) { SetAlignment(static_cast<Qt::Alignment>(align)); }
    int Alignmenti() const { return static_cast<int>(m_align); }

protected:
    virtual void Render();
    virtual void Destroy();

private:
    Qt::Alignment m_align;
    struct _texture_s *m_tex;

    Q_DISABLE_COPY(NETLizardTextureRenderer)
    
};

#endif // _KARIN_NETLIZARDTEXTURERENDERER_H
