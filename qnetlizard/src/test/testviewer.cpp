#include "testviewer.h"

#include <QDebug>

#include "testwidget.h"
#include "netlizard.h"
#include "qdef.h"

TestViewer::TestViewer(QWidget *parent) :
    BaseViewer(parent),
    m_widget(0)
{
    setObjectName("TestViewer");
    Init();
}

TestViewer::~TestViewer()
{
}

void TestViewer::Init()
{
    m_widget = new TestWidget;
    SetCentralWidget(m_widget);
    SetTitle("Test viewer");
}
