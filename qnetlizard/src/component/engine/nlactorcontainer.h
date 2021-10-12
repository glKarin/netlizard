#ifndef _KARIN_NLACTORCONTAINER_H
#define _KARIN_NLACTORCONTAINER_H

#include "nlobjectcontainer.h"

class NLActor;

class NLActorContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLActorContainer(QObject *parent = 0);
    explicit NLActorContainer(NLScene *scene, QObject *parent = 0);
    virtual ~NLActorContainer();
    bool Exists(const NLActor &item) const;
    NLName Find(const NLActor *item);
    bool Add(NLActor *item);
    bool Remove(NLActor *item);
    NLActor * Get(const NLName &name);
    NLActor * Get(int index);
    virtual void Render();
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int button, bool pressed, int x, int y, int modifier);
    virtual bool MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool WheelEventHandler(int orientation, int delta, int x, int y, int modifier);

    NLActor * operator[](const NLName &name);
    NLActor * operator[](int index);
    NLActorContainer * operator<<(NLActor *item);

signals:

public slots:

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLActorContainer)

};
#endif // _KARIN_NLACTORCONTAINER_H
