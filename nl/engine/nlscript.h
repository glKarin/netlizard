#ifndef _KARIN_NLSCRIPTT_H
#define _KARIN_NLSCRIPTT_H

#include "nlobject.h"
#include "template/nlsequencemap.h"

template <class K, class V> class NLSequenceHash;
class NLScriptContainer;
class NLActor;

struct lua_State;

class NLLIB_EXPORT NLScript : public NLObject
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
    const NLActor * Actor() const;
    void SetActor(NLActor *actor);
    NLScriptContainer * Container();
    virtual bool IsActived() const { return NLObject::IsActived() && m_mounted && !m_data.isEmpty(); }
    QString ScriptSource() const { return m_data; }
    QString ScriptFile() const { return m_sourceFile; }
    virtual void Reset();
    bool HasScriptSource() const { return !m_data.trimmed().isEmpty(); }

protected:
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Mount(NLActor *actor);
    virtual void Unmount();
    void SetContainer(NLScriptContainer *container);
    virtual void InitProperty();
    
signals:
    void mounted();
    void unmounted();
    
public slots:
    bool SetScriptFile(const QString &file);
    void SetScriptSource(const QString &src);

private:
    void Construct();
    bool ExecScript(float delta) { return m_lua.Exec(delta); }
    void SetGlobalVariant(const NLSequenceHash<QString, QVariant> &list);
    void ClearGlobalVariant();
    bool InitLua();
    bool DeinitLua();
    void LockGlobalDataUpdate() { m_globalDataUpdateLock = true; }
    void UnlockGlobalDataUpdate() { m_globalDataUpdateLock = false; }
    bool IsLockGlobalDataUpdate() const { return m_globalDataUpdateLock; }
    void SetGlobalDataDirty(bool b) { if(!m_globalDataUpdateLock) m_globalDataDirty = b; }
    bool IsGlobalDataDirty() const { return m_globalDataDirty; }

private slots:
    void OnPropertyChanged(const QString &name, const QVariant &value, int type);

private:
    struct Script_Lua
    {
        enum {
            Script_Lua_Func_Init = 1,
            Script_Lua_Func_Destroy = 1 << 1,
            Script_Lua_Func_Update = 1 << 2,
            Script_Lua_Func_Reset = 1 << 3
        };
        struct lua_State *L;
        NLScript *script;
        int func;

        Script_Lua()
            : L(0),
              script(0),
              func(-1)
        { }
        ~Script_Lua() {
            Deinit();
        }

        bool Init();
        bool Deinit();
        bool Exec(float delta);
        bool Reset();
        NLSequenceHash<QString, QVariant> GetGlobalVariant();
        void RegisterGlobalVariant();
        void UnregisterGlobalVariant();
        void DumpGlobalVariant();
        void RestoreGlobalVariant();
        operator bool() const { return L != 0; }
    };

    bool m_mounted;
    QByteArray m_data;
    QString m_sourceFile;
    Script_Lua m_lua;
    NLSequenceHash<QString, QVariant> m_globalVaraint;
    bool m_globalDataDirty;
    bool m_globalDataUpdateLock;

    friend class NLScriptContainer;
    friend class NLActor;
    friend struct Script_Lua;
    
    Q_DISABLE_COPY(NLScript)
};

typedef QList<NLScript *> NLScriptList;

#endif // _KARIN_NLSCRIPTT_H
