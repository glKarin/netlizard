#include "stringviewer.h"

#include <QDebug>

#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "netlizard.h"
#include "qdef.h"

StringViewer::StringViewer(QWidget *parent) :
    BaseViewer(parent),
    m_encodeLabel(0),
    m_decodeLabel(0),
    m_encodeInput(0),
    m_decodeInput(0),
    m_encodeClearButton(0),
    m_decodeClearButton(0),
    m_encodeButton(0),
    m_decodeButton(0)
{
    setObjectName("StringViewer");
    Init();
}

StringViewer::~StringViewer()
{
}

void StringViewer::Init()
{
    QHBoxLayout *layout = new QHBoxLayout;
    m_encodeInput = new QTextEdit;
    m_decodeInput = new QTextEdit;
    QVBoxLayout *toolLayout = new QVBoxLayout;
    m_encodeLabel = new QLabel;
    m_decodeLabel = new QLabel;
    QVBoxLayout *vLayout;
    QHBoxLayout *thLayout;
    SetTitleLabelVisible(false);

    vLayout = new QVBoxLayout;
    m_encodeLabel->setText("Encode text");
    m_encodeLabel->setWordWrap(true);
    vLayout->addWidget(m_encodeLabel);
    vLayout->addWidget(m_encodeInput);
    connect(m_encodeInput, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
    thLayout = new QHBoxLayout;
    m_encodeClearButton = new QPushButton;
    m_encodeClearButton->setText("Clear");
    m_encodeClearButton->setEnabled(false);
    thLayout->addStretch();
    thLayout->addWidget(m_encodeClearButton);
    thLayout->addStretch();
    connect(m_encodeClearButton, SIGNAL(clicked()), this, SLOT(ClearInput()));
    vLayout->addLayout(thLayout);
    layout->addLayout(vLayout);

    toolLayout->addStretch();
    m_decodeButton = new QPushButton;
    m_decodeButton->setText(">>");
    m_decodeButton->setMaximumWidth(24);
    m_decodeButton->setEnabled(false);
    connect(m_decodeButton, SIGNAL(clicked()), this, SLOT(DecodeString()));
    toolLayout->addWidget(m_decodeButton);
    m_encodeButton = new QPushButton;
    m_encodeButton->setText("<<");
    m_encodeButton->setMaximumWidth(24);
    m_encodeButton->setEnabled(false);
    connect(m_encodeButton, SIGNAL(clicked()), this, SLOT(EncodeString()));
    toolLayout->addWidget(m_encodeButton);
    toolLayout->addStretch();
    layout->addLayout(toolLayout);

    vLayout = new QVBoxLayout;
    m_decodeLabel->setText("Decode text");
    m_decodeLabel->setWordWrap(true);
    vLayout->addWidget(m_decodeLabel);
    vLayout->addWidget(m_decodeInput);
    connect(m_decodeInput, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
    thLayout = new QHBoxLayout;
    m_decodeClearButton = new QPushButton;
    m_decodeClearButton->setText("Clear");
    m_decodeClearButton->setEnabled(false);
    thLayout->addStretch();
    thLayout->addWidget(m_decodeClearButton);
    thLayout->addStretch();
    connect(m_decodeClearButton, SIGNAL(clicked()), this, SLOT(ClearInput()));
    vLayout->addLayout(thLayout);
    layout->addLayout(vLayout);

    TitleLabel()->setVisible(false);
    CentralWidget()->setLayout(layout);
    SetTitle("NETLizard string resource viewer");
}

bool StringViewer::DecodeString()
{
    Reset(2);

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

    char *str = nlDecodeStringi(arr, res.size());
    if(!str)
        return false;
    m_decodeInput->setText(str);
    m_decodeLabel->setText(QString("Decode text - length: %1").arg(strlen(str)));
    free(str);
    delete[] arr;
    return true;
}


bool StringViewer::EncodeString()
{
    Reset(1);

    QString text = m_decodeInput->toPlainText();
    NLint len;
    NLint *arr = nlEncodeStringC(text.toStdString().c_str(), &len);
    if(!arr)
        return false;

    QStringList list;
    for(int i = 0; i < len; i++)
        list << QString::number(arr[i]);

    m_encodeInput->setText(list.join(", "));
    m_encodeLabel->setText(QString("Encode text - length: %1").arg(len));
    free(arr);
    return true;
}

void StringViewer::Reset(int mask)
{
    BaseViewer::Reset();
    m_encodeLabel->setText("Encode text");
    m_decodeLabel->setText("Decode text");
    if(mask & 1)
        m_encodeInput->clear();
    if(mask & 2)
        m_decodeInput->clear();
}

void StringViewer::ClearInput()
{
    QObject *obj = sender();
    if(obj == m_encodeClearButton)
    {
        Reset(1);
        m_encodeInput->setFocus(Qt::MouseFocusReason);
    }
    else if(obj == m_decodeClearButton)
    {
        Reset(2);
        m_decodeInput->setFocus(Qt::MouseFocusReason);
    }
}

void StringViewer::OnTextChanged()
{
    QObject *obj = sender();
    if(obj == m_encodeInput)
    {
        bool empty = m_encodeInput->toPlainText().isEmpty();
        m_decodeButton->setEnabled(!empty);
        m_encodeClearButton->setEnabled(!empty);
    }
    else if(obj == m_decodeInput)
    {
        bool empty = m_decodeInput->toPlainText().isEmpty();
        m_encodeButton->setEnabled(!empty);
        m_decodeClearButton->setEnabled(!empty);
    }
}
