#ifndef _KARIN_FONTSCENE_H
#define _KARIN_FONTSCENE_H

#include "engine/nlscene.h"

struct _GL_NETLizard_Font;
class QColor;
class NETLizardFontRenderer;

NLSCENE(FontScene)
class FontScene : public NLScene
{
    Q_OBJECT
public:
    explicit FontScene(QWidget *parent = 0);
    virtual ~FontScene();
    struct _GL_NETLizard_Font * Font() { return m_font; }
    bool IsValid() const { return m_font != 0; }

public Q_SLOTS:
    bool LoadFile(const QString &cfFile, const QString &fntFile);
    void Reset();
    void SetText(const QString &str);

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    virtual void resizeEvent(QResizeEvent *event);
    
signals:
    
public slots:

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);

private:
    NETLizardFontRenderer *m_renderer;
    struct _GL_NETLizard_Font *m_font;
    QString m_text;
    int m_lineCount;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(FontScene)
    
};

#endif // _KARIN_FONTSCENE_H
