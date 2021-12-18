#include "mainwindow.h"

#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QStackedWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QCloseEvent>

#include "imageviewer.h"
#include "textviewer.h"
#include "stringviewer.h"
#include "mapviewer.h"
#include "fontviewer.h"
#include "itemviewer.h"
#include "animationviewer.h"
#include "indexviewer.h"
#include "spriteviewer.h"
#include "helpdialog.h"
#include "aboutdialog.h"
#include "logdialog.h"
#include "qdef.h"
#include "settings.h"
#include "settingdialog.h"
#include "changelogdialog.h"
#include "nlscene.h"
#include "sceneinfowidget.h"
#include "scenetreewidget.h"
#include "actorpropertywidget.h"
#include "statusbar.h"

#define MAIN_WINDOW_INTERNAL_STATE_VERSION -1

#define MAINWINDOW_MINIMIZE 0
#define MAINWINDOW_RESTORE 1
#define MAINWINDOW_MAXIMIZE 2

#ifdef _DEV_TEST
#include "testviewer.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_toolBar(0),
    m_centralWidget(0),
    m_logDialog(0),
    m_sceneTreeWidget(0),
    m_sceneInfoWidget(0),
    m_actorWidget(0),
    m_statusBar(0),
    m_trayIcon(0)
{
    setObjectName("MainWindow");
    Init();
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    restoreGeometry(settings->GetSetting<QByteArray>("WINDOW/geometry"));
    restoreState(settings->GetSetting<QByteArray>("WINDOW/state"));
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
    m_toolBar = new QToolBar(this);
    m_centralWidget = new QStackedWidget(this);
    m_toolBar->setObjectName("QToolBar_mainToolBar");
    m_statusBar = new StatusBar;
    menuBar = new QMenuBar(this);

    const HomeCellItemList &Map = IndexViewer::ActionMap();
    QStringList actions;
        actions << "resource"
        << "3d"
        << "viewer"
        << "others"
        << "exit"
           ;
    Q_FOREACH(const QString &name, actions)
    {
        Q_FOREACH(const HomeCellItem &item, Map)
        {
#ifndef _DEV_TEST
            if(item.IsOnlyShowInDebug())
                continue;
#endif
            if(item.data.toString() == name)
            {
                if(item.IsItem())
                {
                    menuItem = menuBar->addAction(item.label);
                    menuItem->setData(item.data);
                }
                else
                {
                    menu = menuBar->addMenu(item.label);
                    AddMenuItem(item, menu);
                }
                break;
            }
        }
    }

    connect(menuBar, SIGNAL(triggered(QAction *)), this, SLOT(MenuActionSlot(QAction *)));
    addToolBar(m_toolBar);
    setCentralWidget(m_centralWidget);

    // dock
    m_logDialog = new LogDialog(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_logDialog, Qt::Horizontal);
    restoreDockWidget(m_logDialog);

    m_sceneTreeWidget = new SceneTreeWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_sceneTreeWidget, Qt::Vertical);

    m_sceneInfoWidget = new SceneInfoWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_sceneInfoWidget, Qt::Vertical);

    m_actorWidget = new ActorPropertyWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_actorWidget, Qt::Vertical);
    connect(m_sceneTreeWidget, SIGNAL(actorSelected(NLActor *)), m_actorWidget, SLOT(SetActor(NLActor *)));

    splitDockWidget(m_sceneTreeWidget, m_actorWidget, Qt::Horizontal);
    splitDockWidget(m_sceneTreeWidget, m_sceneInfoWidget, Qt::Vertical);

    restoreDockWidget(m_sceneTreeWidget);
    restoreDockWidget(m_sceneInfoWidget);
    restoreDockWidget(m_actorWidget);

    setMenuBar(menuBar);
//    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
//    int x = settings->GetSetting("WINDOW/x", 0);
//    int y = settings->GetSetting("WINDOW/y", 0);
//    move(x, y);
//    int w = settings->GetSetting("WINDOW/width", 480);
//    int h = settings->GetSetting("WINDOW/height", 360);
//    resize(w, h);

    setStatusBar(m_statusBar);

    setWindowTitle(qApp->applicationName());
    //resize(640, 480);
    MenuActionSlot(0);

    setWindowIcon(QIcon(":/LOGO"));

    m_state = saveState(MAIN_WINDOW_INTERNAL_STATE_VERSION);
}

void MainWindow::CloseCurrentWidget()
{
    BaseViewer *viewer = CurrentWidget();
    if(viewer)
    {
    }
    SetCurrentWidget();
    m_toolBar->clear();
    if(m_sceneTreeWidget)
        m_sceneTreeWidget->Reset();
    if(m_sceneInfoWidget)
        m_sceneInfoWidget->Reset();
    if(m_actorWidget)
        m_actorWidget->Reset();
    m_statusBar->Reset(true);
}

void MainWindow::TrayIconMenuActionSlot(QAction *action)
{
    if(action)
    {
        if(action->data().toString() == "minimize")
            return;
    }
    MenuActionSlot(action);
    if(isHidden())
        SetMainWindowState(MAINWINDOW_RESTORE);
}

void MainWindow::MenuActionSlot(QAction *action)
{
    QString type(action ? action->data().toString() : "index_viewer");
    if(type.isEmpty())
        return;

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
    else if(type == "changelog")
    {
        ChangelogDialog::Show(this);
    }
    else if(type == "scene")
    {
        OpenSceneEditor();
    }
    else if(type == "minimize")
    {
        SetMainWindowState(MAINWINDOW_MINIMIZE);
    }
    else if(type == "maximize")
    {
        SetMainWindowState(MAINWINDOW_MAXIMIZE);
    }
    else if(type == "restore")
    {
        SetMainWindowState(MAINWINDOW_RESTORE);
    }
    else if(type == "changelog")
    {
        ChangelogDialog::Show(this);
    }
    else
    {
        CloseCurrentWidget();
        BaseViewer *viewer = GenViewer(type);
        SetCurrentWidget(viewer);
        NLScene *scene = GetViewerScene();
        if(scene)
        {
            if(m_sceneTreeWidget)
                m_sceneTreeWidget->SetScene(scene);
            if(m_sceneInfoWidget)
                m_sceneInfoWidget->SetScene(scene);
        }
        if(viewer->ToolsCount())
        {
            viewer->SetupToolBar(m_toolBar);
        }
    }
}

void MainWindow::SetCurrentWidget(BaseViewer *viewer)
{
    while(m_centralWidget->count())
    {
        QWidget *w = m_centralWidget->widget(0);
        m_centralWidget->removeWidget(w);
        w->deleteLater();
    }
    if(viewer)
        m_centralWidget->addWidget(viewer);
}

BaseViewer * MainWindow::CurrentWidget()
{
    QWidget *w = m_centralWidget->widget(0);
    if(w)
        return static_cast<BaseViewer *>(w);
    return 0;
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
    else if(type == "animation_viewer")
        viewer = new AnimationViewer;
    else if(type == "sprite_viewer")
        viewer = new SpriteViewer;

#ifdef _DEV_TEST
    else if(type == "test")
        viewer = new TestViewer;
#endif

    else
    {
        IndexViewer *home = new IndexViewer;
        connect(home, SIGNAL(openViewer(QAction *)), this, SLOT(MenuActionSlot(QAction*)));
        viewer = home;
    }

    if(viewer)
    {
        //SetCurrentWidget(viewer);
        connect(viewer, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
        connect(viewer, SIGNAL(statusTextChanged(const QString &)), this, SLOT(SetStatusText(const QString &)));
        connect(viewer, SIGNAL(labelTextChanged(const QString &)), this, SLOT(SetLabelTitleText(const QString &)));
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
        return;
    if(m_logDialog->isVisible())
        m_logDialog->hide();
    else
    {
        //m_logDialog->ResetPosAndSize();
        m_logDialog->show();
        setFocus(Qt::MouseFocusReason);
    }
}

BaseViewer * MainWindow::CentralViewer()
{
    QWidget *w = centralWidget();
    if(!w)
        return 0;
    QStackedWidget *sw = static_cast<QStackedWidget *>(w);
    if(sw->count() == 0)
        return 0;
    return dynamic_cast<BaseViewer *>(sw->widget(0));
}

NLScene * MainWindow::GetViewerScene(BaseViewer *v)
{
    BaseViewer *viewer = v ? v : CentralViewer();
    if(!viewer)
        return 0;
    NLScene *scene = dynamic_cast<NLScene *>(viewer->CentralWidget());
    if(scene)
        return scene;
    return viewer->Scene();
}

void MainWindow::OpenSceneEditor()
{
    NLScene *scene = GetViewerScene();
    bool visible = m_sceneTreeWidget->isVisible();

    if(visible)
    {
        m_sceneTreeWidget->Reset();
        m_sceneTreeWidget->setVisible(false);
    }
    else
    {
        if(scene)
        {
            m_sceneTreeWidget->SetScene(scene);
        }
        m_sceneTreeWidget->setVisible(true);
    }

    if(visible)
    {
        m_sceneInfoWidget->Reset();
        m_sceneInfoWidget->setVisible(false);
    }
    else
    {
        if(scene)
        {
            m_sceneInfoWidget->SetScene(scene);
        }
        m_sceneInfoWidget->setVisible(true);
    }

    if(visible)
    {
        m_actorWidget->Reset();
        m_actorWidget->setVisible(false);
    }
    else
    {
        m_actorWidget->Reset();
        m_actorWidget->setVisible(true);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if(m_logDialog && m_logDialog->isVisible())
    {
        //m_logDialog->ResetPosAndSize();
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
        //m_logDialog->ResetPosAndSize();
    }
    if(isVisible())
    {
        Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
        QPoint pos = event->pos();
        settings->SetSetting("WINDOW/x", pos.x());
        settings->SetSetting("WINDOW/y", pos.y());
    }
}

void MainWindow::SetStatusText(const QString &str)
{
    bool isEmpty = str.isEmpty();
    if(isEmpty)
        m_statusBar->ClearTemporaryMessage();
    else
        m_statusBar->ShowTemporaryMessage(str, 5000);
    m_statusBar->setVisible(true);
}

void MainWindow::SetLabelTitleText(const QString &str)
{
    bool isEmpty = str.isEmpty();
    if(isEmpty)
        m_statusBar->ClearPermanentMessage();
    else
        m_statusBar->ShowPermanentMessage(str);
    m_statusBar->setVisible(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    settings->SetSetting<QByteArray>("WINDOW/geometry", saveGeometry());
    settings->SetSetting<QByteArray>("WINDOW/state", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::Reset()
{
    restoreState(m_state, MAIN_WINDOW_INTERNAL_STATE_VERSION);
}

void MainWindow::AddMenuItem(const HomeCellItem &s, QMenu *parent)
{
    Q_FOREACH(const HomeCellItem &a, s.items)
    {
        if(!a.IsValid())
            continue;

#ifndef _DEV_TEST
            if(a.IsOnlyShowInDebug())
                continue;
#endif
        if(a.IsItem())
        {
            QAction *subItem = parent->addAction(a.label);
            subItem->setData(a.data);
            subItem->setToolTip(a.description);
        }
        else
        {
            QMenu *subMenu = parent->addMenu(a.label);
            subMenu->setToolTip(a.description);
            AddMenuItem(a, subMenu);
        }
    }
}

void MainWindow::SetMainWindowState(int b)
{
    if(!QSystemTrayIcon::isSystemTrayAvailable())
        return;
    if(!m_trayIcon)
    {
        m_trayIcon = new QSystemTrayIcon(this);
        m_trayIcon->setIcon(QIcon(":/LOGO"));
        m_trayIcon->setToolTip(APP_NAME);
        QMenu *menu = new QMenu(APP_NAME, this);
        const HomeCellItemList &Map = IndexViewer::ActionMap();

        QStringList actions;
            actions << "resource"
            << "3d"
               ;
        Q_FOREACH(const QString &name, actions)
        {
            Q_FOREACH(const HomeCellItem &item, Map)
            {
    #ifndef _DEV_TEST
                if(item.IsOnlyShowInDebug())
                    continue;
    #endif
                if(item.data.toString() == name)
                {
                    if(item.IsItem())
                    {
                        menu->addAction(item.label)->setData(item.data);
                    }
                    else
                    {
                        AddMenuItem(item, menu->addMenu(item.label));
                    }
                    break;
                }
            }
        }

        menu->addAction("&" + tr("Setting"))->setData("setting");
        menu->addAction("&" + tr("Restore"))->setData("restore");
        menu->addAction("&" + tr("Maximize"))->setData("maximize");
        menu->addAction("&" + tr("Exit"))->setData("exit");
        connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(TrayIconMenuActionSlot(QAction *)));
        m_trayIcon->setContextMenu(menu);
        connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    }
    if(b == MAINWINDOW_MINIMIZE)
    {
        hide();
        m_trayIcon->show();
    }
    else
    {
        m_trayIcon->hide();
        show();
        if(b == MAINWINDOW_MAXIMIZE)
        {
            //showMaximized();
            setWindowState(Qt::WindowMaximized);
        }
    }
}

void MainWindow::OnTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
        m_trayIcon->hide();
        show();
        break;
        case QSystemTrayIcon::Context:
        case QSystemTrayIcon::Unknown:
        default:
            break;
    }
}
