#include "nlmodelrenderer.h"

#include <QDebug>

#include "nldbg.h"

#define VE4(arr, start, x, y, z, w) \
    arr[start] = x; \
    arr[start + 1] = y; \
    arr[start + 2] = z; \
    arr[start + 3] = w;

#define VE3(arr, start, x, y, z) \
    arr[start] = x; \
    arr[start + 1] = y; \
    arr[start + 2] = z;

void NLRenderModelGLGeneral_coordinate::Init(GLfloat length)
{
    Clear();

    GLfloat vertex[6 * 4];
    GLfloat color[6 * 4];

    VE4(vertex, 0, -length, 0, 0, 1);
    VE4(vertex, 4, length, 0, 0, 1);
    VE4(vertex, 8, 0, -length, 0, 1);
    VE4(vertex, 12, 0, length, 0, 1);
    VE4(vertex, 16, 0, 0, -length, 1);
    VE4(vertex, 20, 0, 0, length, 1);

    VE4(color, 0, 0.2, 0, 0, 0.5);
    VE4(color, 4, 1, 0, 0, 1);
    VE4(color, 8, 0, 0.2, 0, 0.5);
    VE4(color, 12, 0, 1, 0, 1);
    VE4(color, 16, 0, 0, 0.2, 0.5);
    VE4(color, 20, 0, 0, 1, 1);

    for(int i = 0; i < 6; i++)
    {
        Vertex().Add(vertex + 4 * i, 0, 0, color + 4 * i);
    }

    NLRenderModelGLGeneral::NLRenderModelPrimitiveData primitive;
    primitive.SetMode(GL_LINES);
    for(int i = 0; i < 6; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}



void NLRenderModelGLGeneral_cube::Init(GLfloat length)
{
    Clear();

    GLfloat _vertex[32];
    const GLfloat min = -length / 2;
    const GLfloat max = length / 2;
    VE4(_vertex, 0, min, min, max, 1);
    VE4(_vertex, 4, max, min, max, 1);
    VE4(_vertex, 8, min, max, max, 1);
    VE4(_vertex, 12, max, max, max, 1);
    VE4(_vertex, 16, min, min, min, 1);
    VE4(_vertex, 20, max, min, min, 1);
    VE4(_vertex, 24, min, max, min, 1);
    VE4(_vertex, 28, max, max, min, 1);
    GLfloat _color[24] = {
        0.5, 0, 0, 1,
        1, 0, 0, 1,
        0, 0.5, 0, 1,
        0, 1, 0, 1,
        0, 0, 0.5, 1,
        0, 0, 1, 1,
    };

    static const GLushort CubePlane4Index[] =
    {
        // 正
        0, 1, 2, 2, 1, 3,
        // 背
        4, 6, 5, 5, 6, 7,
        // GL下/左
        4, 5, 0, 0, 5, 1,
        // GL上/右
        2, 3, 6, 6, 3, 7,
        // GL左/下
        0, 2, 4, 4, 2, 6,
        // GL右/上
        1, 5, 3, 3, 5, 7,
    };

    GLfloat vertex[4 * 36];
    GLfloat color[4 * 36];
    for(int i = 0; i < 36; i += 3)
    {
        GLfloat *p, *c;
        c = _color + (i / 6) * 4;

        p = _vertex + CubePlane4Index[i] * 4;
        VE4(vertex, i * 4, p[0], p[1], p[2], p[3]);
        VE4(color, i * 4, c[0], c[1], c[2], c[3]);

        p = _vertex + CubePlane4Index[i + 1] * 4;
        VE4(vertex, (i + 1) * 4, p[0], p[1], p[2], p[3]);
        VE4(color, (i + 1) * 4, c[0], c[1], c[2], c[3]);

        p = _vertex + CubePlane4Index[i + 2] * 4;
        VE4(vertex, (i + 2) * 4, p[0], p[1], p[2], p[3]);
        VE4(color, (i + 2) * 4, c[0], c[1], c[2], c[3]);
    }

    for(int i = 0; i < 36; i++)
    {
        Vertex().Add(vertex + 4 * i, 0, 0, color + 4 * i);
    }

    NLRenderModelGLGeneral::NLRenderModelPrimitiveData primitive;
    primitive.SetMode(GL_TRIANGLES);
    for(int i = 0; i < 36; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}

void NLRenderModelGLGeneral_plane::Init(GLfloat length, const QColor &color)
{
    Clear();

    GLfloat vertex[4 * 4];
    const GLfloat min = -length / 2;
    const GLfloat max = length / 2;

    VE4(vertex, 0, min, 0, max, 1);
    VE4(vertex, 4, max, 0, max, 1);
    VE4(vertex, 8, min, 0, min, 1);
    VE4(vertex, 12, max, 0, min, 1);

    for(int i = 0; i < 4; i++)
    {
        Vertex().Add(vertex + 4 * i, 0, 0, 0);
    }

    NLRenderModelGLGeneral::NLRenderModelPrimitiveData primitive;
    primitive.SetMode(GL_TRIANGLE_STRIP);
    primitive.SetColor(color);
    for(int i = 0; i < 4; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}

void NLRenderModelGLGeneral_line::Init(GLfloat length, const QColor &color)
{
    Clear();

    GLfloat vertex[4 * 2];
    const GLfloat min = -length / 2;
    const GLfloat max = length / 2;

    VE4(vertex, 0, min, 0, 0, 1);
    VE4(vertex, 4, max, 0, 0, 1);

    for(int i = 0; i < 2; i++)
    {
        Vertex().Add(vertex + 4 * i, 0, 0, 0);
    }

    NLRenderModelGLGeneral::NLRenderModelPrimitiveData primitive;
    primitive.SetMode(GL_LINES);
    primitive.SetColor(color);
    for(int i = 0; i < 2; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}
#undef VE3
#undef VE4



NLModelRenderer::NLModelRenderer(const NLProperties &prop, NLActor *actor) :
    NLRenderable(prop, actor),
    m_model(new NLRenderModelGLGeneral)
{
    CLASS_NAME(NLModelRenderer);
    setObjectName("NLModelRenderer");
}

NLModelRenderer::NLModelRenderer(NLRenderModelGLGeneral *model, NLActor *actor) :
    NLRenderable(actor),
    m_model(model)
{
    CLASS_NAME(NLModelRenderer);
    setObjectName("NLModelRenderer");
}

NLModelRenderer::~NLModelRenderer()
{
    if(m_model)
        delete m_model;
}

void NLModelRenderer::Render()
{
    if(!IsActived())
        return;
    if(!m_model)
        return;
    m_model->Render();
}

void NLModelRenderer::Destroy()
{
    if(m_model)
    {
        delete m_model;
        m_model = 0;
    }
}

void NLModelRenderer::SetModel(NLRenderModelGLGeneral *model)
{
    if(m_model != model)
    {
        if(m_model)
        {
            delete m_model;
            m_model = 0;
        }
        m_model = model;
    }
}

NLRenderModelGLGeneral * NLModelRenderer::Take()
{
    if(m_model)
    {
        NLRenderModelGLGeneral *m = m_model;
        m_model = 0;
        return m;
    }
    return 0;
}

NLModelRenderer_coordinate::NLModelRenderer_coordinate(const NLProperties &prop, NLActor *actor)
    : NLModelRenderer(prop, actor),
      m_length(9999)
{
    CLASS_NAME(NLModelRenderer_coordinate);
    setObjectName("NLModelRenderer_coordinate");
    SetModel(new NLRenderModelGLGeneral_coordinate(m_length));
}

void NLModelRenderer_coordinate::InitProperty()
{
    NLRenderable::InitProperty();
    float length = GetInitProperty_T<float>("length", 9999);
    SetLength(length);
}

void NLModelRenderer_coordinate::SetLength(float length)
{
    if(m_length != length)
    {
        m_length = length;
        static_cast<NLRenderModelGLGeneral_coordinate *>(Model())->Init(m_length);
        emit propertyChanged("length", m_length);
    }
}

NLModelRenderer_cube::NLModelRenderer_cube(const NLProperties &prop, NLActor *actor)
    : NLModelRenderer(prop, actor),
      m_length(200)
{
    CLASS_NAME(NLModelRenderer_cube);
    setObjectName("NLModelRenderer_cube");
    SetModel(new NLRenderModelGLGeneral_cube(m_length));
}

void NLModelRenderer_cube::InitProperty()
{
    NLRenderable::InitProperty();
    float length = GetInitProperty_T<float>("length", 200);
    SetLength(length);
}

void NLModelRenderer_cube::SetLength(float length)
{
    if(m_length != length)
    {
        m_length = length;
        static_cast<NLRenderModelGLGeneral_cube *>(Model())->Init(m_length);
        emit propertyChanged("length", m_length);
    }
}

NLModelRenderer_plane::NLModelRenderer_plane(const NLProperties &prop, NLActor *actor)
    : NLModelRenderer(prop, actor),
      m_length(2000)
{
    CLASS_NAME(NLModelRenderer_plane);
    setObjectName("NLModelRenderer_plane");
    SetModel(new NLRenderModelGLGeneral_plane(m_length));
}

void NLModelRenderer_plane::InitProperty()
{
    NLRenderable::InitProperty();
    float length = GetInitProperty_T<float>("length", 2000);
    SetLength(length);
    QColor color = GetInitProperty_T<QColor>("color");
    if(color.isValid())
        SetColor(color);
}

void NLModelRenderer_plane::SetLength(float length)
{
    if(m_length != length)
    {
        m_length = length;
        static_cast<NLRenderModelGLGeneral_plane *>(Model())->Init(m_length, m_color);
        emit propertyChanged("length", m_length);
    }
}

void NLModelRenderer_plane::SetColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        static_cast<NLRenderModelGLGeneral_plane *>(Model())->Primitive(0).SetColor(m_color);
        emit propertyChanged("color", m_color);
    }
}

NLModelRenderer_line::NLModelRenderer_line(const NLProperties &prop, NLActor *actor)
    : NLModelRenderer(prop, actor),
      m_length(2000)
{
    CLASS_NAME(NLRenderModelGLGeneral_line);
    setObjectName("NLRenderModelGLGeneral_line");
    SetModel(new NLRenderModelGLGeneral_line(m_length));
}

void NLModelRenderer_line::InitProperty()
{
    NLRenderable::InitProperty();
    float length = GetInitProperty_T<float>("length", 2000);
    SetLength(length);
    QColor color = GetInitProperty_T<QColor>("color");
    if(color.isValid())
        SetColor(color);
}

void NLModelRenderer_line::SetLength(float length)
{
    if(m_length != length)
    {
        m_length = length;
        static_cast<NLRenderModelGLGeneral_line *>(Model())->Init(m_length, m_color);
        emit propertyChanged("length", m_length);
    }
}

void NLModelRenderer_line::SetColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        static_cast<NLRenderModelGLGeneral_line *>(Model())->Primitive(0).SetColor(m_color);
        emit propertyChanged("color", m_color);
    }
}
