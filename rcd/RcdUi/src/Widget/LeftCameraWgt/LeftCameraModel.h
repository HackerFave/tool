#ifndef LeftCameraModel_H
#define LeftCameraModel_H

#include "Interface/IModel.h"

class LeftCameraModel : public IModel
{
    Q_OBJECT
public:
    explicit LeftCameraModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,

        MC_Max
    };

    QString m_url;
    QString m_name;
};

#endif // LeftCameraModel_H
