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

void NLRenderModelGLGeneral_plane::Init(GLfloat length)
{
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
    for(int i = 0; i < 4; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}

void NLRenderModelGLGeneral_line::Init(GLfloat length)
{
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
    for(int i = 0; i < 2; i++)
    {
        primitive.Index().Add(i);
    }
    Primitives().push_back(primitive);
}
#undef VE3
#undef VE4



NLModelRenderer::NLModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(new NLRenderModelGLGeneral)
{
    SetName("NLModelRenderer");
}

NLModelRenderer::NLModelRenderer(NLRenderModelGLGeneral *model, NLActor *actor) :
    NLRenderable(actor),
    m_model(model)
{
    SetName("NLModelRenderer");
}

NLModelRenderer::~NLModelRenderer()
{
    if(m_model)
        delete m_model;
    NLDEBUG_DESTROY(NLModelRenderer)
}

void NLModelRenderer::InitRender()
{

}

void NLModelRenderer::Render()
{
    if(!m_model)
        return;
    m_model->Render();
}

void NLModelRenderer::DeinitRender()
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
