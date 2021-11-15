#ifndef _KARIN_MAINWINDOW_H
#define _KARIN_MAINWINDOW_H

#include <QMainWindow>

class BaseViewer;
class LogDialog;
class SceneTreeInfoWidget;

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

private:
    void Init();

private slots:
    void MenuActionSlot(QAction *action);
    void ToggleLogDialog();

private:
    BaseViewer * GenViewer(const QString &type);
    void CloseCurrentWidget();

private:
    LogDialog *m_logDialog;
    SceneTreeInfoWidget *m_sceneWidget;

    Q_DISABLE_COPY(MainWindow)
    
};

#endif // _KARIN_MAINWINDOW_H
