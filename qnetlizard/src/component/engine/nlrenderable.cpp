#include "nlrenderable.h"

#include <QDebug>

#include "qdef.h"

NLRenderable::NLRenderable(QObject *parent) :
    QObject(parent)
{
    setObjectName("NLObject");
}

NLRenderable::~NLRenderable()
{
    DEBUG_DESTROY_Q
}
