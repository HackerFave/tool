#include "AllInfoModel.h"

AllInfoModel::AllInfoModel(QObject *parent) : IModel(parent)
{
    m_url = "https://www.baidu.com";
}

void AllInfoModel::setUrl(QString &url)
{
    m_url = url;
    emit sendModelUpdate(MC_LoadUrl);
}
