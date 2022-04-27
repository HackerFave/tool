#ifndef VehicleInfoWgt_H
#define VehicleInfoWgt_H

#include "Interface/IFunWidget.h"
#include "VehicleInfoCL.h"
#include "VehicleInfoModel.h"
//#include "QtSimpleMap.h"
namespace Ui {
class VehicleInfoWgt;
}

class Dashboard;
class AnalogClock;

class VehicleInfoWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit VehicleInfoWgt(QWidget *parent = nullptr);
    ~VehicleInfoWgt() override;

    static VehicleInfoWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;


private:
    Ui::VehicleInfoWgt *ui;
    static VehicleInfoWgt        *s_pInstance;
    VehicleInfoCL                *m_pCL;
    VehicleInfoModel             *m_pModel;
    Dashboard                    *m_pDashboard;
    AnalogClock                  *m_pAnalogClock;
//    QtSimpleMap                  *p_mapWidget = nullptr;
};

#endif // VehicleInfoWgt_H
