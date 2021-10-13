#include "mainwindow.h"

#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QResizeEvent>
#include <QMoveEvent>

#include <QMenuBar>

#include "imageviewer.h"
#include "textviewer.h"
#include "stringviewer.h"
#include "mapviewer.h"
#include "fontviewer.h"
#include "itemviewer.h"
#include "helpdialog.h"
#include "aboutdialog.h"
#include "logdialog.h"
#include "qdef.h"
#include "settings.h"
#include "settingdialog.h"

#ifdef _DEV_TEST
#include "testviewer.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_logDialog(0)
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
    menuItem = menu->addAction("&Font viewer");
    menuItem->setData("font_viewer");

    menu = menuBar->addMenu("&3D");
    menuItem = menu->addAction("&Map viewer");
    menuItem->setData("map_viewer");
    menuItem = menu->addAction("&Item viewer");
    menuItem->setData("item_viewer");
    menuItem = menu->addAction("&Character animation viewer");
    menuItem->setData("character_animation_viewer");
    menuItem = menu->addAction("&Sprite viewer");
    menuItem->setData("sprite_viewer");

    menu = menuBar->addMenu("&View");

#ifdef _DEV_TEST
    menuItem = menu->addAction("&Test");
    menuItem->setData("test");
#endif

    menuItem = menu->addAction("&Log");
    menuItem->setData("log");
    menuItem = menu->addAction("&Close");
    menuItem->setData("close");
    menuItem->setData("close_viewer");

    menu = menuBar->addMenu("&Other");
    menuItem = menu->addAction("&Setting");
    menuItem->setData("setting");
    menuItem = menu->addAction("&About");
    menuItem->setData("about");
    menuItem = menu->addAction("&Help");
    menuItem->setData("help");

    menuItem = menuBar->addAction("&Exit");
    menuItem->setData("exit");

    connect(menuBar, SIGNAL(triggered(QAction *)), this, SLOT(MenuActionSlot(QAction *)));

    setMenuBar(menuBar);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    int x = settings->GetSetting("WINDOW/x", 0);
    int y = settings->GetSetting("WINDOW/y", 0);
    move(x, y);
    int w = settings->GetSetting("WINDOW/width", 480);
    int h = settings->GetSetting("WINDOW/height", 360);
    resize(w, h);
    setWindowTitle(qApp->applicationName());
    //resize(640, 480);
}

void MainWindow::MenuActionSlot(QAction *action)
{
    QString type = action->data().toString();

    if(type == "exit")
        qApp->quit();
    else if(type == "log")
    {
        ToggleLogDialog();
    }
    else if(type == "help")
    {
        HelpDialog::Show(this);
    }
    else if(type == "about")
    {
        AboutDialog::Show(this);
    }
    else if(type == "setting")
    {
        SettingDialog::Show(this);
    }
    else
    {
        BaseViewer *viewer = GenViewer(type);
        setCentralWidget(viewer);
    }
}

BaseViewer * MainWindow::GenViewer(const QString &type)
{
    BaseViewer *viewer = 0;
    if(type == "text_viewer")
        viewer = new TextViewer;
    else if(type == "string_viewer")
        viewer = new StringViewer;
    else if(type == "image_viewer")
        viewer = new ImageViewer;
    else if(type == "font_viewer")
        viewer = new FontViewer;

    else if(type == "map_viewer")
        viewer = new MapViewer;
    else if(type == "item_viewer")
        viewer = new ItemViewer;
    else if(type == "character_animation_viewer")
        ;
    else if(type == "sprite_viewer")
        ;

#ifdef _DEV_TEST
    else if(type == "test")
        viewer = new TestViewer;
#endif

    if(viewer)
    {
        setCentralWidget(viewer);
        connect(viewer, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
        setWindowTitle(viewer->Title());
    }
    else
    {
        setWindowTitle(qApp->applicationName());
    }
    return viewer;
}

void MainWindow::ToggleLogDialog()
{
    if(!m_logDialog)
    {
        m_logDialog = new LogDialog(this);
    }
    if(m_logDialog->isVisible())
        m_logDialog->hide();
    else
    {
        m_logDialog->ResetPosAndSize();
        m_logDialog->show();
        setFocus(Qt::MouseFocusReason);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if(m_logDialog && m_logDialog->isVisible())
    {
        m_logDialog->ResetPosAndSize();
    }
    if(isVisible())
    {
        Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
        QSize size = event->size();
        settings->SetSetting("WINDOW/width", size.width());
        settings->SetSetting("WINDOW/height", size.height());
    }
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
    if(m_logDialog && m_logDialog->isVisible())
    {
        m_logDialog->ResetPosAndSize();
    }
    if(isVisible())
    {
        Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
        QPoint pos = event->pos();
        settings->SetSetting("WINDOW/x", pos.x());
        settings->SetSetting("WINDOW/y", pos.y());
    }
}
