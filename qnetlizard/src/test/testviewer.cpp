#include "testviewer.h"

#include <QDebug>

#include "testscene.h"
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
    m_widget = new TestScene;
    SetCentralWidget(m_widget);
    SetTitle("Test viewer");
}
