#ifndef _KARIN_SPRITESCENE_H
#define _KARIN_SPRITESCENE_H

#include "engine/nlscene.h"

struct _GL_NETLizard_Sprite;
class QColor;
class NETLizardSpriteRenderer;
class SimpleImageControlComponent;

NLSCENE(SpriteScene)
class SpriteScene : public NLScene
{
    Q_OBJECT
public:
    explicit SpriteScene(QWidget *parent = 0);
    virtual ~SpriteScene();
    bool IsValid() const { return m_sprite != 0; }
    struct _GL_NETLizard_Sprite * Sprite() { return m_sprite; }
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

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);

private:
    NETLizardSpriteRenderer *m_renderer;
    struct _GL_NETLizard_Sprite *m_sprite;
    int m_index;
    SimpleImageControlComponent *m_control;

    Q_DISABLE_COPY(SpriteScene)
    
};

#endif // _KARIN_SPRITESCENE_H
