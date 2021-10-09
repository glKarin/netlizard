#ifndef _KARIN_RENDERABLE_H
#define _KARIN_RENDERABLE_H

#include <QObject>

class NLActor;

class NLRenderable : public QObject
{
    Q_OBJECT
public:
    explicit NLRenderable(QObject *parent = 0);
    virtual ~NLRenderable();

protected:
    virtual void InitRender() {}
    virtual void Render() = 0;
    virtual void DeinitRender() {}

signals:
    
public slots:

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLRenderable)
};

#endif // _KARIN_RENDERABLE_H
