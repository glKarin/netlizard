#ifndef _KARIN_NLACTORPROPWIDGET_H
#define _KARIN_NLACTORPROPWIDGET_H

#include <QScrollArea>

#include "nldef.h"
#include "nlproperties.h"

class NLActor;
class NLObject;
class QFormLayout;
class QVBoxLayout;
class QMenu;
class QGroupBox;
class QLabel;
class NLComponent;
class NLScript;

class NLActorPropSectionHeader : public QWidget
{
    Q_OBJECT

public:
    explicit NLActorPropSectionHeader(QWidget *widget = 0);
    explicit NLActorPropSectionHeader(const QString &text, QWidget *widget = 0);
    virtual ~NLActorPropSectionHeader();
    void AddAction(QAction *action);

Q_SIGNALS:
    void actionTriggered(QAction *action);

public Q_SLOTS:
    void SetText(const QString &text);

private:
    void Init(const QString &text = QString());

public Q_SLOTS:
    void OnActionTriggered();

private:
    QLabel *m_label;
    QMenu *m_menu;

    Q_DISABLE_COPY(NLActorPropSectionHeader)
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
    void OnItemDestroy(QObject *item = 0);
    void SetupComponent();
    void SetupScript();

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
    void ClearComponentProperties();
    void ClearScriptProperties();

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const NLProperty &value);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);
    void OnStringChanged(const QString &str = QString());
    void OnStringReload(const QString &str = QString());
    void OnActionTriggered(QAction *action);

private:
    typedef QHash<QString, QWidget *> PropWidgetHash;
    typedef QHash<NLObject *, PropWidgetHash> ObjectPropWidgetHash;
    typedef QHash<QString, QList<NLObject *> > ObjectHash;
    NLActor *m_actor;
    QFormLayout *m_actorLayout;
    QVBoxLayout *m_componentLayout;
    QGroupBox *m_actorGroupBox;
    QVBoxLayout *m_scriptLayout;
    NLActorPropSectionHeader *m_componentSectionHeader;
    NLActorPropSectionHeader *m_scriptSectionHeader;
    ObjectPropWidgetHash m_propWidgetMap;
    ObjectHash m_objectMap;

    Q_DISABLE_COPY(NLActorPropWidget)
};

#endif // _KARIN_NLACTORPROPWIDGET_H
