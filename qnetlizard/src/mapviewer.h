#ifndef _KARIN_MAPVIEWER_H
#define _KARIN_MAPVIEWER_H

#include "baseviewer.h"

class QComboBox;
class QFileDialog;
class QSpinBox;
class QString;
class MapScene;
class QPushButton;

class MapViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit MapViewer(QWidget *parent = 0);
    virtual ~MapViewer();
    
signals:
    
public slots:

private:
    void Init();

private Q_SLOTS:
    void OpenFileChooser();
    void OpenResourceDirChooser();
    bool OpenFile();
    void OnTypeCurrentIndexChanged(int index);
    void SetLvlFile(const QString &file);
    void SetResourceDirPath(const QString &file);
    void UpdateSceneInfo();

private:
    MapScene *m_mapScene;
    QComboBox *m_gameComboBox;
    QFileDialog *m_fileChooser;
    QFileDialog *m_resourceDirChooser;
    QSpinBox *m_levelSpinBox;
    QPushButton *m_openLvlButton;
    QPushButton *m_openResourcePathButton;
    QString m_lvlPath;
    QString m_resourceDirPath;

    Q_DISABLE_COPY(MapViewer)
    
};

#endif // _KARIN_MAPVIEWER_H
