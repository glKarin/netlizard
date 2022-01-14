#ifndef _KARIN_NLNLSceneInfoWidget_H
#define _KARIN_NLNLSceneInfoWidget_H

#include <QTabWidget>
#include "nlpropformgroupwidget.h"

class NLScene;
class QTextBrowser;
class QStackedWidget;
class QSpinBox;
class QGroupBox;
class QComboBox;
class QPushButton;

class NLScenePropFormGroupWidget : public NLPropFormGroupWidget
{
    Q_OBJECT
public:
    explicit NLScenePropFormGroupWidget(QWidget *widget = 0);
    explicit NLScenePropFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLScenePropFormGroupWidget();
    virtual void SetObject(QObject *obj);

protected:
    virtual void SetObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual void CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual NLPropertyInfoList GetPropertyInfoList(QObject *obj);
    virtual void SortProperties(NLPropertyInfoList &list);
};

class NLSceneInfoWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit NLSceneInfoWidget(QWidget *parent = 0);
    virtual ~NLSceneInfoWidget();
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    
signals:
    
public slots:
    void Reset();
    void UpdateSceneInfo();

private:
    void Init();

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const NLProperty &value, int action = 0);

private:
    NLScene *m_scene;
    QTextBrowser *m_baseInfo;
    QTextBrowser *m_cameraInfo;
    QStackedWidget *m_cameraMatrix;
    QComboBox *m_matrixList;
    NLScenePropFormGroupWidget *m_settingGroupBox;

    Q_DISABLE_COPY(NLSceneInfoWidget)
    
};

#endif // _KARIN_NLNLSceneInfoWidget_H
