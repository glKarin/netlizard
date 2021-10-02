#ifndef _KARIN_MAPWIDGET_H
#define _KARIN_MAPWIDGET_H

#include "glwidget.h"
#include "gl/nl_gl.h"

class MapWidget : public GLWidget
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent = 0);
    virtual ~MapWidget();
    bool IsValid() const;
    const GL_NETLizard_3D_Model * Model() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int level);
    void Reset();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

private:
    void RenderSky();

private:
    GL_NETLizard_3D_Model *m_model;

    Q_DISABLE_COPY(MapWidget)
};

#endif // _KARIN_MAPWIDGET_H
