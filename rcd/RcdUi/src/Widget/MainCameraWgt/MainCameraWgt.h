#ifndef MainCameraWgt_H
#define MainCameraWgt_H

#include "Interface/IFunWidget.h"
#include "MainCameraCL.h"
#include "MainCameraModel.h"
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include "Component/backTrackLine.h"
namespace Ui {
class MainCameraWgt;
}

class QStackedLayout;

class MainCameraWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit MainCameraWgt(QWidget *parent = nullptr);
    ~MainCameraWgt() override;

    static MainCameraWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;
public slots:

    //virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

    void slotTest();//测试
protected:
    //void drawBackLine4(QLabel * label);//绘制轨迹
private:

    QString getRemoteStatusStr(MainCameraModel::RemoteStatus);
    void    changeLayout(int);

    Ui::MainCameraWgt *ui;
    static MainCameraWgt        *s_pInstance;
    MainCameraCL                *m_pCL;//控制类
    MainCameraModel             *m_pModel;//模型类
    QStackedLayout              *m_pSLayout;//布局
    QTimer _timerTest;//测试
    int _isR = 0;
    backTrackLine               *widget_ReversingTrackLine_b = nullptr;
    backTrackLine               *widget_ReversingTrackLine_f = nullptr;
};

#endif // MainCameraWgt_H
