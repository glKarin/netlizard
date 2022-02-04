#ifndef _KARIN_NLSCRIPTT_H
#define _KARIN_NLSCRIPTT_H

#include "nlobject.h"
#include "template/nlsequencemap.h"

class NLScriptContainer;
class NLActor;
class NLScriptObject;

class NLLIB_EXPORT NLScript : public NLObject
{
    Q_OBJECT
    Q_PROPERTY(QString scriptSource READ ScriptSource WRITE SetScriptSource)
    Q_PROPERTY(QString scriptFile READ ScriptFile WRITE SetScriptFile)
    Q_PROPERTY(NLScriptObject* scriptObject READ ScriptObject FINAL)
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
    NLScriptObject * ScriptObject() const { return m_scriptObject; }
    virtual void Reset();
    bool HasScriptSource() const { return !m_data.trimmed().isEmpty(); }

protected:
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Mount(NLActor *actor);
    virtual void Unmount();
    void SetContainer(NLScriptContainer *container);
    virtual void InitProperty();
    //lua_State * L() { return m_scriptObject.L; }
    
signals:
    void mounted();
    void unmounted();
    
public slots:
    bool SetScriptFile(const QString &file);
    void SetScriptSource(const QString &src);

private:
    void Construct();
    bool InitScriptObject();
    bool DeinitScriptObject();

private:
    bool m_mounted;
    QByteArray m_data;
    QString m_sourceFile;
    NLScriptObject *m_scriptObject;

    friend class NLScriptContainer;
    friend class NLActor;
    friend class NLScriptObject;
    
    Q_DISABLE_COPY(NLScript)
};

Q_DECLARE_METATYPE(NLScript*)
typedef QList<NLScript *> NLScriptList;

#endif // _KARIN_NLSCRIPTT_H
