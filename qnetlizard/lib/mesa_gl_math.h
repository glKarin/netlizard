#ifndef MESA_GL_MATH_H
#define MESA_GL_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesa_math/m_matrix.h"

#define GL_MATRIX_M(matrix) (matrix).m
#define GL_MATRIXV_M(matrix) (matrix)->m

#define GL_MATRIX_MM(matrix) ((float **)(GL_MATRIX_M(matrix)))
#define GL_MATRIXV_MM(matrix) ((float **)(GL_MATRIXV_M(matrix)))

#define GL_MATRIX_M_CR(matrix, col, row) (GL_MATRIX_M(matrix))[row * 4 + col]
#define GL_MATRIXV_M_CR(matrix, col, row) (GL_MATRIXV_M(matrix))[row * 4 + col]

#define GL_MATRIX_M_CR4(matrix, col, row) (GL_MATRIX_M(matrix))[(row - 1) * 4 + (col - 1)]
#define GL_MATRIXV_M_CR4(matrix, col, row) (GL_MATRIXV_M(matrix))[(row - 1) * 4 + (col - 1)]

#define GL_MATRIX_M_I(matrix, i) (GL_MATRIX_M(matrix))[i]
#define GL_MATRIXV_M_I(matrix, i) (GL_MATRIXV_M(matrix))[i]

#define Mesa_glTransform_column(t, o, m) Mesa_glTransform(t, 0, m)
#define Mesa_glTransform4_column(t, o, m) Mesa_glTransform4(t, 0, m)

void Mesa_glTranslate(GLmatrix *mat, float x, float y, float z);
void Mesa_glRotate(GLmatrix *mat, float angle, float x, float y, float z);
void Mesa_glScale(GLmatrix *mat, float x, float y, float z);
void Mesa_glLoadMatrix(GLmatrix *mat, const float *m);
void Mesa_glLoadIdentity(GLmatrix *mat);
void Mesa_glLoadTransposeMatrix(GLmatrix *mat, const float *m);
void Mesa_glMultMatrix(GLmatrix *mat, const float *m);
void Mesa_glMultTransposeMatrix(GLmatrix *mat, const float *m);
void Mesa_glFrustum(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval);
void Mesa_glOrtho(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval);

void Mesa_gluLookAt(GLmatrix *r, GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy, GLfloat upz);
void Mesa_gluPerspective(GLmatrix *r, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
void Mesa_gluOrtho2D(GLmatrix *mat, float left, float right, float bottom, float top);

// Row vector: P = vector * matrix; exam: cale normal transform: varying normal = gl_Normal * vNormalMatrix;
void Mesa_glTransform_row(float r[3], const float p[3], const GLmatrix *mat);
void Mesa_glTransform4_row(float r[4], const float p[4], const GLmatrix *mat);
// Column vector: P = matrix * vector; exam: cale vextex transform: gl_position = vMVPMatrix * gl_Vertex;
void Mesa_glTransform(float r[3], const float p[3], const GLmatrix *mat);
void Mesa_glTransform4(float r[4], const float p[4], const GLmatrix *mat);
void Mesa_glTranspose(GLmatrix *mat);
void Mesa_FreeGLMatrix(GLmatrix *mat);
void Mesa_AllocGLMatrix(GLmatrix *mat);
void Mesa_NormalMatrix(GLmatrix *mat, const GLfloat mv[16]);


#ifdef __cplusplus
}
#endif

#endif // MESA_GL_MATH_H
