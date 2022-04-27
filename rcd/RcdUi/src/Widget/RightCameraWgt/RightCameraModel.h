#ifndef RightCameraModel_H
#define RightCameraModel_H

#include "Interface/IModel.h"

class RightCameraModel : public IModel
{
    Q_OBJECT
public:
    explicit RightCameraModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,

        MC_Max
    };

    quint8    m_select;
    QString   m_url;
    QString   m_name;
};

#endif // RightCameraModel_H
