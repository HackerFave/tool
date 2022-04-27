#ifndef BASETHREAD_H
#define BASETHREAD_H

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
#include <map>
#include <stdlib.h>
#include <list>
#include <QMetaType>
#define INFODATA std::map<std::string,std::list<std::string>>
using namespace std;
class baseThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit baseThread(QObject *parent = nullptr);
    virtual Q_INVOKABLE void startWork()=0;
    virtual Q_INVOKABLE void stopWork()=0;
public slots:
    virtual Q_INVOKABLE void slotPostWork()=0;
    virtual Q_INVOKABLE void slotPostAppendData(const void *,int,int)=0;
    virtual Q_INVOKABLE void slotSubAckAppendData(const void*,int,const uint32_t &)=0;
    virtual Q_INVOKABLE void slotTest()=0;
signals:
    Q_INVOKABLE void signalStartWork();
    Q_INVOKABLE void signalTest(QString);//测试
    Q_INVOKABLE void signalDataInfo(std::map<std::string,std::list<std::string>>,std::list<std::string>);
};

#endif // BASETHREAD_H
