#ifndef AllInfoModel_H
#define AllInfoModel_H

#include "Interface/IModel.h"

class AllInfoModel : public IModel
{
    Q_OBJECT
public:
    explicit AllInfoModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_LoadUrl,

        MC_Max
    };

    void setUrl(QString &url);

    QString m_url;
};

#endif // AllInfoModel_H
