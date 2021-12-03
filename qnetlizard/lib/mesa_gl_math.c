#include "mesa_gl_math.h"

#include "mesa_math/main/imports.h"

#define arrcpy16(n, o) {\
    n[0] = o[0]; \
    n[1] = o[1]; \
    n[2] = o[2]; \
    n[3] = o[3]; \
    n[4] = o[4]; \
    n[5] = o[5]; \
    n[6] = o[6]; \
    n[7] = o[7]; \
    n[8] = o[8]; \
    n[9] = o[9]; \
    n[10] = o[10]; \
    n[11] = o[11]; \
    n[12] = o[12]; \
    n[13] = o[13]; \
    n[14] = o[14]; \
    n[15] = o[15]; \
}


#define IF_NULL_RETURN(p) // if(!p) return;

void Mesa_glTranslate(GLmatrix *mat, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        _math_matrix_translate( mat, x, y, z );
}

void Mesa_glRotate(GLmatrix *mat, float angle, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        if(angle != 0.0F)
            _math_matrix_rotate( mat, angle, x, y, z );
}

void Mesa_glScale(GLmatrix *mat, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        _math_matrix_scale( mat, x, y, z );
}

void Mesa_glLoadMatrix(GLmatrix *mat, const float m[16])
{
    IF_NULL_RETURN(mat)
    if (!m) return;
    _math_matrix_loadf( mat, m );
}

void Mesa_glLoadTransposeMatrix(GLmatrix *mat, const float m[16])
{
    IF_NULL_RETURN(mat)
    float tm[16];
   if (!m) return;
   _math_transposef(tm, m);
   Mesa_glLoadMatrix(mat, tm);
}

void Mesa_glLoadIdentity(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
        _math_matrix_set_identity( mat );
}

void Mesa_glMultMatrix(GLmatrix *mat, const float m[16])
{
   IF_NULL_RETURN(mat)
   if (!m) return;
   _math_matrix_mul_floats( mat, m );
}

void Mesa_glMultTransposeMatrix(GLmatrix *mat, const float m[16])
{
    IF_NULL_RETURN(mat)
    float tm[16];
   if (!m) return;
   _math_transposef(tm, m);
   Mesa_glMultMatrix(mat, tm);
}

void Mesa_glFrustum(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval)
{
   IF_NULL_RETURN(mat)
   if (nearval <= 0.0 ||
       farval <= 0.0 ||
       nearval == farval ||
       left == right ||
       top == bottom)
   {
      _mesa_error( NULL,  GL_INVALID_VALUE, "glFrustum" );
      return;
   }

     _math_matrix_frustum( mat,
             left, right,
             bottom, top,
             nearval, farval );
}

void Mesa_glOrtho(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval)
{
    IF_NULL_RETURN(mat)

   if (left == right ||
       bottom == top ||
       nearval == farval)
   {
       _mesa_error( NULL,  GL_INVALID_VALUE, "glOrtho" );
       return;
   }

    _math_matrix_ortho( mat,
            left, right,
            bottom, top,
            nearval, farval );
}

void Mesa_glTransform_row(float r[3], const float p[3], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    float v[4] = {p[0], p[1], p[2], 1};
    float u[4];
    _mesa_transform_vector(u, v, mat->m);
    r[0] = u[0] / u[3];
    r[1] = u[1] / u[3];
    r[2] = u[2] / u[3];
}

void Mesa_glTransform4_row(float r[4], const float p[4], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    _mesa_transform_vector(r, p, mat->m);
}

// column vector
void Mesa_glTransform(float r[3], const float p[3], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    float u[4];
    TRANSFORM_POINT3(u, mat->m, p);
    r[0] = u[0] / u[3];
    r[1] = u[1] / u[3];
    r[2] = u[2] / u[3];
}

// column vector
void Mesa_glTransform4(float r[4], const float p[4], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    TRANSFORM_POINT(r, mat->m, p);
}

void Mesa_glTranspose(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    float tm[16];

   _math_transposef(tm, mat->m);
    _math_matrix_loadf( mat, tm );
}

void Mesa_AllocGLMatrix(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    _math_matrix_ctr(mat);
    _math_matrix_alloc_inv(mat); // alloc inverse matrix
}

void Mesa_FreeGLMatrix(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    _math_matrix_dtr(mat);
}

int Mesa_GLMatrixIsAlloc(GLmatrix *mat)
{
    return GL_MATRIXV_M(mat) ? 1 : 0;
}

void Mesa_DupGLMatrix(GLmatrix *to, const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    IF_NULL_RETURN(to)
    to->flags = mat->flags;
    to->type = mat->type;
    if(GL_MATRIXV_M(mat))
    {
        arrcpy16(GL_MATRIXV_M(to), GL_MATRIXV_M(mat))
    }
    if(GL_MATRIXV_INV_M(mat))
    {
        arrcpy16(GL_MATRIXV_INV_M(to), GL_MATRIXV_INV_M(mat))
    }
}

void Mesa_InitGLMatrix(GLmatrix *to, const GLmatrix *mat)
{
    IF_NULL_RETURN(to)
    Mesa_AllocGLMatrix(to);
    if(mat)
    {
        Mesa_DupGLMatrix(to, mat);
    }
}

void Mesa_InverseTransposeMatrix(GLmatrix *mat, const GLfloat mv[16])
{
    IF_NULL_RETURN(mat)
    GLmatrix nor;
    GLfloat tmp[16];
    GLfloat dst[16];

    Mesa_AllocGLMatrix(&nor);
    Mesa_glLoadIdentity(&nor);
    _math_matrix_loadf(&nor, mv);
    _math_matrix_analyse(&nor);
    arrcpy16(tmp, nor.inv);
    _math_transposef(dst, tmp);
    Mesa_FreeGLMatrix(&nor);

    _math_matrix_loadf(mat, dst);
}

void Mesa_InverseMatrix(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    _math_matrix_analyse(mat);
}

void Mesa_NormalMatrix(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    GLfloat tmp[16];
    GLfloat mv[16] = {
        GL_MATRIXV_M(mat)[0], GL_MATRIXV_M(mat)[1], GL_MATRIXV_M(mat)[2], 0.0,
        GL_MATRIXV_M(mat)[4], GL_MATRIXV_M(mat)[5], GL_MATRIXV_M(mat)[6], 0.0,
        GL_MATRIXV_M(mat)[8], GL_MATRIXV_M(mat)[9], GL_MATRIXV_M(mat)[10], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    _math_matrix_loadf(mat, mv);
    _math_matrix_analyse(mat);
    arrcpy16(mv, mat->inv);
    _math_transposef(tmp, mv);

    _math_matrix_loadf(mat, tmp);
}


// GLU
#define __glPi 3.14159265358979323846
#define COS cos

static void normalize(float v[3])
{
    float r;

    r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

void Mesa_gluPerspective(GLmatrix *r, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    IF_NULL_RETURN(r)
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians = fovy / 2 * __glPi / 180;

	if(!r)
        return;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
	return;
    }
    cotangent = COS(radians) / sine;

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    Mesa_glMultMatrix(r, &m[0][0]);
}

void Mesa_gluLookAt(GLmatrix *r, GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy, GLfloat upz)
{
    IF_NULL_RETURN(r)
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

	if(!r)
        return;

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    Mesa_glMultMatrix(r, &m[0][0]);
    Mesa_glTranslate(r, -eyex, -eyey, -eyez);
}

void Mesa_gluOrtho2D(GLmatrix *mat, float left, float right, float bottom, float top)
{
    Mesa_glOrtho(mat, left, right, bottom, top, -1, 1);
}
