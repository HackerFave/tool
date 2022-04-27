#ifndef WarnInfoWgt_H
#define WarnInfoWgt_H

#include "Interface/IFunWidget.h"
#include "WarnInfoCL.h"
#include "WarnInfoModel.h"

namespace Ui {
class WarnInfoWgt;
}

class Dashboard;
class AnalogClock;

class WarnInfoWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit WarnInfoWgt(QWidget *parent = nullptr);
    ~WarnInfoWgt() override;

    static WarnInfoWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

private:
    Ui::WarnInfoWgt *ui;
    static WarnInfoWgt        *s_pInstance;
    WarnInfoCL                *m_pCL;
    WarnInfoModel             *m_pModel;
    Dashboard                    *m_pDashboard;
    AnalogClock                  *m_pAnalogClock;
};

#endif // WarnInfoWgt_H
