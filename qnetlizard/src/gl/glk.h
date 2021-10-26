#ifndef _KARIN_GLK_H
#define _KARIN_GLK_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GL_INCR_WRAP
#define GL_INCR_WRAP GL_INCR_WRAP_EXT
#endif
#ifndef GL_DECR_WRAP
#define GL_DECR_WRAP GL_DECR_WRAP_EXT
#endif

typedef void (*glStencilOpSeparateProc)(GLenum, GLenum, GLenum, GLenum);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_GLK_H
