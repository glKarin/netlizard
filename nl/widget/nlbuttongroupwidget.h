#ifndef _KARIN_NLBUTTONGROUPWIDGET_H
#define _KARIN_NLBUTTONGROUPWIDGET_H

#include <QGroupBox>

#include "engine/nlproperties.h"
#include "engine/nldef.h"

class QVBoxLayout;
class QButtonGroup;

class NLLIB_EXPORT NLButtonGroupWidget : public QGroupBox
{
    Q_OBJECT
public:
    enum ResultType_e
    {
        ResultType_Index = 1,
        ResultType_List = 2,
        ResultType_Bit = 3
    };

public:
    explicit NLButtonGroupWidget(QWidget *widget = 0);
    explicit NLButtonGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLButtonGroupWidget();
    void SetList(const NLPropertyPairList &list);
    void SetBitList(const NLPropertyPairList &list, uint init);
    void SetIndexList(const NLPropertyPairList &list, const QSet<uint> &init);
    void SetDataList(const NLPropertyPairList &list, const QVariantList &init);
    void SetBit(uint init);
    void SetIndex(const QSet<uint> &init);
    void SetData(const QVariantList &init);
    int BitResult() const { return m_bitResult; }
    QVariantList ListResult() const { return m_listResult; }
    QVariantList IndexResult() const { return m_indexResult; }

Q_SIGNALS:
    void result();
    void bitResult(int result);
    void listResult(const QVariantList &result);
    void indexResult(const QVariantList &result);

public Q_SLOTS:
    virtual void Reset();

protected:

private:
    void Init();

private Q_SLOTS:
    void OnButtonClicked(int id);

private:
    QVBoxLayout *m_layout;
    QButtonGroup *m_buttonGroup;
    uint m_bitResult;
    QVariantList m_listResult;
    QVariantList m_indexResult;

    Q_DISABLE_COPY(NLButtonGroupWidget)
};

#endif // _KARIN_NLBUTTONGROUPWIDGET_H
