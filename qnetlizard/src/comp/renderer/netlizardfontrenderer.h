#ifndef _KARIN_NETLIZARDFONTENDERER_H
#define _KARIN_NETLIZARDFONTENDERER_H

#include <QStringList>

#include "nlrenderable.h"
#include "gl/nl_gl.h"

NLRENDERER(NETLizardFontRenderer)
class NETLizardFontRenderer : public NLRenderable
{
public:
    explicit NETLizardFontRenderer(NLActor *actor = 0);
    virtual ~NETLizardFontRenderer();
    GL_NETLizard_Font * Font();
    void SetFont(GL_NETLizard_Font *tex);
    void SetText(const QString &str);
    void UpdateLayout();

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderText();

private:
    GL_NETLizard_Font *m_font;
    QString m_text;
    QStringList m_list;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(NETLizardFontRenderer)

};

#endif // _KARIN_NETLIZARDFONTENDERER_H
