#include "mainwindow.h"

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "imageviewer.h"
#include "textviewer.h"
#include "stringviewer.h"
#include "qdef.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("MainWindow");
    Init();
}

MainWindow::~MainWindow()
{
    DEBUG_DESTROY_Q;
}

void MainWindow::Init()
{
    QMenuBar *menuBar;
    QMenu *menu;
    QAction *menuItem;

    menuBar = new QMenuBar(this);
    menu = menuBar->addMenu("&Viewer");
    menuItem = menu->addAction("&Text viewer");
    menuItem->setData("text_viewer");
    menuItem = menu->addAction("&String viewer");
    menuItem->setData("string_viewer");
    menuItem = menu->addAction("&Image viewer");
    menuItem->setData("image_viewer");
    menuItem = menu->addAction("&Close");
    menuItem->setData("close_viewer");

    menu = menuBar->addMenu("&Other");
    menuItem = menu->addAction("&About");
    menuItem->setData("about");

    menuItem = menuBar->addAction("&Exit");
    menuItem->setData("exit");

    connect(menuBar, SIGNAL(triggered(QAction *)), this, SLOT(MenuActionSlot(QAction *)));

    setMenuBar(menuBar);
    resize(480, 360);
}

void MainWindow::MenuActionSlot(QAction *action)
{
    QString type = action->data().toString();
    if(type == "text_viewer")
        setCentralWidget(new TextViewer(this));
    else if(type == "string_viewer")
        setCentralWidget(new StringViewer(this));
    else if(type == "image_viewer")
        setCentralWidget(new ImageViewer(this));
    else if(type == "exit")
        qApp->quit();
    else if(type == "about")
        qDebug() << "TODO: about";
    else
        setCentralWidget(0);
}
