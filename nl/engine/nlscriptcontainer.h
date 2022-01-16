#ifndef _KARIN_NLSCRIPTCONTAINER_H
#define _KARIN_NLSCRIPTCONTAINER_H

#include "nlobjectcontainer.h"

class NLScript;
class NLActor;

class NLLIB_EXPORT NLScriptContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLScriptContainer(NLActor *parent = 0);
    explicit NLScriptContainer(NLScene *scene, NLActor *parent = 0);
    virtual ~NLScriptContainer();
    bool Exists(const NLScript &item) const;
    QString Find(const NLScript *item);
    bool Add(NLScript *item);
    bool Remove(NLScript *item);
    bool Remove(int index);
    bool Remove(const QString &name);
    NLScript * Get(const QString &name);
    NLScript * Get(int index);
    virtual void Clear();

    NLScript * operator[](const QString &name) { return Get(name); }
    NLScript * operator[](int index) { return Get(index); }
    NLScriptContainer & operator<<(NLScript *item) { Add(item); return *this; }
    NLScriptContainer & operator+(NLScript *item) { Add(item); return *this; }
    NLScriptContainer & operator-(NLScript *item) { Remove(item); return *this; }
    NLScriptContainer & operator-(int index) { Remove(index); return *this; }
    NLScriptContainer & operator-(const QString &name) { Remove(name); return *this; }
    NLActor * Actor();
    const NLActor * Actor() const;
    bool ScriptIsAvailable(NLScript *item) const;

signals:

public slots:

protected:
    void SetActor(NLActor *actor);
    virtual bool ItemIsAvailable(NLObject *item) const;

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLScriptContainer)

};
#endif // _KARIN_NLSCRIPTCONTAINER_H
