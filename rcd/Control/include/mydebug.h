#ifndef MYDEBUG_H
#define MYDEBUG_H

#include <QDebug>
#define MY_DEBUG() qDebug() << "[" << this->metaObject()->className() << "] "

#endif // MYDEBUG_H
