#include "stringviewer.h"

#include <QtGui/QtGui>

#include "netlizard.h"

StringViewer::StringViewer(QWidget *parent) :
    QWidget(parent),
    m_encodeLabel(0),
    m_decodeLabel(0),
    m_encodeInput(0),
    m_decodeInput(0)
{
    Init();
}

StringViewer::~StringViewer()
{

}

void StringViewer::Init()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button;
    m_encodeInput = new QTextEdit(this);
    m_decodeInput = new QTextEdit(this);
    QVBoxLayout *toolLayout = new QVBoxLayout(this);
    m_encodeLabel = new QLabel(this);
    m_decodeLabel = new QLabel(this);
    QHBoxLayout *hLayout;
    QHBoxLayout *thLayout;

    hLayout = new QHBoxLayout(this);
    m_encodeLabel->setFixedHeight(18);
    m_encodeLabel->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(m_encodeLabel);
    m_decodeLabel->setFixedHeight(18);
    m_decodeLabel->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(m_decodeLabel);
    layout->addLayout(hLayout);

    toolLayout->addStretch();
    button = new QPushButton(this);
    button->setText(">>");
    button->setMaximumWidth(32);
    connect(button, SIGNAL(clicked()), this, SLOT(DecodeString()));
    toolLayout->addWidget(button);
    button = new QPushButton(this);
    button->setText("<<");
    button->setMaximumWidth(32);
    connect(button, SIGNAL(clicked()), this, SLOT(EncodeString()));
    toolLayout->addWidget(button);
    toolLayout->addStretch();

    hLayout = new QHBoxLayout(this);
    hLayout->addWidget(m_encodeInput);
    hLayout->addLayout(toolLayout);
    hLayout->addWidget(m_decodeInput);
    layout->addLayout(hLayout);

    hLayout = new QHBoxLayout(this);

    thLayout = new QHBoxLayout(this);
    button = new QPushButton(this);
    button->setText("Clear");
    connect(button, SIGNAL(clicked()), m_encodeInput, SLOT(clear()));
    thLayout->addStretch();
    thLayout->addWidget(button);
    thLayout->addStretch();
    hLayout->addLayout(thLayout);

    thLayout = new QHBoxLayout(this);
    button = new QPushButton(this);
    button->setText("Clear");
    connect(button, SIGNAL(clicked()), m_decodeInput, SLOT(clear()));
    thLayout->addStretch();
    thLayout->addWidget(button);
    thLayout->addStretch();
    hLayout->addLayout(thLayout);

    layout->addLayout(hLayout);

    setLayout(layout);
    setWindowTitle("NETLizard string resource viewer");
    resize(480, 360);
}

bool StringViewer::DecodeString()
{
    m_decodeLabel->setText("");
    m_decodeInput->setText("");

    QString text = m_encodeInput->toPlainText();
    text.replace(",", " ");
    QStringList list = text.split(QRegExp("\\s+"));
    if(list.isEmpty())
        return false;

    QList<NLint> res;
    Q_FOREACH(const QString &str, list)
    {
        bool ok;
        NLint i = str.toInt(&ok);
        if(ok)
            res << i;
    }

    NLint *arr = new NLint[res.size()];
    for(int i = 0; i < res.size(); i++)
        arr[i] = res.at(i);

    char *str = nlParseStringi(arr, res.size());
    if(!str)
        return false;
    m_decodeInput->setText(str);
    m_decodeLabel->setText(QString("Decode length: %1").arg(strlen(str)));
    free(str);
    delete[] arr;
    return true;
}


bool StringViewer::EncodeString()
{
    m_encodeLabel->setText("");
    m_encodeInput->setText("");

    QString text = m_decodeInput->toPlainText();
    NLsizei len;
    NLint *arr = nlEncodeStringC(text.toStdString().c_str(), &len);
    if(!arr)
        return false;

    QStringList list;
    for(int i = 0; i < len; i++)
        list << QString::number(arr[i]);

    m_encodeInput->setText(list.join(", "));
    m_encodeLabel->setText(QString("Encode length: %1").arg(len));
    free(arr);
    return true;
}
