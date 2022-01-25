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
    explicit NLModelRenderer(const NLProperties &prop = NLProperties(), NLActor *actor = 0);
    explicit NLModelRenderer(NLRenderModelGLGeneral *model, NLActor *actor = 0);
    virtual ~NLModelRenderer();
    NLRenderModelGLGeneral * Model() { return m_model; }
    NLRenderModelGLGeneral * Take();
    void SetModel(NLRenderModelGLGeneral *model);

protected:
    virtual void Render();
    virtual void Destroy();

private:
    NLRenderModelGLGeneral *m_model;

    Q_DISABLE_COPY(NLModelRenderer)
};

class NLLIB_EXPORT NLModelRenderer_coordinate : public NLModelRenderer
{
public:
    explicit NLModelRenderer_coordinate(const NLProperties &prop = NLProperties(), NLActor *actor = 0)
        : NLModelRenderer(prop, actor)
    {
        CLASS_NAME(NLModelRenderer_coordinate);
        setObjectName("NLModelRenderer_coordinate");
    }

protected:
    virtual void InitProperty() {
        NLRenderable::InitProperty();
        int length = GetInitProperty_T<float>("length", 9999);
        SetModel(new NLRenderModelGLGeneral_coordinate(length));
    }
};

class NLLIB_EXPORT NLModelRenderer_cube : public NLModelRenderer
{
public:
    explicit NLModelRenderer_cube(const NLProperties &prop = NLProperties(), NLActor *actor = 0)
        : NLModelRenderer(prop, actor)
    {
        CLASS_NAME(NLModelRenderer_cube);
        setObjectName("NLModelRenderer_cube");
    }

protected:
    virtual void InitProperty() {
        NLRenderable::InitProperty();
        int length = GetInitProperty_T<float>("length", 200);
        SetModel(new NLRenderModelGLGeneral_cube(length));
    }
};

class NLLIB_EXPORT NLModelRenderer_plane : public NLModelRenderer
{
public:
    explicit NLModelRenderer_plane(const NLProperties &prop = NLProperties(), NLActor *actor = 0)
        : NLModelRenderer(prop, actor)
    {
        CLASS_NAME(NLModelRenderer_plane);
        setObjectName("NLModelRenderer_plane");
    }

protected:
    virtual void InitProperty() {
        NLRenderable::InitProperty();
        int length = GetInitProperty_T<float>("length", 2000);
        SetModel(new NLRenderModelGLGeneral_plane(length));
    }
};

class NLLIB_EXPORT NLModelRenderer_line : public NLModelRenderer
{
public:
    explicit NLModelRenderer_line(const NLProperties &prop = NLProperties(), NLActor *actor = 0)
        : NLModelRenderer(prop, actor)
    {
        CLASS_NAME(NLRenderModelGLGeneral_line);
        setObjectName("NLRenderModelGLGeneral_line");
    }

protected:
    virtual void InitProperty() {
        NLRenderable::InitProperty();
        int length = GetInitProperty_T<float>("length", 2000);
        SetModel(new NLRenderModelGLGeneral_line(length));
    }
};

#endif // _KARIN_NLMODELRENDERER_H
