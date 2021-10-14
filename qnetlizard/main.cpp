#include "netlizard.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "mainwindow.h"
#include "logoutput.h"
#include "qdef.h"

#include "nlactor.h"
#include "nlobject.h"
#include "nlobjectcontainer.h"
#include "nlactorcontainer.h"
#include "test/testscene.h"

static int Test(int argc, char **argv);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLocale::setDefault(QLocale::system());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VER);
    app.setOrganizationName(APP_DEV);

    qRegisterMetaType<NLVector3>("NLVector3");

    int test = Test(argc, argv);
    if(test != 0)
        return test;

    //qInstallMsgHandler(log_output_msg_handler);

    //TestWidget w;
    //w.show();

    MainWindow viewer;
    QWidget *win = &viewer;
    win->show();

    return app.exec();
}

int Test(int argc, char **argv)
{
    NLVector3 nor = VECTOR3(0, 1, 0);
    NLVector3 nor2 = VECTOR3(0, 0, 0);
    NLMatrix4 mat;
    NLMatrix4 idmat;
    Mesa_AllocGLMatrix(&mat);
    Mesa_AllocGLMatrix(&idmat);
    Mesa_glRotate(&idmat, 90, 1, 0, 0);
    Mesa_NormalMatrix(&mat, idmat.m);
    Mesa_glTransform(nor2.v, nor.v, &mat);
    qDebug() << QString().setNum(nor2.v[0]) << QString().setNum(nor2.v[1]) << QString().setNum(nor2.v[2]);
    Mesa_FreeGLMatrix(&mat);
    Mesa_FreeGLMatrix(&idmat);
    return 0;
}
