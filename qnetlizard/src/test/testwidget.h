#ifndef _KARIN_TestWidget_H
#define _KARIN_TestWidget_H

#include "nlscene.h"

class TestRenderer;

class TestWidget : public NLScene
{
    Q_OBJECT
public:
    TestWidget(QWidget *parent = 0);
    virtual ~TestWidget();

public Q_SLOTS:
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

private:
    TestRenderer *m_renderer;

    Q_DISABLE_COPY(TestWidget)
};

#endif // _KARIN_TestWidget_H
