#include "nlpropformgroupwidget.h"

#include <QDebug>
#include <QFormLayout>
#include <QVBoxLayout>
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
#include <QButtonGroup>

#include "engine/nldbg.h"
#include "engine/nlfuncs.h"
#include "engine/nlrenderable.h"
#include "nlvector3widget.h"
#include "nlfilechooserwidget.h"
#include "nlcolorchooserwidget.h"
#include "nltexteditwidget.h"
#include "engine/nlobject.h"
#include "utils/nlguiutility.h"

#define DOUBLE_SPINBOX_SINGLE_STEP 1 //0.1
#define DOUBLE_SPINBOX_DECIMAL 6

#define ACTION_ADD_COMPONENT 1
#define ACTION_ADD_SCRIPT 2
#define ACTION_REMOVE_COMPONENT 3
#define ACTION_REMOVE_SCRIPT 4

#define WIDGET_TYPE_KEY "_QWidget"
#define LAYOUT_COUNTER_NAME "_Count"
#define WIDGET_DESC_KEY "_Description"
#define WIDGET_LABEL_KEY "_Label"
#define WIDGET_ACTION_TYPE_KEY "_Type"
#define WIDGET_ACTION_WIDGET_KEY "_Widget"

#define LABEL(label, name) ((label).isEmpty() ? (name) : (label))
#define ITEM_LABEL(item) LABEL((item).label, (item).name)

#define LAYOUT_COUNTER_NUM(layout) (layout)->property(LAYOUT_COUNTER_NAME).toInt()
#define LAYOUT_COUNTER_INCREMENT(layout) (layout)->setProperty(LAYOUT_COUNTER_NAME, LAYOUT_COUNTER_NUM(layout) + 1)

#define WIDGET_DESC(w) (w)->property(WIDGET_DESC_KEY).toString()
#define WIDGET_SET_DESC(w, d) (w)->setProperty(WIDGET_DESC_KEY, d)
#define WIDGET_LABEL(w) (w)->property(WIDGET_LABEL_KEY).toString()
#define WIDGET_SET_LABEL(w, d) (w)->setProperty(WIDGET_LABEL_KEY, d)

#define ACTION_SET_TYPE(w, d) (w)->setProperty(WIDGET_ACTION_TYPE_KEY, d)
#define ACTION_SET_WIDGET(w, d) (w)->setProperty(WIDGET_ACTION_WIDGET_KEY, d)

#define WIDGET_SET_TYPE(w, T) (w)->setProperty(WIDGET_TYPE_KEY, #T)
#define WIDGET_TYPE(w) (w)->property(WIDGET_TYPE_KEY).toString()
#define WIDGETNAME_IS_TYPE(w, T) (w) == #T
#define WIDGET_IS_TYPE(w, T) (WIDGET_TYPE(w)) == #T

NLFormGroupWidget::NLFormGroupWidget(QWidget *widget)
    : QGroupBox(widget),
      m_layout(0),
      m_actionButton(0),
      m_expand(true),
      m_canExpand(true)
{
    setObjectName("NLPropFormGroupWidget");
    Init();
}

NLFormGroupWidget::NLFormGroupWidget(const QString &title, QWidget *widget)
    : QGroupBox(title, widget),
      m_layout(0),
      m_actionButton(0),
      m_expand(true),
      m_canExpand(true)
{
    setObjectName("NLPropFormGroupWidget");
    Init();
}

NLFormGroupWidget::~NLFormGroupWidget()
{
    NLDEBUG_DESTROY_Q
    Reset();
}

void NLFormGroupWidget::Init()
{
    setCheckable(false);
    setChecked(true);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(ToggleGroupBox(bool)));
}

void NLFormGroupWidget::SetCanExpand(bool b)
{
    if(m_canExpand != b)
    {
        if(!m_expand && !b)
        {
            setChecked(true);
            ToggleGroupBox(true);
        }
        m_canExpand = b;
        setCheckable(m_canExpand);
    }
}

void NLFormGroupWidget::OnItemDestroyed()
{
    NLDEBUG_DESTROY_QQV(sender())
}

void NLFormGroupWidget::ToggleGroupBox(bool on)
{
    if(!m_canExpand)
        return;
    if(m_expand == on)
        return;
    if(on)
    {
        CreateLayout();
        Q_FOREACH(const QString &name, m_nameWidgetMap.SequenceKeys())
        {
            QWidget *widget = m_nameWidgetMap[name];
            widget->setParent(this);
            widget->show();
            QString desc = WIDGET_DESC(widget);
            QString label = WIDGET_LABEL(widget);
            PushLayout(name, widget, label, desc);
        }
    }
    else
    {
        NLGUIUtility::TakeFormLayout(m_layout);
        delete m_layout;
        m_layout = 0;
        CreateLayout();
    }
    m_expand = on;
}

void NLFormGroupWidget::Reset()
{
    if(m_expand)
    {
        if(m_layout)
        {
            if(LAYOUT_COUNTER_NUM(m_layout))
            {
                NLGUIUtility::ClearLayout(m_layout);
                delete m_layout;
                m_layout = 0;
                CreateLayout();
            }
        }
    }
    else
    {
        Q_FOREACH(const QString &name, m_nameWidgetMap.keys())
        {
            QWidget *widget = m_nameWidgetMap[name];
            delete widget;
        }
    }
    m_nameWidgetMap.clear();
    m_expand = true;
    setChecked(true);
    setCheckable(false);
}

void NLFormGroupWidget::CreateLayout()
{
    if(m_layout)
        return;
    static uint _index_no = 0;
    m_layout = new QFormLayout;
    m_layout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    m_layout->setObjectName(objectName() + "::QFormLayout" + QString::number(_index_no++));
    connect(m_layout, SIGNAL(destroyed()), this, SLOT(OnItemDestroyed()));
    setLayout(m_layout);
}

void NLFormGroupWidget::PushLayout(const QString &name, QWidget *widget, const QString &label, const QString &desc)
{
    QString text(LABEL(label, name));
    if(desc.isEmpty())
        m_layout->addRow(text, widget);
    else
    {
        QLabel *l = new QLabel(text);
        l->setToolTip(desc);
        m_layout->addRow(l, widget);
    }
}

void NLFormGroupWidget::AddRow(const QString &name, QWidget *widget, const QString &desc)
{
    AddRow(name, name, widget, desc);
}

void NLFormGroupWidget::AddRow(const QString &name, const QString &label, QWidget *widget, const QString &desc)
{
    CreateLayout();
    WIDGET_SET_DESC(widget, desc);
    WIDGET_SET_LABEL(widget, label);
    PushLayout(name, widget, label, desc);
    LAYOUT_COUNTER_INCREMENT(m_layout);
    m_nameWidgetMap.insert(name, widget);
    if(m_canExpand)
        setCheckable(true);
}

void NLFormGroupWidget::AddAction(QAction *action)
{
    if(!m_actionButton)
    {
        m_actionButton = new QToolButton(this);
        QMenu *menu = new QMenu(m_actionButton);
        //m_actionButton->setArrowType(Qt::DownArrow);
        m_actionButton->setPopupMode(QToolButton::InstantPopup);
        m_actionButton->setAutoRaise(true);
        m_actionButton->setMenu(menu);
        m_actionButton->setFixedSize(16, 16);
        connect(menu, SIGNAL(triggered(QAction *)), this, SIGNAL(actionTriggered(QAction *)));
    }
    m_actionButton->menu()->addAction(action);
}

void NLFormGroupWidget::SetData(const QVariant &data)
{
    if(m_data != data)
        m_data = data;
}

void NLFormGroupWidget::resizeEvent(QResizeEvent *event)
{
    QGroupBox::resizeEvent(event);
    if(m_actionButton)
        m_actionButton->move(width() - m_actionButton->width() - 2, 0);
}



NLPropFormGroupWidget::NLPropFormGroupWidget(QWidget *widget)
    : NLFormGroupWidget(widget),
      m_object(0)
{
    setObjectName("NLPropFormGroupWidget");
}

NLPropFormGroupWidget::NLPropFormGroupWidget(const QString &title, QWidget *widget)
    : NLFormGroupWidget(title, widget),
      m_object(0)
{
    setObjectName("NLPropFormGroupWidget");
}

NLPropFormGroupWidget::~NLPropFormGroupWidget()
{
}

void NLPropFormGroupWidget::SetObject(QObject *obj)
{
    if(m_object != obj)
    {
        Reset();
        m_object = obj;
        SetupObjectProperty();
    }
}

void NLPropFormGroupWidget::OnObjectChanged()
{
    if(!m_object)
        return;
    qDebug() << "OnObjectChanged";
    NLFormGroupWidget::Reset();
    SetupObjectProperty();
}

void NLPropFormGroupWidget::SetupObjectProperty()
{
    if(!m_object)
        return;
    NLPropertyInfoList list = GetPropertyInfoList(m_object);
    SortProperties(list);
    Q_FOREACH(const NLPropertyInfo &item, list)
    {
        QWidget *widget = GenWidget(m_object, item);
        AddRow(item.name, item.label, widget, item.description);
    }
}

QWidget * NLPropFormGroupWidget::GenWidget(QObject *obj, const NLPropertyInfo &item)
{
    QWidget *widget = 0;

    if(item.widget == "spinbox")
    {
        if(item.type == "int")
        {
            QSpinBox *w = new QSpinBox;
            WIDGET_SET_TYPE(w, QSpinBox);
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
            WIDGET_SET_TYPE(w, QDoubleSpinBox);
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
            WIDGET_SET_TYPE(w, QCheckBox);
            w->setChecked(item.value.toBool());
            w->setCheckable(!item.readonly);
            connect(w, SIGNAL(clicked(bool)), this, SLOT(OnBoolChanged(bool)));
            connect(w, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            widget = w;
        }
        else if(item.type == "int")
        {
            QGroupBox *w = new QGroupBox;
            QButtonGroup *g = new QButtonGroup(w);
            WIDGET_SET_TYPE(g, QButtonGroup);
            g->setObjectName(item.name);
            g->setExclusive(false);
            QVBoxLayout *vbox = new QVBoxLayout;
            const NLPropertyPairList items = item.prop.value("option").value<NLPropertyPairList>();
            int target = item.value.toInt();
            for(int i = 0; i < items.size(); i++)
            {
                const NLPropertyPair &p = items[i];
                int iv = p.second.toInt();
                QCheckBox *cb = new QCheckBox(p.first);
                cb->setProperty("_Data", p.second);
                g->addButton(cb, iv);

                if(target & iv)
                    cb->setChecked(true);
                vbox->addWidget(cb);
            }
            vbox->addStretch(1);

            vbox->setContentsMargins(0, 0, 0, 0);
            w->setLayout(vbox);
            //w->setTitle(ITEM_LABEL(item));
            //w->setCheckable(true);
            connect(g, SIGNAL(buttonClicked(int)), this, SLOT(OnIndexChanged(int)));
            widget = w;
        }
    }
    else if(item.widget == "combobox")
    {
        QComboBox *w = new QComboBox;
        WIDGET_SET_TYPE(w, QComboBox);
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
        WIDGET_SET_TYPE(w, NLVector3Widget);
        w->SetVector3(item.value.value<NLVector3>());
        w->SetReadOnly(item.readonly);
        connect(w, SIGNAL(vector3Changed(const NLVector3 &)), this, SLOT(OnVector3Changed(const NLVector3 &)));
        widget = w;
    }
    else if(item.widget == "lineedit")
    {
        QLineEdit *w = new QLineEdit;
        WIDGET_SET_TYPE(w, QLineEdit);
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
            WIDGET_SET_TYPE(w, QTextEdit);
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
            WIDGET_SET_TYPE(w, NLTextEditWidget);
            w->SetText(item.value.toString());
            w->SetReadOnly(item.readonly);
            w->SetSyntax(item.prop.value("syntax").toString());
            connect(w, SIGNAL(textEdited(const QString &)), this, SLOT(OnStringChanged(const QString &)));
            widget = w;
        }
    }
    else if(item.widget == "filedialog")
    {
        NLFileChooserWidget *w = new NLFileChooserWidget;
        WIDGET_SET_TYPE(w, NLFileChooserWidget);
        w->SetFile(item.value.toString());
        w->SetReadOnly(item.readonly);
        connect(w, SIGNAL(fileChanged(const QString &)), this, SLOT(OnStringChanged(const QString &)));
        connect(w, SIGNAL(fileReload(const QString &)), this, SLOT(OnStringReload(const QString &)));
        widget = w;
    }
    else if(item.widget == "colordialog")
    {
        NLColorChooserWidget *w = new NLColorChooserWidget;
        WIDGET_SET_TYPE(w, NLColorChooserWidget);
        w->SetColor(QColor(item.value.toString()));
        w->SetReadOnly(item.readonly);
        connect(w, SIGNAL(colorChanged(const QColor &)), this, SLOT(OnColorChanged(const QColor &)));
        widget = w;
    }
    else if(item.widget == "label")
    {
        QLabel *w = new QLabel;
        WIDGET_SET_TYPE(w, QLabel);
        w->setText(item.value.toString());
        connect(w, SIGNAL(linkActivated(const QString &)), this, SLOT(OnLinkActivated(const QString &)));
        widget = w;
    }
    else/* if(item.widget == "label")*/
    {
        int type = item.value.type();
        QLineEdit *w = new QLineEdit;
        WIDGET_SET_TYPE(w, QLineEdit);
        if(type == QMetaType::QObjectStar || type == QMetaType::VoidStar)
        {
            if(type == QMetaType::QObjectStar)
            {
                QObject *qo = item.value.value<QObject *>();
                if(qo && nlinstanceofv(qo, NLObject))
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

    if(widget)
        widget->setObjectName(item.name);

    return widget;
}

void NLPropFormGroupWidget::NotifyPropertyChanged(const QString &name, const QVariant &value)
{
    QWidget *widget = WidgetForName(name);
    if(!widget)
        return;
    QString type = WIDGET_TYPE(widget);
    if(WIDGETNAME_IS_TYPE(type, QSpinBox))
    {
        static_cast<QSpinBox *>(widget)->setValue(value.toInt());
    }
    else if(WIDGETNAME_IS_TYPE(type, QDoubleSpinBox))
    {
        static_cast<QDoubleSpinBox *>(widget)->setValue(value.toFloat());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLVector3Widget))
    {
        static_cast<NLVector3Widget *>(widget)->SetVector3(value.value<NLVector3>());
    }
    else if(WIDGETNAME_IS_TYPE(type, QCheckBox))
    {
        static_cast<QCheckBox *>(widget)->setChecked(value.toBool());
    }
    else if(WIDGETNAME_IS_TYPE(type, QLineEdit))
    {
        static_cast<QLineEdit *>(widget)->setText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, QComboBox))
    {
        QComboBox *cb = static_cast<QComboBox *>(widget);
        int index = cb->findData(value);
        if(index >= 0)
            cb->setCurrentIndex(index);
    }
    else if(WIDGETNAME_IS_TYPE(type, QTextEdit))
    {
        static_cast<QTextEdit *>(widget)->setPlainText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, QPushButton))
    {
        static_cast<QPushButton *>(widget)->setText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLFileChooserWidget))
    {
        static_cast<NLFileChooserWidget *>(widget)->SetFile(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLTextEditWidget))
    {
        static_cast<NLTextEditWidget *>(widget)->SetText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLColorChooserWidget))
    {
        static_cast<NLColorChooserWidget *>(widget)->SetColor(value.value<QColor>());
    }
    else
        qWarning() << "Unsupported widget type -> " << type;
}

void NLPropFormGroupWidget::OnIntChanged(int i)
{
    QObject *s = sender();
    SetObjectProperty(m_object, s->objectName(), i);
}

void NLPropFormGroupWidget::OnIndexChanged(int i)
{
    QObject *s = sender();
    QString type = WIDGET_TYPE(s);
    if(WIDGETNAME_IS_TYPE(type, QComboBox))
    {
        QComboBox *cb = static_cast<QComboBox *>(s);
        SetObjectProperty(m_object, s->objectName(), cb->itemData(i));
    }
    else if(WIDGETNAME_IS_TYPE(type, QButtonGroup))
    {
        QButtonGroup *bg = static_cast<QButtonGroup *>(s);
        QAbstractButton *btn = bg->button(i);
        QString name = s->objectName();
        int cs = GetObjectProperty(m_object, name).toInt();
        if(btn->isChecked())
        {
            cs |= i;
        }
        else
        {
            cs &= ~i;
        }
        SetObjectProperty(m_object, name, cs);
    }
}

void NLPropFormGroupWidget::OnDoubleChanged(double f)
{
    QObject *s = sender();
    SetObjectProperty(m_object, s->objectName(), (float)f);
}

void NLPropFormGroupWidget::OnBoolChanged(bool b)
{
    QObject *s = sender();
    SetObjectProperty(m_object, s->objectName(), b);
}

void NLPropFormGroupWidget::OnVector3Changed(const NLVector3 &v)
{
    QObject *s = sender();
    SetObjectProperty(m_object, s->objectName(), QVariant::fromValue<NLVector3>(v));
}

void NLPropFormGroupWidget::OnStringChanged(const QString &str)
{
    QObject *s = sender();
    QString type = WIDGET_TYPE(s);
    if(WIDGETNAME_IS_TYPE(type, QTextEdit))
    {
        QTextEdit *t = static_cast<QTextEdit *>(s);
        bool editing = t->hasFocus();
        if(editing)
        {
            SetObjectProperty(m_object, s->objectName(), t->toPlainText());
        }
    }
    else
        SetObjectProperty(m_object, s->objectName(), str);
}

void NLPropFormGroupWidget::OnStringReload(const QString &str)
{
    QObject *s = sender();
    CoverObjectProperty(m_object, s->objectName(), str);
}

void NLPropFormGroupWidget::OnColorChanged(const QColor &color)
{
    QObject *s = sender();
    SetObjectProperty(m_object, s->objectName(), color);
}

void NLPropFormGroupWidget::OnLinkActivated(const QString &link)
{
    QAction action(0);
    ACTION_SET_TYPE(&action, "link");
    ACTION_SET_WIDGET(&action, QVariant::fromValue<QWidget *>(static_cast<QWidget *>(sender())));
    action.setData(link);
    HandleAction(&action);
}

void NLPropFormGroupWidget::OnItemDestroy(QObject *obj)
{
    QObject *o = obj ? obj : sender();
    if(o)
        qDebug() << "Q ~" + objectName() + "::" + obj->objectName();
}

void NLPropFormGroupWidget::Reset()
{
    if(m_object)
    {
        m_object->disconnect(this);
        m_object = 0;
    }
    NLFormGroupWidget::Reset();
}

