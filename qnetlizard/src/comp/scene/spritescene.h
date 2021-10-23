#ifndef _KARIN_SPRITESCENE_H
#define _KARIN_SPRITESCENE_H

#include "nlscene.h"
#include "gl/nl_font.h"

class QColor;
class NETLizardSpriteRenderer;

NLSCENE(SpriteScene)
class SpriteScene : public NLScene
{
    Q_OBJECT
public:
    explicit SpriteScene(QWidget *parent = 0);
    virtual ~SpriteScene();
    bool IsValid() const;
    int Count() const;

public Q_SLOTS:
    bool LoadFile(const QString &cfFile, const QString &fntFile);
    void Reset();
    void SetIndex(int i);

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    
signals:
    
public slots:

private:
    NETLizardSpriteRenderer *m_renderer;
    GL_NETLizard_Sprite *m_sprite;
    int m_index;

    Q_DISABLE_COPY(SpriteScene)
    
};

#endif // _KARIN_SPRITESCENE_H
