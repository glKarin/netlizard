#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "netlizard.h"

#include "mainwindow.h"
#include "logoutput.h"
#include "qdef.h"
#include "nlglobals.h"

#include "nlactor.h"
#include "nlobject.h"
#include "nlobjectcontainer.h"
#include "nlactorcontainer.h"
#include "test/testscene.h"
#include "nlrendermodel.h"

static int Test(int argc, char **argv);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLocale::setDefault(QLocale::system());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VER);
    app.setOrganizationName(APP_DEV);

    Q_INIT_RESOURCE(qnetlizard);

    NL::init_engine();

#ifdef _DEV_TEST
    int test = Test(argc, argv);
    if(test != 0)
        return test;

    //TestWidget w;
    //w.show();
#endif
    qRegisterMetaType<NLVector3>("NLVector3");

    LogOutput::Instance();

    MainWindow viewer;
    QWidget *win = &viewer;
    win->show();

    return app.exec();
}

#ifdef _DEV_TEST
int Test(int argc, char **argv)
{
    return 0;
    NLVector3 nor = VECTOR3(0, 1, 0);
    NLVector3 nor2 = VECTOR3(0, 0, 0);
    NLMatrix4 mat;
    NLMatrix4 idmat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&idmat);
    Mesa_glRotate(&idmat, 90, 1, 0, 0);
    Mesa_NormalMatrix(&mat, idmat.m);
    Mesa_glTransform_row(nor2.v, nor.v, &mat);
    qDebug() << QString().setNum(nor2.v[0]) << QString().setNum(nor2.v[1]) << QString().setNum(nor2.v[2]);
    Mesa_FreeGLMatrix(&mat);
    Mesa_FreeGLMatrix(&idmat);
    return 0;
}
#endif
