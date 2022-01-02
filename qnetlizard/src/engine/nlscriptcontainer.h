#ifndef _KARIN_NLSCRIPTCONTAINER_H
#define _KARIN_NLSCRIPTCONTAINER_H

#include "nlobjectcontainer.h"

class NLScript;
class NLActor;

class NLScriptContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLScriptContainer(NLActor *parent = 0);
    explicit NLScriptContainer(NLScene *scene, NLActor *parent = 0);
    virtual ~NLScriptContainer();
    bool Exists(const NLScript &item) const;
    NLName Find(const NLScript *item);
    bool Add(NLScript *item);
    bool Remove(NLScript *item);
    bool Remove(int index);
    bool Remove(const NLName &name);
    NLScript * Get(const NLName &name);
    NLScript * Get(int index);
    virtual void Clear();

    NLScript * operator[](const NLName &name) { return Get(name); }
    NLScript * operator[](int index) { return Get(index); }
    NLScriptContainer & operator<<(NLScript *item) { Add(item); return *this; }
    NLActor * Actor();

signals:

public slots:

protected:
    void SetActor(NLActor *actor);

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLScriptContainer)

};
#endif // _KARIN_NLSCRIPTCONTAINER_H