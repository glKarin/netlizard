#ifndef _KARIN_NLTEXTEDITWIDGET_H
#define _KARIN_NLTEXTEDITWIDGET_H

#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextBrowser>

#include "engine/nldef.h"

#define NLTEXTEDITWIDGET_DEF(X) \
public: \
    explicit X(QWidget *widget = 0); \
    explicit X(const QString &text, QWidget *widget = 0); \
    virtual ~X(); \
    void SetMaxHeight(int height); \
    int MaxHeight() const { return m_maxHeight; } \
protected: \
    virtual void showEvent(QShowEvent *event); \
private: \
    void Init(); \
    int MinHeight() const; \
    QSizeF ContentsHeight() const; \
private: \
    QSize m_size; \
    int m_maxHeight; \
    Q_DISABLE_COPY(X)

class NLLIB_EXPORT NLTextEditWidget : public QTextEdit
{
    Q_OBJECT

    NLTEXTEDITWIDGET_DEF(NLTextEditWidget);

private Q_SLOTS:
    void OnContentsChanged();
};

class NLLIB_EXPORT NLPlainTextEditWidget : public QPlainTextEdit
{
    Q_OBJECT

    NLTEXTEDITWIDGET_DEF(NLPlainTextEditWidget);

private Q_SLOTS:
    void OnContentsChanged();
};

class NLLIB_EXPORT NLTextBrowserWidget : public QTextBrowser
{
    Q_OBJECT

    NLTEXTEDITWIDGET_DEF(NLTextBrowserWidget);

private Q_SLOTS:
    void OnContentsChanged();
};

#endif // _KARIN_NLTEXTEDITWIDGET_H
