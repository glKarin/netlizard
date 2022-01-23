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
#include <QPlainTextEdit>
#include <QMenu>
#include <QToolButton>
#include <QButtonGroup>
#include <QApplication>

#include "engine/nldbg.h"
#include "engine/nlfuncs.h"
#include "engine/nlrenderable.h"
#include "engine/nlscenecamera.h"
#include "engine/nlscene.h"
#include "nlvector3widget.h"
#include "nlfilechooserwidget.h"
#include "nlcolorchooserwidget.h"
#include "nltexteditwidget.h"
#include "nlbuttongroupwidget.h"
#include "engine/nlobject.h"
#include "engine/nlcomponent.h"
#include "engine/nlactor.h"
#include "engine/nlscript.h"
#include "engine/nlforce.h"
#include "engine/nlrigidbody.h"
#include "utils/nlguiutility.h"
#include "nllineeditwidget.h"
#include "nlmemorypointerwidget.h"

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
#define WIDGET_READONLY_KEY "_Readonly"
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
#define WIDGET_READONLY(w) (w)->property(WIDGET_READONLY_KEY).toBool()
#define WIDGET_SET_READONLY(w, d) (w)->setProperty(WIDGET_READONLY_KEY, d)

#define ACTION_SET_TYPE(w, d) (w)->setProperty(WIDGET_ACTION_TYPE_KEY, d)
#define ACTION_SET_WIDGET(w, d) (w)->setProperty(WIDGET_ACTION_WIDGET_KEY, QVariant::fromValue<QWidget *>(static_cast<QWidget *>(d)))

#define WIDGET_SET_TYPE(w, T) (w)->setProperty(WIDGET_TYPE_KEY, #T)
#define WIDGET_TYPE(w) (w)->property(WIDGET_TYPE_KEY).toString()
#define WIDGETNAME_IS_TYPE(w, T) (w) == #T
#define WIDGET_IS_TYPE(w, T) (WIDGET_TYPE(w)) == #T

class NLFormLabelWidget : public QLabel
{
public:
    enum FormLabelAction_e
    {
        Action_Disable = 0,
        Action_Drag = 1,
        Action_Drop = 1 << 1,
        Action_Drag_And_Drop = 1 | 2
    };

public:
    explicit NLFormLabelWidget(const QString &text, const QString &name, NLFormGroupWidget *parent = 0);
    explicit NLFormLabelWidget(const QString &name, NLFormGroupWidget *parent = 0);
    virtual ~NLFormLabelWidget();
    QString Name() const { return m_name; }
    FormLabelAction_e AllowAction() const { return m_allowAction; }
    void SetAllowAction(FormLabelAction_e a);
    bool IsAllowAction(FormLabelAction_e a) { return m_allowAction & a ? true : false; }

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    //virtual void dragLeaveEvent(QDragLeaveEvent *event);
    //virtual void dragMoveEvent(QDragMoveEvent *event);

private:
    void Init();

private:
    QString m_name;
    NLFormGroupWidget *m_form;
    QPoint m_dragStartPosition;
    FormLabelAction_e m_allowAction;

    friend class NLFormGroupWidget;
};

NLFormLabelWidget::NLFormLabelWidget(const QString &text, const QString &name, NLFormGroupWidget *parent)
    : QLabel(text, parent),
      m_name(name),
      m_form(parent),
      m_allowAction(NLFormLabelWidget::Action_Drag_And_Drop)
{
    setObjectName("NLFormLabelWidget");
    Init();
}

NLFormLabelWidget::NLFormLabelWidget(const QString &name, NLFormGroupWidget *parent)
    : QLabel(parent),
      m_name(name),
      m_form(parent),
      m_allowAction(NLFormLabelWidget::Action_Drag_And_Drop)
{
    setObjectName("NLFormLabelWidget");
    Init();
}

NLFormLabelWidget::~NLFormLabelWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLFormLabelWidget::Init()
{
    if(!m_form->AllowDragDrop())
        SetAllowAction(Action_Disable);
    else
        setAcceptDrops(true);
}

void NLFormLabelWidget::SetAllowAction(FormLabelAction_e a)
{
    if(m_allowAction != a)
    {
        m_allowAction = a;
        setAcceptDrops(IsAllowAction(Action_Drop));
    }
}

void NLFormLabelWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(!IsAllowAction(Action_Drag) || !m_form->AllowDragDrop())
        return;
    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();
}

void NLFormLabelWidget::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    if(!IsAllowAction(Action_Drag) || !m_form->AllowDragDrop())
        return;

    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag *drag = m_form->Drag(m_name, this);
    if(!drag)
        return;

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
    Q_UNUSED(dropAction);
    //delete drag;
}

void NLFormLabelWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(!IsAllowAction(Action_Drop) || !m_form->AllowDragDrop() || event->source() == this)
        return;
    if(m_form->CheckDragData(event->mimeData(), this))
        event->acceptProposedAction();
}

void NLFormLabelWidget::dropEvent(QDropEvent *event)
{
    if(!IsAllowAction(Action_Drop) || !m_form->AllowDragDrop() || event->source() == this)
        return;
    if(m_form->Drop(event->mimeData(), this))
        event->acceptProposedAction();
}



NLFormGroupWidget::NLFormGroupWidget(QWidget *widget)
    : QGroupBox(widget),
      m_layout(0),
      m_actionButton(0),
      m_expand(true),
      m_canExpand(true),
      m_dragDrop(true)
{
    setObjectName("NLPropFormGroupWidget");
    Init();
}

NLFormGroupWidget::NLFormGroupWidget(const QString &title, QWidget *widget)
    : QGroupBox(title, widget),
      m_layout(0),
      m_actionButton(0),
      m_expand(true),
      m_canExpand(true),
      m_dragDrop(true)
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

void NLFormGroupWidget::SetAllowDragDrop(bool on)
{
    if(m_dragDrop != on)
        m_dragDrop = on;
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
            bool readonly = WIDGET_READONLY(widget);
            PushLayout(name, widget, label, desc, readonly);
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

void NLFormGroupWidget::PushLayout(const QString &name, QWidget *widget, const QString &label, const QString &desc, bool readonly)
{
    QString text(LABEL(label, name));
    NLFormLabelWidget *l = new NLFormLabelWidget(text, name, this);
    if(readonly)
        l->SetAllowAction(NLFormLabelWidget::Action_Drag);
    if(!desc.isEmpty())
        l->setToolTip(desc);
    m_layout->addRow(l, widget);
}

void NLFormGroupWidget::AddRow(const QString &name, QWidget *widget, const QString &desc, bool readonly)
{
    AddRow(name, name, widget, desc, readonly);
}

void NLFormGroupWidget::AddRow(const QString &name, const QString &label, QWidget *widget, const QString &desc, bool readonly)
{
    CreateLayout();
    WIDGET_SET_DESC(widget, desc);
    WIDGET_SET_LABEL(widget, label);
    WIDGET_SET_READONLY(widget, readonly);
    PushLayout(name, widget, label, desc, readonly);
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
        AddRow(item.name, item.label, widget, item.description, item.readonly);
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
            NLButtonGroupWidget *w = new NLButtonGroupWidget;
            WIDGET_SET_TYPE(w, NLButtonGroupWidget);
            w->setObjectName(item.name);
            const NLPropertyPairList items = item.prop.value("option").value<NLPropertyPairList>();
            int target = item.value.toInt();
            w->SetBitList(items, target);
            connect(w, SIGNAL(bitResult(int)), this, SLOT(OnIntChanged(int)));
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
            NLLineEditWidget *w = new NLLineEditWidget;
            WIDGET_SET_TYPE(w, NLLineEditWidget);
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
    else
    {
        const QString item_type(item.value.typeName());
        if(item_type.contains("*"))
        {
            QWidget *w = GenMemoryPointerField(item.value);
            WIDGET_SET_TYPE(w, NLTextBrowserWidget);
            widget = w;
        }
        else
        {
            NLTextBrowserWidget *w = new NLTextBrowserWidget;
            WIDGET_SET_TYPE(w, NLTextBrowserWidget);
            w->SetMaxHeight(0);
            w->setReadOnly(true); // TODO: can not edit directly
            w->setLineWrapMode(QTextBrowser::WidgetWidth);
            w->setWordWrapMode(QTextOption::WrapAnywhere);
    //      w->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            w->setPlainText(item.value.toString());
            widget = w;
        }
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
        QString text(value.toString());
        QLineEdit *lineEdit = static_cast<QLineEdit *>(widget);
        if(lineEdit->text() != text)
            lineEdit->setText(text);
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
    else if(WIDGETNAME_IS_TYPE(type, NLLineEditWidget))
    {
        static_cast<NLLineEditWidget *>(widget)->SetText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLColorChooserWidget))
    {
        static_cast<NLColorChooserWidget *>(widget)->SetColor(value.value<QColor>());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLButtonGroupWidget))
    {
        static_cast<NLButtonGroupWidget *>(widget)->SetBit(value.toUInt());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLTextBrowserWidget))
    {
        static_cast<NLTextBrowserWidget *>(widget)->setPlainText(value.toString());
    }
    else if(WIDGETNAME_IS_TYPE(type, NLMemoryPointerWidget))
    {
        //static_cast<NLMemoryPointerWidget *>(widget)->SetPointer(GenEditorFieldString(value));
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
    ACTION_SET_WIDGET(&action, sender());
    action.setData(link);
    HandleAction(&action);
}

void NLPropFormGroupWidget::OnItemDestroy(QObject *obj)
{
    QObject *o = obj ? obj : sender();
    if(o)
        qDebug() << "Q ~" + objectName() + "::" + o->objectName();
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


QDrag * NLPropFormGroupWidget::Drag(const QString &name, QWidget *widget)
{
    QDrag *drag = new QDrag(widget);
    QVariant value = GetObjectProperty(m_object, name);
    QMimeData *mimeData = new QMimeData;
    mimeData->setProperty(NL_FORM_WIDGET_DRAG_DROP_DATA_KEY, value);
    mimeData->setData(NL_FORM_WIDGET_DRAG_DROP_MIME, name.toLocal8Bit());
    drag->setMimeData(mimeData);
    return drag;
}

bool NLPropFormGroupWidget::Drop(const QMimeData *d, QWidget *widget)
{
    QString nameSelf = static_cast<NLFormLabelWidget *>(widget)->Name();
    QVariant v = d->property(NL_FORM_WIDGET_DRAG_DROP_DATA_KEY);
    SetObjectProperty(m_object, nameSelf, v);
    return true;
}

bool NLPropFormGroupWidget::CheckDragData(const QMimeData *d, QWidget *widget)
{
    if (!d->hasFormat(NL_FORM_WIDGET_DRAG_DROP_MIME))
        return false;
//    QByteArray name = d->data(NL_FORM_WIDGET_DRAG_DROP_MIME);
//    if(name.isEmpty())
//        return false;
    QString nameSelf = static_cast<NLFormLabelWidget *>(widget)->Name();
    QVariant v = d->property(NL_FORM_WIDGET_DRAG_DROP_DATA_KEY);
    QVariant vSelf = GetObjectProperty(m_object, nameSelf);
    if(qstrcmp(v.typeName(), vSelf.typeName()) != 0) // not same type
        return false;
    return true;
}

QWidget * NLPropFormGroupWidget::GenMemoryPointerField(const QVariant &item_value)
{
    NLMemoryPointerWidget *widget = new NLMemoryPointerWidget;
    const QString item_type(item_value.typeName());
    void *ptr = 0;
    QString name;
    QString typeName(item_type);
    const int type = item_value.type();

    if(type == QMetaType::QObjectStar)
    {
        QObject *qo = item_value.value<QObject *>();
        typeName = "QObject*";
        if(qo)
        {
            if(nlinstanceofv(qo, NLObject))
            {
                NLObject *nlo = static_cast<NLObject *>(qo);
                name = nlo ? nlo->ClassName() + "::" + nlo->objectName() + "(" + nlo->Name() +")" + "(" + QString::number(static_cast<int>(nlo->Type())) +")" : "";
                typeName = "NLObject*";
            }
            else
                name = qo ? qo->objectName() : "";
        }
        ptr = qo;
    }
    else if(type == QMetaType::QWidgetStar)
    {
        QWidget *qo = item_value.value<QWidget *>();
        typeName = "QWidget*";
        if(qo)
        {
            if(nlinstanceofv(qo, NLScene))
            {
                NLScene *nlo = static_cast<NLScene *>(qo);
                name = nlo ? nlo->objectName() : "";
                typeName = "NLScene*";
            }
            else
                name = qo ? qo->objectName() : "";
        }
        ptr = qo;
    }
    else if(type == QMetaType::VoidStar)
    {
        void *vo = item_value.value<void *>();
        ptr = vo;
        typeName = "void*";
    }
    else
    {
        if(item_type == "NLRenderable*")
        {
            NLRenderable *renderable = item_value.value<NLRenderable *>();
            name = renderable ? renderable->Name() : "";
            typeName = "NLRenderable*";
            ptr = renderable;
        }
        else if(item_type == "NLScene*")
        {
            NLScene *scene = item_value.value<NLScene *>();
            name = scene ? scene->objectName() : "";
            typeName = "NLScene*";
            ptr = scene;
        }
        else if(item_type == "NLSceneCamera*")
        {
            NLSceneCamera *camera = item_value.value<NLSceneCamera *>();
            typeName = "NLSceneCamera*";
            ptr = camera;
        }
#define NLOBJECT_SHOW(T) \
        if(item_type == #T "*") { \
            T *nlo = item_value.value<T *>(); \
            name = nlo ? nlo->ClassName() + "::" + nlo->objectName() + "(" + nlo->Name() +")" : ""; \
            typeName = #T "*"; \
            ptr = nlo; \
        }
        else NLOBJECT_SHOW(NLObject)
        else NLOBJECT_SHOW(NLActor)
        else NLOBJECT_SHOW(NLRigidbody)
        else NLOBJECT_SHOW(NLComponent)
        else NLOBJECT_SHOW(NLScript)
        else NLOBJECT_SHOW(NLForce)
#undef NLOBJECT_SHOW
        else if(item_type == "NLVariantGeneralPointer")
        {
            NLVariantGeneralPointer p = item_value.value<NLVariantGeneralPointer>();
            typeName = QString(p.name) + "*";
            ptr = p.ptr;
        }
        else
        {
        }
    }

    int i = typeName.lastIndexOf("*");
    if(i >= 0)
    {
        typeName.remove(i, 1);
    }
    widget->SetMemoryPointer(typeName, ptr);
    return widget;
}
