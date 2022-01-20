#ifndef _KARIN_NLGUIUTILITY_H
#define _KARIN_NLGUIUTILITY_H

#include <QVariant>

#include "engine/nldef.h"

class QLayout;
class QFormLayout;

class NLLIB_EXPORT NLGUIUtility
{
public:
    static void ClearLayout(QLayout *layout);
    static int TakeLayout(QLayout *layout, QVariantList *ret = 0);
    static int TakeFormLayout(QFormLayout *layout, QVariantList *ret = 0);
    static void ClearLayoutLater(QLayout *layout);

    static void UnsetFixedSize(QWidget *widget);
    static void UnsetFixedWidth(QWidget *widget);
    static void UnsetFixedHeight(QWidget *widget);

private:
    NLGUIUtility();
};

#endif // _KARIN_NLGUIUTILITY_H
