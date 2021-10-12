#include "helpdialog.h"

#include <QDebug>

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>

#include "qdef.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    m_textViewer(0)
{
    setObjectName("HelpDialog");
    QStringList list;

    m_menuText << "General"
          << "3D viewer"
          << "Image viewer"
             ;

    list << "1. This application is not support open/read jar/zip archive file directly, so please extract jar/zip archive first."
            << "2. The `resource path` file choose is root of jar/zip archive extracted(has a named `META-INF` folder) in all viewer.";
    m_contentText.push_back(list);

    list.clear();
    list << "Key map:"
         << "Move action: "
         << "W - Move forward"
         << "S - Move backward"
         << "A - Move left"
         << "D - Move right"
         << "Q / Control - Move down"
         << "E / Space - Move up"
         << ""
         << "Turn action: "
         << "Up - Turn up"
         << "Down - Turn down"
         << "Left - Turn left"
         << "Right - Turn right"
         << ""
         << "Roll action: "
         << "Z - Roll left"
         << "C - Roll right"
         << "X - Roll to 0"
         << ""
         << "Function action: "
         << "M - Grab mouse pointer(If pointer is hidden, moving mouse for turning)"
         << "Esc - Ungrab mouse pointer"
            ;
    m_contentText.push_back(list);

    list.clear();
    list << "Key map:"
         << "Translate action: "
         << "Up - Move up"
         << "Down - Move down"
         << "Left - Move left"
         << "Right - Move right"
         << ""
         << "Zoom action: "
         << "+ / Wheel up - Zoom out"
         << "- / Wheel down - Zoom in"
            ;
    m_contentText.push_back(list);

    Init();
}

HelpDialog::~HelpDialog()
{
    DEBUG_DESTROY_Q
}

void HelpDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *contentLayout = new QHBoxLayout;
    QListWidget *list = new QListWidget;
    m_textViewer = new QTextBrowser;

    list->addItems(m_menuText);
    list->setMaximumWidth(128);
    list->setMinimumWidth(96);
    contentLayout->addWidget(list);
    contentLayout->addWidget(m_textViewer);

    layout->addLayout(contentLayout);

    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetContentText(QListWidgetItem *)));

    setLayout(layout);

    SetContentText(list->item(0));
    list->setCurrentRow(0);

    setWindowTitle("Help");
    resize(360, 320);
}

void HelpDialog::SetContentText(QListWidgetItem *item)
{
    QString label = item->text();
    int index = m_menuText.indexOf(label);
    m_textViewer->setText(m_contentText.at(index).join("\n"));
}

int HelpDialog::Show(QWidget *parent)
{
    HelpDialog dialog(parent);
    return dialog.exec();
}
