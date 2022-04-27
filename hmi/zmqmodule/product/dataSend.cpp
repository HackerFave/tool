#include "dataSend.h"
#include <QDebug>

dataSend::dataSend()
{
    //测试信号槽
    connect(this,SIGNAL(signalChangeState(int,int ,int)),this,SLOT(slotChangeState(int,int ,int)));
}
Q_INVOKABLE void dataSend::dataDoWork()
{
    qDebug()<<"一个关于数据处理的类"<<"";
}
Q_INVOKABLE void dataSend::settingData()
{

}
Q_INVOKABLE QString dataSend::settigData(QString str1, QString str2)
{
    qDebug()<<str1+str2<<"测试";
    return str1+str2;
}

Q_INVOKABLE void dataSend::slotChangeState(int id1,int id2 ,int id3)
{
    qDebug()<<id1<<id2<<id3<<"测试信号槽发射----------------------";
}
