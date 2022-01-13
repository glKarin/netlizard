#include "guiutility.h"

#include <QLayout>
#include <QWidget>
#include <QDebug>

void GuiUtility::ClearLayout(QLayout *layout)
{
    if(!layout)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != 0)
    {
        if(item->widget())
        {
            QWidget *widget = item->widget();
            widget->hide();
            widget->setParent(0);
            delete widget;
        }
        else if(item->layout())
        {
            QLayout *l = item->layout();
            ClearLayout(l);
            //l->setParent(0);
        }
        delete item;
    }
}

void GuiUtility::ClearLayoutLater(QLayout *layout)
{
    if(!layout)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != 0)
    {
        if(item->widget())
        {
            QWidget *widget = item->widget();
            widget->hide();
            widget->setParent(0);
            widget->deleteLater();
        }
        else if(item->layout())
        {
            QLayout *l = item->layout();
            ClearLayout(l);
            //l->setParent(0);
        }
        delete item;
    }
}

int GuiUtility::TakeLayout(QLayout *layout, QVariantList *ret)
{
    if(!layout)
        return 0;
    int c = 0;

    QLayoutItem *item;
    while((item = layout->takeAt(0)) != 0)
    {
        if(item->widget())
        {
            QWidget *widget = item->widget();
            widget->hide();
            qDebug() << widget->objectName() << widget->parent();
            widget->setParent(0);
            qDebug() << widget << widget->parent();
            if(ret)
                ret->push_back(QVariant::fromValue<QWidget *>(widget));
            c++;
        }
        else if(item->layout())
        {
            QLayout *l = item->layout();
            if(ret)
            {
                QVariantList list;
                c += TakeLayout(l, &list);
                ret->push_back(list);
            }
            else
                c += TakeLayout(l);
            //l->setParent(0);
        }
        delete item;
    }
    return c;
}
