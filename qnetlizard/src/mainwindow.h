#ifndef _KARIN_MAINWINDOW_H
#define _KARIN_MAINWINDOW_H

#include <QMainWindow>

class BaseViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    
signals:
    
public slots:

private:
    void Init();

private slots:
    void MenuActionSlot(QAction *action);

private:
    BaseViewer * GenViewer(const QString &type);

    Q_DISABLE_COPY(MainWindow)
    
};

#endif // _KARIN_MAINWINDOW_H
