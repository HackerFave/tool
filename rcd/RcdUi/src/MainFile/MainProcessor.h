#ifndef MAINPROCESSOR_H
#define MAINPROCESSOR_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include <QMap>
#include "System.h"

class MainWindow;
class IFunWidget;
class QTimer;
class IpcManager;

class MainProcessor : public QObject
{
    enum MessageType
    {
        MT_ThisProc=0, //本进程消息
        MT_Broadcast,   //广播全进程消息
        MT_LeftProc,    //左屏幕进程
        MT_MidProc,     //中屏幕进程
        MT_RightProc    //右屏幕进程
    };

    Q_OBJECT
public:
    explicit MainProcessor(QObject *parent = nullptr);
    ~MainProcessor();
    bool eventFilter(QObject *target, QEvent *event);
    void run();

private:
    void initSystem();
    void initFunModule();
    void initStyle();
    void initMainWindow();
    MessageType whichMsg(ModuleType &,int &,QVariant &);
    void switchLayout(int layoutType);
    void switchWidget(ScreenAreaType module,int wgtId);
    void mainProcess(ModuleDir &moduleDir,int &cmd, QVariant &data);
    void keyPressHandle(QObject *target,int key);

    void doFromAny(ModuleDir moduleDir,int &cmd, QVariant &data);

public slots:
    void getFromAny(ModuleDir moduleDir,int cmd, QVariant data);  //主要命令槽  接收处理所有模块命令
    void slotGifForm(QMap<QString,bool>);//启动录屏
signals:
    void killProc();//关闭进程
private slots:
    void procFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:

    MainWindow                              *m_mainWinddow;//界面进程
    IpcManager                              *m_pIpcManager;//通信进程
    QMap<ScreenAreaType,IFunWidget*>        m_currentWgt;//每个屏幕映射
};

#endif // MAINPROCESSOR_H


