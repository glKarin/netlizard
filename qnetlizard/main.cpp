#include "netlizard.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "mainwindow.h"
#include "qdef.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLocale::setDefault(QLocale::system());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VER);

    MainWindow viewer;
    QWidget *win = &viewer;
    win->show();

    return app.exec();
}
