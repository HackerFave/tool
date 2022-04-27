#include "PluginManager.h"
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include "baseInterface/PluginInterface.h"
#include <QApplication>
#include <QWidget>
#include <QProcess>
#include <QWindow>
#include <qsystemdetection.h>

//定义函数指针
typedef void* (*funUi)(const void *node);
typedef std::string (*plugin)();
typedef void (*releaseUi)(void*);
PluginManager* PluginManager::P_PluginManager = nullptr;
std::mutex PluginManager::_mutex ;
PluginManager::PluginManager(QObject *parent) : QObject(parent)
{

}

PluginManager::~PluginManager()
{
    unloadAllPlugins();
    delInstance();
}

//加载所有插件
void PluginManager::loadAllPlugins()
{
    QDir pluginsdir(QDir::currentPath());
    pluginsdir.cd("plugins");

    QFileInfoList pluginsInfo = pluginsdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QHash<QString,QWidget*> hash;
    //加载插件
    //    for(QFileInfo fileinfo : pluginsInfo)
    //        loadPlugin(fileinfo.absoluteFilePath(),hash);
}

//加载其中某个插件
void PluginManager::loadPlugin(const QString &filepath,QHash<QString,QWidget*>& hashwidgets, const void *node)
{

    QLibrary *pluginLib = NULL;
    //写清楚库的路径，如果放在当前工程的目录下，路径为./
    pluginLib = new QLibrary(filepath);
    //加载动态库
    pluginLib->load();
    if (!pluginLib->isLoaded())
    {

        qDebug()<<QString("load ***%1***failed![error:%2]\n").arg(filepath)
                  .arg(pluginLib->errorString());
        return ;
    }

    //resolve得到库中函数地址
    funUi childUi = (funUi)pluginLib->resolve("CreateGui");
    plugin pluginName = (plugin)pluginLib->resolve("getPluginName");
    hashwidgets.insert(pluginName().c_str(),static_cast<QWidget*>(childUi(node)));
}

//卸载所有插件
void PluginManager::unloadAllPlugins()
{
    for(QString filepath : m_loaders.keys())
        unloadPlugin(filepath);
}
//#ifdef Q_OS_LINUX
void PluginManager::unloadPlugin(const QString &filepath)
{
    //    //卸载插件，并从内部数据结构中移除w
    //    QString pluginsPath = filepath;
    //    if (pluginsPath.isEmpty())
    //    {
    //        pluginsPath = QApplication::applicationDirPath();
    //    }
    //    QDir pluginsDir(pluginsPath);
    //    pluginsDir.cd("Plugins");

    //    QFileInfoList pluginsFile = pluginsDir.entryInfoList(QStringList() << "*.so *.dll *.lib", QDir::Files);
    //    foreach(QFileInfo fileInfo, pluginsFile)
    //    {
    //        QLibrary *pluginLib = NULL;
    //        pluginLib = new QLibrary(fileInfo.absoluteFilePath());
    //         qDebug()<<"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"<<fileInfo.absoluteFilePath();
    //        //卸载动态库
    //        if( pluginLib->unload())
    //        {

    //            delete pluginLib;
    //            pluginLib = nullptr;
    //        }
    //    }

    QString pluginsPath = filepath;
    if (filepath.isEmpty())
    {
        pluginsPath = QApplication::applicationDirPath();
    }
    QDir pluginsDir(pluginsPath);
    pluginsDir.cd("Plugins");

    QFileInfoList pluginsFile = pluginsDir.entryInfoList(QStringList() << "*.so"<<"*.dll", QDir::Files);
    foreach(QFileInfo fileInfo, pluginsFile)
    {

        QLibrary *pluginLib = NULL;
        //写清楚库的路径，如果放在当前工程的目录下，路径为./libhello.so
        pluginLib = new QLibrary(fileInfo.absoluteFilePath());

        //卸载动态库
        if( pluginLib->unload())
        {
            delete pluginLib;
            pluginLib = nullptr;
        }
    }

    //    //resolve得到库中函数地址
    //    funUi childUi = (funUi)pluginLib->resolve("CreateGui");
    //    plugin pluginName = (plugin)pluginLib->resolve("getPluginName");
    //    widgesHash.insert(pluginName().c_str(),static_cast<QWidget*>(childUi(node)));
    //    listPLuginName.append(pluginName().c_str());
    //}
}
//#endif


//获取某个插件名称
QVariant PluginManager::getPluginName(QPluginLoader *loader)
{
    if(loader)
        return m_names.value(m_loaders.key(loader));
    else
        return "nothing";
}

//根据名称获得插件
QPluginLoader *PluginManager::getPlugin(const QString &name)
{
    return m_loaders.value(m_names.key(name));
}
//热加载插件信息
void PluginManager::ReadPluginsInfo( QHash<QString,QString> &pluginNamePath,const QString & pluginsDirPath /*= ""*/,QHash<QString,QWidget*> & widgesHash, const void *node )
{
    QString pluginsPath = pluginsDirPath;
    if (pluginsDirPath.isEmpty())
    {
        pluginsPath = QApplication::applicationDirPath();
    }
    QDir pluginsDir(pluginsPath);
    pluginsDir.cd("Plugins");

    QFileInfoList pluginsFile = pluginsDir.entryInfoList(QStringList() << "*.so"<<"*.dll", QDir::Files);
    foreach(QFileInfo fileInfo, pluginsFile)
    {
        QLibrary *pluginLib = NULL;
        //写清楚库的路径，如果放在当前工程的目录下，路径为./libhello.so
        pluginLib = new QLibrary(fileInfo.absoluteFilePath());
        //加载动态库
        pluginLib->load();
        if (!pluginLib->isLoaded())
        {

            qDebug()<<QString("load ***%1***failed![error:%2]\n").arg(fileInfo.fileName())
                      .arg(pluginLib->errorString());
            return ;
        }
        //resolve得到库中函数地址
        funUi childUi = (funUi)pluginLib->resolve("CreateGui");
        plugin pluginName = (plugin)pluginLib->resolve("getPluginName");
        widgesHash.insert(pluginName().c_str(),static_cast<QWidget*>(childUi(node)));//插件名与插件指针
        pluginNamePath.insert(pluginName().c_str(),fileInfo.absoluteFilePath());//插件名与路径映射表
    }
}
