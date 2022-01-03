#ifndef _KARIN_NLFILECHOOSERWIDGET_H
#define _KARIN_NLFILECHOOSERWIDGET_H

#include <QWidget>
#include <QLineEdit>

class QPushButton;

class NLFileChooserWidgetLabel : public QLineEdit
{
    Q_OBJECT

public:
    NLFileChooserWidgetLabel(QWidget *parent = 0)
        : QLineEdit(parent) { setObjectName("NLFileChooserWidgetLabel"); }
    NLFileChooserWidgetLabel(const QString &contents, QWidget *parent = 0)
        : QLineEdit(contents, parent) { setObjectName("NLFileChooserWidgetLabel"); }
    ~NLFileChooserWidgetLabel();

Q_SIGNALS:
    void dblClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

class NLFileChooserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLFileChooserWidget(QWidget *widget = 0);
    virtual ~NLFileChooserWidget();
    QString File() const { return m_file; }

public Q_SLOTS:
    void SetFile(const QString &f);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void fileChanged(const QString &file);
    void fileReload(const QString &file);

private Q_SLOTS:
    void OpenFileDialog();
    void OpenFile();
    void EditOrChooseFile();

private:
    void Init();
    void UpdateWidget();

private:
    QString m_file;
    NLFileChooserWidgetLabel *m_fileLabel;
    QPushButton *m_openButton;
    //QPushButton *m_reloadButton;

    Q_DISABLE_COPY(NLFileChooserWidget)
};

#endif // _KARIN_NLFILECHOOSERWIDGET_H
