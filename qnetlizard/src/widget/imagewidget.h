#ifndef _KARIN_IMAGEWIDGET_H
#define _KARIN_IMAGEWIDGET_H

#include "glwidget.h"
#include "netlizard.h"

typedef struct _texture_s texture_s;

class ImageWidget : public GLWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = 0);
    virtual ~ImageWidget();
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

private:
    texture_s *m_tex;
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

    Q_DISABLE_COPY(ImageWidget)
};

#endif // _KARIN_IMAGEWIDGET_H
