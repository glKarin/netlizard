#include "nlmodelrenderer.h"

#include <QDebug>

#include "qdef.h"

NLModelRenderer::NLModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(new NLRenderModelGLGeneral)
{
    SetName("NLModelRenderer");
}

NLModelRenderer::~NLModelRenderer()
{
    if(m_model)
        delete m_model;
    DEBUG_DESTROY("NLModelRenderer")
}

void NLModelRenderer::InitRender()
{

}

void NLModelRenderer::Render()
{
    if(!m_model)
        return;
    glPushMatrix();
    {
        m_model->Render();
    }
    glPopMatrix();
}

void NLModelRenderer::DeinitRender()
{
    if(m_model)
    {
        delete m_model;
        m_model = 0;
    }
}

NLRenderModelGLGeneral * NLModelRenderer::Model()
{
    return m_model;
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
