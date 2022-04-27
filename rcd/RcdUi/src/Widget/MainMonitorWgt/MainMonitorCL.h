#ifndef MainMonitorCL_H
#define MainMonitorCL_H

#include "Interface/IController.h"
#include "MainMonitorModel.h"

class MainMonitorCL : public IController
{
    Q_OBJECT
public:
    explicit MainMonitorCL(QObject *parent = nullptr);
    ~MainMonitorCL() override;

    enum CLCmdType
    {
        CL_UpdateAll=0,
        CL_UpdateVehicle,
        CL_UpdateSelect,
        CL_InputKey,

        CL_Max
    };

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(MainMonitorModel *model);

private:

    void inputHandler(Qt::Key key);
    void vehicleStatusHandler(QVariant &data);
    void refreshDisplayData();

    MainMonitorModel *m_pModel;
};

#endif // MainMonitorCL_H
