#include "matrix.h"

#include <math.h>
#include <string.h>

#include "vector3.h"
#include "ortho.h"
#include "frustum.h"

void matrix_cale_frustum(const matrix4_t *proj_mat, const matrix4_t *view_mat, float frustum[6][4])
{
    memset(frustum, 0, sizeof(float) * 4 * 6);
    const float *proj;
    const float *modl;
    float clip[16] = {0};
    float t;

    /* Get the current PROJECTION matrix from OpenGL */
    //glGetFloatv( GL_PROJECTION_MATRIX, proj );
    proj = GL_MATRIXV_M(proj_mat);
    /* Get the current MODELVIEW matrix from OpenGL */
    //glGetFloatv( GL_MODELVIEW_MATRIX, modl );
    modl = GL_MATRIXV_M(view_mat);
    /* Combine the two matrices (multiply projection by modelview)    */

    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] +    modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] +    modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] +    modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] +    modl[ 3] * proj[15];
    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4]    + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] +    modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] +    modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] +    modl[ 7] * proj[15];
    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4]    + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] +    modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] +    modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] +    modl[11] * proj[15];
    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4]    + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] +    modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] +    modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] +    modl[15] * proj[15];
    /* Extract the numbers for the RIGHT plane */
    frustum[0][0] = clip[ 3] - clip[ 0];
    frustum[0][1] = clip[ 7] - clip[ 4];
    frustum[0][2] = clip[11] - clip[ 8];
    frustum[0][3] = clip[15] - clip[12];
    /* Normalize the result */
    t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2]    * frustum[0][2] );
    frustum[0][0] /= t;
    frustum[0][1] /= t;
    frustum[0][2] /= t;
    frustum[0][3] /= t;
    /* Extract the numbers for the LEFT plane */
    frustum[1][0] = clip[ 3] + clip[ 0];
    frustum[1][1] = clip[ 7] + clip[ 4];
    frustum[1][2] = clip[11] + clip[ 8];
    frustum[1][3] = clip[15] + clip[12];
    /* Normalize the result */
    t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2]    * frustum[1][2] );
    frustum[1][0] /= t;
    frustum[1][1] /= t;
    frustum[1][2] /= t;
    frustum[1][3] /= t;
    /* Extract the BOTTOM plane */
    frustum[2][0] = clip[ 3] + clip[ 1];
    frustum[2][1] = clip[ 7] + clip[ 5];
    frustum[2][2] = clip[11] + clip[ 9];
    frustum[2][3] = clip[15] + clip[13];
    /* Normalize the result */
    t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2]    * frustum[2][2] );
    frustum[2][0] /= t;
    frustum[2][1] /= t;
    frustum[2][2] /= t;
    frustum[2][3] /= t;
    /* Extract the TOP plane */
    frustum[3][0] = clip[ 3] - clip[ 1];
    frustum[3][1] = clip[ 7] - clip[ 5];
    frustum[3][2] = clip[11] - clip[ 9];
    frustum[3][3] = clip[15] - clip[13];
    /* Normalize the result */
    t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2]    * frustum[3][2] );
    frustum[3][0] /= t;
    frustum[3][1] /= t;
    frustum[3][2] /= t;
    frustum[3][3] /= t;
    /* Extract the FAR plane */
    frustum[4][0] = clip[ 3] - clip[ 2];
    frustum[4][1] = clip[ 7] - clip[ 6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];
    /* Normalize the result */
    t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2]    * frustum[4][2] );
    frustum[4][0] /= t;
    frustum[4][1] /= t;
    frustum[4][2] /= t;
    frustum[4][3] /= t;
    /* Extract the NEAR plane */
    frustum[5][0] = clip[ 3] + clip[ 2];
    frustum[5][1] = clip[ 7] + clip[ 6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];
    /* Normalize the result */
    t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2]    * frustum[5][2] );
    frustum[5][0] /= t;
    frustum[5][1] /= t;
    frustum[5][2] /= t;
    frustum[5][3] /= t;
}

void matrix_translate(matrix4_t *mat, const vector3_t *v)
{
    Mesa_glTranslate(mat, VECTOR3V_X(v), VECTOR3V_Y(v), VECTOR3V_Z(v));
}

void matrix_rotate(matrix4_t *mat, float angle, const vector3_t *v)
{
    Mesa_glRotate(mat, angle, VECTOR3V_X(v), VECTOR3V_Y(v), VECTOR3V_Z(v));
}

void matrix_scale(matrix4_t *mat, const vector3_t *v)
{
    Mesa_glScale(mat, VECTOR3V_X(v), VECTOR3V_Y(v), VECTOR3V_Z(v));
}

void matrix_lookat(matrix4_t *mat, const vector3_t *eye, const vector3_t *center, const vector3_t *up)
{
    Mesa_gluLookAt(mat,
                   VECTOR3V_X(eye), VECTOR3V_Y(eye), VECTOR3V_Z(eye),
                   VECTOR3V_X(center), VECTOR3V_Y(center), VECTOR3V_Z(center),
                   VECTOR3V_X(up), VECTOR3V_Y(up), VECTOR3V_Z(up)
                   );
}

void matrix_ortho(matrix4_t *mat, const ortho_t *v)
{
    Mesa_glOrtho(mat, ORTHOV_LEFT(v), ORTHOV_RIGHT(v), ORTHOV_BOTTOM(v), ORTHOV_TOP(v), ORTHOV_ZNEAR(v), ORTHOV_ZFAR(v));
}

void matrix_ortho2d(matrix4_t *mat, const ortho_t *v)
{
    Mesa_gluOrtho2D(mat, ORTHOV_LEFT(v), ORTHOV_RIGHT(v), ORTHOV_BOTTOM(v), ORTHOV_TOP(v));
}

void matrix_frustum(matrix4_t *mat, const frustum_t *v)
{
    Mesa_glFrustum(mat, FRUSTUMV_LEFT(v), FRUSTUMV_RIGHT(v), FRUSTUMV_BOTTOM(v), FRUSTUMV_TOP(v), FRUSTUMV_ZNEAR(v), FRUSTUMV_ZFAR(v));
}

void matrix_perspective(matrix4_t *mat, const perspective_t *v)
{
    Mesa_gluPerspective(mat, PERSPECTIVEV_FOVY(v), PERSPECTIVEV_ASPECT(v), PERSPECTIVEV_ZNEAR(v), PERSPECTIVEV_ZFAR(v));
}

void matrix_normal_matrix(const matrix4_t *mat, matrix4_t *v)
{
    GLfloat mv[16] = {
        GL_MATRIXV_M(mat)[0], GL_MATRIXV_M(mat)[1], GL_MATRIXV_M(mat)[2], 0.0,
        GL_MATRIXV_M(mat)[4], GL_MATRIXV_M(mat)[5], GL_MATRIXV_M(mat)[6], 0.0,
        GL_MATRIXV_M(mat)[8], GL_MATRIXV_M(mat)[9], GL_MATRIXV_M(mat)[10], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    Mesa_InverseTransposeMatrix(v, mv);
}

void matrix_identity(matrix4_t *mat)
{
    Mesa_glLoadIdentity(mat);
}

void matrix_transpose(matrix4_t *mat)
{
    Mesa_glTranspose(mat);
}

void matrix_inverse(matrix4_t *mat)
{
    Mesa_InverseMatrix(mat);
}

void matrix_load(matrix4_t *mat, const matrix4_t *v)
{
    Mesa_glLoadMatrix(mat, GL_MATRIXV_M(v));
}

void matrix_multi_matrix(matrix4_t *mat, const matrix4_t *v)
{
    Mesa_glMultMatrix(mat, GL_MATRIXV_M(v));
}

vector3_t matrix_transform_row(const matrix4_t *mat, const vector3_t *v)
{
    vector3_t r;
    matrix_transformv_row(mat, v, &r);
    return r;
}

void matrix_transformv_row(const matrix4_t *mat, const vector3_t *v, vector3_t *t)
{
    Mesa_glTransform_row(VECTOR3V_V(t), VECTOR3V_V(v), mat);
}

void matrix_transformv_self_row(const matrix4_t *mat, vector3_t *v)
{
    float u[3];
    Mesa_glTransform_row(u, VECTOR3V_V(v), mat);
    VECTOR3V_X(v) = u[0];
    VECTOR3V_Y(v) = u[1];
    VECTOR3V_Z(v) = u[2];
}

vector3_t matrix_transform(const matrix4_t *mat, const vector3_t *v)
{
    vector3_t r;
    matrix_transformv(mat, v, &r);
    return r;
}

void matrix_transformv(const matrix4_t *mat, const vector3_t *v, vector3_t *t)
{
    Mesa_glTransform(VECTOR3V_V(t), VECTOR3V_V(v), mat);
}

void matrix_transformv_self(const matrix4_t *mat, vector3_t *v)
{
    float u[3];
    Mesa_glTransform(u, VECTOR3V_V(v), mat);
    VECTOR3V_X(v) = u[0];
    VECTOR3V_Y(v) = u[1];
    VECTOR3V_Z(v) = u[2];
}
