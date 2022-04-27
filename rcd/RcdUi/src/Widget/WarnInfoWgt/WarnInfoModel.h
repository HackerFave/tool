#ifndef WarnInfoModel_H
#define WarnInfoModel_H

#include "Interface/IModel.h"

class WarnInfoModel : public IModel
{
    Q_OBJECT
public:
    explicit WarnInfoModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_LoadUrl,

        MC_Max
    };

    void setUrl(QString &url);

    QString m_url;
};

#endif // WarnInfoModel_H
