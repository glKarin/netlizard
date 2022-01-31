#include "nlfilechooserwidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QUrl>
#include <QTimer>

#include "common/nldbg.h"
#include "nllineeditwidget.h"

NLFileChooserWidget::NLFileChooserWidget(QWidget *widget)
    : QWidget(widget),
      m_fileLabel(0),
      m_openButton(0)
      //,m_reloadButton(0)
{
    setObjectName("NLFileChooserWidget");
    Init();
}

NLFileChooserWidget::~NLFileChooserWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLFileChooserWidget::Init()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_fileLabel = new NLLineEditWidget;
    m_openButton = new QPushButton(tr("Open"));
    //m_reloadButton = new QPushButton(tr("Reload"));

    //m_reloadButton->setEnabled(false);
    QMargins margins = mainLayout->contentsMargins();
    margins.setLeft(0);
    margins.setRight(0);
    mainLayout->setContentsMargins(margins);
    m_fileLabel->setPlaceholderText(tr("Click button to choose file or input path directly"));

    mainLayout->addWidget(m_fileLabel, 1);
    mainLayout->addWidget(m_openButton);
    //mainLayout->addWidget(m_reloadButton);

    //connect(m_fileLabel, SIGNAL(textEdited(const QString &)), this, SLOT(SetFile(const QString &)));
    connect(m_openButton, SIGNAL(clicked()), this, SLOT(OpenFileDialog()));
    connect(m_fileLabel, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
    connect(m_fileLabel, SIGNAL(dblClicked()), this, SLOT(EditOrChooseFile()));

    UpdateWidget();
    setAcceptDrops(true);

    setLayout(mainLayout);
}

void NLFileChooserWidget::UpdateWidget()
{
    m_fileLabel->setText(m_file);
    m_openButton->setToolTip(m_file);
    m_fileLabel->setToolTip(m_file);
    //m_reloadButton->setEnabled(!m_file.isEmpty());
    //m_reloadButton->setToolTip(m_file);
}

void NLFileChooserWidget::SetFile(const QString &f)
{
    if(m_file != f)
    {
        m_file = f;
        UpdateWidget();
        emit fileChanged(m_file);
    }
}

void NLFileChooserWidget::SetReadOnly(bool b)
{
    setEnabled(!b);
}

void NLFileChooserWidget::OpenFileDialog()
{
    QString dir;
    if(!m_file.isEmpty())
    {
        QFileInfo info(m_file);
        if(info.exists())
            dir = info.absolutePath();
    }
    QString chooseFileName = QFileDialog::getOpenFileName(this, dir);
    if(chooseFileName.isEmpty())
        return;
    SetFile(chooseFileName);
}

void NLFileChooserWidget::OnEditFinished()
{
    QTimer::singleShot(0, this, SLOT(OpenFile())); // !! can not call OpenFile directly
}

void NLFileChooserWidget::OpenFile()
{
    QString str(m_fileLabel->text());
    if(m_file != str)
    {
        SetFile(str);
    }
    else
    {
        if(!m_file.isEmpty())
            emit fileReload(m_file);
    }
}

void NLFileChooserWidget::EditOrChooseFile()
{
    if(m_fileLabel->text().isEmpty())
        OpenFileDialog();
}

void NLFileChooserWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(!isEnabled())
        return;
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void NLFileChooserWidget::dropEvent(QDropEvent *event)
{
    if(!isEnabled())
        return;
    const QMimeData *mimedata = event->mimeData();
    QList<QUrl> urls = mimedata->urls();
    if(!urls.isEmpty())
    {
        const QUrl &url = urls[0];
        if(url.scheme() == "file")
        {
            SetFile(url.toLocalFile());
            event->acceptProposedAction();
        }
    }
}
