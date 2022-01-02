#ifndef _KARIN_NLSCRIPTT_H
#define _KARIN_NLSCRIPTT_H

#include "nlobject.h"

class NLScriptContainer;
class NLActor;

struct lua_State;

class NLScript : public NLObject
{
    Q_OBJECT
    Q_PROPERTY(QString scriptSource READ ScriptSource WRITE SetScriptSource)
    Q_PROPERTY(QString scriptFile READ ScriptFile WRITE SetScriptFile)
public:
    explicit NLScript(NLActor *parent = 0);
    explicit NLScript(const NLProperties &prop, NLActor *parent = 0);
    explicit NLScript(NLScene *scene, NLActor *parent = 0);
    explicit NLScript(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLScript();
    bool IsMounted() const { return m_mounted; }
    NLActor * Actor();
    void SetActor(NLActor *actor);
    NLScriptContainer * Container();
    virtual bool IsActived() const { return NLObject::IsActived() && m_mounted && !m_data.isEmpty(); }
    QString ScriptSource() const { return m_data; }
    QString ScriptFile() const { return m_sourceFile; }
    void M(NLActor *a) {Mount(a);}
    void U(float d) {Update(d);}

protected:
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Mount(NLActor *actor);
    virtual void Unmount();
    void SetContainer(NLScriptContainer *container);
    
signals:
    void mounted();
    void unmounted();
    
public slots:
    bool SetScriptFile(const QString &file);
    void SetScriptSource(const QString &src);

private:
    void Construct();
    bool InitLua();
    bool DeinitLua();
    bool ExecScript(float delta);

private:
    struct lua_State *m_L;
    bool m_mounted;
    QByteArray m_data;
    QString m_sourceFile;

    friend class NLScriptContainer;
    friend class NLActor;
    
    Q_DISABLE_COPY(NLScript)
};

typedef QList<NLScript *> NLScriptList;

#endif // _KARIN_NLSCRIPTT_H
