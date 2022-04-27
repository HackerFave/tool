﻿#ifndef BASETHREAD_H
#define BASETHREAD_H

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
class baseThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit baseThread(QObject *parent = nullptr);
    virtual Q_INVOKABLE void startWork()=0;
    virtual Q_INVOKABLE void stopWork()=0;
public slots:
    virtual Q_INVOKABLE void slotPostWork()=0;
    virtual Q_INVOKABLE void slotPostAppendData(const void *,size_t,int)=0;
    virtual Q_INVOKABLE void slotSubAckAppendData(const void*,int,const uint32_t &)=0;

signals:
    Q_INVOKABLE void signalStartWork();
};

#endif // BASETHREAD_H
