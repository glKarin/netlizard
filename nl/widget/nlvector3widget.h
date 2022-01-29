#ifndef _KARIN_NLVECTOR3WIDGET_H
#define _KARIN_NLVECTOR3WIDGET_H

#include <QWidget>

#include "common/nlvec.h"

class QDoubleSpinBox;

class NLVector3Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NLVector3Widget(QWidget *widget = 0);
    virtual ~NLVector3Widget();
    NLVector3 Vector3() const { return m_vector3; }

public Q_SLOTS:
    void SetVector3(const NLVector3 &v);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void vector3Changed(const NLVector3 &vector);

private Q_SLOTS:
    void OnValueChanged(double d);

private:
    void Init();
    void UpdateWidget();

private:
    NLVector3 m_vector3;
    QDoubleSpinBox *m_xSpinBox;
    QDoubleSpinBox *m_ySpinBox;
    QDoubleSpinBox *m_zSpinBox;

    Q_DISABLE_COPY(NLVector3Widget)
};

#endif // _KARIN_NLVECTOR3WIDGET_H
