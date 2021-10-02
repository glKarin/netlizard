#include "imagewidget.h"

#include <QDebug>
#include <QFileInfo>

#include "gl/nl_gl.h"
#include "gl/gl_texture.h"
#include "gl/nl_texture.h"
#include "utils/ioutility.h"
#include "qdef.h"

ImageWidget::ImageWidget(QWidget *parent)
    : GLWidget(parent),
      m_tex(0),
      m_align(Qt::AlignCenter)
{
    memset(&m_data.data, 0, sizeof(m_data.data));
    m_data.type = NL_TEXTURE_UNKNOWN;
    setObjectName("ImageWidget");
    SetEnableDefaultKeyHandler(true);
    SetEnableDefaultMouseHandler(false);
    SetEnableDefaultWheelHandler(true);
}

ImageWidget::~ImageWidget()
{
    DEBUG_DESTROY_Q;
}

void ImageWidget::Init()
{
    GLWidget::Init();
    glDisable(GL_DEPTH_TEST);
}

void ImageWidget::Update(float delta)
{
    //qDebug() << delta;
    GLWidget::Update(delta);
}

void ImageWidget::paintGL()
{
    if(!m_tex)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    const GLfloat _Vertex[] = {
//        0.0, 0.0,
//        w, 0.0,
//        0.0, h,
//        w, h
//    };
//    const GLfloat _Vertex[] = {
//        -w / 2, -h / 2,
//        w / 2, -h / 2,
//        -w / 2, h / 2,
//        w / 2, h / 2
//    };
    GLfloat texcoord[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };
    GLfloat w = m_tex->width;
    GLfloat h = m_tex->height;
    GLfloat start_x = 0;
    GLfloat start_y = 0;
    GLfloat end_x = w;
    GLfloat end_y = h;
    int ww = width();
    int wh = height();
    GLfloat ortho[4] = {0};
    if(m_align & Qt::AlignLeft)
    {
        ortho[1] = ww;
    }
    else
    {
        start_x = -w / 2;
        end_x = w / 2;
        ortho[0] = -ww / 2;
        ortho[1] = ww / 2;
    }

    if(m_align & Qt::AlignBottom)
    {
        ortho[3] = wh;
    }
    else if(m_align & Qt::AlignTop)
    {
        start_y = h;
        end_y = 0;
        ortho[2] = wh;
        ortho[3] = 0;
    }
    else
    {
        start_y = -h / 2;
        end_y = h / 2;
        ortho[2] = -wh / 2;
        ortho[3] = wh / 2;
    }
    GLfloat vertex[8] = {
        start_x, start_y,
        end_x, start_y,
        start_x, end_y,
        end_x, end_y,
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(ortho[0], ortho[1], ortho[2], ortho[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(
                VECTOR3_X(m_cam.position),
                VECTOR3_Y(m_cam.position),
                0
                );
    glScalef(VECTOR3_X(m_cam.scale), VECTOR3_Y(m_cam.scale), VECTOR3_Z(m_cam.scale));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
    glBindTexture(GL_TEXTURE_2D, m_tex->texid);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glFlush();
}

void ImageWidget::SetAlignment(Qt::Alignment align)
{
    Qt::Alignment a = align;
    a = a & (~Qt::AlignRight);
    if(m_align != a)
    {
        m_align = (Qt::Alignment)a;
        updateGL();
    }
}

Qt::Alignment ImageWidget::Alignment() const
{
    return m_align;
}

void ImageWidget::Deinit()
{
    GLWidget::Deinit();
    Reset();
}

bool ImageWidget::GenTexture(const NETLizard_Texture *tex)
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

bool ImageWidget::GenTexture(const char *data, int length)
{
    texture_s tex;
    if(!new_OpenGL_texture_2d_from_memory((const unsigned char *)data, length, &tex))
        return false;
    m_tex = (texture_s *)malloc(sizeof(texture_s));
    memset(m_tex, 0, sizeof(texture_s));
    memcpy(m_tex, &tex, sizeof(texture_s));
    return true;
}

bool ImageWidget::LoadFile(const QString &file, int type, int index)
{
    Reset();
    newcam(&m_cam);
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
    return true;
}

bool ImageWidget::IsValid() const
{
    return m_tex != 0 && m_data.type != NL_TEXTURE_UNKNOWN;
}

QString ImageWidget::GetSaveTextureSuffix() const
{
    if(!IsValid())
        return QString();
    return m_data.ext;
}

void ImageWidget::Reset()
{
    if(m_tex)
    {
        if(glIsTexture(m_tex->texid))
            glDeleteTextures(1, &m_tex->texid);
        free(m_tex);
        m_tex = 0;
    }
    UnsetData();
}

void ImageWidget::UnsetData()
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

bool ImageWidget::SaveData(const QString &file)
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

void ImageWidget::SetData(int type, void *data, int len, const QString &ext)
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

const texture_s * ImageWidget::Texture() const
{
    return m_tex;
}
