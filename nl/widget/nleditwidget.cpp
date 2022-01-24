#include "nleditwidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>

#include "engine/nldbg.h"
#include "nltexteditdialog.h"
#include "nllineeditwidget.h"

NLEditWidget::NLEditWidget(QWidget *widget)
    : QWidget(widget),
      m_textLabel(0),
      m_editButton(0)
{
    setObjectName("NLEditWidget");
    Init();
}

NLEditWidget::~NLEditWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLEditWidget::Init()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_textLabel = new NLLineEditWidget;
    m_editButton = new QPushButton(tr("Edit"));

    QMargins margins = mainLayout->contentsMargins();
    margins.setLeft(0);
    margins.setRight(0);
    mainLayout->setContentsMargins(margins);
    m_textLabel->setPlaceholderText(tr("Click button or double click input to edit"));

    mainLayout->addWidget(m_textLabel, 1);
    mainLayout->addWidget(m_editButton);

    m_textLabel->setReadOnly(true);
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(OpenTextEditor()));
    connect(m_textLabel, SIGNAL(dblClicked()), this, SLOT(OpenTextEditor()));

    UpdateWidget();

    setLayout(mainLayout);
}

void NLEditWidget::UpdateWidget()
{
    m_textLabel->setText(m_text);
    m_editButton->setToolTip(m_text);
    m_textLabel->setToolTip(m_text);
}

void NLEditWidget::SetText(const QString &t)
{
    if(m_text != t)
    {
        m_text = t;
        UpdateWidget();
        emit textChanged(m_text);
    }
}

void NLEditWidget::SetEditText(const QString &t)
{
    if(m_text != t)
    {
        m_text = t;
        UpdateWidget();
        emit textChanged(m_text);
        emit textEdited(m_text);
    }
}

void NLEditWidget::SetReadOnly(bool b)
{
    setEnabled(!b);
}

void NLEditWidget::OpenTextEditor()
{
    QString text = NLTextEditDialog::Edit(m_text, m_syntax, this);
    if(text.isNull())
        return;
    SetEditText(text);
}

void NLEditWidget::SetSyntax(const QString &type)
{
    if(m_syntax != type)
        m_syntax = type;
}
