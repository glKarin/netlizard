#ifndef _KARIN_GUIUTILITY_H
#define _KARIN_GUIUTILITY_H

#include <QVariant>

class QLayout;
class QFormLayout;

class GuiUtility
{
public:
    static void ClearLayout(QLayout *layout);
    static int TakeLayout(QLayout *layout, QVariantList *ret = 0);
    static int TakeFormLayout(QFormLayout *layout, QVariantList *ret = 0);
    static void ClearLayoutLater(QLayout *layout);

private:
    GuiUtility();
};

#endif // _KARIN_GUIUTILITY_H
