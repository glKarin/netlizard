#ifndef _KARIN_NETLIZARDFONTENDERER_H
#define _KARIN_NETLIZARDFONTENDERER_H

#include <QStringList>

#include "engine/nlrenderable.h"

struct _GL_NETLizard_Font;

NLRENDERER(NETLizardFontRenderer)
class NETLizardFontRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(QString text READ Text WRITE SetText FINAL)
    Q_PROPERTY(int paddingWidth READ PaddingWidth WRITE SetPaddingWidth FINAL)
    Q_PROPERTY(int lineSpacing READ LineSpacing WRITE SetLineSpacing FINAL)
    Q_PROPERTY(QVariant font READ FontPtr FINAL)
public:
    explicit NETLizardFontRenderer(NLActor *actor = 0);
    virtual ~NETLizardFontRenderer();
    struct _GL_NETLizard_Font * Font() { return m_font; }
    void SetFont(struct _GL_NETLizard_Font *font);
    void SetText(const QString &str);
    void UpdateLayout();
    QVariant FontPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_Font, m_font)); }
    int PaddingWidth() const { return m_paddingWidth; }
    int LineSpacing() const { return m_lineSpacing; }
    void SetPaddingWidth(int i);
    void SetLineSpacing(int i);
    QString Text() const { return m_text; }

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
