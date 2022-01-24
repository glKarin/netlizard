#include "nlcolorchooserwidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QPushButton>

#include "engine/nldbg.h"

#define DEFAULT_COLOR QColor::fromRgbF(0, 0, 0, 1)

class NLColorChooserWidgetLabel : public QPushButton
{
public:
    explicit NLColorChooserWidgetLabel(QWidget *parent = 0);
    virtual ~NLColorChooserWidgetLabel() {
        NLDEBUG_DESTROY_Q;
    }
    QColor Color() const { return m_color; }
    void SetColor(const QColor &color);

private:
    void Init();

private:
    QColor m_color;
};

NLColorChooserWidgetLabel::NLColorChooserWidgetLabel(QWidget *parent)
    : QPushButton(parent),
      m_color(DEFAULT_COLOR)
{
    setObjectName("NLColorChooserWidgetLabel");
    Init();
}

void NLColorChooserWidgetLabel::Init()
{
    QString target(m_color.name().toUpper());
    setText(target);
    setStyleSheet(QString("QPushButton { color: %1; }").arg(target));
    setToolTip(target);
}

void NLColorChooserWidgetLabel::SetColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        Init();
    }
}



NLColorChooserWidget::NLColorChooserWidget(QWidget *widget)
    : QWidget(widget),
      m_color(DEFAULT_COLOR),
      m_openButton(0)
{
    setObjectName("NLColorChooserWidget");
    Init();
}

NLColorChooserWidget::~NLColorChooserWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLColorChooserWidget::Init()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_openButton = new NLColorChooserWidgetLabel;

    mainLayout->addWidget(m_openButton, 1);

    connect(m_openButton, SIGNAL(clicked()), this, SLOT(OpenColorDialog()));

    UpdateWidget();

    setLayout(mainLayout);
}

void NLColorChooserWidget::UpdateWidget()
{
    m_openButton->SetColor(m_color);
}

void NLColorChooserWidget::SetColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        UpdateWidget();
        emit colorChanged(m_color);
    }
}

void NLColorChooserWidget::SetReadOnly(bool b)
{
    setEnabled(!b);
}

void NLColorChooserWidget::OpenColorDialog()
{
    QColor chooseColor = QColorDialog::getColor(m_color, this);
    if(!chooseColor.isValid())
        return;
    SetColor(chooseColor);
}
