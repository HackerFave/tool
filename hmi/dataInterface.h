#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>

class dataInterface : public QObject
{
    Q_OBJECT
public:
    explicit dataInterface(QObject *parent = nullptr);

signals:

};

#endif // DATAINTERFACE_H
