#include "nlvector3widget.h"

#include <QDebug>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <limits>

#include "common/nldbg.h"

#define DOUBLE_SPINBOX_SINGLE_STEP 1 //0.1
#define DOUBLE_SPINBOX_DECIMAL 6

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
    NLDEBUG_DESTROY_Q
}

void NLVector3Widget::Init()
{
    QGridLayout *mainLayout = new QGridLayout;
    QFormLayout *layout;
    m_xSpinBox = new QDoubleSpinBox;
    m_ySpinBox = new QDoubleSpinBox;
    m_zSpinBox = new QDoubleSpinBox;
#if 0
    const float Max = std::numeric_limits<float>::max();
    const float Min = -std::numeric_limits<float>::max();
#else
    const float Max = std::numeric_limits<int>::max();
    const float Min = std::numeric_limits<int>::min();
#endif
    m_xSpinBox->setMaximum(Max);
    m_ySpinBox->setMaximum(Max);
    m_zSpinBox->setMaximum(Max);
    m_xSpinBox->setMinimum(Min);
    m_ySpinBox->setMinimum(Min);
    m_zSpinBox->setMinimum(Min);
    m_xSpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_ySpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_zSpinBox->setDecimals(DOUBLE_SPINBOX_DECIMAL);
    m_xSpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);
    m_ySpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);
    m_zSpinBox->setSingleStep(DOUBLE_SPINBOX_SINGLE_STEP);

    Qt::Alignment align = Qt::AlignVCenter | Qt::AlignRight;
    m_xSpinBox->setAlignment(align);
    m_ySpinBox->setAlignment(align);
    m_zSpinBox->setAlignment(align);

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
    setEnabled(!b);
}
