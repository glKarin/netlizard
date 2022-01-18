#ifndef _KARIN_HELPDIALOG_H
#define _KARIN_HELPDIALOG_H

#include <QDialog>
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
    bool LoadHelp();

private:
    struct HelpItem
    {
        QString category;
        QStringList items;
        HelpItem(const QString &category, const QStringList &items = QStringList())
            : category(category),
              items(items)
        {
        }
    };

    typedef QList<HelpItem> HelpItemList;

private:
    HelpItemList m_helpList;
    QTextBrowser *m_textViewer;
    
    Q_DISABLE_COPY(HelpDialog)
};

#endif // _KARIN_HELPDIALOG_H
