#ifndef DriveCameraWgt_H
#define DriveCameraWgt_H

#include "Interface/IFunWidget.h"
#include "DriveCameraCL.h"

namespace Ui {
class DriveCameraWgt;
}

class TitleWgt;

class DriveCameraWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit DriveCameraWgt(QWidget *parent = nullptr);
    ~DriveCameraWgt() override;

    static DriveCameraWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;

private:
    Ui::DriveCameraWgt *ui;
    static DriveCameraWgt        *s_pInstance;
    DriveCameraCL                *m_pCL;

    TitleWgt                     *m_pTitleWgt;

};

#endif // DriveCameraWgt_H
