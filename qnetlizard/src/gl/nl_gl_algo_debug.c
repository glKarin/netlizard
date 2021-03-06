#include "nl_gl_algo_debug.h"

#include <stdlib.h>
#include <stdio.h>

#include "nl_gl.h"
#include "math/vector3.h"
#include "math/line.h"
#include "math/bound.h"
#include "nl_util.h"
#include "linkedlist.h"

#define PRINT(fmt, args...) { fprintf(stderr, fmt, ##args); fprintf(stderr, "\n"); fflush(stderr); }

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
#define HL_LINE_COLOR 0,1,0,1

#define BITS_FALSE(b, t) (((b) & (t)) == 0)
#define BITS_TRUE(b, t) (((b) & (t)) != 0)

//#define POINT_CLIP(p, x) plane_point_clip(p, x)
#define POINT_CLIP(p, x) plane_point_clip_precision(p, x, 1)

typedef linkedlist(vector3_t) Vector3List;
typedef linkedlist(line_t) LineList;

static GLboolean NETLizard_IsInteractiveItem(int item_type)
{
    if((item_type & NL_3D_ITEM_TYPE_SWITCH)
            || (item_type & NL_3D_ITEM_TYPE_PORTAL)
            || (item_type & NL_3D_ITEM_TYPE_FAN_Z_AXIS)
            || (item_type & NL_3D_ITEM_TYPE_FAN_Y_AXIS)
            || (item_type & NL_3D_ITEM_TYPE_FAN_X_AXIS)
            || (item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL)
            || (item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL)
            )
        return GL_TRUE;
    return GL_FALSE;
}

static int push_edge_line(LineList *list, const line_t *lp)
{
    int has = 0;
    list_node(line_t) *p;
    line_t *line;

    LISTV_DATA_FOREACH_T(line_t, p, line, list) // find in lines list
    {
        if(line_equals_ignore_seq(lp, line))
        {
            //printf("%P exist\n", line);
            has = 1;
            break;
        }
    }

    if(has) // if exists, remove this line
    {
        List_DeleteNode(list, p);
    }
    else // if not exists, add new line to list
    {
        List_PushBack(list, lp);
    }
    return has;
}

static void render_highlight_lines(const LineList *lines, const GLfloat color[4], const GLfloat position[3], const GLfloat rotation[3])
{
    if(List_Empty(lines))
        return;
    BEGIN_DEBUG_RENDER
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
        {
            if(position)
                glTranslatef(position[0], position[1], position[2]);
            if(rotation)
            {
                glRotatef(rotation[0], 1.0f, 0.0f, 0.0f);
                glRotatef(rotation[1], 0.0f, 0.0f, 1.0f);
                glRotatef(rotation[2], 0.0f, 1.0f, 0.0f);
            }
            if(color)
                glColor4fv(color);
            else
                glColor4f(LINE_COLOR);

            const list_node(line_t) *p;
            const line_t *line;
            LISTV_DATA_FOREACH_T(line_t, p, line, lines)
            {
                GLfloat vs[] = {
                    LINEV_A_X(line), LINEV_A_Y(line), LINEV_A_Z(line),
                    LINEV_B_X(line), LINEV_B_Y(line), LINEV_B_Z(line),
                };
                glVertexPointer(3, GL_FLOAT, 0, vs);
                glDrawArrays(GL_LINES, 0, 2);
            }
        }
        glPopMatrix();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    END_DEBUG_RENDER
}

void NETLizard_DebugHighlightRenderGL3DModelPlane(const GL_NETLizard_3D_Model *model, unsigned int scene, unsigned int plane_index)
{
    if(!model)
        return;

    const GL_NETLizard_3D_Mesh *mesh = model->meshes + scene;
    const GL_NETLizard_3D_Plane *p = mesh->plane + plane_index;
    const GL_NETLizard_3D_Vertex *vertex = mesh->vertex_data.vertex;
    const plane_t plane = PLANEV(p->position, p->normal);
    LineList lines = LIST(line_t);

    unsigned int i;
    for(i = 0; i < mesh->count; i++)
    {
        const GL_NETLizard_3D_Material *m = mesh->materials + i;
        unsigned int j;
        for(j = 0; j < m->index_count; j += 3) // GL_TRIANGLES
        {
            int index1 = m->index[j];
            int index2 = m->index[j + 1];
            int index3 = m->index[j + 2];
            const GL_NETLizard_3D_Vertex *pa[] = {
                vertex + index1,
                vertex + index2,
                vertex + index3,
            };

            int n;
            unsigned all_in = 1;
            // check point in plane
            for(n = 0; n < 3; n++) // 3 points of triangle
            {
                vector3_t point = VECTOR3V(pa[n]->position);
                if(POINT_CLIP(&plane, &point) != 0) // !!!
                {
                    all_in = 0;
                    break;
                }
            }
            if(!all_in)
                continue;

            for(n = 0; n < 3; n++) // 3 points of triangle
            {
                int next_index = (n + 1) % 3;

                line_t lp = LINEV(pa[n]->position, pa[next_index]->position);
                push_edge_line(&lines, &lp);
            }
        }
    }
    render_highlight_lines(&lines, NULL, mesh->position, mesh->rotation);

    List_DeleteAll(&lines);
}

void NETLizard_DebugHighlightRenderGL3DItemModelEdge(const GL_NETLizard_3D_Model *model, unsigned int scene, unsigned int item_index, const vector3_t *pos, const vector3_t *dir)
{
    if(!model || !pos)
        return;

    const GL_NETLizard_3D_Mesh *mesh = model->item_meshes + item_index;
    const GL_NETLizard_3D_Vertex *vertex = mesh->vertex_data.vertex;
    LineList lines = LIST(line_t);
    GLmatrix mat, nor_mat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&nor_mat);
    Mesa_glTranslate(&mat, mesh->position[0], mesh->position[1], mesh->position[2]);
    Mesa_glRotate(&mat, mesh->rotation[0], 1.0f, 0.0f, 0.0f);
    Mesa_glRotate(&mat, mesh->rotation[1], 0.0f, 0.0f, 1.0f);
    Mesa_glRotate(&mat, mesh->rotation[2], 0.0f, 1.0f, 0.0f);

    matrix_normal_matrix(&mat, &nor_mat);

    unsigned int i;
    for(i = 0; i < mesh->count; i++)
    {
        const GL_NETLizard_3D_Material *m = mesh->materials + i;
        unsigned int j;
        for(j = 0; j < m->index_count; j += 3) // GL_TRIANGLES
        {
            int index1 = m->index[j];
            int index2 = m->index[j + 1];
            int index3 = m->index[j + 2];
            const GL_NETLizard_3D_Vertex *pa[] = {
                vertex + index1,
                vertex + index2,
                vertex + index3,
            };

            vector3_t p2l;
            Mesa_glTransform(VECTOR3_V(p2l), pa[0]->normal, &nor_mat);

            float dot_p = 0;
            if(dir)
            {
                vector3_t d2 = *dir;
                vector3_invertv(&d2);
                dot_p = vector3_dot(&p2l, &d2);
            }
            else
            {
                vector3_t p;
                Mesa_glTransform(VECTOR3_V(p), pa[0]->position, &mat);
                vector3_t nor;
                vector3_directionv(&nor, &p, pos);
                dot_p = vector3_dot(&p2l, &nor);
            }

            if(dot_p <= 0.0)
                continue;

            int n;
            for(n = 0; n < 3; n++) // 3 points of triangle
            {
                int next_index = (n + 1) % 3;

                line_t lp = LINEV(pa[n]->position, pa[next_index]->position);
                push_edge_line(&lines, &lp);
            }
        }
    }

    GLfloat color[4] = {LINE_COLOR};
    if(NETLizard_IsInteractiveItem(mesh->item_type))
    {
        color[0] = 0;
        color[1] = 1;
        color[2] = 0;
    }

    Mesa_FreeGLMatrix(&mat);
    Mesa_FreeGLMatrix(&nor_mat);
    render_highlight_lines(&lines, color, mesh->position, mesh->rotation);
    List_DeleteAll(&lines);
}
