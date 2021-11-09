#ifndef _KARIN_SCENEDIALOG_H
#define _KARIN_SCENEDIALOG_H

#include <QDialog>

class QTextBrowser;
class NLScene;

class SceneDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SceneDialog(QWidget *parent = 0);
    virtual ~SceneDialog();
    void SetScene(NLScene *scene);
    NLScene * Scene();
    
signals:
    
public slots:
    void ResetPosAndSize();
    void UpdateSceneInfo();

private:
    void Init();

private slots:
    void Reset();

private:
    NLScene *m_scene;
    QTextBrowser *m_textViewer;

    Q_DISABLE_COPY(SceneDialog)
    
};

#endif // _KARIN_SCENEDIALOG_H
