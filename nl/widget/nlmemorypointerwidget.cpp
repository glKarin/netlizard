#include "nlmemorypointerwidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDragEnterEvent>
#include <QUrl>

#include "engine/nldbg.h"

class NLMemoryPointerWidgetLabel : public QLineEdit
{
public:
   explicit  NLMemoryPointerWidgetLabel(NLMemoryPointerWidget *widget, QWidget *parent = 0)
        : QLineEdit(parent),
          m_pointerWidget(widget)
    {
        setObjectName("NLMemoryPointerWidgetLabel");
    }
    virtual ~NLMemoryPointerWidgetLabel() {
        NLDEBUG_DESTROY_Q
    }

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) {
        QLineEdit::mouseDoubleClickEvent(event);
        //emit dblClicked();
    }

private:
    NLMemoryPointerWidget *m_pointerWidget;

    friend class NLMemoryPointerWidget;
};



NLMemoryPointerWidget::NLMemoryPointerWidget(QWidget *widget)
    : QWidget(widget),
      m_typeName("void"),
      m_ptr(0),
      m_typeLabel(0),
      m_fileLabel(0)
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
    m_fileLabel = new NLMemoryPointerWidgetLabel(this);
    QLabel *label = new QLabel("0x");

    m_typeLabel->setScaledContents(true);
    QMargins margins = mainLayout->contentsMargins();
    margins.setLeft(0);
    margins.setRight(0);
    mainLayout->setContentsMargins(margins);
    margins = label->contentsMargins();
    margins.setRight(0);
    margins.setLeft(8);
    label->setContentsMargins(margins);

    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_typeLabel);
    mainLayout->addWidget(label);
    mainLayout->addWidget(m_fileLabel, 1);

    //connect(m_fileLabel, SIGNAL(editingFinished()), this, SLOT(OpenFile()));

    UpdateWidget();
    setAcceptDrops(true);

    setLayout(mainLayout);
}

void NLMemoryPointerWidget::UpdateWidget()
{
    QString typeName = m_typeName + " *";
    m_typeLabel->setText(typeName);
    m_typeLabel->setToolTip(typeName);
    QString ptrName(QString::number(reinterpret_cast<uintptr_t>(m_ptr), 16));
    m_fileLabel->setText(ptrName);
    m_fileLabel->setToolTip("0x" + ptrName);
}

void NLMemoryPointerWidget::SetMemoryPointer(const QString &type, void *ptr)
{
    SetTypeName(type);
    SetPointer(ptr);
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
