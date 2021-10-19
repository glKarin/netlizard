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
               << "Game file"
          << "3D viewer"
          << "Image viewer"
          << "Coordinate"
             ;

    list << "1. This application is not support open/read jar/zip archive file directly, so please extract jar/zip archive first."
         << "2. The `" _B(_I("Resource path")) "` file choose is root of jar/zip archive extracted(has a named `META-INF` folder) in all viewer.";
    m_contentText.push_back(list);

    list.clear();
    list << "1. " _B(_F("3D scene map model", "#FF0000")) " file name(* is a digit): "
         << _B("lv*.png") " - CT 3D, Spacnaz 3D, CT-Ep2 3D, Egypt 3D(main menu scene is lv0.png), Clone 3D, CT-Ep3 3D"
            << _B("dm.png") " - Egypt 3D(servive mode)"
            << _B("track*.png") " - Racing 3D"
            << ""
            << "2. " _B(_F("3D item model", "#FF0000")) " file name(* is a digit): "
            << _B("obj/obj*.png") " - CT 3D"
            << _B("o/o*.png") " - Spacnaz 3D, CT-Ep2 3D, Egypt 3D, Clone 3D, CT-Ep3 3D"
            << _B("car*.png") " - Racing 3D(car), Set `index` to a number for changing car skin texture image(file name is `car*_tex.png`) in `Item viewer`."
            << ""
            << "3. " _B(_F("3D model texture", "#FF0000")) " file name(* is a digit): "
            << _B("tex/l*.png") " - CT 3D(texture v2)"
            << _B("w/l*.png") " - CT-Ep2 3D(texture v2), CT-Ep3 3D(texture v3)"
            << _B("w/w*.png") " - Spacnaz 3D(texture v2), Egypt 3D(texture v3), Clone 3D(texture v3)"
            << _B("map*.png") " - Racing 3D map scene(encode PNG)."
            << _B("car*_tex.png") " - Racing 3D car(encode PNG). Set `index` to a number for changing car skin texture image in `Item viewer`."
            << ""
            << "4. " _B(_F("Font", "#FF0000")) " file name: "
            << _B("cf.png") " - Config of font"
            << _B("fnt.png") " - Font texture image"
            << ""
            << "5. " _B(_F("Text", "#FF0000")) " file name(need `Russian` language environment): "
            << _B("ab.png") " - `About` menu text"
            << _B("hl.png") " - `Help` menu text"
               ;
    m_contentText.push_back(list);

    list.clear();
    list << _B(_F("Key map: ", "#FF0000"))
         << _I("Move action: ")
         << _B("W") " - Move forward"
         << _B("S") " - Move backward"
         << _B("A") " - Move left"
         << _B("D") " - Move right"
         << _B("Q / Control") " - Move down"
         << _B("E / Space") " - Move up"
         << ""
         << _I("Turn action: ")
         << _B("Up") " - Turn up"
         << _B("Down") " - Turn down"
         << _B("Left") " - Turn left"
         << _B("Right") " - Turn right"
         << ""
         << _I("Roll action: ")
         << _B("Z") " - Roll left"
         << _B("C") " - Roll right"
         << _B("X") " - Roll to 0"
         << ""
         << _I("Other action: ")
         << _B("M") " - Grab mouse pointer(If pointer is hidden, moving mouse for turning)"
         << _B("Esc") " - Ungrab mouse pointer"
            ;
    m_contentText.push_back(list);

    list.clear();
    list << _B(_F("Key map: ", "#FF0000"))
         << _I("Translate action: ")
         << _B("Up") " - Move up"
         << _B("Down") " - Move down"
         << _B("Left") " - Move left"
         << _B("Right") " - Move right"
         << ""
         << _I("Rotate action: ")
         << _B(",") " - Rotate left"
         << _B(".") " - Rotate right"
         << _B("/") " - Rotate to 0"
         << ""
         << _I("Zoom action: ")
         << _B("+ / Wheel up") " - Zoom out"
         << _B("- / Wheel down") " - Zoom in"
         << ""
         << _I("Other action: ")
         << _B("Esc") " - All reset"
            ;
    m_contentText.push_back(list);

    list.clear();
    list << _F("All 3D coordinate system is base " _B(_I("Right-Hand")) ", like " _B(_I("OpenGL")) ".", "#FF0000")
         << ""
         << _B("CT 3D, Spacnaz 3D, CT-Ep2 3D, Egypt 3D, Clone 3D, CT-Ep3 3D") ": (Z-axis is up-down direction. Roll clockwise 90 degree in OpenGL coordinate system (Rotate -90 degree with vector[1, 0, 0] in OpenGL))"
         << _B("Right") " - positive X"
         << _B("Up") " - positive Z"
         << _B("Far") " - positive Y"
         << ""
         << _B("Racing 3D") ": (Same as OpenGL)"
         << _B("Right") " - positive X"
         << _B("Up") " - positive Y"
         << _B("Far") " - nagative Z"
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
    resize(480, 360);
}

void HelpDialog::SetContentText(QListWidgetItem *item)
{
    QString label = item->text();
    int index = m_menuText.indexOf(label);
    //m_textViewer->setText(m_contentText.at(index).join("\n"));
    m_textViewer->setHtml(m_contentText.at(index).join("<br/>"));
}

int HelpDialog::Show(QWidget *parent)
{
    HelpDialog dialog(parent);
    return dialog.exec();
}
