#ifndef BASEPRODUCT_H
#define BASEPRODUCT_H

#include <QObject>

class baseProduct : public QObject
{
    Q_OBJECT
public:
    explicit baseProduct(QObject *parent = nullptr);
    virtual Q_INVOKABLE void dataDoWork()=0;
    virtual Q_INVOKABLE void settingData() = 0;
    virtual Q_INVOKABLE QString settigData(QString str1, QString str2) = 0;
signals:
    Q_INVOKABLE void signalChangeState(int,int,int);
public slots:
    virtual Q_INVOKABLE void slotChangeState(int,int ,int) = 0;
};

#endif // BASEPRODUCT_H
