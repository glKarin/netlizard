#ifndef _KARIN_NLMEMORYPOINTERWIDGET_H
#define _KARIN_NLMEMORYPOINTERWIDGET_H

#include <QWidget>

class QLabel;
class NLLineEditWidget;
class NLLabelWidget;
class QMenu;

class NLMemoryPointerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NLMemoryPointerWidget(QWidget *widget = 0);
    virtual ~NLMemoryPointerWidget();
    void * Pointer() const { return m_ptr; }
    QString TypeName() const { return m_typeName; }
    void SetMemoryPointer(const QString &type, void *ptr);
    void SetMemoryPointer(const QVariant &value);
    virtual QVariant ToVariant() const;

public Q_SLOTS:
    void SetPointer(void *ptr);
    void SetTypeName(const QString &type);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void memoryPointerChanged(void *ptr, const QString &type = QString());
    void memoryPointerDeleted(void *ptr, const QString &type = QString());

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual QString GetPtrName(const QString &typeName, void *ptr) { return QString(); }
    virtual bool FromVariant(const QVariant &value, QString &typeName, void *&ptr);

private Q_SLOTS:

private:
    void Init();
    void UpdateWidget();
    int ShowMenu(const QPoint &pos);
    QString GenPtrStr(void *ptr, int cs = 1, bool prefix = false) const;

private:
    QString m_typeName;
    void *m_ptr;
    QLabel *m_typeLabel;
    NLLineEditWidget *m_pointerLabel;
    NLLabelWidget *m_nameLabel;
    QMenu *m_menu;

    Q_DISABLE_COPY(NLMemoryPointerWidget)
};

#endif // _KARIN_NLMEMORYPOINTERWIDGET_H
