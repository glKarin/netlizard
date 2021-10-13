#include "testrenderer.h"

#include <QDebug>

#include <GL/gl.h>

#include "qdef.h"

TestRenderer::RenderItem::RenderItem(GLenum mode, GLfloat *vertex, GLuint start, GLsizei count, GLfloat color4[], GLfloat *texcoord, GLint tex, GLfloat *color)
    : mode(mode),
      vertex(vertex),
      texcoord(texcoord),
      color(color),
      index(0),
      start(start),
      count(count),
      tex(tex)
{
    this->color4[0] = 1;
    this->color4[1] = 1;
    this->color4[2] = 1;
    this->color4[3] = 1;
    if(color4)
    {
        memcpy(this->color4, color4, sizeof(GLfloat) * 4);
    }
}

TestRenderer::RenderItem::RenderItem(GLenum mode, GLfloat *vertex, GLushort *index, GLsizei count, GLfloat color4[], GLfloat *texcoord, GLint tex, GLfloat *color)
    : mode(mode),
      vertex(vertex),
      texcoord(texcoord),
      color(color),
      index(index),
      start(0),
      count(count),
      tex(tex)
{
    this->color4[0] = 1;
    this->color4[1] = 1;
    this->color4[2] = 1;
    this->color4[3] = 1;
    if(color4)
    {
        memcpy(this->color4, color4, sizeof(GLfloat) * 4);
    }
}

TestRenderer::RenderItem::~RenderItem()
{
}

void TestRenderer::RenderItem::Free()
{
    delete[] vertex;
    delete[] texcoord;
    delete[] color;
    delete[] index;
}

void TestRenderer::RenderItem::Render() const
{
    glLineWidth(10);
    //glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, vertex);
    bool usingTex = glIsTexture(tex) && texcoord;
    if(usingTex)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
    if(color)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, color);
    }

    glColor4fv(color4);
    if(index)
        glDrawElements(mode, count, GL_UNSIGNED_SHORT, index);
    else
        glDrawArrays(mode, start, count);

    if(color)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if(usingTex)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

TestRenderer::TestRenderer(NLActor *actor) :
    NLRenderable(actor)
{
}

TestRenderer::~TestRenderer()
{
    DEBUG_DESTROY("TestRenderer");
}

void TestRenderer::InitRender()
{

}

void TestRenderer::Render()
{
    Q_FOREACH(const TestRenderer::RenderItem &item, m_items)
    {
        glPushMatrix();
        {
            item.Render();
        }
        glPopMatrix();
    }
}

void TestRenderer::DeinitRender()
{
}

TestRenderer & TestRenderer::operator<<(const RenderItem &item)
{
    m_items.push_back(item);
    return *this;
}

void TestRenderer::Clear()
{
    Q_FOREACH(const TestRenderer::RenderItem &item, m_items)
    {
        (const_cast<TestRenderer::RenderItem &>(item)).Free();
    }

    m_items.clear();
}
