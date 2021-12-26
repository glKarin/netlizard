#ifndef _KARIN_ANIMATIONVIEWER_H
#define _KARIN_ANIMATIONVIEWER_H

#include "baseviewer.h"

class QComboBox;
class QFileDialog;
class QSpinBox;
class QString;
class AnimationScene;
class QColorDialog;
class QPushButton;
class QSlider;
class QCheckBox;
class QToolButton;
class QToolBar;

class AnimationViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit AnimationViewer(QWidget *parent = 0);
    virtual ~AnimationViewer();
    virtual NLScene * Scene();

signals:

public slots:
    virtual void Reset();

private:
    void Init();
    void UpdatePlayState(bool b);

private Q_SLOTS:
    void OpenObjFileChooser();
    void OpenResourceDirChooser();
    void OpenBackgroundColorChooser();
    bool OpenFile();
    void SetObjFile(const QString &file);
    void SetResourceDirPath(const QString &file);
    void SetBackgroundColor(const QColor &color);
    void OnAnimChanged(int anim);
    void NextFrame();
    void PrevFrame();
    void OnPlaying();
    void OnStopped();
    void SetAutoscanIndex(bool b);
    void SetAutoscanResourcePath(bool b);

private:
    AnimationScene *m_animationScene;
    QComboBox *m_gameComboBox;
    QFileDialog *m_fileChooser;
    QFileDialog *m_resourceDirChooser;
    QSpinBox *m_indexSpinBox;
    QColorDialog *m_colorChooser;
    QPushButton *m_openObjButton;
    QPushButton *m_openResourcePathButton;
    QComboBox *m_animComboBox;
    QSlider *m_frameSlider;
    QToolButton *m_playButton;
    QSpinBox *m_animFPSSpinBox;
    QCheckBox *m_playSeqCheckBox;
    QToolBar *m_toolbar;
    QString m_objPath;
    QString m_resourceDirPath;
    bool m_autoscanIndex;
    QCheckBox *m_autoscanIndexCheckBox;
    bool m_autoscanResource;
    QCheckBox *m_autoscanResourceCheckBox;

    Q_DISABLE_COPY(AnimationViewer)
};

#endif // _KARIN_ANIMATIONVIEWER_H
