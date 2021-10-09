#ifndef _KARIN_NETLIZARDMAPMODELRENDERER_H
#define _KARIN_NETLIZARDMAPMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer : public NLRenderable
{
    Q_OBJECT
public:
    explicit NETLizardMapModelRenderer(QObject *parent = 0);
    virtual ~NETLizardMapModelRenderer();
    GL_NETLizard_3D_Model * Model();
    void SetModel(GL_NETLizard_3D_Model *model);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();
    
signals:
    
public slots:

private:
    void RenderSky();

private:
    GL_NETLizard_3D_Model *m_model;

    Q_DISABLE_COPY(NETLizardMapModelRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELRENDERER_H
