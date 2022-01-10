#include "nlpropsectionwidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QToolButton>

#include "qdef.h"

NLPropSectionHeader::NLPropSectionHeader(QWidget *widget)
    : QWidget(widget),
      m_label(0),
      m_menu(0),
      m_expandButton(0)
{
    setObjectName("NLPropSectionHeader");
    Init();
}

NLPropSectionHeader::NLPropSectionHeader(const QString &text, QWidget *widget)
    : QWidget(widget),
      m_label(0),
      m_menu(0),
      m_expandButton(0)
{
    setObjectName("NLPropSectionHeader");
    Init(text);
}

NLPropSectionHeader::~NLPropSectionHeader()
{
    DEBUG_DESTROY_Q
}

void NLPropSectionHeader::AddAction(QAction *action)
{
    if(!m_menu)
    {
        QToolButton *button = new QToolButton;
        m_menu = new QMenu(button);
        layout()->addWidget(button);
        //button->setArrowType(Qt::DownArrow);
        button->setPopupMode(QToolButton::InstantPopup);
        button->setAutoRaise(true);
        button->setMenu(m_menu);
        connect(m_menu, SIGNAL(triggered(QAction *)), this, SIGNAL(actionTriggered(QAction *)));
    }
    m_menu->addAction(action);
}

void NLPropSectionHeader::SetText(const QString &text)
{
    m_label->setText(text);
}

void NLPropSectionHeader::Init(const QString &text)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_label = new QLabel(text);
    m_expandButton = new QToolButton;

    m_expandButton->setArrowType(Qt::DownArrow);
    m_expandButton->setCheckable(true);
    m_expandButton->setChecked(true);
    m_expandButton->setStyleSheet("QToolButton{ border: none; }");
    connect(m_expandButton, SIGNAL(clicked(bool)), this, SLOT(OnToggleExpand(bool)));
    mainLayout->addWidget(m_expandButton);

    mainLayout->addWidget(m_label, 1);
    QMargins margins = mainLayout->contentsMargins();
    margins.setBottom(0);
    margins.setTop(0);
    mainLayout->setContentsMargins(margins);

    setLayout(mainLayout);
}

void NLPropSectionHeader::OnToggleExpand(bool on)
{
    SetExpand(on);
    emit toggleExpand(on);
}

void NLPropSectionHeader::Reset()
{
    SetExpand(true);
    SetCanExpand(false);
}

void NLPropSectionHeader::SetCanExpand(bool b)
{
    m_expandButton->setEnabled(b);
    m_expandButton->setChecked(b);
}

void NLPropSectionHeader::SetExpand(bool b)
{
    m_expandButton->setArrowType(b ? Qt::DownArrow : Qt::RightArrow);
}



NLPropSectionContent::NLPropSectionContent(QWidget *widget)
    : QWidget(widget),
      m_layout(0),
      m_expand(true)
{
    setObjectName("NLPropSectionContent");
    Init();
}

NLPropSectionContent::~NLPropSectionContent()
{
    DEBUG_DESTROY_Q
    Reset();
}

void NLPropSectionContent::Init()
{
    m_layout = new QVBoxLayout;
    QMargins margins = m_layout->contentsMargins();
    margins.setBottom(0);
    margins.setTop(0);
    m_layout->setContentsMargins(margins);
    setLayout(m_layout);
}

void NLPropSectionContent::AddWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
    m_layout->addSpacing(1);
    m_widgets.push_back(widget);
}

void NLPropSectionContent::Toggle(bool on)
{
    if(m_expand == on)
        return;
    if(on)
    {
        Q_FOREACH(QWidget *widget, m_widgets)
        {
            widget->show();
            m_layout->addWidget(widget);
            m_layout->addSpacing(1);
        }
    }
    else
    {
        while(!m_layout->isEmpty())
        {
            QLayoutItem *item = m_layout->takeAt(0);
            QWidget *widget = item->widget();
            if(widget)
                widget->hide();
            delete item;
        }
    }
    m_expand = on;
}

void NLPropSectionContent::Reset()
{
    QLayoutItem *item;
    while(!m_layout->isEmpty())
    {
        item = m_layout->takeAt(0);
        QWidget *widget = item->widget();
        if(widget)
        {
            widget->deleteLater();
            // delete widget; // because maybe deleted by itself.
        }
        delete item;
    }
    m_widgets.clear();
    m_expand = true;
}



NLPropSection::NLPropSection(QWidget *widget)
    : QWidget(widget),
      m_header(0),
      m_content(0)
{
    setObjectName("NLPropSection");
    Init();
}

NLPropSection::~NLPropSection()
{
    DEBUG_DESTROY_Q
    Reset();
}

void NLPropSection::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_header = new NLPropSectionHeader;
    m_content = new NLPropSectionContent;
    layout->addWidget(m_header);
    layout->addSpacing(1);
    layout->addWidget(m_content, 1);
    QMargins margins = layout->contentsMargins();
    margins.setBottom(0);
    margins.setTop(0);
    layout->setContentsMargins(margins);
    setLayout(layout);
    connect(m_header, SIGNAL(actionTriggered(QAction *)), this, SIGNAL(actionTriggered(QAction *)));
    connect(m_header, SIGNAL(toggleExpand(bool)), m_content, SLOT(Toggle(bool)));
}

void NLPropSection::AddWidget(QWidget *widget)
{
    m_content->AddWidget(widget);
    m_header->SetCanExpand(true);
}

void NLPropSection::Reset()
{
    m_header->Reset();
    m_content->Reset();
}

void NLPropSection::SetTitle(const QString &str)
{
    m_header->SetText(str);
}

void NLPropSection::AddAction(QAction *action)
{
    m_header->AddAction(action);
}
