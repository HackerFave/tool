#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "baseWidget_global.h"
#include <QWidget>
#include <QHash>
#include <QObject>

#ifdef Q_OS_WIN
class BaseWidget :public QWidget
#endif

#ifdef Q_OS_LINUX
class BASEWIDGET_EXPORT BaseWidget :public QWidget
#endif
{
    Q_OBJECT
public:
    BaseWidget(QWidget *parent = nullptr,const void *node=nullptr);
public slots:
    virtual Q_INVOKABLE void slotRecvBackendData(const std::string&)=0;//从主框架获取数据
    virtual Q_INVOKABLE void slotPluginWidgetsPointHash(QHash<QString,QWidget*>)=0;//插件界面列表
    virtual Q_INVOKABLE void slotSetThreadStatus(bool){};//设置线程状态
signals:
    virtual Q_INVOKABLE void signalPluginToBackendData(const std::string&);//发送数据到主框架 只需要emit
private:
    const void * _node = nullptr;
};

#endif // BASEWIDGET_H
