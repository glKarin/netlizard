#include "nlactorpropwidget.h"

#include <QDebug>
#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <limits>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>
#include <QMenu>
#include <QToolButton>

#include "qdef.h"
#include "nlfuncs.h"
#include "nlscene.h"
#include "nlcomponent.h"
#include "nlrenderable.h"
#include "nlactor.h"
#include "nlactorcontainer.h"
#include "nlscript.h"
#include "nlvector3widget.h"
#include "nlfilechooserwidget.h"
#include "nltexteditwidget.h"

#define NLOBJECT_PTR_PROPERTY_NAME "NLObject"
#define DOUBLE_SPINBOX_SINGLE_STEP 1 //0.1
#define DOUBLE_SPINBOX_DECIMAL 6
#define ACTION_ADD_COMPONENT 1
#define ACTION_ADD_SCRIPT 2

static bool NLPropertyInfoCmp(const NLPropertyInfo &a, const NLPropertyInfo &b)
{
    if(a.name == b.name)
        return false;

    QStringList list;
    list << "objectName"
         << "enabled"
         << "position"
         << "rotation"
         << "scale"
         << "renderable"
        ;

    int ai = list.indexOf(a.name);
    int bi = list.indexOf(b.name);
    if(ai == -1 && bi == -1)
        return QString::compare(a.name, b.name) < 0;

    if(ai == -1)
        return false;
    if(bi == -1)
        return true;
    return ai < bi;
}

NLActorPropSectionHeader::NLActorPropSectionHeader(QWidget *widget)
    : QWidget(widget),
      m_label(0),
      m_menu(0)
{
    setObjectName("NLActorPropSectionHeader");
    Init();
}

NLActorPropSectionHeader::NLActorPropSectionHeader(const QString &text, QWidget *widget)
    : QWidget(widget),
      m_label(0),
      m_menu(0)
{
    setObjectName("NLActorPropSectionHeader");
    Init(text);
}

NLActorPropSectionHeader::~NLActorPropSectionHeader()
{
    DEBUG_DESTROY_Q
}

void NLActorPropSectionHeader::AddAction(QAction *action)
{
    if(!m_menu)
    {
        m_menu = new QMenu(this);
        QToolButton *button = new QToolButton;
        layout()->addWidget(button);
        //button->setArrowType(Qt::DownArrow);
        button->setPopupMode(QToolButton::InstantPopup);
        button->setAutoRaise(true);
        button->setMenu(m_menu);
    }
    m_menu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(OnActionTriggered()));
}

void NLActorPropSectionHeader::OnActionTriggered()
{
    QObject *obj = sender();
    if(obj)
        emit actionTriggered(static_cast<QAction *>(obj));
}

void NLActorPropSectionHeader::SetText(const QString &text)
{
    m_label->setText(text);
}

void NLActorPropSectionHeader::Init(const QString &text)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_label = new QLabel(text);

    mainLayout->addWidget(m_label, 1);
    QMargins margins = mainLayout->contentsMargins();
    margins.setBottom(0);
    margins.setTop(0);
    mainLayout->setContentsMargins(margins);

    setLayout(mainLayout);
}



NLActorPropWidget::NLActorPropWidget(QWidget *widget)
    : QScrollArea(widget),
      m_actor(0),
      m_actorLayout(0),
      m_componentLayout(0),
      m_actorGroupBox(0),
      m_scriptLayout(0),
      m_componentSectionHeader(0),
      m_scriptSectionHeader(0)
{
    setObjectName("NLActorPropWidget");
    Init();
}

NLActorPropWidget::~NLActorPropWidget()
{
    Reset();
    DEBUG_DESTROY_Q
}

void NLActorPropWidget::Init()
{
    QLabel *label;
    //QGroupBox *groupBox;
    m_actorLayout = new QFormLayout;
    m_componentLayout = new QVBoxLayout;
    m_actorLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    QWidget *root = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_actorGroupBox = new QGroupBox;
    m_scriptLayout = new QVBoxLayout;
    QAction *action;

    label = new QLabel(tr("Actor: "));
    mainLayout->addWidget(label);
    mainLayout->addSpacing(1);

    m_actorGroupBox->setLayout(m_actorLayout);
    m_actorGroupBox->setTitle(tr("Properties"));
    m_actorGroupBox->setCheckable(true);
    connect(m_actorGroupBox, SIGNAL(toggled(bool)), this, SLOT(ToggleGroupBox(bool)));
    mainLayout->addWidget(m_actorGroupBox);
    mainLayout->addSpacing(2);

    m_componentSectionHeader = new NLActorPropSectionHeader(tr("Component: "));
    action = new QAction(tr("Add"), m_componentSectionHeader);
    action->setData(ACTION_ADD_COMPONENT);
    connect(m_componentSectionHeader, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    m_componentSectionHeader->AddAction(action);
    m_componentSectionHeader->hide();
    mainLayout->addWidget(m_componentSectionHeader);
    mainLayout->addSpacing(1);
    mainLayout->addLayout(m_componentLayout);

    mainLayout->addSpacing(2);
    m_scriptSectionHeader = new NLActorPropSectionHeader(tr("Script: "));
    action = new QAction(tr("Add"), m_scriptSectionHeader);
    action->setData(ACTION_ADD_SCRIPT);
    connect(m_scriptSectionHeader, SIGNAL(actionTriggered(QAction *)), this, SLOT(OnActionTriggered(QAction *)));
    m_scriptSectionHeader->AddAction(action);
    m_scriptSectionHeader->hide();
    mainLayout->addWidget(m_scriptSectionHeader);
    mainLayout->addSpacing(1);
    mainLayout->addLayout(m_scriptLayout);

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
            connect(m_actor, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
            connect(m_actor, SIGNAL(componentChanged(const NLComponent *)), this, SLOT(SetupComponent()));
            connect(m_actor, SIGNAL(scriptChanged(const NLScript *)), this, SLOT(SetupScript()));
            connect(m_actor, SIGNAL(destroying()), this, SLOT(Reset()));
        }
        m_componentSectionHeader->show();
        m_scriptSectionHeader->show();
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

    ClearSection(m_actorGroupBox);
    ClearComponentProperties();
    ClearScriptProperties();
    m_propWidgetMap.clear();
    m_objectMap.clear();
    m_actorGroupBox->setTitle(tr("Properties"));
    m_actorGroupBox->setProperty("_Layout_visible", QVariant());
    m_actorGroupBox->setProperty("_Layout_items", QVariant());
    m_actorGroupBox->setProperty("_Layout_item_maps", QVariant());
    m_actorGroupBox->setCheckable(false);
    m_componentSectionHeader->hide();
    m_scriptSectionHeader->hide();
}

void NLActorPropWidget::UpdateActorData()
{
    if(!m_actor)
        return;

    m_actorGroupBox->setTitle(m_actor->ClassName() + "(" + m_actor->Name() + ")");
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
    NLPropertyInfoList list = NL::object_propertics(m_actor);
    SortProperties(list);
    QStringList items;
    QVariantHash itemMaps;
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(m_actor, item);
        m_propWidgetMap[m_actor].insert(item.name, widget);
        m_actorLayout->addRow(item.name, widget);
        items.push_back(item.name);
        itemMaps.insert(item.name, QVariant::fromValue(widget));
    }
    m_actorGroupBox->setCheckable(true);
    m_actorGroupBox->setChecked(true);
    m_actorGroupBox->setProperty("_Layout_visible", true);
    m_actorGroupBox->setProperty("_Layout_items", items);
    m_actorGroupBox->setProperty("_Layout_item_maps", itemMaps);
    m_objectMap["NLActor"].push_back(m_actor);
}

void NLActorPropWidget::SortProperties(NLPropertyInfoList &list)
{
    qSort(list.begin(), list.end(), NLPropertyInfoCmp);
}

void NLActorPropWidget::SetupComponentProperty(NLComponent *comp)
{
    QFormLayout *layout = new QFormLayout;
    QGroupBox *groupBox = new QGroupBox;
    QStringList items;
    QVariantHash itemMaps;
    groupBox->setTitle(comp->ClassName() + "(" + comp->Name() + ")");
    NLPropertyInfoList list = NL::object_propertics(comp);
    SortProperties(list);
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(comp, item);
        m_propWidgetMap[comp].insert(item.name, widget);
        layout->addRow(item.name, widget);
        items.push_back(item.name);
        itemMaps.insert(item.name, QVariant::fromValue(widget));
    }
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    groupBox->setProperty("_Layout_visible", true);
    groupBox->setProperty("_Layout_items", items);
    groupBox->setProperty("_Layout_item_maps", itemMaps);
    connect(groupBox, SIGNAL(toggled(bool)), this, SLOT(ToggleGroupBox(bool)));
    connect(comp, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
    groupBox->setLayout(layout);
    m_componentLayout->addWidget(groupBox);
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

void NLActorPropWidget::ToggleGroupBox(bool on)
{
    QObject *s = sender();
    if(!s)
        return;
    QGroupBox *groupBox = dynamic_cast<QGroupBox *>(s);
    if(!groupBox)
        return;
    QVariant va = s->property("_Layout_visible");
    if(!va.isValid())
        return;
    bool visible = va.toBool();
    if(visible == on)
        return;
    QStringList list = s->property("_Layout_items").toStringList();
    QVariantHash map = s->property("_Layout_item_maps").toHash();
    QFormLayout *layout = static_cast<QFormLayout *>(groupBox->layout());
    if(on)
    {
        Q_FOREACH(const QString &name, list)
        {
            QWidget *widget = map[name].value<QWidget *>();
            widget->setVisible(true);
            layout->addRow(name, widget);
        }
    }
    else
    {
        while(!layout->isEmpty())
        {
            QLayoutItem *item = layout->takeAt(0);
            item->widget()->setVisible(false);
            delete item;
        }
    }
    s->setProperty("_Layout_visible", on);
}

QWidget * NLActorPropWidget::GenWidget(NLObject *obj, const NLPropertyInfo &item)
{
    QWidget *widget = 0;

    if(item.widget == "spinbox")
    {
        if(item.type == "int")
        {
            QSpinBox *w = new QSpinBox;
            w->setSingleStep(1);
            const QVariantHash prop = item.prop.value("range").toHash();
            w->setMinimum(prop.value("min", -std::numeric_limits<int>::max()).toInt());
            w->setMaximum(prop.value("max", std::numeric_limits<int>::max()).toInt());
            w->setReadOnly(item.readonly);
            w->setValue(item.value.toInt());
            connect(w, SIGNAL(valueChanged(int)), this, SLOT(OnIntChanged(int)));
            connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            widget = w;
        }
        else
        {
            QDoubleSpinBox *w = new QDoubleSpinBox;
            w->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);
            w->setDecimals(DOUBLE_SPINBOX_DECIMAL);
            const QVariantHash prop = item.prop.value("range").toHash();
            w->setMinimum(prop.value("min", -std::numeric_limits<float>::max()).toDouble());
            w->setMaximum(prop.value("max", std::numeric_limits<float>::max()).toDouble());
            w->setReadOnly(item.readonly);
            w->setValue(item.value.toFloat());
            //connect(w, SIGNAL(valueChanged(const QString &)), this, SLOT(OnValueChanged(const QString &)));
            connect(w, SIGNAL(valueChanged(double)), this, SLOT(OnDoubleChanged(double)));
            connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            widget = w;
        }
    }
    else if(item.widget == "checkbox")
    {
        if(item.type == "bool")
        {
            QCheckBox *w = new QCheckBox;
            w->setChecked(item.value.toBool());
            w->setCheckable(!item.readonly);
            connect(w, SIGNAL(clicked(bool)), this, SLOT(OnBoolChanged(bool)));
            connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            widget = w;
        }
#if 0
        else if(item.type == "int")
        {
            QGroupBox *w = new QGroupBox;
            QButtonGroup *g = new QButtonGroup(w);
            g->setObjectName(item.name);
            g->setExclusive(false);
            QVBoxLayout *vbox = new QVBoxLayout;
            const QVariantList & items = item.prop.value("option").toList();
            int target = settings->GetSetting<int>(item.name, item.value.toInt());
            for(int i = 0; i < items.size(); i++)
            {
                const QVariantHash p = items[i].toHash();
                const QVariant v = p.value("value");
                int iv = v.toInt();
                QCheckBox *cb = new QCheckBox(p.value("label").toString());
                g->addButton(cb, iv);

                if(target & iv)
                    cb->setChecked(true);
                vbox->addWidget(cb);
            }
            vbox->addStretch(1);

            w->setLayout(vbox);
            w->setTitle(item.title);
            label.clear();
            //w->setCheckable(true);
            connect(g, SIGNAL(buttonClicked(int)), this, SLOT(OnIntChanged(int)));
            widget = w;
        }
#endif
    }
    else if(item.widget == "combobox")
    {
        QComboBox *w = new QComboBox;
        const NLPropertyPairList items = item.prop.value("enum").value<NLPropertyPairList>();
        int cur = 0;
        for(int i = 0; i < items.size(); i++)
        {
            const NLPropertyPair &p = items[i];
            w->addItem(p.first, p.second);
            if(p.second == item.value)
                cur = i;
        }
        w->setEnabled(!item.readonly);
        w->setCurrentIndex(cur);
        connect(w, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIndexChanged(int)));
        connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
        widget = w;
    }
    else if(item.widget == "vector3")
    {
        NLVector3Widget *w = new NLVector3Widget;
        w->SetVector3(item.value.value<NLVector3>());
        w->SetReadOnly(item.readonly);
        connect(w, SIGNAL(vector3Changed(const NLVector3 &)), this, SLOT(OnVector3Changed(const NLVector3 &)));
        widget = w;
    }
    else if(item.widget == "lineedit")
    {
        QLineEdit *w = new QLineEdit;
        w->setText(item.value.toString());
        w->setReadOnly(item.readonly);
        connect(w, SIGNAL(textEdited(const QString &)), this, SLOT(OnStringChanged(const QString &)));
        connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
        widget = w;
    }
    else if(item.widget == "textedit")
    {
        if(item.prop.value("direct", true).toBool())
        {
            QTextEdit *w = new QTextEdit;
            w->setText(item.value.toString());
            w->setReadOnly(item.readonly);
            w->setAcceptRichText(false);
            connect(w, SIGNAL(textChanged()), this, SLOT(OnStringChanged()));
            connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            widget = w;
        }
        else
        {
            NLTextEditWidget *w = new NLTextEditWidget;
            w->SetText(item.value.toString());
            w->SetReadOnly(item.readonly);
            connect(w, SIGNAL(textEdited(const QString &)), this, SLOT(OnStringChanged(const QString &)));
            widget = w;
        }
    }
    else if(item.widget == "filedialog")
    {
        NLFileChooserWidget *w = new NLFileChooserWidget;
        w->SetFile(item.value.toString());
        w->SetReadOnly(item.readonly);
        connect(w, SIGNAL(fileChanged(const QString &)), this, SLOT(OnStringChanged(const QString &)));
        connect(w, SIGNAL(fileReload(const QString &)), this, SLOT(OnStringReload(const QString &)));
        widget = w;
    }
    else/* if(item.widget == "label")*/
    {
        int type = item.value.type();
        QLineEdit *w = new QLineEdit;
        if(type == QMetaType::QObjectStar || type == QMetaType::VoidStar)
        {
            if(type == QMetaType::QObjectStar)
            {
                QObject *qo = item.value.value<QObject *>();
                if(qo && instanceofv(qo, NLObject))
                {
                    NLObject *nlo = static_cast<NLObject *>(qo);
                    w->setText(nlo->ClassName() + "::" + nlo->objectName() + "(" + nlo->Name() +")");
                    w->setReadOnly(item.readonly);
                }
            }
            else if(type == QMetaType::VoidStar)
            {
                void *vo = item.value.value<void *>();
                if(/*instanceofv(vo, NLRenderable)*/vo)
                {
                    NLRenderable *renderable = static_cast<NLRenderable *>(vo);
                    w->setText(renderable->Name());
                    w->setReadOnly(item.readonly);
                }
            }
        }
        else
        {
            w->setText(item.value.toString());
            w->setReadOnly(item.readonly);
        }
        connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
        widget = w;
    }

    widget->setObjectName(item.name);
    widget->setProperty(NLOBJECT_PTR_PROPERTY_NAME, QVariant::fromValue<QObject *>(obj));

    return widget;
}

void NLActorPropWidget::OnPropertyChanged(const QString &name, const NLProperty &value)
{
    QObject *s = sender();
    if(!s)
        return;
    NLObject *obj = static_cast<NLObject *>(s);
    QWidget *widget = m_propWidgetMap[obj].value(name);
    if(!widget)
        return;
    if(instanceofv(widget, QSpinBox))
    {
        static_cast<QSpinBox *>(widget)->setValue(value.toInt());
    }
    else if(instanceofv(widget, QDoubleSpinBox))
    {
        static_cast<QDoubleSpinBox *>(widget)->setValue(value.toFloat());
    }
    else if(instanceofv(widget, NLVector3Widget))
    {
        static_cast<NLVector3Widget *>(widget)->SetVector3(value.value<NLVector3>());
    }
    else if(instanceofv(widget, QCheckBox))
    {
        static_cast<QCheckBox *>(widget)->setChecked(value.toBool());
    }
    else if(instanceofv(widget, QLineEdit))
    {
        static_cast<QLineEdit *>(widget)->setText(value.toString());
    }
    else if(instanceofv(widget, QComboBox))
    {
        QComboBox *cb = static_cast<QComboBox *>(widget);
        int index = cb->findData(value);
        if(index >= 0)
            cb->setCurrentIndex(index);
    }
    else if(instanceofv(widget, QTextEdit))
    {
        static_cast<QTextEdit *>(widget)->setPlainText(value.toString());
    }
    else if(instanceofv(widget, QPushButton))
    {
        static_cast<QPushButton *>(widget)->setText(value.toString());
    }
    else if(instanceofv(widget, NLFileChooserWidget))
    {
        static_cast<NLFileChooserWidget *>(widget)->SetFile(value.toString());
    }
    else if(instanceofv(widget, NLTextEditWidget))
    {
        static_cast<NLTextEditWidget *>(widget)->SetText(value.toString());
    }
}

void NLActorPropWidget::OnIntChanged(int i)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    obj->SetProperty(s->objectName(), i);
}

void NLActorPropWidget::OnIndexChanged(int i)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    if(instanceofv(s, QComboBox))
    {
        QComboBox *cb = static_cast<QComboBox *>(s);
        obj->SetProperty(s->objectName(), cb->itemData(i));
    }
}

void NLActorPropWidget::OnDoubleChanged(double f)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    obj->SetProperty(s->objectName(), (float)f);
}

void NLActorPropWidget::OnBoolChanged(bool b)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    obj->SetProperty(s->objectName(), b);
}

void NLActorPropWidget::OnVector3Changed(const NLVector3 &v)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    obj->SetProperty(s->objectName(), NLProperty::fromValue<NLVector3>(v));
}

void NLActorPropWidget::OnStringChanged(const QString &str)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    if(instanceofv(s, QTextEdit))
    {
        QTextEdit *t = static_cast<QTextEdit *>(s);
        bool editing = t->hasFocus();
        if(editing)
        {
            obj->SetProperty(s->objectName(), t->toPlainText());
        }
    }
    else
        obj->SetProperty(s->objectName(), str);
}

void NLActorPropWidget::OnStringReload(const QString &str)
{
    QObject *s = sender();
    if(!s)
        return;
    QObject *o = s->property(NLOBJECT_PTR_PROPERTY_NAME).value<QObject *>();
    if(!o)
        return;
    NLObject *obj = static_cast<NLObject *>(o);
    if(!obj)
        return;
    obj->CoverProperty(s->objectName(), str);
}

void NLActorPropWidget::ClearSection(QGroupBox *groupBox)
{
    QLayoutItem *item;
    QLayout *layout = groupBox->layout();
    QVariant va = groupBox->property("_Layout_visible");
    bool visible = va.isValid() ? va.toBool() : true;
    if(visible)
    {
        while(!layout->isEmpty())
        {
            item = layout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }
    else
    {
        QVariantHash map = groupBox->property("_Layout_item_maps").toHash();
        Q_FOREACH(const QString &name, map.keys())
        {
            QWidget *widget = map[name].value<QWidget *>();
            delete widget;
        }
    }
}

void NLActorPropWidget::OnItemDestroy(QObject *obj)
{
    QObject *o = obj ? obj : sender();
    if(o)
        DEBUG_DESTROY_QQV(obj)
}

void NLActorPropWidget::SetupScriptProperty(NLScript *script)
{
    QFormLayout *layout = new QFormLayout;
    QGroupBox *groupBox = new QGroupBox;
    QStringList items;
    QVariantHash itemMaps;
    groupBox->setTitle(script->ClassName() + "(" + script->Name() + ")");
    NLPropertyInfoList list = NL::object_propertics(script);
    SortProperties(list);
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(script, item);
        m_propWidgetMap[script].insert(item.name, widget);
        layout->addRow(item.name, widget);
        items.push_back(item.name);
        itemMaps.insert(item.name, QVariant::fromValue(widget));
    }
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    groupBox->setProperty("_Layout_visible", true);
    groupBox->setProperty("_Layout_items", items);
    groupBox->setProperty("_Layout_item_maps", itemMaps);
    connect(groupBox, SIGNAL(toggled(bool)), this, SLOT(ToggleGroupBox(bool)));
    connect(script, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
    groupBox->setLayout(layout);
    m_scriptLayout->addWidget(groupBox);
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
    QLayoutItem *item;
    while(!m_componentLayout->isEmpty())
    {
        item = m_componentLayout->takeAt(0);
        QGroupBox *groupBox = static_cast<QGroupBox *>(item->widget());
        ClearSection(groupBox);
        delete groupBox;
        delete item;
    }
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
    QLayoutItem *item;
    while(!m_scriptLayout->isEmpty())
    {
        item = m_scriptLayout->takeAt(0);
        QGroupBox *groupBox = static_cast<QGroupBox *>(item->widget());
        ClearSection(groupBox);
        delete groupBox;
        delete item;
    }
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
}
