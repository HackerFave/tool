#ifndef IPCMANAGER
#define IPCMANAGER

#include "Interface/IController.h"
#include "../include/qtzmq.h"
#include <QTimer>
#define ZMQ_ADDR_UI_L "ipc://@UILPub"
#define ZMQ_ADDR_UI_R "ipc://@UIRPub"
#define ZMQ_ADDR_UI_M "ipc://@UIMPub"

#define ZMQ_TOPIC_PROC_LEFT "ProcessLeft"
#define ZMQ_TOPIC_PROC_MID  "ProcessMid"
#define ZMQ_TOPIC_PROC_RIGHT "ProcessRight"

// 进程通讯管理器，使用ZMQ进行进程通信
class IpcManager : public IController
{
    Q_OBJECT
public:
    explicit IpcManager(QObject *parent = nullptr);
    ~IpcManager();

    static IpcManager *GetInstance(QObject *parent = nullptr);

    static void ctrlDataHandler(const std::string &topic, const void *data, int len);
    static void ctrlStatusHandler(const std::string &topic, const void *data, int len);
    static void ctrlButtonHandler(const std::string &topic, const void *data, int len);
    static void vehicleRawDataHandler(const std::string &topic, const void *data, int len);
    static void vehicleStatusHandler(const std::string &topic, const void *data, int len);
    static void vehicleDelayHandler(const std::string &topic, const void *data, int len);
    static void respRemotelHandler(const std::string &topic, const void *data, int len);
    static void respHmimsgHandler(const std::string &topic, const void *data, int len);

    static void forwardHandler(const std::string &topic, const void *data, int len);

    void ipcForward(ProcessType type,ModuleDir &moduleDir, int &cmd, QVariant &data);
    void getFromAny(ModuleDir moduleDir, int cmd, QVariant data) override;
private slots:
    /***********************测试********************/
    void slotTestZmq(const void* fbuf,int id,const uint32_t &len);
    void slotTimerTest();
    /**************************/
private:

    static IpcManager       *s_pInstance;

    ZmqSubscriber           *m_pSubControl;
    ZmqSubscriber           *m_pSubGateway;

    ZmqPublisher            *m_pPubUI;

    ZmqPublisher            *m_pPubUIL;
    ZmqPublisher            *m_pPubUIR;
    ZmqPublisher            *m_pPubUIM;
    ZmqSubscriber           *m_pSubUIL;
    ZmqSubscriber           *m_pSubUIR;
    ZmqSubscriber           *m_pSubUIM;
    QTimer                  _testTimer;
    uint   _currentId = 0;
};

#endif // IPCMANAGER
