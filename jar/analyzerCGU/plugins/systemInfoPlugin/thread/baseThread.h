#ifndef BASETHREAD_H
#define BASETHREAD_H

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
#define USE_BACKEND 1
class baseThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit baseThread(QObject *parent = nullptr);
    virtual Q_INVOKABLE void startWork()=0;
    virtual Q_INVOKABLE void stopWork()=0;
public slots:
    virtual Q_INVOKABLE void slotDoWork()=0;
    virtual Q_INVOKABLE void slotDoWork(void *monitor){};
signals:
    Q_INVOKABLE void signalStartWork();
    Q_INVOKABLE void signalNoticeThread(void *);
    Q_INVOKABLE void signalBackendData(const std::string &data);
    Q_INVOKABLE void signalShellCMDResult(const void* data);
    Q_INVOKABLE void signalDataDiskMonitor(const void* data);
    Q_INVOKABLE void signalDataCPUMEMMonitor(const void* data);
};

#endif // BASETHREAD_H
