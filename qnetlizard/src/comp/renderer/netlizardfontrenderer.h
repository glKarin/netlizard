#ifndef _KARIN_NETLIZARDFONTENDERER_H
#define _KARIN_NETLIZARDFONTENDERER_H

#include "nlrenderable.h"
#include "gl/nl_font.h"

NLRENDERER(NETLizardFontRenderer)
class NETLizardFontRenderer : public NLRenderable
{
public:
    explicit NETLizardFontRenderer(NLActor *actor = 0);
    virtual ~NETLizardFontRenderer();
    font_s * Font();
    void SetFont(font_s *tex);
    void SetText(const QString &str);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderText();

private:
    font_s *m_font;
    QString m_text;
    int m_lineCount;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(NETLizardFontRenderer)

};

#endif // _KARIN_NETLIZARDFONTENDERER_H
