#ifndef _KARIN_NLRENDERMODEL_H
#define _KARIN_NLRENDERMODEL_H

#include <QVector>
#include <QList>
#include <QColor>
#include <QDebug>

#include <GL/gl.h>

#include "qdef.h"

typedef QVector<char> ByteArray;

#define VERTEX_ELEMENT 4
#define TEXCOORD_ELEMENT 2
#define COLOR_ELEMENT 4
#define ELEMENT_SIZE (VERTEX_ELEMENT + TEXCOORD_ELEMENT + 3 + COLOR_ELEMENT)

#define IS_GLtype(GLtype) \
    template <class T> inline bool is_##GLtype(T t) { (void)t; return false; } \
    template <> inline bool is_##GLtype<GLtype>(GLtype t) { (void)t; return true; }
//inline bool is_##GLtype(GLtype t) { (void)t; return true; }

namespace NL
{
// no RTTI
IS_GLtype(GLfloat)
IS_GLtype(GLdouble)
IS_GLtype(GLint)
IS_GLtype(GLuint)
IS_GLtype(GLshort)
IS_GLtype(GLushort)
IS_GLtype(GLubyte)
IS_GLtype(GLbyte)
}

#undef IS_GLtype

template <class T>
struct NLRenderModelVertexItem
{
    T vertex[VERTEX_ELEMENT];
    T texcoord[TEXCOORD_ELEMENT];
    T normal[3];
#if(COLOR_ELEMENT > 0)
    T color[COLOR_ELEMENT];
#endif
    NLRenderModelVertexItem(const T vertex[VERTEX_ELEMENT], const T texcoord[TEXCOORD_ELEMENT] = 0, const T normal[3] = 0
#if(COLOR_ELEMENT > 0)
            , const T color[COLOR_ELEMENT] = 0
#endif
            )
    {
        memcpy(this->vertex, vertex, sizeof(T) * VERTEX_ELEMENT);
        if(texcoord)
            memcpy(this->texcoord, texcoord, sizeof(T) * TEXCOORD_ELEMENT);
        else
        {
            for(int i = 0; i < TEXCOORD_ELEMENT; i++)
                this->texcoord[i] = 0;
        }
        if(normal)
            memcpy(this->normal, normal, sizeof(T) * 3);
        else
            this->normal[0] = this->normal[1] = this->normal[2] = 0;
#if(COLOR_ELEMENT > 0)
        if(color)
            memcpy(this->color, color, sizeof(T) * COLOR_ELEMENT);
        else
        {
            for(int i = 0; i < COLOR_ELEMENT; i++)
                this->color[i] = 1;
        }
#endif
    }
};

template <class T>
class NLRenderModelData
{
public:
    typedef T NLRenderModelDataItem_t;
    typedef QList<NLRenderModelDataItem_t> NLRenderModelDataItemList;
    explicit NLRenderModelData();
    virtual ~NLRenderModelData();
    virtual NLRenderModelData<T> & Add(const T &item);
    virtual NLRenderModelData<T> & Add(const NLRenderModelDataItemList &items);
    virtual NLRenderModelData<T> & Add(const T item[], int count);
    virtual NLRenderModelData<T> & operator<<(const T &item);
    virtual NLRenderModelData<T> & operator<<(const NLRenderModelDataItemList &items);
    virtual NLRenderModelData<T> & operator()(const T item[], int count);
    T * operator[](int i);
    T operator[](int i) const;
    T At(int i) const;
    int Count() const;
    operator QVector<T> &();
    operator const QVector<T> &() const;
    char * Data();
    const char * Data() const;
    void Clear();
    T * ElementData();
    const T * ElementData() const;
    uint DataSize() const;
    uint ElementSize() const;

private:
    QVector<T> m_data;
    int m_count;
};

template <class T>
class NLRenderModelVertex : public NLRenderModelData<T>
{
public:
    explicit NLRenderModelVertex();
    virtual ~NLRenderModelVertex();
    NLRenderModelVertex<T> & Add(const NLRenderModelVertexItem<T> &item);
    NLRenderModelVertex<T> & Add(const QList<NLRenderModelVertexItem<T> > &items);
    NLRenderModelVertex<T> & Add(const T vertex[VERTEX_ELEMENT], const T texcoord[TEXCOORD_ELEMENT] = 0, const T normal[3] = 0
#if(COLOR_ELEMENT > 0)
            , const T color[COLOR_ELEMENT] = 0
            );
#endif
    NLRenderModelVertex<T> & operator<<(const NLRenderModelVertexItem<T> &item);
    NLRenderModelVertex & operator<<(const QList<NLRenderModelVertexItem<T> > &items);
    NLRenderModelVertex<T> & operator()(const T vertex[VERTEX_ELEMENT], const T texcoord[TEXCOORD_ELEMENT] = 0, const T normal[3] = 0
#if(COLOR_ELEMENT > 0)
            , const T color[COLOR_ELEMENT] = 0
#endif
            );

    NLRenderModelVertex<T> & Add(const T &item);
    NLRenderModelVertex<T> & Add(const QList<T> &items);
    NLRenderModelVertex<T> & Add(const T item[], int count);
    NLRenderModelVertex<T> & operator<<(const T &item);
    NLRenderModelVertex<T> & operator<<(const QList<T> &items);
    NLRenderModelVertex<T> & operator()(const T item[], int count);
    int VertexCount() const;
};

template <class T, class TextureT, class ModeT>
class NLRenderModelPrimitive
{
public:
    typedef ModeT NLRenderModelPrimitiveMode_t;
    typedef TextureT NLRenderModelPrimitiveTexture_t;
    typedef T NLRenderModelPrimitiveElement_t;
    explicit NLRenderModelPrimitive();
    virtual ~NLRenderModelPrimitive();
    ModeT Mode() const;
    void SetMode(const ModeT &t);
    TextureT Texture() const;
    void SetTexture(const TextureT &t);
    operator NLRenderModelData<T> &();
    operator const NLRenderModelData<T> &() const;
    NLRenderModelData<T> & Index();
    const NLRenderModelData<T> & Index() const;
    QColor Color() const;
    void SetColor(const QColor &color);

private:
    NLRenderModelData<T> m_index;
    TextureT m_texture;
    ModeT m_mode;
    QColor m_color;
};

template <class VertexT, class IndexT, class TextureT, class ModeT>
class NLRenderModel
{
public:
    typedef NLRenderModelVertex<VertexT> NLRenderModelVertexData;
    typedef NLRenderModelPrimitive<IndexT, TextureT, ModeT> NLRenderModelPrimitiveData;
    typedef QList<NLRenderModelPrimitiveData> NLRenderModelPrimitiveDataList;
    explicit NLRenderModel();
    virtual ~NLRenderModel();
    virtual void Render() = 0;
    operator NLRenderModelVertexData &();
    operator NLRenderModelPrimitiveDataList &();
    operator const NLRenderModelVertexData &() const;
    operator const NLRenderModelPrimitiveDataList &() const;
    NLRenderModelVertexData & Vertex();
    NLRenderModelPrimitiveDataList & Primitives();
    const NLRenderModelVertexData & Vertex() const;
    const NLRenderModelPrimitiveDataList & Primitives() const;
    NLRenderModelPrimitiveData & Primitive(int i);
    const NLRenderModelPrimitiveData & Primitive(int i) const;
    int PrimitiveCount() const;

private:
    NLRenderModelVertexData m_vertex;
    NLRenderModelPrimitiveDataList m_primitive;
};

template <class VertexT = GLfloat, class IndexT = GLushort>
class NLRenderModelGL : public NLRenderModel<VertexT, IndexT, GLint, GLenum>
{
public:
    explicit NLRenderModelGL();
    virtual ~NLRenderModelGL();
    virtual void Render();

protected:
    GLenum GetVertexType();
    GLenum GetIndexType();

private:
    GLenum m_vertexType;
    GLenum m_indexType;
};

typedef NLRenderModelGL<GLfloat, GLushort> NLRenderModelGLGeneral;



template <class T>
NLRenderModelData<T>::NLRenderModelData()
    : m_count(0)
{

}

template <class T>
NLRenderModelData<T>::~NLRenderModelData()
{
    DEBUG_DESTROY("NLRenderModelData<T>")
}

template <class T>
NLRenderModelData<T> & NLRenderModelData<T>::Add(const T &item)
{
    //uint s = ElementSize();
    //m_data.append((char *)(&item), s);
//    for(int i = 0; i < s; i++)
//    {
//        m_data.append(*((char *)(&item) + i));
//    }
    m_data.append(item);
    m_count++;
    return *this;
}

template <class T>
NLRenderModelData<T> & NLRenderModelData<T>::Add(const NLRenderModelDataItemList &items)
{
    Q_FOREACH(const NLRenderModelDataItem_t &item, items)
        Add(item);
    return *this;
}

template <class T>
NLRenderModelData<T> & NLRenderModelData<T>::Add(const T items[], int count)
{
    for(int i = 0; i < count; i++)
        Add(items[i]);
    return *this;
}

template <class T>
inline NLRenderModelData<T> & NLRenderModelData<T>::operator<<(const T &item)
{
    return Add(item);
}

template <class T>
inline NLRenderModelData<T> & NLRenderModelData<T>::operator<<(const NLRenderModelDataItemList &items)
{
    return Add(items);
}

template <class T>
inline NLRenderModelData<T> & NLRenderModelData<T>::operator()(const T items[], int count)
{
    return Add(items, count);
}

template <class T>
T * NLRenderModelData<T>::operator[](int i)
{
    int index = i >= 0 ? i : m_count + i;
    if(index >= m_count)
        return 0;
    return reinterpret_cast<T *>(m_data.data() + ElementSize() * index);
}

template <class T>
inline T NLRenderModelData<T>::operator[](int i) const
{
    return At(i);
}

template <class T>
T NLRenderModelData<T>::At(int i) const
{
    int index = i >= 0 ? i : m_count + i;
    if(index >= m_count)
        return 0;
    return *reinterpret_cast<const T *>(m_data.constData() + ElementSize() * index);
}

template <class T>
inline int NLRenderModelData<T>::Count() const
{
    return m_count;
}

template <class T>
inline NLRenderModelData<T>::operator QVector<T> &()
{
    return m_data;
}

template <class T>
inline NLRenderModelData<T>::operator const QVector<T> &() const
{
    return m_data;
}

template <class T>
inline char * NLRenderModelData<T>::Data()
{
    return m_data.data();
}

template <class T>
inline const char * NLRenderModelData<T>::Data() const
{
    return m_data.constData();
}

template <class T>
void NLRenderModelData<T>::Clear()
{
    m_data.clear();
    m_count = 0;
}

template <class T>
inline T * NLRenderModelData<T>::ElementData()
{
    return reinterpret_cast<T *>(m_data.data());
}

template <class T>
inline const T * NLRenderModelData<T>::ElementData() const
{
    return reinterpret_cast<const T *>(m_data.constData());
}

template <class T>
uint NLRenderModelData<T>::DataSize() const
{
    return m_data.size();
    return m_count * ElementSize();
}

template <class T>
inline uint NLRenderModelData<T>::ElementSize() const
{
    return sizeof(T);
}



template <class T>
NLRenderModelVertex<T>::NLRenderModelVertex()
    : NLRenderModelData<T>()
{
}

template <class T>
NLRenderModelVertex<T>::~NLRenderModelVertex()
{
    DEBUG_DESTROY("NLRenderModelVertex<T>")
}

template <class T>
NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const NLRenderModelVertexItem<T> &item)
{
    Add(item.vertex, VERTEX_ELEMENT);
    Add(item.texcoord, TEXCOORD_ELEMENT);
    Add(item.normal, 3);
#if(COLOR_ELEMENT > 0)
    Add(item.color, COLOR_ELEMENT);
#endif
    return *this;
}

template <class T>
NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const QList<NLRenderModelVertexItem<T> > &items)
{
    Q_FOREACH(const NLRenderModelVertexItem<T> &item, items)
        Add(item);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator<<(const NLRenderModelVertexItem<T> &item)
{
    return Add(item);
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator<<(const QList<NLRenderModelVertexItem<T> > &items)
{
    return Add(items);
}

template <class T>
NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const T vertex[VERTEX_ELEMENT], const T texcoord[TEXCOORD_ELEMENT], const T normal[3]
#if(COLOR_ELEMENT > 0)
                                                     , const T color[COLOR_ELEMENT]
#endif
                                                     )
{
    Add(vertex, VERTEX_ELEMENT);
    if(texcoord)
        Add(texcoord, TEXCOORD_ELEMENT);
    else
    {
        T arr[TEXCOORD_ELEMENT] = {0};
        Add(arr, TEXCOORD_ELEMENT);
    }
    if(normal)
        Add(normal, 3);
    else
    {
        T arr[3] = {0};
        Add(arr, 3);
    }
#if(COLOR_ELEMENT > 0)
    if(color)
        Add(color, COLOR_ELEMENT);
    else
    {
        T arr[COLOR_ELEMENT] = {1, 1, 1, 1};
        Add(arr, COLOR_ELEMENT);
    }
#endif
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator()(const T vertex[VERTEX_ELEMENT], const T texcoord[TEXCOORD_ELEMENT], const T normal[3]
#if(COLOR_ELEMENT > 0)
                                                            , const T color[COLOR_ELEMENT]
#endif
                                                            )
{
    return Add(vertex, texcoord, normal, color);
}

template <class T>
inline int NLRenderModelVertex<T>::VertexCount() const
{
    return NLRenderModelVertex<T>::Count() / ELEMENT_SIZE;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const T &item)
{
    NLRenderModelData<T>::Add(item);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const QList<T> &items)
{
    NLRenderModelData<T>::Add(items);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::Add(const T item[], int count)
{
    NLRenderModelData<T>::Add(item, count);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator<<(const T &item)
{
    NLRenderModelData<T>::operator<<(item);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator<<(const QList<T> &items)
{
    NLRenderModelData<T>::operator<<(items);
    return *this;
}

template <class T>
inline NLRenderModelVertex<T> & NLRenderModelVertex<T>::operator()(const T item[], int count)
{
    NLRenderModelData<T>::operator()(item, count);
    return *this;
}



template <class T, class TextureT, class ModeT>
NLRenderModelPrimitive<T, TextureT, ModeT>::NLRenderModelPrimitive()
    : m_texture(0),
      m_mode(0)
{

}

template <class T, class TextureT, class ModeT>
NLRenderModelPrimitive<T, TextureT, ModeT>::~NLRenderModelPrimitive()
{
    DEBUG_DESTROY("NLRenderModelPrimitive<T, TextureT, ModeT>")
}

template <class T, class TextureT, class ModeT>
inline ModeT NLRenderModelPrimitive<T, TextureT, ModeT>::Mode() const
{
    return m_mode;
}

template <class T, class TextureT, class ModeT>
void NLRenderModelPrimitive<T, TextureT, ModeT>::SetMode(const ModeT &t)
{
    if(m_mode != t)
        m_mode = t;
}

template <class T, class TextureT, class ModeT>
inline TextureT NLRenderModelPrimitive<T, TextureT, ModeT>::Texture() const
{
    return m_texture;
}

template <class T, class TextureT, class ModeT>
void NLRenderModelPrimitive<T, TextureT, ModeT>::SetTexture(const TextureT &t)
{
    if(m_texture != t)
        m_texture = t;
}

template <class T, class TextureT, class ModeT>
inline NLRenderModelPrimitive<T, TextureT, ModeT>::operator NLRenderModelData<T> &()
{
    return m_index;
}

template <class T, class TextureT, class ModeT>
inline NLRenderModelPrimitive<T, TextureT, ModeT>::operator const NLRenderModelData<T> &() const
{
    return m_index;
}

template <class T, class TextureT, class ModeT>
inline NLRenderModelData<T> & NLRenderModelPrimitive<T, TextureT, ModeT>::Index()
{
    return m_index;
}

template <class T, class TextureT, class ModeT>
inline const NLRenderModelData<T> & NLRenderModelPrimitive<T, TextureT, ModeT>::Index() const
{
    return m_index;
}

template <class T, class TextureT, class ModeT>
inline QColor NLRenderModelPrimitive<T, TextureT, ModeT>::Color() const
{
    return m_color;
}

template <class T, class TextureT, class ModeT>
void NLRenderModelPrimitive<T, TextureT, ModeT>::SetColor(const QColor &color)
{
    if(m_color != color)
        m_color = color;
}



template <class VertexT, class IndexT, class TextureT, class ModeT>
NLRenderModel<VertexT, IndexT, TextureT, ModeT>::NLRenderModel()
{

}

template <class VertexT, class IndexT, class TextureT, class ModeT>
NLRenderModel<VertexT, IndexT, TextureT, ModeT>::~NLRenderModel()
{

}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline NLRenderModel<VertexT, IndexT, TextureT, ModeT>::operator NLRenderModel<VertexT, IndexT, TextureT, ModeT>::NLRenderModelVertexData &()
{
    return m_vertex;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline NLRenderModel<VertexT, IndexT, TextureT, ModeT>::operator NLRenderModel<VertexT, IndexT, TextureT, ModeT>::NLRenderModelPrimitiveDataList &()
{
    return m_primitive;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline NLRenderModel<VertexT, IndexT, TextureT, ModeT>::operator const NLRenderModel<VertexT, IndexT, TextureT, ModeT>::NLRenderModelVertexData &() const
{
    return m_vertex;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline NLRenderModel<VertexT, IndexT, TextureT, ModeT>::operator const NLRenderModel<VertexT, IndexT, TextureT, ModeT>::NLRenderModelPrimitiveDataList &() const
{
    return m_primitive;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline int NLRenderModel<VertexT, IndexT, TextureT, ModeT>::PrimitiveCount() const
{
    return m_primitive.count();
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline NLRenderModelVertex<VertexT> &
NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Vertex()
{
    return m_vertex;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline QList<NLRenderModelPrimitive<IndexT, TextureT, ModeT> > & NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Primitives()
{
    return m_primitive;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline const NLRenderModelVertex<VertexT> & NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Vertex() const
{
    return m_vertex;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
inline const QList<NLRenderModelPrimitive<IndexT, TextureT, ModeT> > & NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Primitives() const
{
    return m_primitive;
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
NLRenderModelPrimitive<IndexT, TextureT, ModeT> & NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Primitive(int i)
{
    int index = i >= 0 ? i : m_primitive.count() + i;
    return m_primitive[index];
}

template <class VertexT, class IndexT, class TextureT, class ModeT>
const NLRenderModelPrimitive<IndexT, TextureT, ModeT> & NLRenderModel<VertexT, IndexT, TextureT, ModeT>::Primitive(int i) const
{
    int index = i >= 0 ? i : m_primitive.count() + i;
    return m_primitive[index];
}



template <class VertexT, class IndexT>
NLRenderModelGL<VertexT, IndexT>::NLRenderModelGL()
    : NLRenderModel<VertexT, IndexT, GLint, GLenum>()
{
    m_vertexType = GetVertexType();
    m_indexType = GetIndexType();
}

template <class VertexT, class IndexT>
NLRenderModelGL<VertexT, IndexT>::~NLRenderModelGL()
{

}

template <class VertexT, class IndexT>
void NLRenderModelGL<VertexT, IndexT>::Render()
{
    const int Count = NLRenderModelGL<VertexT, IndexT>::PrimitiveCount();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    const char *vertex = (char *)NLRenderModelGL<VertexT, IndexT>::Vertex().ElementData();
    const int Size = NLRenderModelGL<VertexT, IndexT>::Vertex().ElementSize();

    glVertexPointer(VERTEX_ELEMENT, m_vertexType, Size * ELEMENT_SIZE, vertex);
    glTexCoordPointer(TEXCOORD_ELEMENT, m_vertexType, Size * ELEMENT_SIZE, vertex + Size * VERTEX_ELEMENT);
    glNormalPointer(m_vertexType, Size * ELEMENT_SIZE, vertex + Size * (VERTEX_ELEMENT + TEXCOORD_ELEMENT));

    GLfloat color[4];
    glGetFloatv(GL_CURRENT_COLOR, color);

    for(int i = 0; i < Count; i++)
    {
        const NLRenderModelPrimitive<IndexT, GLint, GLenum> &p = NLRenderModelGL<VertexT, IndexT>::Primitive(i);
        GLint tex = p.Texture();
        if(glIsTexture(tex))
            glBindTexture(GL_TEXTURE_2D, tex);
        QColor c = p.Color();
        if(c.isValid())
        {
            const GLfloat cv[4] = {c.redF(), c.greenF(), c.blueF(), c.alphaF()};
            glColor4fv(cv);
        }
        else
        {
#if(COLOR_ELEMENT > 0)
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(COLOR_ELEMENT, m_vertexType, Size * ELEMENT_SIZE, vertex + Size * (VERTEX_ELEMENT + TEXCOORD_ELEMENT + 3));
#endif
        }
        glDrawElements(p.Mode(), p.Index().Count(), m_indexType, p.Index().ElementData());
        glBindTexture(GL_TEXTURE_2D, 0);
        if(!c.isValid())
        {
#if(COLOR_ELEMENT > 0)
            glDisableClientState(GL_COLOR_ARRAY);
#endif
        }
    }

    glColor4fv(color);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

#define GLtype_ENUM(t) \
    if(NL::is_GLfloat(t)) return GL_FLOAT; \
    if(NL::is_GLushort(t)) return GL_UNSIGNED_SHORT; \
    if(NL::is_GLuint(t)) return GL_UNSIGNED_INT; \
    if(NL::is_GLbyte(t)) return GL_BYTE; \
    if(NL::is_GLubyte(t)) return GL_UNSIGNED_BYTE; \
    if(NL::is_GLshort(t)) return GL_SHORT; \
    if(NL::is_GLint(t)) return GL_INT; \
    if(NL::is_GLdouble(t)) return GL_DOUBLE;

template <class VertexT, class IndexT>
GLenum NLRenderModelGL<VertexT, IndexT>::GetVertexType()
{
    VertexT t = VertexT();
    GLtype_ENUM(t)
    return GL_FLOAT;
}

template <class VertexT, class IndexT>
GLenum NLRenderModelGL<VertexT, IndexT>::GetIndexType()
{
    IndexT t = IndexT();
    GLtype_ENUM(t)
    return GL_UNSIGNED_SHORT;
}

#undef GLtype_ENUM

#endif // _KARIN_NLRENDERMODEL_H
