#include "settingdialog.h"

#include <QDebug>

#include "settings.h"
#include "def.h"

struct SettingItem {
    QString name;
    QVariant value;
    QString type;
    QVariantHash prop;
};

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    setObjectName("SettingDialog");
}

SettingDialog::~SettingDialog()
{
    DEBUG_DESTROY_Q
}

void SettingDialog::Init()
{

}
