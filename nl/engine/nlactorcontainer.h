#ifndef _KARIN_NLACTORCONTAINER_H
#define _KARIN_NLACTORCONTAINER_H

#include "nlobjectcontainer.h"

class NLActor;

class NLLIB_EXPORT NLActorContainer : public NLObjectContainer
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
    bool Remove(int index);
    bool Remove(const NLName &name);
    NLActor * Get(const NLName &name);
    NLActor * Get(int index);
    virtual void Clear();
    virtual void Render();

    NLActor * operator[](const NLName &name) { return Get(name); }
    NLActor * operator[](int index) { return Get(index); }
    NLActorContainer & operator<<(NLActor *item) { Add(item); return *this; }
    NLActorContainer & operator+(NLActor *item) { Add(item); return *this; }
    NLActorContainer & operator-(NLActor *item) { Remove(item); return *this; }
    NLActorContainer & operator-(int index) { Remove(index); return *this; }
    NLActorContainer & operator-(const NLName &name) { Remove(name); return *this; }
    NLActor * Actor();
    const NLActor * Actor() const;
    int TotalCount() const;
    bool ActorIsAvailable(NLActor *item) const;

signals:

public slots:

protected:
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int button, bool pressed, int x, int y, int modifier);
    virtual bool MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier);
    void SetActor(NLActor *actor);
    virtual bool ItemIsAvailable(NLObject *item) const;

private:
    void Construct();

private:
    friend class NLActor;
    friend class NLScene;
    Q_DISABLE_COPY(NLActorContainer)

};
#endif // _KARIN_NLACTORCONTAINER_H
