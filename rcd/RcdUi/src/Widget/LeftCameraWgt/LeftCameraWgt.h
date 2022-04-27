#ifndef LeftCameraWgt_H
#define LeftCameraWgt_H

#include "Interface/IFunWidget.h"
#include "LeftCameraCL.h"
#include "LeftCameraModel.h"

namespace Ui {
class LeftCameraWgt;
}

class TitleWgt;

class LeftCameraWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit LeftCameraWgt(QWidget *parent = nullptr);
    ~LeftCameraWgt() override;

    static LeftCameraWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

private:
    Ui::LeftCameraWgt *ui;
    static LeftCameraWgt        *s_pInstance;
    LeftCameraCL                *m_pCL;
    LeftCameraModel             *m_pModel;

    TitleWgt                    *m_pTitleWgt;
};

#endif // LeftCameraWgt_H
