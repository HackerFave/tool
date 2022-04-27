#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <mutex>
#include <QHash>
class QPluginLoader;
class PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance()
    {
        if (P_PluginManager == nullptr)
        {
            std::lock_guard<std::mutex> lgd(_mutex);
            if (P_PluginManager == nullptr)
            {
                P_PluginManager = new PluginManager;
            }
        }
        return P_PluginManager;
    }
    static void delInstance()
    {
        std::lock_guard<std::mutex> lgd(_mutex);
        if (P_PluginManager)
        {
            delete P_PluginManager;
            P_PluginManager = nullptr;
        }
    }

    void loadAllPlugins();
    void loadPlugin(const QString &filepath,QHash<QString,QWidget*>&, const void *node);
    void unloadPlugin(const QString &filepath);
    void unloadAllPlugins();
    QPluginLoader* getPlugin(const QString &name);
    QVariant getPluginName(QPluginLoader *loader);
    void ReadPluginsInfo(QHash<QString,QString> &LuginNamePath, const QString & pluginsDirPath /*= ""*/,QHash<QString,QWidget*> & widgesHash ,const void *node);
private:
    explicit PluginManager(QObject *parent = nullptr);
    PluginManager & operator=(const PluginManager & ) = delete;
    PluginManager(const PluginManager &) = delete;

    ~PluginManager();

    QHash<QString, QPluginLoader *> m_loaders; //插件路径--QPluginLoader实例
    QHash<QString, QString> m_names; //插件路径--插件名称

    static PluginManager *P_PluginManager;
    static std::mutex _mutex;
signals:

};




#endif // PLUGINMANAGER_H
