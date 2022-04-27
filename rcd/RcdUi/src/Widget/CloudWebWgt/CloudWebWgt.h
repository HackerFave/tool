#ifndef CloudWebWgt_H
#define CloudWebWgt_H

#include <QResizeEvent>
#include "Interface/IFunWidget.h"
#include "CloudWebCL.h"
#include "CloudWebModel.h"

namespace Ui {
class CloudWebWgt;
}

class WebEngine;

class CloudWebWgt : public IFunWidget
{
    Q_OBJECT

public:
    explicit CloudWebWgt(QWidget *parent = nullptr);
    ~CloudWebWgt() override;

    static CloudWebWgt* GetInstance();
    virtual bool isQuit() override;
    virtual void initWidget() override;
    virtual void updateWidget() override;

protected:

    virtual void resizeEvent(QResizeEvent *event) override;

public slots:

    virtual void onClDataCome(int cmd, QVariant data = INT_INVALID) override;
    virtual void onModelUpdate(int cmd) override;

private:
    Ui::CloudWebWgt *ui;
    static CloudWebWgt        *s_pInstance;
    CloudWebCL                *m_pCL;
    CloudWebModel             *m_pModel;
    WebEngine                 *m_pWeb;
};

#endif // CloudWebWgt_H
