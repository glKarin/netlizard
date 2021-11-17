#include "mainwindow.h"

#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QResizeEvent>
#include <QMoveEvent>

#include <QMenuBar>
#include <QToolBar>

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
#include "scenetreeinfowidget.h"
#include "actorpropertywidget.h"

#ifdef _DEV_TEST
#include "testviewer.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_toolBar(0),
    m_logDialog(0),
    m_sceneWidget(0),
    m_actorWidget(0)
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
    MenuActionSlot(0);
}

void MainWindow::CloseCurrentWidget()
{
    BaseViewer *viewer = dynamic_cast<BaseViewer *>(centralWidget());
    if(viewer)
    {
    }
    if(m_toolBar)
    {
        removeToolBar(m_toolBar);
        m_toolBar->clear();
        delete m_toolBar;
        m_toolBar = 0;
    }
    if(m_sceneWidget)
        m_sceneWidget->Reset();
    if(m_actorWidget)
        m_actorWidget->Reset();
}

void MainWindow::MenuActionSlot(QAction *action)
{
    CloseCurrentWidget();

    if(!action)
    {
        BaseViewer *viewer = GenViewer("home_viewer");
        setCentralWidget(viewer);
        return;
    }

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
    else if(type == "changelog")
    {
        ChangelogDialog::Show(this);
    }
    else if(type == "scene")
    {
        OpenSceneEditor();
    }
    else
    {
        BaseViewer *viewer = GenViewer(type);
        setCentralWidget(viewer);
        if(viewer->ToolsCount())
        {
            m_toolBar = new QToolBar(this);
            viewer->SetupToolBar(m_toolBar);
            addToolBar(m_toolBar);
        }
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
        //setCentralWidget(viewer);
        connect(viewer, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
        setWindowTitle(viewer->Title());
        NLScene *scene = dynamic_cast<NLScene *>(viewer->CentralWidget());
        if(scene && m_sceneWidget)
            m_sceneWidget->SetScene(scene);
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
        addDockWidget(Qt::BottomDockWidgetArea, m_logDialog, Qt::Horizontal);
    }
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
    return dynamic_cast<BaseViewer *>(w);
}

void MainWindow::OpenSceneEditor()
{
    BaseViewer *viewer = CentralViewer();
    NLScene *scene = viewer ? dynamic_cast<NLScene *>(viewer->CentralWidget()) : 0;
    if(!m_sceneWidget)
    {
        m_sceneWidget = new SceneTreeInfoWidget(this);
        if(scene)
            m_sceneWidget->SetScene(scene);
        addDockWidget(Qt::RightDockWidgetArea, m_sceneWidget, Qt::Vertical);
    }
    if(!m_actorWidget)
    {
        m_actorWidget = new ActorPropertyWidget;
        addDockWidget(Qt::RightDockWidgetArea, m_actorWidget, Qt::Vertical);
        connect(m_sceneWidget, SIGNAL(actorSelected(NLActor *)), m_actorWidget, SLOT(SetActor(NLActor *)));
    }
    if(!m_sceneWidget->isVisible())
    {
        if(scene)
            m_sceneWidget->SetScene(scene);
        m_sceneWidget->setFloating(false);
        m_sceneWidget->setVisible(true);
    }
    if(!m_actorWidget->isVisible())
    {
        m_actorWidget->Reset();
        m_actorWidget->setFloating(false);
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

//void MainWindow::SetupSceneDialog()
//{
//    if(!m_sceneDialog)
//        return;
//    m_sceneDialog->UpdateSceneInfo();
//    if(!m_sceneDialog->isVisible())
//        return;
//    BaseViewer *viewer = dynamic_cast<BaseViewer *>(centralWidget());
//    if(viewer)
//    {
//        NLScene *scene = dynamic_cast<NLScene *>(viewer->CentralWidget());
//        m_sceneDialog->SetScene(scene);
//    }
//}
