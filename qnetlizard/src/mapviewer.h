#ifndef _KARIN_MAPVIEWER_H
#define _KARIN_MAPVIEWER_H

#include "baseviewer.h"

class QComboBox;
class QFileDialog;
class QSpinBox;
class QString;
class MapScene;
class QPushButton;
class QCheckBox;

class MapViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit MapViewer(QWidget *parent = 0);
    virtual ~MapViewer();
    
signals:
    
public slots:
    virtual void Reset();

private:
    void Init();

private Q_SLOTS:
    void OpenFileChooser();
    void OpenResourceDirChooser();
    bool OpenFile();
    void SetGameLevels(int index);
    void SetLvlFile(const QString &file);
    void SetResourceDirPath(const QString &file);
    void UpdateSceneInfo();
    void SetAutoscanLevel(bool b);
    void SetAutoscanResourcePath(bool b);

private:
    MapScene *m_mapScene;
    QComboBox *m_gameComboBox;
    QFileDialog *m_fileChooser;
    QFileDialog *m_resourceDirChooser;
    QComboBox *m_levelSpinBox;
    QPushButton *m_openLvlButton;
    QPushButton *m_openResourcePathButton;
    QString m_lvlPath;
    QString m_resourceDirPath;
    bool m_autoscanLevel;
    QCheckBox *m_autoscanLevelCheckBox;
    bool m_autoscanResource;
    QCheckBox *m_autoscanResourceCheckBox;

    Q_DISABLE_COPY(MapViewer)
    
};

#endif // _KARIN_MAPVIEWER_H
