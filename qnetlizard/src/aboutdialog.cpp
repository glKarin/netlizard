#include "aboutdialog.h"

#include <QDebug>

#include "qdef.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QMessageBox(parent)
{
    setObjectName("AboutDialog");
    Init();
}

AboutDialog::~AboutDialog()
{
    DEBUG_DESTROY_Q
}

void AboutDialog::Init()
{
#define T_ENDL "<br/>"
    QString text(
                 "<b>" APP_NAME "</b> is a tools for <i>`" LIB_NAME "`</i> using Qt4." T_ENDL
                 "<b>" LIB_NAME "</b> is a tools of NETLizard J2ME game resource." T_ENDL
                T_ENDL
                "Version: " APP_VER T_ENDL
                "Dev: " APP_DEV T_ENDL
                "Github: <a href='" APP_GITHUB "'>glKarin/netlizard</a>" T_ENDL
                 );
    setText(APP_NAME);
    setInformativeText(text);
    addButton("Close", QMessageBox::AcceptRole);
    setWindowTitle(QString("About ") + APP_NAME);
#undef T_ENDL
}

int AboutDialog::Show(QWidget *parent)
{
    AboutDialog dialog(parent);
    return dialog.exec();
}
