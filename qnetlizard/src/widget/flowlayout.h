#ifndef _KARIN_FLOWLAYOUT_H
#define _KARIN_FLOWLAYOUT_H

#include <QLayout>
#include <QStyle>

class QRect;

class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent = 0, int hSpacing = -1, int vSpacing = -1);
    virtual ~FlowLayout();

    int HorizontalSpacing() const;
    int VerticalSpacing() const;
    virtual void addItem(QLayoutItem *item);
    virtual Qt::Orientations expandingDirections() const;
    virtual bool hasHeightForWidth() const;
    virtual int heightForWidth(int) const;
    virtual int count() const;
    virtual QLayoutItem *itemAt(int index) const;
    virtual QSize minimumSize() const;
    virtual void setGeometry(const QRect &rect);
    virtual QSize sizeHint() const;
    virtual QLayoutItem *takeAt(int index);

private:
    int DoLayout(const QRect &rect, bool testOnly) const;
    int SmartSpacing(QStyle::PixelMetric pm) const;

private:
    QList<QLayoutItem *> m_itemList;
    int m_horizontalSpacing;
    int m_verticalSpacing;

    Q_DISABLE_COPY(FlowLayout)
};

#endif // _KARIN_FLOWLAYOUT_H
