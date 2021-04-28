#include "netlizard.h"

#include <QtCore/QtCore>
#include <QtGui>/QtGui>

#include "io.h"
#include "qt/imageviewer.h"

int main(int argc, char *argv[])
{
#define CT3D_PATH "E:\\qobject\\netlizard-game\\ct3d"
    const char text_file[] = "E:\\qobject\\ab_rus.png";
    const char png_file[] = "E:\\qobject\\r6.png";
    const char ct3d_map_file[] = CT3D_PATH"\\lvl1.png";

    QApplication app(argc, argv);
    QLocale::setDefault(QLocale::system());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    ImageViewer viewer;
    viewer.show();

    return app.exec();
}
