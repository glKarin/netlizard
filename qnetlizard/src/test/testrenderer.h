#ifndef _KARIN_TESTRENDERER_H
#define _KARIN_TESTRENDERER_H

#include <GL/gl.h>

#include <QList>

#include "nlrenderable.h"

class TestRenderer : public NLRenderable
{
public:
    struct RenderItem {
        GLenum mode;
        GLfloat *vertex;
        GLfloat *texcoord;
        GLfloat *color;
        GLfloat color4[4];
        GLushort *index;
        GLuint start;
        GLsizei count;
        GLint tex;

        explicit RenderItem(GLenum mode, GLfloat *vertex, GLuint start, GLsizei count, GLfloat color4[] = 0, GLfloat *texcoord = 0, GLint tex = 0, GLfloat *color = 0);
        explicit RenderItem(GLenum mode, GLfloat *vertex, GLushort *index, GLsizei count, GLfloat color4[] = 0, GLfloat *texcoord = 0, GLint tex = 0, GLfloat *color = 0);
        ~RenderItem();
        void Free();
        void Render() const;
    };
    typedef QList<RenderItem> RenderItemList;

public:
    explicit TestRenderer(NLActor *actor = 0);
    virtual ~TestRenderer();
    TestRenderer & operator<<(const RenderItem &item);
    void Clear();

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    RenderItemList m_items;

    Q_DISABLE_COPY(TestRenderer)
    
};

#endif // _KARIN_TESTRENDERER_H
