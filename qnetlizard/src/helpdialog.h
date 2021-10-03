#ifndef _KARIN_HELPDIALOG_H
#define _KARIN_HELPDIALOG_H

#include <QDialog>
#include <QVector>
#include <QStringList>

class QListWidgetItem;
class QTextBrowser;

class HelpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HelpDialog(QWidget *parent = 0);
    virtual ~HelpDialog();
    static int Show(QWidget *parent);
    
signals:
    
public slots:

private Q_SLOTS:
    void SetContentText(QListWidgetItem *item);

private:
    void Init();

private:
    QStringList m_menuText;
    QVector<QStringList> m_contentText;
    QTextBrowser *m_textViewer;
    
    Q_DISABLE_COPY(HelpDialog)
};

#endif // _KARIN_HELPDIALOG_H
