#ifndef _KARIN_NLDBG_H
#define _KARIN_NLDBG_H

#ifdef _DEV_TEST
#define NLDEBUG_DESTROY_Q qDebug() << "Q ~" + objectName();
#define NLDEBUG_DESTROY(name) qDebug() << "~" #name;
#define NLDEBUG_DESTROY_QQV(obj) qDebug() << "Q ~" + obj->objectName();
#else
#define NLDEBUG_DESTROY_Q
#define NLDEBUG_DESTROY(name)
#define NLDEBUG_DESTROY_QQV(obj)
#endif

#define NLDEBUG_VECTOR3(v) qDebug() << QString(#v "(%1, %2, %3)").arg(VECTOR3_X(v), 0, 'f', 6).arg(VECTOR3_Y(v), 0, 'f', 6).arg(VECTOR3_Z(v), 0, 'f', 6);

#define NLDEBUG_MATRIX4(v) qDebug() << QString(#v "[\n|%1, %2, %3, %4|\n|%5, %6, %7, %8|\n|%9, %10, %11, %12|\n|%13, %14, %15, %16|\n]")\
.arg(GL_MATRIX_M_CR4(v, 1, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 1), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 2), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 3), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 4), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 4, 4), 16, 'f', 6)\
;
#define NLDEBUG_MATRIX3(v) qDebug() << QString(#v "[\n|%1, %2, %3|\n|%4, %5, %6|\n|%7, %8, %9|\n|%10, %11, %12|\n]")\
.arg(GL_MATRIX_M_CR4(v, 1, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 1), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 1), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 2), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 2), 16, 'f', 6)\
.arg(GL_MATRIX_M_CR4(v, 1, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 2, 3), 16, 'f', 6).arg(GL_MATRIX_M_CR4(v, 3, 3), 16, 'f', 6)\
;

#endif // _KARIN_NLDBG_H
