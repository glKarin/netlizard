#ifndef _KARIN_NLACTORWIDGET_H
#define _KARIN_NLACTORWIDGET_H

#include <QScrollArea>

#include "nldef.h"
#include "nlproperties.h"

class NLActor;
class NLObject;
class QFormLayout;
class QVBoxLayout;
class QDoubleSpinBox;

class NLVector3Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NLVector3Widget(QWidget *widget = 0);
    virtual ~NLVector3Widget();
    NLVector3 Vector3();

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

class NLActorWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit NLActorWidget(QWidget *widget = 0);
    virtual ~NLActorWidget();
    NLActor * Actor();
    void SetActor(NLActor *sctor);

public Q_SLOTS:
    void Reset();

Q_SIGNALS:

protected:
    void UpdateActorData();

private:
    void Init();
    QWidget * GenWidget(NLObject *obj, const NLPropertyInfo &prop);
    void SetupActorProperty();
    void SetupComponentProperty();
    void SortProperties(NLPropertyInfoList &list);

private:
    NLActor *m_actor;
    QFormLayout *m_actorLayout;
    QVBoxLayout *m_componentLayout;

    Q_DISABLE_COPY(NLActorWidget)
};

#endif // _KARIN_NLACTORWIDGET_H
