//#include "nl_gl_debug.h"

//#include <stdlib.h>

//#include "lib/vector3.h"
//#include "lib/line.h"
//#include "lib/bound.h"
//#include "nl_util.h"

#define BEGIN_DEBUG_RENDER \
    glDepthMask(GL_FALSE); \
    glDisable(GL_DEPTH_TEST); \
    glDisable(GL_ALPHA_TEST); \
    glDisable(GL_BLEND); \
    glDisable(GL_TEXTURE_2D); \
    glDisable(GL_CULL_FACE); \
    glLineWidth(2); \
    glPointSize(6); \
    glEnable(GL_LINE_SMOOTH); \
    glEnable(GL_POINT_SMOOTH); \
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); \
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); \
    GLfloat _cur_color[4]; \
    glGetFloatv(GL_CURRENT_COLOR, _cur_color);

#define END_DEBUG_RENDER \
    glDepthMask(GL_TRUE); \
    glEnable(GL_DEPTH_TEST); \
    glEnable(GL_ALPHA_TEST); \
    glEnable(GL_BLEND); \
    glEnable(GL_TEXTURE_2D); \
    glEnable(GL_CULL_FACE); \
    glLineWidth(1); \
    glPointSize(1); \
    glDisable(GL_LINE_SMOOTH); \
    glDisable(GL_POINT_SMOOTH); \
    glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE); \
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE); \
    glColor4fv(_cur_color);

#define NORMAL_LENGTH 100
#define POINT_COLOR 0,0,1,1
#define LINE_COLOR 1,0,0,1

#define BITS_FALSE(b, t) (((b) & (t)) == 0)
#define BITS_TRUE(b, t) (((b) & (t)) != 0)

#if 0
void NETLizard_DebugRenderGL3DModel(const GL_NETLizard_3D_Model *model, GLuint type, GL_NETLizard_Debug_Render_Mesh_f func)
{
    if(!model || !func)
		return;
    if(BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_ITEM) && BITS_FALSE(type, NETLIZARD_DEBUG_TYPE_SCENE))
        return;

    if(model->meshes && BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_SCENE))
	{
		GLuint i;
		for(i = 0; i < model->count; i++)
        {
            const GL_NETLizard_3D_Mesh *m = model->meshes + i;
            func(m);
#if 0
            if(model->item_meshes)
			{
				GLuint j;
				for(j = m->item_index_range[0]; j < m->item_index_range[1]; j++) 
                {
					GL_NETLizard_3D_Item_Mesh *im = model->item_meshes + j;
                    if(!im->materials) // REDO
                        continue;
                    func(im);
				}
			}
#endif
		}
	}

    if(model->item_meshes && BITS_TRUE(type, NETLIZARD_DEBUG_TYPE_ITEM))
	{
		GLuint i;
		for(i = 0; i < model->item_count; i++)
        {
            const GL_NETLizard_3D_Item_Mesh *m = model->item_meshes + i;
            if(!m->materials) // REDO
				continue;
            if(m->item_type & NL_3D_ITEM_TYPE_SKY_BOX)
                continue;
            func(m);
		}
	}
}
#endif
