#ifndef _KARIN_NLBUTTONGROUPWIDGET_H
#define _KARIN_NLBUTTONGROUPWIDGET_H

#include <QGroupBox>
#include <QList>

#include "engine/nldef.h"

class QVBoxLayout;
template <class K, class V> class NLSequenceHash;
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
    void SetList(const NLSequenceHash<QString, QVariant> &hash);
    void SetBitList(const NLSequenceHash<QString, QVariant> &hash, uint init);
    void SetIndexList(const NLSequenceHash<QString, QVariant> &hash, const QSet<uint> &init);
    void SetDataList(const NLSequenceHash<QString, QVariant> &hash, const QVariantList &init);
    void SetBit(uint init);
    void SetIndex(const QSet<uint> &init);
    void SetData(const QVariantList &init);
    uint BitResult() const { return m_bitResult; }
    QVariantList ListResult() const { return m_listResult; }
    QVariantList IndexResult() const { return m_indexResult; }

Q_SIGNALS:
    void result();
    void bitResult(uint result);
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
