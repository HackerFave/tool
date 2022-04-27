#ifndef RESENDCLASS_H
#define RESENDCLASS_H

#include "udpdealclass.h"
#include <QMap>
#include <QByteArray>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

typedef struct
{
    quint16 id;
    qint64 lastTime;
    //long long currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QByteArray array;
}ReSendMsgST;

#define RESEND_TIMEVAL      5000

class ReSendClass : public UdpDealClass
{
    Q_OBJECT
public:
    ReSendClass();
    void addReSendMsg(quint16 id, QByteArray array);
    void removeReSendMsg(quint16 id);

private:
    QTimer *reSendTimer;
    QMap<quint16, ReSendMsgST> reSendMap;
    QMutex reSendMutex;

public slots:
    void timeoutReSendMsgSlot();
};

#endif // RESENDCLASS_H
