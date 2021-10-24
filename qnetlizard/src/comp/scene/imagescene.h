#ifndef _KARIN_IMAGESCENE_H
#define _KARIN_IMAGESCENE_H

#include "nlscene.h"
#include "netlizard.h"

class NETLizardTextureRenderer;
class SimpleControl2DComponent;
class SimpleImageControlComponent;

typedef struct _texture_s texture_s;

NLSCENE(ImageScene)
class ImageScene : public NLScene
{
    Q_OBJECT
public:
    explicit ImageScene(QWidget *parent = 0);
    virtual ~ImageScene();
    bool IsValid() const;
    const texture_s * Texture() const;
    QString GetSaveTextureSuffix() const;
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, int type = NL_TEXTURE_UNKNOWN, int index = -1);
    void Reset();
    bool SaveData(const QString &file);

private:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    void SetData(int type, void *data, int len, const QString &ext);
    void UnsetData();
    bool GenTexture(const NETLizard_Texture *tex);
    bool GenTexture(const char *data, int length);

    private Q_SLOTS:
    void OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);

private:
    texture_s *m_tex;
    NETLizardTextureRenderer *m_renderer;
    Qt::Alignment m_align;
    struct TextureData_s {
        int type;
        union{
            char *image;
            NETLizard_Texture *texture;
        } data;
        int len;
        QString ext;
    } m_data;
    SimpleControl2DComponent *m_control;
    SimpleImageControlComponent *m_imageControl;

    Q_DISABLE_COPY(ImageScene)
};

#endif // _KARIN_IMAGESCENE_H
