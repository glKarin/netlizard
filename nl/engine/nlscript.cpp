#include "nlscript.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "common/nldbg.h"
#include "nlactor.h"
#include "template/nlsequencemap.h"
#include "nlscriptcontainer.h"
#include "nlluascriptobject.h"

NLScript::NLScript(NLActor *parent) :
    NLObject(NLObject::Type_Script, 0, NLProperties(), parent),
    m_mounted(false),
    m_scriptObject(0)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLScript::NLScript(const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Script, 0, prop, parent),
    m_mounted(false),
    m_scriptObject(0)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, NLActor *parent) :
    NLObject(NLObject::Type_Script, scene, NLProperties(), parent),
    m_mounted(false),
    m_scriptObject(0)
{
    Construct();
}

NLScript::NLScript(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Script, scene, prop, parent),
    m_mounted(false),
    m_scriptObject(0)
{
    Construct();
}

NLScript::~NLScript()
{
    Destroy(); // !! vitrual
}

void NLScript::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLScript);
    if(objectName().isEmpty() || !GetInitProperty("name").isValid())
        setObjectName("NLScript");
    SetType(NLObject::Type_Script);

    NLProperties props = PropertyConfig();

    props.Insert("scriptFile", NLProperties("file", true));
    props.Insert("scriptSource", NLProperties("multiline", true)
                 ("direct", false)
                 ("syntax", "lua")
                 );
    props.Insert("scriptObject", NLProperties("expand", true));

    SetPropertyConfig(props);
}

void NLScript::Update(float delta)
{
    if(!IsActived())
        return;
    if(m_scriptObject)
    /*qDebug() << */m_scriptObject->Update(delta);
}

void NLScript::Destroy()
{
    if(!IsInited())
        return;
    if(m_scriptObject)
    {
        m_scriptObject->Destroy();
        delete m_scriptObject;
        m_scriptObject = 0;
    }
    if(IsMounted())
        Unmount();
    NLObject::Destroy();
}

NLActor * NLScript::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLScript::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLScript::Mount(NLActor *actor)
{
    if(!actor)
        return;
    if(IsMounted())
        return;
    if(!IsInited())
        Init();
    SetActor(actor);
    m_mounted = true;
    InitScriptObject(); // If has script source, try to load.
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> MOUNTED";
#endif
    emit mounted();
}

void NLScript::Unmount()
{
    if(!IsMounted())
        return;
    NLActor *actor = Actor();
    if(actor)
    {
        actor->TellScriptRemoved();
        SetActor(0);
    }
    DeinitScriptObject();
    m_mounted = false;
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> UNMOUNTED";
#endif
    emit unmounted();
}

NLScriptContainer * NLScript::Container()
{
    return static_cast<NLScriptContainer *>(NLObject::Container());
}

void NLScript::SetContainer(NLScriptContainer *container)
{
    NLObject::SetContainer(container);
}

bool NLScript::SetScriptFile(const QString &file)
{
    if(file != m_sourceFile)
    {
        m_sourceFile = file;
        emit propertyChanged("scriptFile", m_sourceFile);
    }
    if(!m_sourceFile.isEmpty())
    {
        QFile f(file);
        if(!f.exists())
        {
            SetScriptSource(QString());
            return false;
        }
        if(!f.open(QIODevice::ReadOnly))
        {
            SetScriptSource(QString());
            return false;
        }
        QTextStream is(&f);
        SetScriptSource(is.readAll());
        f.close();
        return true;
    }
    return false;
}

void NLScript::SetScriptSource(const QString &src)
{
    QByteArray ba;
    ba.append(src);
    if(m_data != ba)
    {
        m_data.clear();
        DeinitScriptObject();
        if(!src.isEmpty())
        {
            m_data = ba;
            InitScriptObject();
        }
        emit propertyChanged("scriptSource", QString(m_data));
    }
}

bool NLScript::InitScriptObject()
{
    if(!IsMounted() || !HasScriptSource())
        return false;
    DeinitScriptObject();
    m_scriptObject = new NLLuaScriptObject(m_data, this);
    m_scriptObject->Init();
    if(m_scriptObject->IsInited())
    {
        emit propertyChanged("scriptObject", QVariant::fromValue<NLScriptObject *>(m_scriptObject));
        return true;
    }
    else
    {
        delete m_scriptObject;
        m_scriptObject = 0;
        return false;
    }
}

bool NLScript::DeinitScriptObject()
{
    if(!IsMounted())
        return false;
    if(m_scriptObject)
    {
        m_scriptObject->Destroy();
        delete m_scriptObject;
        m_scriptObject = 0;
        emit propertyChanged("scriptObject", QVariant::fromValue<NLScriptObject *>(m_scriptObject));
    }
    return true;
}

void NLScript::InitProperty()
{
    NLObject::InitProperty();
    QString str(GetInitProperty_T<QString>("scriptSource"));
    if(!str.isEmpty())
        SetScriptSource(str);
    else
    {
        str = GetInitProperty_T<QString>("scriptFile");
        if(!str.isEmpty())
            SetScriptFile(str);
    }
}

void NLScript::Reset()
{
    NLObject::Reset();
    if(m_scriptObject)
        m_scriptObject->Reset();
}
