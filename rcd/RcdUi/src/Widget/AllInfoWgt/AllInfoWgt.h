#ifndef AllInfoWgt_H
#define AllInfoWgt_H

#include "Interface/IFunWidget.h"
#include "AllInfoCL.h"
#include "AllInfoModel.h"

namespace Ui {
class AllInfoWgt;
}

class Dashboard;
class AnalogClock;

class AllInfoWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit AllInfoWgt(QWidget *parent = nullptr);
    ~AllInfoWgt() override;

    static AllInfoWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

private:
    Ui::AllInfoWgt *ui;
    static AllInfoWgt        *s_pInstance;
    AllInfoCL                *m_pCL;
    AllInfoModel             *m_pModel;
    Dashboard                    *m_pDashboard;
    AnalogClock                  *m_pAnalogClock;
};

#endif // AllInfoWgt_H
