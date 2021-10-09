#ifndef _KARIN_ITEMWIDGET_H
#define _KARIN_ITEMWIDGET_H

#include "nlscene.h"
#include "gl/nl_gl.h"

class ItemWidget : public NLScene
{
    Q_OBJECT
public:
    ItemWidget(QWidget *parent = 0);
    virtual ~ItemWidget();
    bool IsValid() const;
    const GL_NETLizard_3D_Model * Model() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int index);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

private:
    GL_NETLizard_3D_Model *m_model;

    Q_DISABLE_COPY(ItemWidget)
};

#endif // _KARIN_ITEMWIDGET_H
