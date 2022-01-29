#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "netlizard.h"

#include "mainwindow.h"
#include "logoutput.h"
#include "qdef.h"
#include "common/nlglobals.h"
#include "lang.h"
#include "engineregisterobject.h"

static int Test(int argc, char **argv);
static int nl_log_func(int type, const char *str);
static bool load_i18n(const QLocale &locale = QLocale());

int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(new QApplication(argc, argv));

    app->setApplicationName(APP_NAME);
    app->setApplicationVersion(APP_VER);
    app->setOrganizationName(APP_DEV);

    Q_INIT_RESOURCE(qnetlizard);

    load_i18n();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    NL::init_engine();
    NL::register_engine(new EngineRegisterObject);

#ifdef _DEV_TEST
    int test = Test(argc, argv);
    if(test != 0)
        return test;
#endif

    LogOutput::Instance();
    nlEnable(NL_LOG);
    nlLogFunc(NL_LOG_OUT, NL_LOG_USER, (void *)nl_log_func);
    nlLogFunc(NL_LOG_ERR, NL_LOG_USER, (void *)nl_log_func);

    QScopedPointer<MainWindow> win(new MainWindow);
    win->show();

    int res = app->exec();
    NL::deinit_engine();

    return res;
}

bool load_i18n(const QLocale &l)
{
    QTranslator *translator = new QTranslator(qApp);
    const QString locale = l.name();
    const QString qmFile(APP_PKG "." + locale);

    QLocale::setDefault(l);

    qDebug() << "Load i18n -> " << qmFile << ".qm";

    bool res = translator->load(qmFile, "i18n");

    if(res)
    {
        qDebug() << "Done";
        qApp->installTranslator(translator);
    }
    else
        qDebug() << "Fail";

    SINGLE_INSTANCE_OBJ(Lang)->Load();

    return res;
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
