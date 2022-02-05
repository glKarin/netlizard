#include "nlscriptobject.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "common/nldbg.h"
#include "nlscript.h"

NLScriptObject::NLScriptObject(const QByteArray &data, NLScript *parent) :
    NLObject(parent),
    m_data(data),
    m_func(-1),
    m_globalDataDirty(false),
    m_globalDataUpdateLock(false)
{
    CLASS_NAME(NLScriptObject);
}

NLScriptObject::~NLScriptObject()
{
    Destroy(); // !! vitrual
}

void NLScriptObject::RestoreGlobalVariant()
{
    if(!IsInited() || !IsGlobalDataDirty())
        return;
    SetGlobalVariant(m_globalVaraint);
}

void NLScriptObject::RegisterGlobalVariant()
{
    if(!IsInited())
        return;
    NLVariantSequenceHash props = GetGlobalVariant();
    ResetGlobalVariant(props);
}

void NLScriptObject::UnregisterGlobalVariant()
{
    if(!IsInited())
        return;
    ClearGlobalVariant();
}

void NLScriptObject::DumpGlobalVariant()
{
    if(!IsInited())
        return;
    NLVariantSequenceHash props = GetGlobalVariant();
    ResetGlobalVariant(props);
}

void NLScriptObject::Update(float delta)
{
    if(!IsActived())
        return;
    /*qDebug() << */UpdateScript(delta);
}

void NLScriptObject::Destroy()
{
    if(!IsInited())
        return;
    m_func = -1;
    disconnect(this, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
    UnregisterGlobalVariant();
    NLObject::Destroy();
}

void NLScriptObject::Reset()
{
    if(!IsInited())
        return;
    ResetScript();
    NLObject::Reset();
}

void NLScriptObject::Init()
{
    if(IsInited())
        return;
    if(InitScript())
    {
        connect(this, SIGNAL(propertyChanged(const QString &, const QVariant &, int)), this, SLOT(OnPropertyChanged(const QString &, const QVariant &, int)));
        NLObject::Init();
    }
}

NLScript * NLScriptObject::Script()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLScript *>(p);
    return 0;
}

void NLScriptObject::OnPropertyChanged(const QString &name, const QVariant &value, int type)
{
    if(IsLockGlobalDataUpdate())
        return;
    if(name == "objectName"
            || name == "enabled"
            || name == "scriptFile"
            || name == "scriptSource"
            )
        return;
    if(type == -1)
        m_globalVaraint.remove(name);
    else
        m_globalVaraint[name] = value;
    SetGlobalDataDirty(true);
}

void NLScriptObject::ResetGlobalVariant(const NLVariantSequenceHash &list)
{
    LockGlobalDataUpdate();
    const QList<QString> keys = list.SequenceKeys();
    Q_FOREACH(const QString &key, keys)
    {
        //qDebug() << "Set -> " << key << GetProperty(key) << list.value(key) << GetProperty(key) == list.value(key);
        SetProperty(key, list.value(key));
    }

    Q_FOREACH(const QString &key, m_globalVaraint.SequenceKeys())
    {
        if(!keys.contains(key))
        {
            RemoveProperty(key);
            //qDebug() << "Remove unused -> " << key << GetProperty(key);
        }
    }
    m_globalVaraint = list;
    UnlockGlobalDataUpdate();
    SetGlobalDataDirty(false);
}

void NLScriptObject::ClearGlobalVariant()
{
    LockGlobalDataUpdate();
    Q_FOREACH(const QString &key, m_globalVaraint.SequenceKeys())
    {
        //qDebug() << "REMOVE -> " << key << GetProperty(key);
        RemoveProperty(key);
    }
    m_globalVaraint.clear();
    UnlockGlobalDataUpdate();
    SetGlobalDataDirty(false);
}
