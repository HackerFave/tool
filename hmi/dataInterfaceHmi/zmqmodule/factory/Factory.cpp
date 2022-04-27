#include "Factory.h"
#include <QDebug>
REGISTER(dataSend);//注册dataSend类

Factory::Factory(QObject *parent) : QObject(parent)
{

}
void Factory::createProductClass(string classname)//不需要判断语句，直接根据字符创生产对应实例，方便扩展
{
    this->_Product = (baseProduct*)factoryObject::GetInstance().GetClassByName(classname);

}
void Factory::initMethods()
{
    if(_Product){
        qDebug() << QMetaObject::invokeMethod( _Product, "dataDoWork");
        qDebug() << QMetaObject::invokeMethod( _Product, "settingData");
        QString str;
        qDebug() << QMetaObject::invokeMethod(_Product
                                              , "settigData"
                                              ,Qt::AutoConnection
                                              ,Q_RETURN_ARG(QString, str)
                                              ,Q_ARG(QString, ("1111111111"))
                                              ,Q_ARG(QString, ("222222222222"))
                                              );

        QMetaObject::invokeMethod(_Product, "signalChangeState", Q_ARG(int, 1), Q_ARG(int, 1), Q_ARG(int, 0));
    }
}
