#ifndef _KARIN_NLACTORPROPWIDGET_H
#define _KARIN_NLACTORPROPWIDGET_H

#include <QScrollArea>

#include "nldef.h"
#include "nlproperties.h"

class NLActor;
class NLObject;
class QFormLayout;
class QVBoxLayout;
class QDoubleSpinBox;
class QGroupBox;
class NLComponent;

class NLVector3Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NLVector3Widget(QWidget *widget = 0);
    virtual ~NLVector3Widget();
    NLVector3 Vector3() const { return m_vector3; }

public Q_SLOTS:
    void SetVector3(const NLVector3 &v);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void vector3Changed(const NLVector3 &vector);

private Q_SLOTS:
    void OnValueChanged(double d);

private:
    void Init();
    void UpdateWidget();

private:
    NLVector3 m_vector3;
    QDoubleSpinBox *m_xSpinBox;
    QDoubleSpinBox *m_ySpinBox;
    QDoubleSpinBox *m_zSpinBox;

    Q_DISABLE_COPY(NLVector3Widget)
};

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

private:
    void Init();
    QWidget * GenWidget(NLObject *obj, const NLPropertyInfo &prop);
    void SetupActorProperty();
    void SetupComponentProperties();
    void SetupComponentProperty(NLComponent *comp);
    void SortProperties(NLPropertyInfoList &list);

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const NLProperty &value);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);

private:
    typedef QHash<QString, QWidget *> PropWidgetHash;
    typedef QHash<NLObject *, PropWidgetHash> ComponentPropWidgetHash;
    NLActor *m_actor;
    QFormLayout *m_actorLayout;
    QVBoxLayout *m_componentLayout;
    QGroupBox *m_actorGroupBox;
    ComponentPropWidgetHash m_propWidgetMap;

    Q_DISABLE_COPY(NLActorPropWidget)
};

#endif // _KARIN_NLACTORPROPWIDGET_H
