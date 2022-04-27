#ifndef DATAINFOTHREAD_H
#define DATAINFOTHREAD_H
#include "baseInterface/baseThread.h"
#ifdef Q_OS_LINUX
#include "DynamicDataApi.h"
#endif
class dataInfoThread : public baseThread
{
public:
    dataInfoThread();

    Q_INVOKABLE void startWork() override;
    Q_INVOKABLE void stopWork() override;
    Q_INVOKABLE void run(){};
public slots:
    Q_INVOKABLE void slotDoWork() override;
    Q_INVOKABLE void slotDoWork(void *monitor) override;
private:
    static void callBackData(const std::string &str);
    void emitData(const std::string &str);
    void initPoint();
private:
    bool _isStop = false;
    bool _isRun = false;
    int _numCount = 0;
    std::map<std::string,std::list<std::string>> _data;
    static dataInfoThread *P_dataInfoThread;
};

#endif // DATAINFOTHREAD_H
