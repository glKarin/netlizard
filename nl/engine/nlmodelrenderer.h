#ifndef _KARIN_NLMODELRENDERER_H
#define _KARIN_NLMODELRENDERER_H

#include "nlrenderable.h"
#include "nlrendermodel.h"

class NLLIB_EXPORT NLRenderModelGLGeneral_coordinate : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_coordinate(GLfloat length = 9999)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }

private:
    void Init(GLfloat length = 9999);
};

class NLLIB_EXPORT NLRenderModelGLGeneral_cube : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_cube(GLfloat length = 200)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }

private:
    void Init(GLfloat length = 200);
};

class NLLIB_EXPORT NLRenderModelGLGeneral_plane : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_plane(GLfloat length = 2000)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }

private:
    void Init(GLfloat length = 2000);
};

class NLLIB_EXPORT NLRenderModelGLGeneral_line : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_line(GLfloat length = 2000)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }

private:
    void Init(GLfloat length = 2000);
};

class NLLIB_EXPORT NLModelRenderer : public NLRenderable
{
public:
    explicit NLModelRenderer(NLActor *actor = 0);
    explicit NLModelRenderer(NLRenderModelGLGeneral *model, NLActor *actor = 0);
    virtual ~NLModelRenderer();
    NLRenderModelGLGeneral * Model() { return m_model; }
    NLRenderModelGLGeneral * Take();
    void SetModel(NLRenderModelGLGeneral *model);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    NLRenderModelGLGeneral *m_model;

    Q_DISABLE_COPY(NLModelRenderer)
};

class NLLIB_EXPORT NLModelRenderer_coordinate : public NLModelRenderer
{
public:
    explicit NLModelRenderer_coordinate(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_coordinate, actor)
    {}
    explicit NLModelRenderer_coordinate(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_coordinate(length), actor)
    {}
};

class NLLIB_EXPORT NLModelRenderer_cube : public NLModelRenderer
{
public:
    explicit NLModelRenderer_cube(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_cube, actor)
    {}
    explicit NLModelRenderer_cube(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_cube(length), actor)
    {}
};

class NLLIB_EXPORT NLModelRenderer_plane : public NLModelRenderer
{
public:
    explicit NLModelRenderer_plane(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_plane, actor)
    {}
    explicit NLModelRenderer_plane(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_plane(length), actor)
    {}
};

class NLLIB_EXPORT NLModelRenderer_line : public NLModelRenderer
{
public:
    explicit NLModelRenderer_line(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_line, actor)
    {}
    explicit NLModelRenderer_line(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_line(length), actor)
    {}
};

#endif // _KARIN_NLMODELRENDERER_H
