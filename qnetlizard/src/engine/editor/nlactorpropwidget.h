#ifndef _KARIN_NLACTORPROPWIDGET_H
#define _KARIN_NLACTORPROPWIDGET_H

#include <QScrollArea>
#include <QGroupBox>

#include "nldef.h"
#include "nlproperties.h"

class NLActor;
class NLObject;
class QFormLayout;
class QVBoxLayout;
class QMenu;
class QToolButton;
class QGroupBox;
class QLabel;
class NLComponent;
class NLScript;

class NLPropSectionHeader : public QWidget
{
    Q_OBJECT

public:
    explicit NLPropSectionHeader(QWidget *widget = 0);
    explicit NLPropSectionHeader(const QString &text, QWidget *widget = 0);
    virtual ~NLPropSectionHeader();
    void AddAction(QAction *action);
    void Reset();
    void SetCanExpand(bool b);

Q_SIGNALS:
    void actionTriggered(QAction *action);
    void toggleExpand(bool on);

public Q_SLOTS:
    void SetText(const QString &text);

private:
    void Init(const QString &text = QString());
    void SetExpand(bool b);

public Q_SLOTS:
    void OnToggleExpand(bool on);

private:
    QLabel *m_label;
    QMenu *m_menu;
    QToolButton *m_expandButton;

    Q_DISABLE_COPY(NLPropSectionHeader)
};

class NLFormGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit NLFormGroupBox(QWidget *widget = 0);
    virtual ~NLFormGroupBox();
    void AddRow(const QString &name, QWidget *widget);
    void Reset();
    QFormLayout * Layout() { return m_layout; }
    void AddAction(QAction *action);
    QVariant Data() const { return m_data; }
    void SetData(const QVariant &data);

Q_SIGNALS:
    void actionTriggered(QAction *action);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void Init();

private Q_SLOTS:
    void ToggleGroupBox(bool on);

private:
    typedef QHash<QString, QWidget *> WidgetHash;
    QFormLayout *m_layout;
    QToolButton *m_actionButton;
    WidgetHash m_widgetItemMaps;
    QStringList m_names;
    bool m_expand;
    QVariant m_data;

    Q_DISABLE_COPY(NLFormGroupBox)
};

class NLPropSectionContent : public QWidget
{
    Q_OBJECT
public:
    explicit NLPropSectionContent(QWidget *widget = 0);
    virtual ~NLPropSectionContent();
    void AddWidget(QWidget *widget);
    void Reset();

private:
    void Init();

private Q_SLOTS:
    void Toggle(bool on);

private:
    typedef QList<QWidget *> WidgetList;
    QVBoxLayout *m_layout;
    WidgetList m_widgets;
    bool m_expand;

    Q_DISABLE_COPY(NLPropSectionContent)
};

class NLPropSection : public QWidget
{
    Q_OBJECT
public:
    explicit NLPropSection(QWidget *widget = 0);
    virtual ~NLPropSection();
    void AddWidget(QWidget *widget);
    void SetTitle(const QString &str);
    void AddAction(QAction *action);
    void Reset();

Q_SIGNALS:
    void actionTriggered(QAction *action);

private:
    void Init();

private:
    NLPropSectionHeader *m_header;
    NLPropSectionContent *m_content;

    Q_DISABLE_COPY(NLPropSection)
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
    NLFormGroupBox *m_actorGroupBox;
    NLPropSection *m_componentSection;
    NLPropSection *m_scriptSection;
    ObjectPropWidgetHash m_propWidgetMap;
    ObjectHash m_objectMap;

    Q_DISABLE_COPY(NLActorPropWidget)
};

#endif // _KARIN_NLACTORPROPWIDGET_H
