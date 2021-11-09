#ifndef _KARIN_OPENGL_RENDER_H
#define _KARIN_OPENGL_RENDER_H

#ifdef __cplusplus
extern "C" {
#endif

struct vector3_s;
struct triangle_s;
struct line_s;
struct ray_s;
struct plane_s;

void opengl_render_points(const struct vector3_s *v, unsigned int count, float size, const float color[4]);
void opengl_render_lines(const struct line_s *v, unsigned int count, float size, float width, const float color[4], const float color2[4], const float color3[4]);
void opengl_render_planes(const struct plane_s *v, unsigned int count, float length, float size, float width, const float color[4], const float color2[4], const float color3[4]);
void opengl_render_rays(const struct ray_s *v, unsigned int count, float length, float size, float width, const float color[4], const float color2[4]);
void opengl_render_polygons(unsigned mode, const struct vector3_s *v, unsigned int count, float size, float width, const float color[4]);

void opengl_render_triangles(const struct triangle_s *v, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_OPENGL_RENDER_H
