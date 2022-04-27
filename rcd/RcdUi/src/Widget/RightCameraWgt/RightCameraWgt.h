#ifndef RightCameraWgt_H
#define RightCameraWgt_H

#include "Interface/IFunWidget.h"
#include "RightCameraCL.h"
#include "RightCameraModel.h"

namespace Ui {
class RightCameraWgt;
}

class TitleWgt;

class RightCameraWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit RightCameraWgt(QWidget *parent = nullptr);
    ~RightCameraWgt() override;

    static RightCameraWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

private:

    Ui::RightCameraWgt           *ui;
    static RightCameraWgt        *s_pInstance;
    RightCameraCL                *m_pCL;
    RightCameraModel             *m_pModel;

    TitleWgt                     *m_pTitleWgt;

};

#endif // RightCameraWgt_H
