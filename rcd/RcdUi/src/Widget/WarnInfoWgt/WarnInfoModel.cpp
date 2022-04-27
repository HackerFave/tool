#include "WarnInfoModel.h"

WarnInfoModel::WarnInfoModel(QObject *parent) : IModel(parent)
{
    m_url = "https://www.baidu.com";
}

void WarnInfoModel::setUrl(QString &url)
{
    m_url = url;
    emit sendModelUpdate(MC_LoadUrl);
}
