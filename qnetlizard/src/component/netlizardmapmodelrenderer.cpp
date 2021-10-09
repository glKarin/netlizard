#include "netlizardmapmodelrenderer.h"

#include <QDebug>

NETLizardMapModelRenderer::NETLizardMapModelRenderer(QObject *parent) :
    NLRenderable(parent),
    m_model(0)
{
    setObjectName("NETLizardMapModelRenderer");
}

NETLizardMapModelRenderer::~NETLizardMapModelRenderer()
{

}

void NETLizardMapModelRenderer::InitRender()
{

}

void NETLizardMapModelRenderer::Render()
{
    if(!m_model)
        return;
    glPushMatrix();
    {
        RenderSky();
        NETLizard_RenderGL3DModel(m_model);
    }
    glPopMatrix();
}

void NETLizardMapModelRenderer::DeinitRender()
{

}

GL_NETLizard_3D_Model * NETLizardMapModelRenderer::Model()
{
    return m_model;
}

void NETLizardMapModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    m_model = model;
}

void NETLizardMapModelRenderer::RenderSky()
{
    if(!m_model)
        return;

#if 0
    if(m_model->game == NL_SHADOW_OF_EGYPT_3D
             //&& (level == 0 || level == 8 || level == 9 || level == 10 || level == 12)
            )
    {
        for(int i = 0; i < m_model->count; i++)
        {
            const GL_NETLizard_3D_Mesh *mesh = m_model->meshes + i;
            unsigned int k;
            for(k = mesh->item_index_range[0]; k < mesh->item_index_range[1]; k++)
            {
                int item_type = nlGetItemType(NL_SHADOW_OF_EGYPT_3D, k);
                qDebug() << item_type;
                if(item_type == Item_Box_Type)
                {
                    const GL_NETLizard_3D_Item_Mesh *im = m_model->item_meshes + k;
                    if(!im->item_mesh.materials)
                        break;
                    glPushMatrix();
                    {
                        glDisable(GL_DEPTH_TEST);
                        glDepthMask(GL_FALSE);
                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        gluPerspective(45, width() / height(), 0.01, 999999);
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
//                        glRotatef(x_r_3d, 1.0, 0.0 ,0.0);
//                        glRotatef(y_r_3d, 0.0, 1.0, 0.0);
                        glRotatef(-90, 1, 0, 0);
                        GLfloat xs = im->item_mesh.ortho[0] - im->item_mesh.ortho[3];
                        GLfloat ys = im->item_mesh.ortho[1] - im->item_mesh.ortho[4];
                        GLfloat zs = im->item_mesh.ortho[2] - im->item_mesh.ortho[5];
                        glScalef(999999 / xs, 999999 / ys, 999999 / zs);
                        NETLizard_RenderGL3DMesh(&(im->item_mesh), m_model->texes);
                        glDepthMask(GL_TRUE);
                        glEnable(GL_DEPTH_TEST);
                    }
                    glPopMatrix();
                    break;
                }
            }
        }
    }
    else if(m_model->bg_tex)
    {
        GLfloat w = width();
        GLfloat h = height();
        const GLfloat _Vertex[] = {
            0.0, 0.0,
            w, 0.0,
            0.0, h,
            w, h
        };
        const GLfloat _Texcoord[] = {
            0.0, 1.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0
        };
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, _Vertex);
        glTexCoordPointer(2, GL_FLOAT, 0, _Texcoord);
        glBindTexture(GL_TEXTURE_2D, m_model->bg_tex->texid);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }
#endif
}
