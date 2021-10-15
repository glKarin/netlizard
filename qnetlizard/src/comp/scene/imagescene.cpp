#include "imagescene.h"

#include <QDebug>
#include <QFileInfo>

#include "gl/nl_gl.h"
#include "gl/gl_texture.h"
#include "gl/nl_texture.h"
#include "utils/ioutility.h"
#include "qdef.h"
#include "nlscenecamera.h"
#include "simplecameraactor.h"
#include "nlsceneorthocamera.h"
#include "simpleimagecontrolcomponent.h"
#include "netlizardtexturerenderer.h"

ImageScene::ImageScene(QWidget *parent)
    : NLScene(parent),
      m_tex(0),
      m_renderer(0),
      m_align(Qt::AlignCenter)
{
    memset(&m_data.data, 0, sizeof(m_data.data));
    m_data.type = NL_TEXTURE_UNKNOWN;
    setObjectName("ImageScene");

    NLPropperties prop;
    prop.insert("type", QVariant::fromValue((int)NLSceneCamera::Type_Ortho));
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    AddActor(camera);
    NLActor *actor = new NLActor;
    AddActor(actor);
    SimpleImageControlComponent *control = new SimpleImageControlComponent(NLPropperties(), actor);
    actor->AddComponent(control);
    m_renderer = new NETLizardTextureRenderer;
    actor->SetRenderable(m_renderer);
    NLSceneOrthoCamera *orthoCam = static_cast<NLSceneOrthoCamera *>(camera->Camera());
    orthoCam->SetAlignment(m_align);
    SetCurrentCamera(orthoCam);
}

ImageScene::~ImageScene()
{
}

void ImageScene::Init()
{
    NLScene::Init();
}

void ImageScene::Update(float delta)
{
    NLScene::Update(delta);
}

void ImageScene::paintGL()
{
    if(!m_tex)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

void ImageScene::SetAlignment(Qt::Alignment align)
{
    Qt::Alignment a = align;
    a = a & (~Qt::AlignRight);
    if(m_align != a)
    {
        m_align = (Qt::Alignment)a;
        vector3_s startPos = VECTOR3(0, 0, 0);
        vector3_s startRotate = VECTOR3(0, 0, 0);

        SimpleCameraActor *camera = GetActor_T<SimpleCameraActor>(0);
        camera->SetPosition(startPos);
        camera->SetRotation(startRotate);
        static_cast<NLSceneOrthoCamera *>(camera->Camera())->SetAlignment(align);
    }
}

Qt::Alignment ImageScene::Alignment() const
{
    return m_align;
}

void ImageScene::Deinit()
{
    NLScene::Deinit();
    Reset();
}

bool ImageScene::GenTexture(const NETLizard_Texture *tex)
{
    if(!tex)
        return false;
    int len;
    NLuchar *data = nlMakePixelData(tex, &len);
    if(!data)
        return false;
    texture_s gltex;
    bool res = false;
    int format = tex->format == NL_RGB ? GL_RGB : GL_RGBA;
    if(make_OpenGL_texture_2d((char *)data, len, tex->width, tex->height, format, &gltex))
    {
        m_tex = (texture_s *)malloc(sizeof(texture_s));
        memset(m_tex, 0, sizeof(texture_s));
        memcpy(m_tex, &gltex, sizeof(texture_s));
        res = true;
    }
    free(data);
    return res;
}

bool ImageScene::GenTexture(const char *data, int length)
{
    texture_s tex;
    if(!new_OpenGL_texture_2d_from_memory((const unsigned char *)data, length, &tex))
        return false;
    m_tex = (texture_s *)malloc(sizeof(texture_s));
    memset(m_tex, 0, sizeof(texture_s));
    memcpy(m_tex, &tex, sizeof(texture_s));
    return true;
}

bool ImageScene::LoadFile(const QString &file, int type, int index)
{
    Reset();
    QByteArray ba = file.toLocal8Bit();
    int t = type;
    if(t == NL_TEXTURE_UNKNOWN)
    {
        t = nlGetPNGFileType(ba.constData());
    }

    switch(t)
    {
    case NL_TEXTURE_NORMAL_PNG:
    {
        quint64 len;
        char *data = IOUtility::file_get_contents(ba.constData(), &len);
        if(!data)
            return false;

        bool res = GenTexture(data, len);
        if(!res)
        {
            delete[] data;
            return false;
        }
        SetData(NL_TEXTURE_NORMAL_PNG, data, len, "png");
    }
        break;
    case NL_TEXTURE_ENCODE_PNG:
    {
        NLint len;
        char *data = nlReadAndHandlePNGFile(ba.constData(), &len);
        if(!data)
            return false;

        bool res = GenTexture(data, len);
        if(!res)
        {
            free(data);
            return false;
        }
        SetData(NL_TEXTURE_ENCODE_PNG, data, len, "png");
    }
        break;
    case NL_TEXTURE_3D_ENGINE_V2:
    {
        NETLizard_Texture tex;
        memset(&tex, 0, sizeof(NETLizard_Texture));
        NLboolean ok = nlReadTextureV2File(ba.constData(), &tex);
        if(!ok)
            return false;

        bool res = GenTexture(&tex);
        if(!res)
        {
            delete_NETLizard_Texture(&tex);
            return false;
        }
        SetData(NL_TEXTURE_3D_ENGINE_V2, &tex, 1, "bmp");
    }
        break;
    case NL_TEXTURE_3D_ENGINE_V3:
    {
        NETLizard_Texture tex;
        memset(&tex, 0, sizeof(NETLizard_Texture));
        NLboolean ok = nlReadTextureV3File(ba.data(), index, &tex);
        if(!ok)
            return false;

        bool res = GenTexture(&tex);
        if(!res)
        {
            delete_NETLizard_Texture(&tex);
            return false;
        }
        SetData(NL_TEXTURE_3D_ENGINE_V3, &tex, 1, "bmp");
    }
        break;
    case NL_TEXTURE_3D_ENGINE_V3_COMPRESS:
    {
        NETLizard_Texture tex;
        memset(&tex, 0, sizeof(NETLizard_Texture));
        NLboolean ok = nlReadCompressTextureV3File(ba.data(), &tex);
        if(!ok)
            return false;

        bool res = GenTexture(&tex);
        if(!res)
        {
            delete_NETLizard_Texture(&tex);
            return false;
        }
        SetData(NL_TEXTURE_3D_ENGINE_V3, &tex, 1, "bmp");
    }
        break;
    default:
    {
        NLint len;
        char *data = nlEncodeDecodeFile(ba.constData(), &len);
        if(!data)
            return false;
        bool res = GenTexture(data, len);
        qDebug() << res;
        if(!res)
        {
            free(data);
            return false;
        }
        SetData(NL_TEXTURE_ENCODE_PNG, data, len, QFileInfo(file).completeSuffix());
    }
        break;
    }
    m_renderer->SetTexture(m_tex);

    return true;
}

bool ImageScene::IsValid() const
{
    return m_tex != 0 && m_data.type != NL_TEXTURE_UNKNOWN;
}

QString ImageScene::GetSaveTextureSuffix() const
{
    if(!IsValid())
        return QString();
    return m_data.ext;
}

void ImageScene::Reset()
{
    if(m_tex)
    {
        if(glIsTexture(m_tex->texid))
            glDeleteTextures(1, &m_tex->texid);
        free(m_tex);
        m_tex = 0;
    }
    UnsetData();
    m_renderer->SetTexture(0);

    NLScene::Reset();
}

void ImageScene::UnsetData()
{
    switch(m_data.type)
    {
        case NL_TEXTURE_NORMAL_PNG:
    case NL_TEXTURE_ENCODE_PNG:
        if(m_data.data.image)
        {
            free(m_data.data.image);
        }
        break;
    case NL_TEXTURE_3D_ENGINE_V2:
    case NL_TEXTURE_3D_ENGINE_V3:
    case NL_TEXTURE_3D_ENGINE_V3_COMPRESS:
        if(m_data.data.texture)
        {
            delete_NETLizard_Texture(m_data.data.texture);
            free(m_data.data.texture);
        }
        break;
    case NL_TEXTURE_UNKNOWN:
    default:
        break;
    }
    memset(&m_data.data, 0, sizeof(m_data.data));
    m_data.type = NL_TEXTURE_UNKNOWN;
    m_data.ext.clear();
}

bool ImageScene::SaveData(const QString &file)
{
    if(m_data.type == NL_TEXTURE_UNKNOWN)
        return false;
    QByteArray ba = file.toLocal8Bit();
    bool res = false;
    switch(m_data.type)
    {
        case NL_TEXTURE_NORMAL_PNG:
        case NL_TEXTURE_ENCODE_PNG:
            res = IOUtility::file_put_contents(file, (char *)m_data.data.image, m_data.len);
            break;
        case NL_TEXTURE_3D_ENGINE_V2:
        res = nlSaveTextureV2DataToImageFile(m_data.data.texture, ba.constData(), 1);
            break;
        case NL_TEXTURE_3D_ENGINE_V3:
        case NL_TEXTURE_3D_ENGINE_V3_COMPRESS:
        res = nlSaveTextureV3DataToImageFile(m_data.data.texture, ba.constData(), 1);
        break;
    case NL_TEXTURE_UNKNOWN:
    default:
        break;
    }
    return res;
}

void ImageScene::SetData(int type, void *data, int len, const QString &ext)
{
    UnsetData();
    if(type != NL_TEXTURE_UNKNOWN && data)
    {
        switch(type)
        {
            case NL_TEXTURE_NORMAL_PNG:
        case NL_TEXTURE_ENCODE_PNG:
            m_data.data.image = (char *)data;
            break;
        case NL_TEXTURE_3D_ENGINE_V2:
        case NL_TEXTURE_3D_ENGINE_V3:
        case NL_TEXTURE_3D_ENGINE_V3_COMPRESS:
            m_data.data.texture = (NETLizard_Texture *)malloc(sizeof(NETLizard_Texture));
            memset(m_data.data.texture, 0, sizeof(NETLizard_Texture));
            memcpy(m_data.data.texture, data, sizeof(NETLizard_Texture));
            break;
        case NL_TEXTURE_UNKNOWN:
        default:
            break;
        }
        m_data.type = type;
        m_data.len = len;
        m_data.ext = ext;
    }
}

const texture_s * ImageScene::Texture() const
{
    return m_tex;
}
