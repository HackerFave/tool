#include "resendclass.h"
#include "log.h"

ReSendClass::ReSendClass()
{
    reSendTimer = new QTimer(this);
}

void ReSendClass::addReSendMsg(quint16 id, QByteArray array)
{
    ReSendMsgST msg;
    msg.id = id;
    msg.array = array;
    msg.lastTime = QDateTime::currentMSecsSinceEpoch();
    //msg.lastTime = QDateTime.currentDateTime().toMSecsSinceEpoch();

    LOG_DEBUG("addReSendMgs,id[0x%02x]", id);
	QMutexLocker lock(&reSendMutex);
    reSendMap[id] = msg;
    if(reSendMap.size() == 1) {
        QTimer::singleShot(RESEND_TIMEVAL, this, SLOT(timeoutReSendMsgSlot()));
    }
}

void ReSendClass::removeReSendMsg(quint16 id)
{
    QMutexLocker lock(&reSendMutex);
    LOG_DEBUG("removeReSendMgs,id[0x%02x]", id);
    reSendMap.remove(id);
}

void ReSendClass::timeoutReSendMsgSlot()
{
    qint64 nextVal = RESEND_TIMEVAL;

    qint64 curTime = QDateTime::currentMSecsSinceEpoch();
    QMutexLocker lock(&reSendMutex);
    if(reSendMap.isEmpty()) {
        return;
        //goto ret;
    }
    QMap<quint16, ReSendMsgST>::iterator it;
    for (it = reSendMap.begin(); it != reSendMap.end(); ++it ) {
        if(curTime - it.value().lastTime > RESEND_TIMEVAL) {
            udpSendMsg(it.value().array.data(), it.value().array.size());
            it.value().lastTime = curTime;
        } else {
            nextVal = (nextVal<curTime-it.value().lastTime)?nextVal:(curTime-it.value().lastTime);
        }
    }
    QTimer::singleShot(nextVal, this, SLOT(timeoutReSendMsgSlot()));
}
