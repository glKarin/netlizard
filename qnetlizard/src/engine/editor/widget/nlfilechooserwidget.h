#ifndef _KARIN_NLFILECHOOSERWIDGET_H
#define _KARIN_NLFILECHOOSERWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;

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

private:
    void Init();
    void UpdateWidget();

private:
    QString m_file;
    QLineEdit *m_fileLabel;
    QPushButton *m_openButton;
    //QPushButton *m_reloadButton;

    Q_DISABLE_COPY(NLFileChooserWidget)
};

#endif // _KARIN_NLFILECHOOSERWIDGET_H
