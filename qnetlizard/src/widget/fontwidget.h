#ifndef _KARIN_FONTWIDGET_H
#define _KARIN_FONTWIDGET_H

#include "nlscene.h"
#include "gl/nl_font.h"

class QColor;

class FontWidget : public NLScene
{
    Q_OBJECT
public:
    explicit FontWidget(QWidget *parent = 0);
    virtual ~FontWidget();
    bool IsValid() const;

public Q_SLOTS:
    bool LoadFile(const QString &cfFile, const QString &fntFile);
    void Reset();
    void SetText(const QString &str);

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();
    
signals:
    
public slots:

private:
    void RenderText();

private:
    font_s *m_font;
    QString m_text;
    int m_lineCount;
    int m_paddingWidth;
    int m_lineSpacing;

    Q_DISABLE_COPY(FontWidget)
    
};

#endif // _KARIN_FONTWIDGET_H
