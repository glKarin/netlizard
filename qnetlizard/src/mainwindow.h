#ifndef _KARIN_MAINWINDOW_H
#define _KARIN_MAINWINDOW_H

#include <QMainWindow>

class QToolBar;
class QStackedWidget;
class BaseViewer;
class LogDialog;
class SceneTreeInfoWidget;
class ActorPropertyWidget;

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
    BaseViewer * CentralViewer();

private:
    void Init();

private slots:
    void MenuActionSlot(QAction *action);
    void ToggleLogDialog();
    void OpenSceneEditor();
    void SetStatusText(const QString &str);

private:
    BaseViewer * GenViewer(const QString &type);
    void CloseCurrentWidget();
    void SetCurrentWidget(BaseViewer *viewer = 0);
    BaseViewer * CurrentWidget();

private:
    QToolBar *m_toolBar;
    QStackedWidget *m_centralWidget;
    LogDialog *m_logDialog;
    SceneTreeInfoWidget *m_sceneWidget;
    ActorPropertyWidget *m_actorWidget;

    Q_DISABLE_COPY(MainWindow)
    
};

#endif // _KARIN_MAINWINDOW_H
