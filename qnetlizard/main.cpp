#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "netlizard.h"

#include "mainwindow.h"
#include "logoutput.h"
#include "qdef.h"
#include "nlglobals.h"
#include "lang.h"
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

    QTranslator translator;
    const QString locale = QLocale::system().name();
    const QString qmFile(APP_PKG "." + locale);

    qDebug() << "Load i18n -> " << qmFile << ".qm";

    if(translator.load(qmFile, "i18n"))
    {
        qDebug() << "Done";
        app.installTranslator(&translator);
    }
    else
        qDebug() << "Fail";

    SINGLE_INSTANCE_OBJ(Lang)->Load();

    NL::init_engine();

#ifdef _DEV_TEST
    int test = Test(argc, argv);
    if(test != 0)
        return test;

    //TestWidget w;
    //w.show();
#endif

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
