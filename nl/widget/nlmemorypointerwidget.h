#ifndef _KARIN_NLMEMORYPOINTERWIDGET_H
#define _KARIN_NLMEMORYPOINTERWIDGET_H

#include <QWidget>
#include <QLineEdit>

class QLabel;

class NLMemoryPointerWidgetLabel : public QLineEdit
{
    Q_OBJECT

public:
    explicit NLMemoryPointerWidgetLabel(QWidget *parent = 0)
        : QLineEdit(parent) { setObjectName("NLMemoryPointerWidgetLabel"); }
   explicit  NLMemoryPointerWidgetLabel(const QString &contents, QWidget *parent = 0)
        : QLineEdit(contents, parent) { setObjectName("NLMemoryPointerWidgetLabel"); }
    virtual ~NLMemoryPointerWidgetLabel();

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

class NLMemoryPointerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLMemoryPointerWidget(QWidget *widget = 0);
    virtual ~NLMemoryPointerWidget();
    void * Pointer() const { return m_ptr; }
    QString TypeName() const { return m_typeName; }

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
    NLMemoryPointerWidgetLabel *m_fileLabel;

    Q_DISABLE_COPY(NLMemoryPointerWidget)
};

#endif // _KARIN_NLMEMORYPOINTERWIDGET_H
