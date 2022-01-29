#include "nlactorpropwidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QMouseEvent>
#include <QApplication>

#include "common/nldbg.h"
#include "utils/nlfuncs.h"
#include "engine/nlcomponent.h"
#include "engine/nlactor.h"
#include "engine/nlscript.h"
#include "widget/nlpropsectionwidget.h"

#define ACTION_ADD_COMPONENT 1
#define ACTION_ADD_SCRIPT 2
#define ACTION_REMOVE_COMPONENT 3
#define ACTION_REMOVE_SCRIPT 4

class NLPropertyInfoCmp
{
public:
    explicit NLPropertyInfoCmp(NLObject *obj)
        : m_obj(obj) {}
    virtual ~NLPropertyInfoCmp() { m_obj = 0; }
    bool operator()(const NLPropertyInfo &a, const NLPropertyInfo &b)
    {
        if(a.name == b.name)
            return false;
        if(a.user != b.user)
            return a.user < b.user;

        QStringList list;
        list << "objectName"
             << "enabled"
            ;
        if(m_obj && m_obj->Type() == NLObject::Type_Actor)
        {
            list << "position"
                 << "rotation"
                 << "scale"
                 << "renderable"
                ;
        }

        int ai = list.indexOf(a.name);
        int bi = list.indexOf(b.name);
        if(ai == -1 && bi == -1)
            return true; //QString::compare(a.name, b.name) < 0;

        if(ai == -1)
            return false;
        if(bi == -1)
            return true;
        return ai < bi;
    }
private:
    NLObject *m_obj;
};

NLObjectPropFormGroupWidget::NLObjectPropFormGroupWidget(QWidget *widget)
    : NLPropFormGroupWidget(widget)
{
    setObjectName("NLObjectPropFormGroupWidget");
}

NLObjectPropFormGroupWidget::NLObjectPropFormGroupWidget(const QString &title, QWidget *widget)
    : NLPropFormGroupWidget(title, widget)
{
    setObjectName("NLObjectPropFormGroupWidget");
}

NLObjectPropFormGroupWidget::~NLObjectPropFormGroupWidget()
{

}

void NLObjectPropFormGroupWidget::SetObject(QObject *obj)
{
    NLPropFormGroupWidget::SetObject(obj);
    if(obj)
    {
        NLObject *nlo = static_cast<NLObject *>(obj);
        connect(nlo, SIGNAL(propertyChanged(const QString &, const QVariant &, int)), this, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
        connect(obj, SIGNAL(destroying()), this, SLOT(Reset()));
    }
}

QVariant NLObjectPropFormGroupWidget::GetObjectProperty(QObject *obj, const QString &name)
{
    return static_cast<NLObject *>(obj)->GetProperty(name);
}

void NLObjectPropFormGroupWidget::SetObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<NLObject *>(obj)->SetProperty(name, value);
}

void NLObjectPropFormGroupWidget::CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<NLObject *>(obj)->CoverProperty(name, value);
}

NLPropertyInfoList NLObjectPropFormGroupWidget::GetPropertyInfoList(QObject *obj)
{
    return NL::object_propertics(static_cast<NLObject *>(obj));
}

void NLObjectPropFormGroupWidget::SortProperties(QObject *obj, NLPropertyInfoList &list)
{
    qSort(list.begin(), list.end(), NLPropertyInfoCmp(static_cast<NLObject *>(obj)));
}

NLPropFormGroupWidget * NLObjectPropFormGroupWidget::GenFormGroup(QObject *obj)
{
    NLObject *nlo = static_cast<NLObject *>(obj);
    NLObjectPropFormGroupWidget *groupBox = new NLObjectPropFormGroupWidget;
    groupBox->SetObject(nlo);
    return groupBox;
}

void NLObjectPropFormGroupWidget::OnPropertyChanged(const QString &name, const QVariant &value, int action)
{
    NLObject *obj = static_cast<NLObject *>(Object());
    if(!obj)
        return;
    if(action == 0)
        NotifyPropertyChanged(name, value);
    else
        OnObjectChanged();
}

void NLObjectPropFormGroupWidget::mousePressEvent(QMouseEvent *event)
{
    NLFormGroupWidget::mousePressEvent(event);
    if(!AllowDragDrop())
        return;
    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();
}

void NLObjectPropFormGroupWidget::mouseMoveEvent(QMouseEvent *event)
{
    NLFormGroupWidget::mouseMoveEvent(event);
    if(!AllowDragDrop())
        return;

    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    NLObject *nlo = static_cast<NLObject *>(Object());
    QVariant value = NL::object_to_qvaraint(nlo);
    if(!value.isValid())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setProperty(NLFormGroupWidget::FormFieldDragDropDataKey, value);
    mimeData->setData(NLFormGroupWidget::FormFieldDragDropMIME, QByteArray());
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
    Q_UNUSED(dropAction);
    //delete drag;
}




NLActorPropWidget::NLActorPropWidget(QWidget *widget)
    : QScrollArea(widget),
      m_actor(0),
      m_actorGroupBox(0),
      m_componentSection(0),
      m_scriptSection(0)
{
    setObjectName("NLActorPropWidget");
    Init();
}

NLActorPropWidget::~NLActorPropWidget()
{
    Reset();
    NLDEBUG_DESTROY_Q
}

void NLActorPropWidget::Init()
{
    QLabel *label;
    m_componentSection = new NLPropSection;
    QWidget *root = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_actorGroupBox = new NLObjectPropFormGroupWidget;
    m_scriptSection = new NLPropSection;
    QAction *action;

    m_actorGroupBox->setObjectName(m_actorGroupBox->objectName() + "_actor");
    label = new QLabel(tr("Actor: "));
    mainLayout->addWidget(label);
    mainLayout->addSpacing(1);

    m_actorGroupBox->setTitle(tr("Properties"));
    mainLayout->addWidget(m_actorGroupBox);
    mainLayout->addSpacing(2);

    m_componentSection->SetTitle(tr("Component: "));
    action = new QAction(tr("Add"), m_componentSection);
    action->setData(ACTION_ADD_COMPONENT);
    connect(m_componentSection, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    m_componentSection->AddAction(action);
    m_componentSection->hide();
    mainLayout->addWidget(m_componentSection);

    mainLayout->addSpacing(2);
    m_scriptSection->SetTitle(tr("Script: "));
    action = new QAction(tr("Add"), m_scriptSection);
    action->setData(ACTION_ADD_SCRIPT);
    connect(m_scriptSection, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    m_scriptSection->AddAction(action);
    m_scriptSection->hide();
    mainLayout->addWidget(m_scriptSection);

    mainLayout->addStretch(1);

    root->setLayout(mainLayout);
    setWidgetResizable(true);
    setWidget(root);
}

void NLActorPropWidget::SetActor(NLActor *actor)
{
    if(m_actor != actor)
    {
        Reset();
        m_actor = actor;
        if(m_actor)
        {
            connect(m_actor, SIGNAL(componentChanged(const NLComponent *)), this, SLOT(SetupComponent()));
            connect(m_actor, SIGNAL(scriptChanged(const NLScript *)), this, SLOT(SetupScript()));
            connect(m_actor, SIGNAL(destroying()), this, SLOT(Reset()));
        }
        m_componentSection->show();
        m_scriptSection->show();
        UpdateActorData();
    }
}

void NLActorPropWidget::Reset()
{
    if(m_actor)
    {
        m_actor->disconnect(this);
        m_actor = 0;
    }

    m_actorGroupBox->Reset();
    m_actorGroupBox->setTitle(tr("Properties"));
    ClearComponentProperties();
    ClearScriptProperties();
    m_propWidgetMap.clear();
    m_objectMap.clear();
    m_componentSection->hide();
    m_scriptSection->hide();
}

void NLActorPropWidget::UpdateActorData()
{
    if(!m_actor)
        return;

    m_actorGroupBox->setTitle(m_actor->ClassName() + "(" + m_actor->Name() + " " + QString().sprintf("%p", m_actor) + ")");
    SetupActorProperty();
    SetupComponentProperties();
    SetupScriptProperties();
}

void NLActorPropWidget::OnActorChanged()
{
    Reset();
    UpdateActorData();
}

void NLActorPropWidget::SetupActorProperty()
{
    m_actorGroupBox->SetObject(m_actor);
    m_propWidgetMap.insert(m_actor, m_actorGroupBox);
    m_objectMap["NLActor"].push_back(m_actor);
}

void NLActorPropWidget::SetupComponentProperty(NLComponent *comp)
{
    NLObjectPropFormGroupWidget *groupBox = new NLObjectPropFormGroupWidget;
    groupBox->setObjectName(m_actorGroupBox->objectName() + "_component");
    groupBox->setTitle(comp->ClassName() + "(" + comp->Name() + " " + QString().sprintf("%p", comp) + ")");
    groupBox->SetObject(comp);
    m_propWidgetMap.insert(comp, groupBox);
    QAction *action = new QAction(tr("Remove"), groupBox);
    groupBox->AddAction(action);
    action->setData(ACTION_REMOVE_COMPONENT);
    action->setProperty("NLObject", QVariant::fromValue<QObject *>(comp));
    connect(groupBox, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    groupBox->AddAction(action);
    m_componentSection->AddWidget(groupBox);
    m_objectMap["NLComponent"].push_back(comp);
}

void NLActorPropWidget::SetupComponentProperties()
{
    const int Count = m_actor->ComponentCount();
    if(Count == 0)
        return;
    for(int i = 0; i < Count; i++)
    {
        NLComponent *comp = m_actor->GetComponent(i);
        SetupComponentProperty(comp);
    }
}

void NLActorPropWidget::SetupScriptProperty(NLScript *script)
{
    NLObjectPropFormGroupWidget *groupBox = new NLObjectPropFormGroupWidget;
    groupBox->setObjectName(m_actorGroupBox->objectName() + "_script");
    groupBox->setTitle(script->ClassName() + "(" + script->Name() + " " + QString().sprintf("%p", script) + ")");
    groupBox->SetObject(script);
    m_propWidgetMap.insert(script, groupBox);
    QAction *action = new QAction(tr("Remove"), groupBox);
    action->setData(ACTION_REMOVE_SCRIPT);
    action->setProperty("NLObject", QVariant::fromValue<QObject *>(script));
    groupBox->AddAction(action);
    connect(groupBox, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    m_scriptSection->AddWidget(groupBox);
    m_objectMap["NLScript"].push_back(script);
}

void NLActorPropWidget::SetupScriptProperties()
{
    const int Count = m_actor->ScriptCount();
    if(Count == 0)
        return;
    for(int i = 0; i < Count; i++)
    {
        NLScript *script = m_actor->GetScript(i);
        SetupScriptProperty(script);
    }
}

void NLActorPropWidget::ClearComponentProperties()
{
    if(m_objectMap.contains("NLComponent"))
    {
        Q_FOREACH(NLObject *obj, m_objectMap["NLComponent"])
        {
            m_propWidgetMap.remove(obj);
            obj->disconnect(this);
        }
        m_objectMap.remove("NLComponent");
    }
    m_componentSection->Reset();
}

void NLActorPropWidget::ClearScriptProperties()
{
    if(m_objectMap.contains("NLScript"))
    {
        Q_FOREACH(NLObject *obj, m_objectMap["NLScript"])
        {
            m_propWidgetMap.remove(obj);
            obj->disconnect(this);
        }
        m_objectMap.remove("NLScript");
    }
    m_scriptSection->Reset();
}

void NLActorPropWidget::SetupComponent()
{
    if(!m_actor)
        return;
    ClearComponentProperties();
    SetupComponentProperties();
}

void NLActorPropWidget::SetupScript()
{
    if(!m_actor)
        return;
    ClearScriptProperties();
    SetupScriptProperties();
}

void NLActorPropWidget::OnActionTriggered(QAction *action)
{
    if(!m_actor)
        return;
    int data = action->data().toInt();
    if(data == ACTION_ADD_COMPONENT)
    {
        m_actor->CreateComponent()->setObjectName("new_NLComponent");
    }
    else if(data == ACTION_ADD_SCRIPT)
    {
        m_actor->CreateScript()->setObjectName("new_NLScript");
    }
    // QAction's parent is this
    else if(data == ACTION_REMOVE_COMPONENT)
    {
        QObject * d = action->property("NLObject").value<QObject *>();
        NLComponent *comp = static_cast<NLComponent *>(d);
        m_actor->RemoveComponent(comp);
    }
    else if(data == ACTION_REMOVE_SCRIPT)
    {
        QObject * d = action->property("NLObject").value<QObject *>();
        NLScript *script = static_cast<NLScript *>(d);
        m_actor->RemoveScript(script);
    }
}
