#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#include <QtPlugin>

class PluginInterface {

public:
//    virtual ~PluginInterface(){};
    virtual QString getPluginName() const = 0;

};

Q_DECLARE_INTERFACE(PluginInterface,"FunNing.Plugin.PluginInterface");//注册当前类为接口 参数1注册类 参数2插件身份

#endif // PLUGININTERFACE_H
