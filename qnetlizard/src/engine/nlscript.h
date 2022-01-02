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
    bool InitLua() { return m_lua.Init(); }
    bool DeinitLua() { return m_lua.Deinit(); }

private:
    void Construct();
    bool ExecScript(float delta) { return m_lua.Exec(delta); }

private:
    struct Script_Lua
    {
        enum {
            Script_Lua_Func_Init = 1,
            Script_Lua_Func_Destroy = 2,
            Script_Lua_Func_Update = 4
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
        operator bool() const { return L != 0; }
    };

    bool m_mounted;
    QByteArray m_data;
    QString m_sourceFile;
    Script_Lua m_lua;

    friend class NLScriptContainer;
    friend class NLActor;
    friend struct Script_Lua;
    
    Q_DISABLE_COPY(NLScript)
};

typedef QList<NLScript *> NLScriptList;

#endif // _KARIN_NLSCRIPTT_H
