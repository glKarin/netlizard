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
    QString Title() const { return m_title; }
    QString StatusText() const { return m_statusText; }
    QWidget * CentralWidget() { return m_centralWidget; }
    void SetupToolBar(QToolBar *toolbar);
    int ToolsCount() const { return m_tools.size(); }
    
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
    QHBoxLayout * ToolLayout() { return m_toolLayout; }
    QLabel * TitleLabel() { return m_titleLabel; }
    void AddTool(QWidget *w = 0) { m_tools.push_back(w); }
    void ClearTools() { m_tools.clear(); }
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
