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
    void Init(GLfloat length = 2000, const QColor &color = QColor());
};

class NLLIB_EXPORT NLRenderModelGLGeneral_line : public NLRenderModelGLGeneral
{
public:
    explicit NLRenderModelGLGeneral_line(GLfloat length = 2000)
        : NLRenderModelGLGeneral()
    {
        Init(length);
    }
    void Init(GLfloat length = 2000, const QColor &color = QColor());
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
    Q_OBJECT
    Q_PROPERTY(float length READ Length WRITE SetLength FINAL)
public:
    explicit NLModelRenderer_coordinate(const NLProperties &prop = NLProperties(), NLActor *actor = 0);
    float Length() const { return m_length; }
    void SetLength(float length);

protected:
    virtual void InitProperty();

private:
    float m_length;
};

class NLLIB_EXPORT NLModelRenderer_cube : public NLModelRenderer
{
    Q_OBJECT
    Q_PROPERTY(float length READ Length WRITE SetLength FINAL)
public:
    explicit NLModelRenderer_cube(const NLProperties &prop = NLProperties(), NLActor *actor = 0);
    float Length() const { return m_length; }
    void SetLength(float length);

protected:
    virtual void InitProperty();

private:
    float m_length;
};

class NLLIB_EXPORT NLModelRenderer_plane : public NLModelRenderer
{
    Q_OBJECT
    Q_PROPERTY(float length READ Length WRITE SetLength FINAL)
    Q_PROPERTY(QColor color READ Color WRITE SetColor FINAL)
public:
    explicit NLModelRenderer_plane(const NLProperties &prop = NLProperties(), NLActor *actor = 0);
    float Length() const { return m_length; }
    void SetLength(float length);
    QColor Color() const { return m_color; }
    void SetColor(const QColor &color);

protected:
    virtual void InitProperty();

private:
    float m_length;
    QColor m_color;
};

class NLLIB_EXPORT NLModelRenderer_line : public NLModelRenderer
{
    Q_OBJECT
    Q_PROPERTY(float length READ Length WRITE SetLength FINAL)
    Q_PROPERTY(QColor color READ Color WRITE SetColor FINAL)
public:
    explicit NLModelRenderer_line(const NLProperties &prop = NLProperties(), NLActor *actor = 0);
    float Length() const { return m_length; }
    void SetLength(float length);
    QColor Color() const { return m_color; }
    void SetColor(const QColor &color);

protected:
    virtual void InitProperty();

private:
    float m_length;
    QColor m_color;
};

#endif // _KARIN_NLMODELRENDERER_H
