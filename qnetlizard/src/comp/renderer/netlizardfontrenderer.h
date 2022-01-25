#ifndef _KARIN_NETLIZARDFONTENDERER_H
#define _KARIN_NETLIZARDFONTENDERER_H

#include <QStringList>

#include "engine/nlrenderable.h"

struct _GL_NETLizard_Font;

NLRENDERER(NETLizardFontRenderer)
class NETLizardFontRenderer : public NLRenderable
{
public:
    explicit NETLizardFontRenderer(NLActor *actor = 0);
    virtual ~NETLizardFontRenderer();
    struct _GL_NETLizard_Font * Font() { return m_font; }
    void SetFont(struct _GL_NETLizard_Font *font);
    void SetText(const QString &str);
    void UpdateLayout();

protected:
    virtual void Render();
    virtual void Destroy();

private:
    void RenderText();

private:
    struct _GL_NETLizard_Font *m_font;
    QString m_text;
    QStringList m_list;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(NETLizardFontRenderer)

};

#endif // _KARIN_NETLIZARDFONTENDERER_H
