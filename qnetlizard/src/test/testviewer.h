#ifndef _KARIN_TESTVIEWER_H
#define _KARIN_TESTVIEWER_H

#include "baseviewer.h"

class TestScene;

class TestViewer : public BaseViewer
{
    Q_OBJECT
public:
    explicit TestViewer(QWidget *parent = 0);
    virtual ~TestViewer();
    
signals:
    
public slots:

private:
    void Init();

private Q_SLOTS:

private:
    TestScene *m_widget;

    Q_DISABLE_COPY(TestViewer)
    
};

#endif // _KARIN_TESTVIEWER_H
