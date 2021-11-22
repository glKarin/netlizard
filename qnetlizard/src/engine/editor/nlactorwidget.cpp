#include "nlactorwidget.h"

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

#include "qdef.h"
#include "nlfuncs.h"
#include "nlscene.h"
#include "nlcomponent.h"
#include "nlactor.h"
#include "nlactorcontainer.h"

#define NLOBJECT_PTR_PROPERTY_NAME "NLObject"
#define DOUBLE_SPINBOX_SINGLE_STEP 0.1
#define DOUBLE_SPINBOX_DECIMAL 6

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

NLVector3Widget::NLVector3Widget(QWidget *widget)
    : QWidget(widget),
      m_xSpinBox(0),
      m_ySpinBox(0),
      m_zSpinBox(0)
{
    setObjectName("NLVector3Widget");
    vector3_identityv(&m_vector3);
    Init();
}

NLVector3Widget::~NLVector3Widget()
{
    DEBUG_DESTROY_Q
}

void NLVector3Widget::Init()
{
    QGridLayout *mainLayout = new QGridLayout;
    QFormLayout *layout;
    m_xSpinBox = new QDoubleSpinBox;
    m_ySpinBox = new QDoubleSpinBox;
    m_zSpinBox = new QDoubleSpinBox;
    m_xSpinBox->setMaximum(std::numeric_limits<float>::max());
    m_ySpinBox->setMaximum(std::numeric_limits<float>::max());
    m_zSpinBox->setMaximum(std::numeric_limits<float>::max());
    m_xSpinBox->setMinimum(-std::numeric_limits<float>::max());
    m_ySpinBox->setMinimum(-std::numeric_limits<float>::max());
    m_zSpinBox->setMinimum(-std::numeric_limits<float>::max());
    m_xSpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_ySpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_zSpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_xSpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);
    m_ySpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);
    m_zSpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);

    layout = new QFormLayout;
    m_xSpinBox->setObjectName("X");
    connect(m_xSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
    layout->addRow("X", m_xSpinBox);
    mainLayout->addLayout(layout, 1, 0, 1, 1);

    layout = new QFormLayout;
    m_ySpinBox->setObjectName("Y");
    connect(m_ySpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
    layout->addRow("Y", m_ySpinBox);
    mainLayout->addLayout(layout, 1, 1, 1, 1);

    layout = new QFormLayout;
    m_zSpinBox->setObjectName("Z");
    connect(m_zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
    layout->addRow("Z", m_zSpinBox);
    mainLayout->addLayout(layout, 1, 2, 1, 1);

    UpdateWidget();

    setLayout(mainLayout);
}

void NLVector3Widget::UpdateWidget()
{
    m_xSpinBox->setValue(VECTOR3_X(m_vector3));
    m_ySpinBox->setValue(VECTOR3_Y(m_vector3));
    m_zSpinBox->setValue(VECTOR3_Z(m_vector3));
}

void NLVector3Widget::OnValueChanged(double d)
{
    QString name(sender()->objectName());
    if(name == "X")
        VECTOR3_X(m_vector3) = (float)d;
    else if(name == "Y")
        VECTOR3_Y(m_vector3) = (float)d;
    else if(name == "Z")
        VECTOR3_Z(m_vector3) = (float)d;
    emit vector3Changed(m_vector3);
}

void NLVector3Widget::SetVector3(const NLVector3 &v)
{
    if(!vector3_equals(&v, &m_vector3))
    {
        m_vector3 = v;
        UpdateWidget();
        emit vector3Changed(m_vector3);
    }
}

void NLVector3Widget::SetReadOnly(bool b)
{
    m_xSpinBox->setReadOnly(b);
    m_ySpinBox->setReadOnly(b);
    m_zSpinBox->setReadOnly(b);
}



NLActorWidget::NLActorWidget(QWidget *widget)
    : QScrollArea(widget),
      m_actor(0)
{
    setObjectName("NLActorWidget");
    Init();
}

NLActorWidget::~NLActorWidget()
{
    m_actor = 0;
    DEBUG_DESTROY_Q
}

void NLActorWidget::Init()
{
    QLabel *label;
    QGroupBox *groupBox;
    m_actorLayout = new QFormLayout;
    m_componentLayout = new QVBoxLayout;
    m_actorLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    QWidget *root = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    label = new QLabel("Actor: ");
    mainLayout->addWidget(label);
    mainLayout->addSpacing(1);

    groupBox = new QGroupBox;
    groupBox->setLayout(m_actorLayout);
    groupBox->setTitle("Properties");
    mainLayout->addWidget(groupBox);
    mainLayout->addSpacing(2);

    label = new QLabel("Component: ");
    mainLayout->addWidget(label);
    mainLayout->addSpacing(1);

    mainLayout->addLayout(m_componentLayout);

    mainLayout->addStretch(1);

    root->setLayout(mainLayout);
    setWidgetResizable(true);
    setWidget(root);
}

void NLActorWidget::SetActor(NLActor *actor)
{
    if(m_actor != actor)
    {
        Reset();
        m_actor = actor;
        if(m_actor)
        {
            connect(m_actor, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
            connect(m_actor, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
            connect(m_actor, SIGNAL(componentChanged(const NLComponent *)), this, SLOT(OnActorChanged()));
            connect(m_actor, SIGNAL(destroyed()), this, SLOT(Reset()));
        }
        UpdateActorData();
    }
}

void NLActorWidget::Reset()
{
    if(m_actor)
    {
        m_actor->disconnect(this);
        const int Count = m_actor->ComponentCount();
        for(int i = 0; i < Count; i++)
        {
            NLComponent *comp = m_actor->GetComponent(i);
            comp->disconnect(this);
        }
    }
    QLayoutItem *item;
    while(!m_actorLayout->isEmpty())
    {
        item = m_actorLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    while(!m_componentLayout->isEmpty())
    {
        item = m_componentLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    m_propWidgetMap.clear();
    m_actor = 0;
}

void NLActorWidget::UpdateActorData()
{
    if(!m_actor)
    {
        return;
    }
    SetupActorProperty();
    SetupComponentProperties();
}

void NLActorWidget::OnActorChanged()
{
    Reset();
    UpdateActorData();
}

void NLActorWidget::SetupActorProperty()
{
    NLPropertyInfoList list = NL::object_propertics(m_actor);
    SortProperties(list);
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(m_actor, item);
        m_propWidgetMap[m_actor].insert(item.name, widget);
        m_actorLayout->addRow(item.name, widget);
    }
}

void NLActorWidget::SortProperties(NLPropertyInfoList &list)
{
    qSort(list.begin(), list.end(), NLPropertyInfoCmp);
}

void NLActorWidget::SetupComponentProperty(NLComponent *comp)
{
    QFormLayout *layout = new QFormLayout;
    QGroupBox *groupBox = new QGroupBox;
    groupBox->setTitle(comp->objectName() + "(" + comp->Name() + ")");
    NLPropertyInfoList list = NL::object_propertics(comp);
    SortProperties(list);
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(comp, item);
        m_propWidgetMap[comp].insert(item.name, widget);
        layout->addRow(item.name, widget);
    }
    connect(comp, SIGNAL(propertyChanged(const QString &, const NLProperty &)), this, SLOT(OnPropertyChanged(const QString &, const NLProperty &)));
    groupBox->setLayout(layout);
    m_componentLayout->addWidget(groupBox);
    m_componentLayout->addSpacing(1);
}

void NLActorWidget::SetupComponentProperties()
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

QWidget * NLActorWidget::GenWidget(NLObject *obj, const NLPropertyInfo &item)
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
    else// if(item.widget == "lineedit")
    {
        QLineEdit *w = new QLineEdit;
        w->setText(item.value.toString());
        w->setReadOnly(item.readonly);
        //connect(w, SIGNAL(clicked(bool)), this, SLOT(OnBoolChanged(bool)));
        widget = w;
    }

    widget->setObjectName(item.name);
    widget->setProperty(NLOBJECT_PTR_PROPERTY_NAME, QVariant::fromValue<QObject *>(obj));

    return widget;
}

void NLActorWidget::OnPropertyChanged(const QString &name, const NLProperty &value)
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
}

void NLActorWidget::OnIntChanged(int i)
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

void NLActorWidget::OnIndexChanged(int i)
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

void NLActorWidget::OnDoubleChanged(double f)
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

void NLActorWidget::OnBoolChanged(bool b)
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

void NLActorWidget::OnVector3Changed(const NLVector3 &v)
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
