#include "nlguiutility.h"

#include <QLayout>
#include <QFormLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>

void NLGUIUtility::ClearLayout(QLayout *layout)
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
            delete item;
            item = 0;
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

void NLGUIUtility::ClearLayoutLater(QLayout *layout)
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

int NLGUIUtility::TakeLayout(QLayout *layout, QVariantList *ret)
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
            //qDebug() << c << widget << widget->parent();
            widget->setParent(0);
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

int NLGUIUtility::TakeFormLayout(QFormLayout *layout, QVariantList *ret)
{
    if(!layout)
        return 0;
    int c = 0;

    QLayoutItem *item = 0;
    QList<QWidget *> labels;
    for(int i = 0; i < layout->count(); i++)
    {
        QFormLayout::ItemRole role;
        int row = -1;
        item = layout->itemAt(i);
        QWidget *widget = item->widget();
        if(widget)
        {
            layout->getWidgetPosition(widget, &row, &role);
            if(row >= 0 && role == QFormLayout::LabelRole)
                labels.push_back(widget);
        }
    }
    // TODO: rowCount() will not be 0
    while((item = layout->takeAt(0)))
    {
        if(item->widget())
        {
            QWidget *widget = item->widget();
            //qDebug()  << widget << widget->parent();
            widget->hide();
            widget->setParent(0);

            if(labels.contains(widget))
            {
                //qDebug() << "label" << static_cast<QLabel *>(widget)->text() << layout->count() << layout->rowCount();
                delete widget;
            }
            else
            {
                //qDebug() << "field" << widget << layout->count() << layout->rowCount();
                if(ret)
                    ret->push_back(QVariant::fromValue<QWidget *>(widget));
                c++;
            }
        }
        else if(item->layout())
        {
            QLayout *l = item->layout();
            if(ret)
            {
                QVariantList list;
                QFormLayout *fl = dynamic_cast<QFormLayout *>(l);
                if(fl)
                    c += TakeFormLayout(fl, &list);
                else
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
