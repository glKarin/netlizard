#ifndef _KARIN_NLMODELRENDERER_H
#define _KARIN_NLMODELRENDERER_H

#include "nlrenderable.h"
#include "nlrendermodel.h"

class NLRenderModelGLGeneral_coordinate : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_coordinate(GLfloat length = 9999)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }
    virtual ~NLRenderModelGLGeneral_coordinate() {}

private:
    void Init(GLfloat length = 9999);
};

class NLRenderModelGLGeneral_cube : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_cube(GLfloat length = 200)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }
    virtual ~NLRenderModelGLGeneral_cube() {}

private:
    void Init(GLfloat length = 200);
};

class NLModelRenderer : public NLRenderable
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

class NLModelRenderer_coordinate : public NLModelRenderer
{
public:
    explicit NLModelRenderer_coordinate(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_coordinate, actor)
    {}
    explicit NLModelRenderer_coordinate(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_coordinate(length), actor)
    {}
};

class NLModelRenderer_cube : public NLModelRenderer
{
public:
    explicit NLModelRenderer_cube(NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_cube, actor)
    {}
    explicit NLModelRenderer_cube(float length, NLActor *actor = 0)
        : NLModelRenderer(new NLRenderModelGLGeneral_cube(length), actor)
    {}
};

#endif // _KARIN_NLMODELRENDERER_H
