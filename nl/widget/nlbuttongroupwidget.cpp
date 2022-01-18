#include "nlbuttongroupwidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>

#include "engine/nldbg.h"
#include "template/nlsequencemap.h"
#include "utils/nlguiutility.h"

#define DATA_KEY "_Data"
#define BIT_KEY "_Bit"
#define SET_DATA(cb, v) (cb)->setProperty(DATA_KEY, (v))
#define GET_DATA(cb) (cb)->property(DATA_KEY)
#define SET_BIT(cb, v) (cb)->setProperty(BIT_KEY, (v).toUInt())
#define GET_BIT(cb) (cb)->property(BIT_KEY).toUInt()

NLButtonGroupWidget::NLButtonGroupWidget(QWidget *widget)
    : QGroupBox(widget),
      m_layout(0),
      m_buttonGroup(0),
      m_bitResult(0)
{
    setObjectName("NLPropFormGroupWidget");
    Init();
}

NLButtonGroupWidget::NLButtonGroupWidget(const QString &title, QWidget *widget)
    : QGroupBox(title, widget),
      m_layout(0),
      m_buttonGroup(0),
      m_bitResult(0)
{
    setObjectName("NLButtonGroupWidget");
    Init();
}

NLButtonGroupWidget::~NLButtonGroupWidget()
{
    NLDEBUG_DESTROY_Q
}

void NLButtonGroupWidget::Init()
{
    m_layout = new QVBoxLayout;
    m_buttonGroup = new QButtonGroup(this);

    m_buttonGroup->setExclusive(false);
    setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);

    setCheckable(false);
    setChecked(false);
    //connect(this, SIGNAL(toggled(bool)), this, SLOT(ToggleGroupBox(bool)));
}

void NLButtonGroupWidget::Reset()
{
    m_buttonGroup->disconnect(this);
    QList<QAbstractButton *> buttons = m_buttonGroup->buttons();
    ;
    Q_FOREACH(QAbstractButton *b, buttons)
        m_buttonGroup->removeButton(b);
    NLGUIUtility::ClearLayout(m_layout);
    m_bitResult = 0;
    m_listResult.clear();
    m_indexResult.clear();
    setChecked(false);
    setCheckable(false);
}

void NLButtonGroupWidget::SetBitList(const NLPropertyPairList &list, uint init)
{
    SetList(list);
    SetBit(init);
}

void NLButtonGroupWidget::SetIndexList(const NLPropertyPairList &list, const QSet<uint> &init)
{
    SetList(list);
    SetIndex(init);
}

void NLButtonGroupWidget::SetDataList(const NLPropertyPairList &list, const QVariantList &init)
{
    SetList(list);
    SetData(init);
}

void NLButtonGroupWidget::SetList(const NLPropertyPairList &list)
{
    Reset();
    int i = 0;
    Q_FOREACH(const NLPropertyPair &p, list)
    {
        QCheckBox *cb = new QCheckBox(p.first);
        SET_DATA(cb, p.second);
        SET_BIT(cb, p.second);
        m_buttonGroup->addButton(cb, i);
        m_layout->addWidget(cb);
        i++;
    }
    m_layout->addStretch(1);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClicked(int)));
}

void NLButtonGroupWidget::SetBit(uint init)
{
    m_bitResult = 0;
    m_listResult.clear();
    m_indexResult.clear();
    QList<QAbstractButton *> buttons = m_buttonGroup->buttons();
    int i = 0;
    Q_FOREACH(QAbstractButton *b, buttons)
    {
        uint m = GET_BIT(b);
        QVariant d = GET_DATA(b);
        if(init & m)
        {
            b->setChecked(true);
            m_bitResult |= m;
            m_indexResult.push_back(i);
            m_listResult.push_back(d);
        }
        else
        {
            b->setChecked(false);
        }
        i++;
    }
}

void NLButtonGroupWidget::SetIndex(const QSet<uint> &init)
{
    m_bitResult = 0;
    m_listResult.clear();
    m_indexResult.clear();
    QList<QAbstractButton *> buttons = m_buttonGroup->buttons();
    int i = 0;
    Q_FOREACH(QAbstractButton *b, buttons)
    {
        uint m = GET_BIT(b);
        QVariant d = GET_DATA(b);
        if(init.contains(i))
        {
            b->setChecked(true);
            m_bitResult |= m;
            m_indexResult.push_back(i);
            m_listResult.push_back(d);
        }
        else
        {
            b->setChecked(false);
        }
        i++;
    }
}

void NLButtonGroupWidget::SetData(const QVariantList &init)
{
    m_bitResult = 0;
    m_listResult.clear();
    m_indexResult.clear();
    QList<QAbstractButton *> buttons = m_buttonGroup->buttons();
    int i = 0;
    Q_FOREACH(QAbstractButton *b, buttons)
    {
        uint m = GET_BIT(b);
        QVariant d = GET_DATA(b);
        if(init.contains(d))
        {
            b->setChecked(true);
            m_bitResult |= m;
            m_indexResult.push_back(i);
            m_listResult.push_back(d);
        }
        else
        {
            b->setChecked(false);
        }
        i++;
    }
}

void NLButtonGroupWidget::OnButtonClicked(int id)
{
    QAbstractButton *cb = m_buttonGroup->button(id);
    QVariant data = GET_DATA(cb);
    uint bit = GET_BIT(cb);
    int index = m_buttonGroup->buttons().indexOf(cb);
    if(cb->isChecked())
    {
        m_bitResult |= bit;
        m_listResult.push_back(data);
        m_indexResult.push_back(index);
    }
    else
    {
        m_bitResult &= (~bit);
        m_listResult.removeAll(data);
        m_indexResult.removeAll(index);
    }
    emit indexResult(m_indexResult);
    emit listResult(m_listResult);
    emit bitResult(m_bitResult);
    emit result();
}
