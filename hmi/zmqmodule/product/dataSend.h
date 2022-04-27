#ifndef DATASEND_H
#define DATASEND_H
#include "baseProduct.h"
#include <weak_ptr.hpp>
using namespace std;
class dataSend : public baseProduct
{
public:
    dataSend();
    Q_INVOKABLE void dataDoWork() override;
    Q_INVOKABLE void settingData() override;
    Q_INVOKABLE QString settigData(QString str1, QString str2) override;
signals:
public slots:
    Q_INVOKABLE void slotChangeState(int,int ,int) override;
private:
    std::shared_ptr<dataSend> P_dataSend;
};

#endif // DATASEND_H
