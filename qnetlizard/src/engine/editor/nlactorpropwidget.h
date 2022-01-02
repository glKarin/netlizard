#ifndef _KARIN_NLACTORPROPWIDGET_H
#define _KARIN_NLACTORPROPWIDGET_H

#include <QScrollArea>

#include "nldef.h"
#include "nlproperties.h"

class NLActor;
class NLObject;
class QFormLayout;
class QVBoxLayout;
class QGroupBox;
class NLComponent;
class NLScript;

class NLActorPropWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit NLActorPropWidget(QWidget *widget = 0);
    virtual ~NLActorPropWidget();
    NLActor * Actor() { return m_actor; }
    void SetActor(NLActor *sctor);

public Q_SLOTS:
    void Reset();

protected:
    void UpdateActorData();

private Q_SLOTS:
    void OnActorChanged();
    void ToggleGroupBox(bool on);
    void OnItemDestroy(QObject *item = 0);

private:
    void Init();
    QWidget * GenWidget(NLObject *obj, const NLPropertyInfo &prop);
    void SetupActorProperty();
    void SetupComponentProperties();
    void SetupComponentProperty(NLComponent *comp);
    void SortProperties(NLPropertyInfoList &list);
    void ClearSection(QGroupBox *groupBox);
    void SetupScriptProperties();
    void SetupScriptProperty(NLScript *script);

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const NLProperty &value);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);
    void OnStringChanged(const QString &str = QString());
    void OnStringReload(const QString &str = QString());

private:
    typedef QHash<QString, QWidget *> PropWidgetHash;
    typedef QHash<NLObject *, PropWidgetHash> ComponentPropWidgetHash;
    NLActor *m_actor;
    QFormLayout *m_actorLayout;
    QVBoxLayout *m_componentLayout;
    QGroupBox *m_actorGroupBox;
    ComponentPropWidgetHash m_propWidgetMap;
    QVBoxLayout *m_scriptLayout;

    Q_DISABLE_COPY(NLActorPropWidget)
};

#endif // _KARIN_NLACTORPROPWIDGET_H
