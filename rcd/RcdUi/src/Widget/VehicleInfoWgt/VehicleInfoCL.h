#ifndef VehicleInfoCL_H
#define VehicleInfoCL_H

#include "Interface/IController.h"
#include "VehicleInfoModel.h"

class VehicleInfoCL : public IController
{
    Q_OBJECT
public:
    explicit VehicleInfoCL(QObject *parent = nullptr);
    ~VehicleInfoCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(VehicleInfoModel *model);

private:

    void g2hCmdMgsDeal(QVariant &data);

    VehicleInfoModel *m_pModel;
};

#endif // VehicleInfoCL_H
