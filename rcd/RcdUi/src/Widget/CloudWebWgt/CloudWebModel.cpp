#include "CloudWebModel.h"
#include <QSettings>

CloudWebModel::CloudWebModel(QObject *parent) : IModel(parent)
{
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    m_url = config.value(QString("Url/MapUrl")).toString();
}

void CloudWebModel::setUrl(QString &url)
{
    m_url = url;
    emit sendModelUpdate(MC_LoadUrl);
}
