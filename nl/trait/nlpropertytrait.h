#ifndef _KARIN_NLPROPERTYTRAIT_H
#define _KARIN_NLPROPERTYTRAIT_H

// #define NAMELIST QStringList
// #define NAMELIST QList<QByteArray>
#define NAMELIST QList<QString>

#define NLPROPERTY_DEF_TRAIT \
    public: \
    QVariant GetProperty(const QString &name, const QVariant &def = QVariant()) const; \
    void SetProperty(const QString &name, const QVariant &value); \
    void CoverProperty(const QString &name, const QVariant &value); \
    void RemoveProperty(const QString &name); \
    bool HasProperty(const QString &name) const; \
    bool IsDynamicProperty(const QString &name) const; \
    bool IsDeclareProperty(const QString &name) const; \
    bool IsInheritProperty(const QString &name) const; \
    bool IsUserProperty(const QString &name) const; \
    NAMELIST DynamicPropertyNames() const; \
    NAMELIST DeclarePropertyNames() const; \
    NAMELIST InheritPropertyNames() const; \
    NAMELIST UserPropertyNames() const; \
    NAMELIST PropertyNames() const; \
    QVariantHash GetDynamicProperties() const; \
    QVariantHash GetDeclareProperties() const; \
    QVariantHash GetInheritProperties() const; \
    QVariantHash GetUserProperties() const; \
    QVariantHash GetProperties() const; \
    template<class T> T GetProperty_T(const QString &name, const T &def = T()) const; \
    template<class T> void SetProperty_T(const QString &name, const T &value); \
    template<class T> void CoverProperty_T(const QString &name, const T &value);

#if 0
#define NLPROPERTY_HASPROPERTY_TRAIT(Clazz) \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0 /*metaObj->propertyOffset()*/; i < metaObj->propertyCount(); i++) { \
        if(name == metaObj->property(i).name()) \
            return true; \
    } \
    return dynamicPropertyNames().contains(name.toLocal8Bit());
#else
#define NLPROPERTY_HASPROPERTY_TRAIT(Clazz) \
    const QByteArray ba = name.toLocal8Bit(); \
    return property(ba.constData()).isValid();
#endif

#define NLPROPERTY_DECL_TRAIT(Clazz, var_equals_func, signal_func) \
void Clazz::RemoveProperty(const QString &name) { \
    if(!HasProperty(name)) \
        return; \
    const QByteArray ba = name.toLocal8Bit(); \
    setProperty(ba.constData(), QVariant()); \
    emit signal_func(name, QVariant(), -1); \
} \
bool Clazz::HasProperty(const QString &name) const { \
    NLPROPERTY_HASPROPERTY_TRAIT(name) \
} \
QVariant Clazz::GetProperty(const QString &name, const QVariant &def) const { \
    const QByteArray ba = name.toLocal8Bit(); \
    QVariant p = property(ba.constData()); \
    if(!p.isValid()) \
        return def; \
    return p; \
} \
void Clazz::SetProperty(const QString &name, const QVariant &value) { \
    bool has = HasProperty(name); \
    if(has && var_equals_func(GetProperty(name), value)) \
        return; \
    const QByteArray ba = name.toLocal8Bit(); \
    /*qDebug() << */setProperty(ba.constData(), value); \
    emit signal_func(name, value, has ? 0 : 1); \
} \
void Clazz::CoverProperty(const QString &name, const QVariant &value) { \
    const QByteArray ba = name.toLocal8Bit(); \
    /*qDebug() << */setProperty(ba.constData(), value); \
    emit signal_func(name, value); \
} \
bool Clazz::IsDynamicProperty(const QString &name) const { \
    return dynamicPropertyNames().contains(name.toLocal8Bit()); \
} \
bool Clazz::IsDeclareProperty(const QString &name) const { \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyCount(); i++) { \
        if(name == metaObj->property(i).name()) \
            return true; \
    } \
    return false; \
} \
bool Clazz::IsInheritProperty(const QString &name) const { \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyOffset(); i++) { \
        if(name == metaObj->property(i).name()) \
            return true; \
    } \
    return false; \
} \
bool Clazz::IsUserProperty(const QString &name) const { \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); i++) { \
        if(name == metaObj->property(i).name()) \
            return true; \
    } \
    return false; \
} \
NAMELIST Clazz::DynamicPropertyNames() const { \
    NAMELIST list; \
    QList<QByteArray> names = dynamicPropertyNames(); \
    for(QList<QByteArray>::const_iterator itor = names.constBegin(); itor != names.constEnd(); ++itor) { \
        list.push_back(QString(*itor)); \
    } \
    return list; \
} \
NAMELIST Clazz::DeclarePropertyNames() const { \
    NAMELIST list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyCount(); i++) { \
        list.push_back(metaObj->property(i).name()); \
    } \
    return list; \
} \
NAMELIST Clazz::InheritPropertyNames() const { \
    NAMELIST list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyOffset(); i++) { \
        list.push_back(metaObj->property(i).name()); \
    } \
    return list; \
} \
NAMELIST Clazz::UserPropertyNames() const { \
    NAMELIST list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); i++) { \
        list.push_back(metaObj->property(i).name()); \
    } \
    return list; \
} \
NAMELIST Clazz::PropertyNames() const { \
    NAMELIST list = DeclarePropertyNames(); \
    list << DynamicPropertyNames(); \
    return list; \
} \
QVariantHash Clazz::GetDynamicProperties() const { \
    QVariantHash list; \
    QList<QByteArray> names = dynamicPropertyNames(); \
    for(QList<QByteArray>::const_iterator itor = names.constBegin(); itor != names.constEnd(); ++itor) { \
        list.insert(QString(*itor), property(itor->constData())); \
    } \
    return list; \
} \
QVariantHash Clazz::GetDeclareProperties() const { \
    QVariantHash list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyCount(); i++) { \
        QMetaProperty prop = metaObj->property(i); \
        list.insert(prop.name(), prop.read(this)); \
    } \
    return list; \
} \
QVariantHash Clazz::GetInheritProperties() const { \
    QVariantHash list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyOffset(); i++) { \
        QMetaProperty prop = metaObj->property(i); \
        list.insert(prop.name(), prop.read(this)); \
    } \
    return list; \
} \
QVariantHash Clazz::GetUserProperties() const { \
    QVariantHash list; \
    const QMetaObject *metaObj = metaObject(); \
    for(int i = 0; i < metaObj->propertyCount(); i++) { \
        QMetaProperty prop = metaObj->property(i); \
        list.insert(prop.name(), prop.read(this)); \
    } \
    return list; \
} \
QVariantHash Clazz::GetProperties() const { \
    QVariantHash list = GetDeclareProperties(); \
    list.unite(GetDynamicProperties()); \
    return list; \
} \

#define NLPROPERTY_TEMPLATE_DECL_TRAIT(Clazz, signal_func) \
template <class T> T Clazz::GetProperty_T(const QString &name, const T &def) const { \
    const QByteArray ba = name.toLocal8Bit(); \
    QVariant p = property(ba.constData()); \
    if(!p.isValid()) \
        return def; \
    return p.value<T>(); \
} \
template<class T> void Clazz::SetProperty_T(const QString &name, const T &value) { \
    bool has = HasProperty(name); \
    if(has && GetProperty(name).value<T>() == value) \
        return; \
    const QByteArray ba = name.toLocal8Bit(); \
    setProperty(ba.constData(), value); \
    emit signal_func(name, QVariant::fromValue(value), has ? 0 : 1); \
} \
template<class T> void Clazz::CoverProperty_T(const QString &name, const T &value) { \
    const QByteArray ba = name.toLocal8Bit(); \
    setProperty(ba.constData(), value); \
    emit signal_func(name, QVariant::fromValue<T>(value)); \
}

#endif // _KARIN_NLPROPERTYTRAIT_H
