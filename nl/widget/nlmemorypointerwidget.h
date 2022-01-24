#ifndef _KARIN_NLMEMORYPOINTERWIDGET_H
#define _KARIN_NLMEMORYPOINTERWIDGET_H

#include <QWidget>

class QLabel;
class NLLineEditWidget;

class NLMemoryPointerWidget : public QWidget
{
    Q_OBJECT
public:
    typedef QString (*GetPtrName_f)(const QString &typeName, void *ptr);

public:
    explicit NLMemoryPointerWidget(QWidget *widget = 0);
    virtual ~NLMemoryPointerWidget();
    void * Pointer() const { return m_ptr; }
    QString TypeName() const { return m_typeName; }
    void SetGetPtrNameFunc(GetPtrName_f f) { m_getPtrNameFunc = f; }

public Q_SLOTS:
    void SetMemoryPointer(const QString &type, void *ptr);
    void SetPointer(void *ptr);
    void SetTypeName(const QString &type);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void memoryPointerChanged(const QString &type, void *ptr);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

private Q_SLOTS:

private:
    void Init();
    void UpdateWidget();

private:
    QString m_typeName;
    void *m_ptr;
    QLabel *m_typeLabel;
    NLLineEditWidget *m_fileLabel;
    GetPtrName_f m_getPtrNameFunc;
    QLabel *m_nameLabel;

    Q_DISABLE_COPY(NLMemoryPointerWidget)
};

#endif // _KARIN_NLMEMORYPOINTERWIDGET_H
