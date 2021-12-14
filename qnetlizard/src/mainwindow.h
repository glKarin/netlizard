#ifndef _KARIN_MAINWINDOW_H
#define _KARIN_MAINWINDOW_H

#include <QMainWindow>

class QToolBar;
class QStackedWidget;
class BaseViewer;
class LogDialog;
class SceneTreeWidget;
class SceneInfoWidget;
class ActorPropertyWidget;
class NLScene;
class StatusBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    
signals:
    
public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void closeEvent(QCloseEvent *event);
    BaseViewer * CentralViewer();

private:
    void Init();
    NLScene * GetViewerScene(BaseViewer *viewer = 0);
    void Reset();

private slots:
    void MenuActionSlot(QAction *action);
    void ToggleLogDialog();
    void OpenSceneEditor();
    void SetLabelTitleText(const QString &str);
    void SetStatusText(const QString &str);

private:
    BaseViewer * GenViewer(const QString &type);
    void CloseCurrentWidget();
    void SetCurrentWidget(BaseViewer *viewer = 0);
    BaseViewer * CurrentWidget();

private:
    QByteArray m_state;
    QToolBar *m_toolBar;
    QStackedWidget *m_centralWidget;
    LogDialog *m_logDialog;
    SceneTreeWidget *m_sceneTreeWidget;
    SceneInfoWidget *m_sceneInfoWidget;
    ActorPropertyWidget *m_actorWidget;
    StatusBar *m_statusBar;

    Q_DISABLE_COPY(MainWindow)
    
};

#endif // _KARIN_MAINWINDOW_H
