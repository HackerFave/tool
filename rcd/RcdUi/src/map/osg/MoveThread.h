#pragma once
#include <QObject>

class MoveThread:public QObject{
    Q_OBJECT
signals:
    void moveThread(QThread*);
};


