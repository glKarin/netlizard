#ifndef _KARIN_TESTSCENE_H
#define _KARIN_TESTSCENE_H

#include "nlscene.h"

class NLModelRenderer;

NLSCENE(TestScene)
class TestScene : public NLScene
{
    Q_OBJECT
public:
    TestScene(QWidget *parent = 0);
    virtual ~TestScene();

public Q_SLOTS:
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

    Q_DISABLE_COPY(TestScene)
};

#endif // _KARIN_TESTSCENE_H
