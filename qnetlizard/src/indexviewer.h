#ifndef _KARIN_INDEXVIEWER_H
#define _KARIN_INDEXVIEWER_H

#include <QPushButton>
#include <QVariant>

#include "baseviewer.h"

class QAction;
class FlowLayout;
class QColor;

struct MenuItem
{
    QString label;
    QVariant data;
    QString description;
    QString show;
    QString shortcut;
    QList<MenuItem> items;
    // icon
    explicit MenuItem(const QString &label = QString(), const QVariant &data = QVariant(), const QString &desc = QString(), const QString &show = QString(), const QString &shortcut = QString())
        : label(label),
          data(data),
          description(desc),
          show(show),
          shortcut(shortcut)
    {
    }
    virtual ~MenuItem()
    {
    }
    bool IsValid() const
    {
        if(label.isEmpty())
            return false;
        if(IsItem())
            return !data.isNull();
        return true;
    }
    void Invalid()
    {
        data.clear();
    }
    bool IsItem() const
    {
        return items.isEmpty();
    }
    bool IsMenu() const
    {
        return !items.isEmpty();
    }
    bool IsOnlyShowInDebug() const
    {
        return show.toLower() == "debug";
    }
    MenuItem & operator<<(const MenuItem &item)
    {
        items.push_back(item);
        return *this;
    }
};

typedef QList<MenuItem> MenuItemList;

class HomeCell : public QPushButton
{
    Q_OBJECT
public:
    explicit HomeCell(const QString &text = QString(), const QVariant &data = QVariant(), QWidget *parent = 0);
    virtual ~HomeCell();
    QAction * Action() { return m_action; }

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
    static const MenuItemList & ActionMap();
    
signals:
    void openViewer(QAction *action);

public slots:

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void Init();
    void Layout();
    QString RandomColor() const;
    static bool LoadMenus(MenuItemList &map);
    
private:
    FlowLayout *m_layout;
    QWidget *m_tools;
    
    Q_DISABLE_COPY(IndexViewer)
};

#endif // _KARIN_INDEXVIEWER_H
