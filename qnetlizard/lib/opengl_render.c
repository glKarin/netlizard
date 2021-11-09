#include "opengl_render.h"

#include <GL/gl.h>

#include "vector3.h"
#include "line.h"
#include "plane.h"
#include "triangle.h"

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
    glGetFloatv(GL_CURRENT_COLOR, _cur_color); \
    glEnableClientState(GL_VERTEX_ARRAY);

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
    glColor4fv(_cur_color); \
    glDisableClientState(GL_VERTEX_ARRAY); \
    glFlush();

void opengl_render_points(const struct vector3_s *v, unsigned int count, float size, const float color[4])
{
    BEGIN_DEBUG_RENDER
    {
        glPointSize(size);
        if(color)
            glColor4fv(color);
        int i;
        for(i = 0; i < count; i++)
        {
            glVertexPointer(3, GL_FLOAT, 0, VECTOR3_V(v[i]));
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }
    END_DEBUG_RENDER
}

void opengl_render_lines(const struct line_s *v, unsigned int count, float size, float width, const float color[4], const float color2[4], const float color3[4])
{
    BEGIN_DEBUG_RENDER
    {
        glPointSize(size);
        glLineWidth(width);
        int i;
        for(i = 0; i < count; i++)
        {
            const line_t *l = v + i;
            GLfloat vs[] = {
                LINEV_A_X(l), LINEV_A_Y(l), LINEV_A_Z(l),
                LINEV_B_X(l), LINEV_B_Y(l), LINEV_B_Z(l),
            };
            glVertexPointer(3, GL_FLOAT, 0, vs);
            if(color)
                glColor4fv(color);
            glDrawArrays(GL_LINES, 0, 2);
            if(color2)
                glColor4fv(color2);
            glDrawArrays(GL_POINTS, 0, 1);
            if(color3)
                glColor4fv(color3);
            glDrawArrays(GL_POINTS, 1, 1);
        }
    }
    END_DEBUG_RENDER
}

void opengl_render_rays(const struct ray_s *v, unsigned int count, float length, float size, float width, const float color[4], const float color2[4])
{
    BEGIN_DEBUG_RENDER
    {
        glPointSize(size);
        glLineWidth(width);
        int i;
        for(i = 0; i < count; i++)
        {
            const ray_t *r = v + i;
            vector3_t vec = RAYV_POSITION(r);
            vector3_moveve(&vec, &RAYV_DIRECTION(r), length);
            GLfloat vs[] = {
                RAYV_POSITION_X(r), RAYV_POSITION_Y(r), RAYV_POSITION_Z(r),
                VECTOR3_X(vec), VECTOR3_Y(vec), VECTOR3_Z(vec),
            };
            glVertexPointer(3, GL_FLOAT, 0, vs);
            if(color)
                glColor4fv(color);
            glDrawArrays(GL_LINES, 0, 2);
            if(color2)
                glColor4fv(color2);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }
    END_DEBUG_RENDER
}

void opengl_render_planes(const struct plane_s *v, unsigned int count, float length, float size, float width, const float color[4], const float color2[4], const float color3[4])
{
    BEGIN_DEBUG_RENDER
    {
        glPointSize(size);
        glLineWidth(width);
        int i;
        for(i = 0; i < count; i++)
        {
            const plane_t *p = v + i;
            vector3_t vec = PLANEV_POSITION(p);
            vector3_moveve(&vec, &PLANEV_NORMAL(p), length);
            GLfloat vs[] = {
                PLANEV_POSITION_X(p), PLANEV_POSITION_Y(p), PLANEV_POSITION_Z(p),
                VECTOR3_X(vec), VECTOR3_Y(vec), VECTOR3_Z(vec),
            };
            glVertexPointer(3, GL_FLOAT, 0, vs);
            if(color)
                glColor4fv(color);
            glDrawArrays(GL_LINES, 0, 2);
            if(color2)
                glColor4fv(color2);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }
    END_DEBUG_RENDER
}

void opengl_render_polygons(unsigned mode, const struct vector3_s *v, unsigned int count, float size, float width, const float color[4])
{
    BEGIN_DEBUG_RENDER
    {
        glPointSize(size);
        glLineWidth(width);
        if(color)
            glColor4fv(color);
        glVertexPointer(3, GL_FLOAT, 0, VECTOR3V_V(v));
        glDrawArrays(mode, 0, count);
    }
    END_DEBUG_RENDER
}
