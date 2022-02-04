#ifndef _KARIN_NLSCRIPTTOBJECT_H
#define _KARIN_NLSCRIPTTOBJECT_H

#include "nlobject.h"
#include "template/nlsequencemap.h"

class NLScript;

class NLLIB_EXPORT NLScriptObject : public NLObject
{
    Q_OBJECT
public:
    enum {
        Script_Lua_Func_Init = 1,
        Script_Lua_Func_Destroy = 1 << 1,
        Script_Lua_Func_Update = 1 << 2,
        Script_Lua_Func_Reset = 1 << 3
    };

    explicit NLScriptObject(const QByteArray &data, NLScript *parent);
    virtual ~NLScriptObject();
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();
    NLScript * Script();
    QByteArray Data() const { return m_data; }
    NLVariantSequenceHash GlobalVariant() const { return m_globalVaraint; }

protected:
    virtual bool InitScript() = 0;
    virtual bool DestroyScript() = 0;
    virtual bool UpdateScript(float delta) = 0;
    virtual bool ResetScript() = 0;
    void RegisterGlobalVariant();
    void UnregisterGlobalVariant();
    void DumpGlobalVariant();
    void RestoreGlobalVariant();
    int & Func() { return m_func; }
    virtual NLVariantSequenceHash GetGlobalVariant() = 0;
    virtual void SetGlobalVariant(const NLVariantSequenceHash &vars) = 0;
    virtual void RegisterInitialGlobalVariant(float delta) = 0;

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const QVariant &value, int type);

private:
    void ResetGlobalVariant(const NLSequenceHash<QString, QVariant> &list);
    void ClearGlobalVariant();
    void LockGlobalDataUpdate() { m_globalDataUpdateLock = true; }
    void UnlockGlobalDataUpdate() { m_globalDataUpdateLock = false; }
    bool IsLockGlobalDataUpdate() const { return m_globalDataUpdateLock; }
    void SetGlobalDataDirty(bool b) { if(!m_globalDataUpdateLock) m_globalDataDirty = b; }
    bool IsGlobalDataDirty() const { return m_globalDataDirty; }

private:
    QByteArray m_data;
    int m_func;
    bool m_globalDataDirty;
    bool m_globalDataUpdateLock;
    NLSequenceHash<QString, QVariant> m_globalVaraint;
};

Q_DECLARE_METATYPE(NLScriptObject*)

#endif // _KARIN_NLSCRIPTTOBJECT_H
