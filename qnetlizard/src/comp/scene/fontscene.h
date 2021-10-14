#ifndef _KARIN_FONTSCENE_H
#define _KARIN_FONTSCENE_H

#include "nlscene.h"
#include "gl/nl_font.h"

class QColor;
class NETLizardFontRenderer;

NLSCENE(FontScene)
class FontScene : public NLScene
{
    Q_OBJECT
public:
    explicit FontScene(QWidget *parent = 0);
    virtual ~FontScene();
    bool IsValid() const;

public Q_SLOTS:
    bool LoadFile(const QString &cfFile, const QString &fntFile);
    void Reset();
    void SetText(const QString &str);

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    
signals:
    
public slots:

private:
    NETLizardFontRenderer *m_renderer;
    font_s *m_font;
    QString m_text;
    int m_lineCount;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(FontScene)
    
};

#endif // _KARIN_FONTSCENE_H
