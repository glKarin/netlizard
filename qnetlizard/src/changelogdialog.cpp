#include "changelogdialog.h"

#include <QDebug>

#include <QLabel>
#include <QStringList>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

#include "qdef.h"

struct Changelog
{
    QString state;
    QString version;
    QString urgency;
    QStringList items;
    QDateTime time;

    Changelog(const QString &state, const QString &version, const QString &urgency, const QDateTime &time, const QStringList &items = QStringList())
        : state(state),
          version(version),
          urgency(urgency),
          items(items),
          time(time)
    {
    }

    Changelog & operator<<(const QString &item)
    {
        items.push_back(item);
        return *this;
    }

    QString toString() const
    {
        QString item;
        for(int i = 0; i < items.size(); i++)
        {
            item += "  * " + items[i];
            if(i < items.size() - 1)
                item += "\n";
        }
        return QString("%1 (%2) %3; urgency=%4\n"
                "\n"
                "%5"
                "\n\n"
                " -- %6 <%7>  %8 %9"
                )
                .arg(APP_NAME)
                .arg(APP_VER)
                .arg(state)
                .arg(urgency)
                .arg(item)
                .arg(APP_DEV)
                .arg(APP_EMAIL)
                .arg(TimeToEnglish())
                .arg(BUILD_TIMEZONE)
                ;
    }
    QString TimeToEnglish() const
    {
        QLocale locale(QLocale::English);
        return locale.toString(time, "ddd, dd MMM yyyy HH:mm:ss");
    }
};

typedef QList<Changelog> ChangelogList;

static ChangelogList get_changelog_list()
{
    ChangelogList list;
    list
            << (Changelog("unstable", APP_VER, "low", BUILD_TIME) << "Initial Release.")
               ;
    return list;
}

ChangelogDialog::ChangelogDialog(QWidget *parent) :
    QDialog(parent)
{
    setObjectName("ChangelogDialog");

    if(!LoadChangelog())
    {
        ChangelogList list = get_changelog_list();
        QStringList strList;
        Q_FOREACH(const Changelog &cl, list)
            strList.push_back(cl.toString());
        m_text = strList.join("\n\n");
    }
    Init();
}

ChangelogDialog::~ChangelogDialog()
{
    DEBUG_DESTROY_Q
}

void ChangelogDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QTextBrowser *textViewer = new QTextBrowser;
    QLabel *label = new QLabel;

    textViewer->setText(m_text);

    label->setAlignment(Qt::AlignCenter);
    label->setText(QString("%1 (%2) - %3").arg(APP_NAME).arg(APP_VER).arg(APP_DEV));
    layout->addWidget(label);
    layout->addWidget(textViewer, 1);

    setLayout(layout);

    setWindowTitle(tr("Changelog"));
    resize(480, 360);
}

int ChangelogDialog::Show(QWidget *parent)
{
    ChangelogDialog dialog(parent);
    return dialog.exec();
}

bool ChangelogDialog::LoadChangelog()
{
    const QString ChangelogFile(":/CHANGELOG");

    QFile f(ChangelogFile);
    if(!f.exists())
        return false;
    if(!f.open(QIODevice::ReadOnly))
        return false;
    QTextStream is(&f);
    m_text = is.readAll();
    f.close();
    return true;
}
