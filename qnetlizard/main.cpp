#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "netlizard.h"

#include "mainwindow.h"
#include "logoutput.h"
#include "qdef.h"
#include "nlglobals.h"
#include "nldef.h"

static int Test(int argc, char **argv);
static int nl_log_func(int type, const char *str);

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
    nlEnable(NL_LOG);
    nlLogFunc(NL_LOG_OUT, NL_LOG_USER, (void *)nl_log_func);
    nlLogFunc(NL_LOG_ERR, NL_LOG_USER, (void *)nl_log_func);

    MainWindow viewer;
    QWidget *win = &viewer;
    win->show();

    return app.exec();
}

int nl_log_func(int type, const char *str)
{
    LogOutput *lo = LogOutput::Instance();
    lo->Push(type == NL_LOG_ERR ? QtCriticalMsg : QtDebugMsg, str);
    return strlen(str);
}

#ifdef _DEV_TEST
int Test(int argc, char **argv)
{
    return 0;
}
#endif
