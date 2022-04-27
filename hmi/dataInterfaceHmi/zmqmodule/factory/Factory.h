#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <QMetaObject>
#include "factoryObject.h"
#include "../product/dataSend.h"
class Factory : public QObject
{
    Q_OBJECT
public:
    explicit Factory(QObject *parent = nullptr);
    void createProductClass(string classname);//不需要判断语句，直接根据字符创生产对应实例，方便扩展
    void initMethods();
public:
    baseProduct *_Product = nullptr;
signals:

};

class RegisterAction{
public:
    RegisterAction(string className,create_fun ptrCreateFn){
        factoryObject::GetInstance().RegisterClass(className,ptrCreateFn);
    }
};
#endif // FACTORY_H
