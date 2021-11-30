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

#define MAIN_WINDOW_INTERNAL_STATE_VERSION -1

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
    m_actorWidget(0)
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

    menuBar = new QMenuBar(this);

    const HomeCellItemMap &Map = IndexViewer::ActionMap();
    QStringList actions;
        actions << "&Resource"
        << "&3D"
        << "&Viewer"
        << "&Others"
        << "&Exit"
           ;
    Q_FOREACH(const QString &name, actions)
    {
        const HomeCellItemList &list = Map[name];
        if(list.size() == 1)
        {
            const HomeCellItem &item = list[0];
            menuItem = menuBar->addAction(item.label);
            menuItem->setData(item.data);
        }
        else
        {
            menu = menuBar->addMenu(name);
            Q_FOREACH(const HomeCellItem &item, list)
            {
                menuItem = menu->addAction(item.label);
                menuItem->setData(item.data);
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
    setWindowTitle(qApp->applicationName());
    //resize(640, 480);
    MenuActionSlot(0);

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
    SetStatusText();
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
    else if(type == "reset")
    {
        Reset();
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

    if(scene)
    {
        m_sceneTreeWidget->SetScene(scene);
        m_sceneInfoWidget->SetScene(scene);
    }
    if(!m_sceneTreeWidget->isVisible())
    {
        //m_sceneTreeWidget->setFloating(false);
        m_sceneTreeWidget->setVisible(true);
    }
    if(!m_sceneInfoWidget->isVisible())
    {
        //m_sceneInfoWidget->setFloating(false);
        m_sceneInfoWidget->setVisible(true);
    }
    if(!m_actorWidget->isVisible())
    {
        m_actorWidget->Reset();
        //m_actorWidget->setFloating(false);
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
    QStatusBar *bar = statusBar();
    bar->showMessage(str);
    bar->setVisible(!str.isEmpty());
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

