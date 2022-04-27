#ifndef CloudWebModel_H
#define CloudWebModel_H

#include "Interface/IModel.h"

class CloudWebModel : public IModel
{
    Q_OBJECT
public:
    explicit CloudWebModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_LoadUrl,

        MC_Max
    };

    void setUrl(QString &url);

    QString m_url;
};

#endif // CloudWebModel_H
