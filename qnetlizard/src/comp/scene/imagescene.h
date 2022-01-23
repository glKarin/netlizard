#ifndef _KARIN_IMAGESCENE_H
#define _KARIN_IMAGESCENE_H

#include "engine/nlscene.h"
#include "netlizard.h"

class NETLizardTextureRenderer;
class SimpleControl2DComponent;
class SimpleImageControlComponent;

struct _texture_s;

NLSCENE(ImageScene)
class ImageScene : public NLScene
{
    Q_OBJECT
    Q_ENUMS(Qt::Alignment)
    Q_PROPERTY(int alignment READ Alignmenti WRITE SetAlignmenti FINAL)
    Q_PROPERTY(QVariant texture READ TexturePtr FINAL)
public:
    explicit ImageScene(QWidget *parent = 0);
    virtual ~ImageScene();
    bool IsValid() const;
    const struct _texture_s * Texture() const { return m_tex; }
    QVariant TexturePtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _texture_s, m_tex)); }
    QString GetSaveTextureSuffix() const;
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const { return m_align; }
    void SetAlignmenti(int align) { SetAlignment(static_cast<Qt::Alignment>(align)); }
    int Alignmenti() const { return static_cast<int>(m_align); }

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
    struct _texture_s *m_tex;
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
