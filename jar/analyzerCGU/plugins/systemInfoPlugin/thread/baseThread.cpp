#include "baseThread.h"

baseThread::baseThread(QObject *parent ) : QObject(parent)
{
    connect(this,SIGNAL(signalStartWork()),this,SLOT(slotDoWork()));

}
