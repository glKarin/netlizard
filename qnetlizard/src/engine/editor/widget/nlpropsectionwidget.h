#ifndef _KARIN_NLPROPSECTIONWIDGET_H
#define _KARIN_NLPROPSECTIONWIDGET_H

#include <QWidget>

class QFormLayout;
class QVBoxLayout;
class QMenu;
class QToolButton;
class QLabel;

class NLPropSectionHeader : public QWidget
{
    Q_OBJECT

public:
    explicit NLPropSectionHeader(QWidget *widget = 0);
    explicit NLPropSectionHeader(const QString &text, QWidget *widget = 0);
    virtual ~NLPropSectionHeader();
    void AddAction(QAction *action);
    void Reset();
    void SetCanExpand(bool b);

Q_SIGNALS:
    void actionTriggered(QAction *action);
    void toggleExpand(bool on);

public Q_SLOTS:
    void SetText(const QString &text);

private:
    void Init(const QString &text = QString());
    void SetExpand(bool b);

public Q_SLOTS:
    void OnToggleExpand(bool on);

private:
    QLabel *m_label;
    QMenu *m_menu;
    QToolButton *m_expandButton;

    Q_DISABLE_COPY(NLPropSectionHeader)
};

class NLPropSectionContent : public QWidget
{
    Q_OBJECT
public:
    explicit NLPropSectionContent(QWidget *widget = 0);
    virtual ~NLPropSectionContent();
    void AddWidget(QWidget *widget);
    void Reset();

private:
    void Init();

private Q_SLOTS:
    void Toggle(bool on);

private:
    typedef QList<QWidget *> WidgetList;
    QVBoxLayout *m_layout;
    WidgetList m_widgets;
    bool m_expand;

    Q_DISABLE_COPY(NLPropSectionContent)
};

class NLPropSection : public QWidget
{
    Q_OBJECT
public:
    explicit NLPropSection(QWidget *widget = 0);
    virtual ~NLPropSection();
    void AddWidget(QWidget *widget);
    void SetTitle(const QString &str);
    void AddAction(QAction *action);
    void Reset();

Q_SIGNALS:
    void actionTriggered(QAction *action);

private:
    void Init();

private:
    NLPropSectionHeader *m_header;
    NLPropSectionContent *m_content;

    Q_DISABLE_COPY(NLPropSection)
};

#endif // _KARIN_NLPROPSECTIONWIDGET_H
