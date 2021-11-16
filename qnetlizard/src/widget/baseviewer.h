#ifndef _KARIN_BASEVIEWER_H
#define _KARIN_BASEVIEWER_H

#include <QWidget>
#include <QList>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QToolBar;

class BaseViewer : public QWidget
{
    Q_OBJECT
public:
    explicit BaseViewer(QWidget *parent = 0);
    virtual ~BaseViewer();
    QString Title() const;
    QString StatusText() const;
    QWidget * CentralWidget();
    void SetupToolBar(QToolBar *toolbar);
    int ToolsCount() const;
    
signals:
    void titleChanged(const QString &title);
    void statusTextChanged(const QString &statusText);
    
public slots:
    virtual void Reset();
    
protected Q_SLOTS:
    void SetTitleLabel(const QString &str = QString());
    void SetTitle(const QString &str);
    void SetStatusText(const QString &str);

protected:
    void SetCentralWidget(QWidget *widget = 0);
    QHBoxLayout * ToolLayout();
    QLabel * TitleLabel();
    void AddTool(QWidget *w = 0);
    void ClearTools();
    void SetupToolBar(QHBoxLayout *toolbar);

private:
    void Init();

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_toolLayout;
    QWidget *m_centralWidget;
    QLabel *m_titleLabel;
    QString m_title;
    QString m_statusText;
    QList<QWidget *> m_tools;

    Q_DISABLE_COPY(BaseViewer)
};

#endif // _KARIN_BASEVIEWER_H
