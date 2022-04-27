#ifndef FRONTPLAYTHREAD_H
#define FRONTPLAYTHREAD_H

#include <QObject>

class frontPlayThread : public QObject
{
    Q_OBJECT
public:
    explicit frontPlayThread(QObject *parent = nullptr);

signals:

};

#endif // FRONTPLAYTHREAD_H
