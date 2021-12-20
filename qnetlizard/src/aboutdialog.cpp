#include "aboutdialog.h"

#include <QDebug>
#include <QDateTime>
#include <QRegExp>

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
    QString text = QString(
                "<p align='center'><img src=':LZ' height='48'></img><p>" T_ENDL
                 "<b>" APP_NAME "</b> is a tools for <i>`" LIB_NAME "`</i> using Qt4." T_ENDL
                 "<b>" LIB_NAME "</b> is a tools of NETLizard J2ME game resource." T_ENDL
                T_ENDL
                + tr("Version: ") + APP_VER T_ENDL
                + tr("Build: ") + "%1(" APP_STATE ")" T_ENDL
                + tr("Dev: ") + "<a href='mailto:" APP_EMAIL "'>" APP_DEV "</a>" T_ENDL
                "Github: <a href='" APP_GITHUB "'>" APP_GITHUB_USER_NAME "/" APP_GITHUB_REPO_NAME "</a>" T_ENDL
                ).arg(NETLizard::build_time_string());
    setText("<h2 align='center'>" APP_NAME "</h2>");
    setInformativeText(text);
    addButton(tr("Close"), QMessageBox::AcceptRole);
    setWindowTitle(QString(tr("About ")) + APP_NAME);
#undef T_ENDL
}

int AboutDialog::Show(QWidget *parent)
{
    AboutDialog dialog(parent);
    return dialog.exec();
}
