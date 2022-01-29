#include "nlmemorypointerwidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDragEnterEvent>
#include <QUrl>
#include <QMenu>
#include <QRegExpValidator>

#include "nllineeditwidget.h"
#include "nllabelwidget.h"
#include "common/nldbg.h"

NLMemoryPointerWidget::NLMemoryPointerWidget(QWidget *widget)
    : QWidget(widget),
      m_typeName("void"),
      m_ptr(0),
      m_typeLabel(0),
      m_pointerLabel(0),
      m_nameLabel(0),
      m_menu(0)
{
    setObjectName("NLMemoryPointerWidget");
    Init();
}

NLMemoryPointerWidget::~NLMemoryPointerWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLMemoryPointerWidget::Init()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_typeLabel = new QLabel;
    m_nameLabel = new NLLabelWidget;
    m_pointerLabel = new NLLineEditWidget(this);
    QLabel *label = new QLabel("0x");
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("[0-9a-f]*"), m_pointerLabel);

    m_pointerLabel->setValidator(validator);
    m_nameLabel->SetElideMode(Qt::ElideMiddle);
    //m_pointerLabel->SetDoubleClickEdit(true);
    m_pointerLabel->setReadOnly(true);
    m_pointerLabel->SetAutoSize(true);
    m_pointerLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_typeLabel->setScaledContents(true);
    m_nameLabel->setMaximumWidth(240);
    QMargins margins = mainLayout->contentsMargins();
    margins.setLeft(0);
    margins.setRight(0);
    mainLayout->setContentsMargins(margins);
    margins = label->contentsMargins();
    margins.setRight(0);
    margins.setLeft(8);
    label->setContentsMargins(margins);
    margins = m_pointerLabel->textMargins();
    margins.setLeft(0);
    m_pointerLabel->setTextMargins(margins);

    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_typeLabel);
    mainLayout->addWidget(label);
    mainLayout->addWidget(m_pointerLabel);
    mainLayout->addWidget(m_nameLabel);
    mainLayout->addStretch(1);

    //connect(m_pointerLabel, SIGNAL(editingFinished()), this, SLOT(OpenFile()));

    UpdateWidget();
    setAcceptDrops(true);

    setLayout(mainLayout);
}


QString NLMemoryPointerWidget::GenPtrStr(void *ptr, int cs, bool prefix) const
{
    QString ptrStr = QString().sprintf("%p", ptr);
    ptrStr = ptrStr.right(ptrStr.length() - 2);
    int len = ptrStr.length();
    if(ptr && len < 8)
        ptrStr.insert(0, QString().fill('0', 8 - len));
    if(cs == 2)
        ptrStr = ptrStr.toUpper();
    if(prefix)
        ptrStr.insert(0, cs == 2 ? "0X" : "0x");
    return ptrStr;
}

void NLMemoryPointerWidget::UpdateWidget()
{
    QString typeName = m_typeName + "*";
    m_typeLabel->setText(typeName);
    m_typeLabel->setToolTip(typeName);
    QString ptrStr = GenPtrStr(m_ptr);
    m_pointerLabel->setText(ptrStr);
    m_pointerLabel->setToolTip("0x" + ptrStr);
    m_nameLabel->setText(GetPtrName(m_typeName, m_ptr));
}

void NLMemoryPointerWidget::SetMemoryPointer(const QString &type, void *ptr)
{
    SetTypeName(type);
    SetPointer(ptr);
}

void NLMemoryPointerWidget::SetMemoryPointer(const QVariant &value)
{
    QString typeName;
    void *ptr;
    if(FromVariant(value, typeName, ptr))
        SetMemoryPointer(typeName, ptr);
}

bool NLMemoryPointerWidget::FromVariant(const QVariant &item_value, QString &typeName, void *&ptr)
{
    if(!item_value.isValid())
        return false;
    int type = item_value.type();
    if(type == QMetaType::QObjectStar)
    {
        QObject *qo = item_value.value<QObject *>();
        typeName = "QObject";
        ptr = qo;
    }
    else if(type == QMetaType::QWidgetStar)
    {
        QWidget *qo = item_value.value<QWidget *>();
        typeName = "QWidget";
        ptr = qo;
    }
    else if(type == QMetaType::VoidStar)
    {
        void *vo = item_value.value<void *>();
        typeName = "void";
        ptr = vo;
    }
    else
        return false;
    return true;
}

void NLMemoryPointerWidget::SetPointer(void *ptr)
{
    if(m_ptr != ptr)
    {
        m_ptr = ptr;
        UpdateWidget();
        emit memoryPointerChanged(m_typeName, m_ptr);
    }
}

void NLMemoryPointerWidget::SetTypeName(const QString &type)
{
    if(m_typeName != type)
    {
        m_typeName = type;
        UpdateWidget();
        emit memoryPointerChanged(m_typeName, m_ptr);
    }
}

void NLMemoryPointerWidget::SetReadOnly(bool b)
{
    setEnabled(!b);
}

#define ACTION_INVALID -2
#define ACTION_RESET 0
#define ACTION_FREE -1
int NLMemoryPointerWidget::ShowMenu(const QPoint &pos)
{
    if(!m_menu)
    {
        m_menu = new QMenu(this);
        m_menu->addAction(tr("Reset null"))->setData(ACTION_RESET);
        m_menu->addAction(tr("Free"))->setData(ACTION_FREE);
    }
    QAction *action = m_menu->exec(pos);
    if(action)
        return action->data().toInt();
    return ACTION_INVALID;
}

void NLMemoryPointerWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(!m_ptr)
    {
        QWidget::contextMenuEvent(event);
        return;
    }
    int action = ShowMenu(event->globalPos());
    if(action == ACTION_INVALID)
    {
        QWidget::contextMenuEvent(event);
        return;
    }
    if(action == ACTION_RESET)
    {
        void *ptr = m_ptr;
        m_ptr = 0;
        UpdateWidget();
        emit memoryPointerChanged(m_typeName, ptr);
        event->accept();
    }
    else if(action == ACTION_FREE)
    {
        void *ptr = m_ptr;
        m_ptr = 0;
        UpdateWidget();
        emit memoryPointerDeleted(m_typeName, ptr);
        event->accept();
    }
    else
    {
        QWidget::contextMenuEvent(event);
    }
}

void NLMemoryPointerWidget::dragEnterEvent(QDragEnterEvent *event)
{
//    if(!isEnabled())
//        return;
//    if(event->mimeData()->hasUrls())
//        event->acceptProposedAction();
}

void NLMemoryPointerWidget::dropEvent(QDropEvent *event)
{
//    if(!isEnabled())
//        return;
//    const QMimeData *mimedata = event->mimeData();
//    QList<QUrl> urls = mimedata->urls();
//    if(!urls.isEmpty())
//    {
//        const QUrl &url = urls[0];
//        if(url.scheme() == "file")
//        {
//            SetFile(url.toLocalFile());
//            event->acceptProposedAction();
//        }
//    }
}
