#ifndef _KARIN_INDEXVIEWER_H
#define _KARIN_INDEXVIEWER_H

#include <QPushButton>
#include <QVariant>
#include <QList>

#include "baseviewer.h"

class QAction;
class QGridLayout;
class QColor;

struct HomeCellItem
{
    QString label;
    QVariant data;
    // icon
    explicit HomeCellItem(const QString &label, const QVariant &data = QVariant())
        : label(label),
          data(data)
    {
    }
};

typedef QList<HomeCellItem> HomeCellItemList;

class HomeCell : public QPushButton
{
    Q_OBJECT
public:
    explicit HomeCell(const QString &text = QString(), const QVariant &data = QVariant(), QWidget *parent = 0);
    virtual ~HomeCell();
    QAction * Action();

Q_SIGNALS:
    void actionTrigger(QAction *action);

private Q_SLOTS:
    void TriggerAction();

private:
    QAction *m_action;
};

class IndexViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit IndexViewer(QWidget *parent = 0);
    virtual ~IndexViewer();
    
signals:
    void openViewer(QAction *action);

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void Init();
    void Layout();
    QString RandomColor() const;
    
private:
    QGridLayout *m_layout;
    QWidget *m_tools;
    bool m_inited;
    HomeCellItemList m_list;
    
    Q_DISABLE_COPY(IndexViewer)
};

#endif // _KARIN_INDEXVIEWER_H
