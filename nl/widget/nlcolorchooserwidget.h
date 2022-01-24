#ifndef _KARIN_NLCOLORCHOOSERWIDGET_H
#define _KARIN_NLCOLORCHOOSERWIDGET_H

#include <QWidget>

class NLColorChooserWidgetLabel;

class NLColorChooserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NLColorChooserWidget(QWidget *widget = 0);
    virtual ~NLColorChooserWidget();
    QColor Color() const { return m_color; }

public Q_SLOTS:
    void SetColor(const QColor &color);
    void SetReadOnly(bool b);

Q_SIGNALS:
    void colorChanged(const QColor &color);

private Q_SLOTS:
    void OpenColorDialog();

private:
    void Init();
    void UpdateWidget();

private:
    QColor m_color;
    NLColorChooserWidgetLabel *m_openButton;

    Q_DISABLE_COPY(NLColorChooserWidget)
};

#endif // _KARIN_NLCOLORCHOOSERWIDGET_H
