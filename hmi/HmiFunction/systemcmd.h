#ifndef SYSTEMCMD_H
#define SYSTEMCMD_H

#include <stdlib.h>
#include <stdio.h>

#include <QProcess>
#include <QObject>
#include <QString>
#include <QStringList>

#include <QDateTime>

#include <QDir>
#include <QFile>
#include <HmiFunction/paraparsing.h>
#include "md5.h"

class SystemCmd : public QObject
{
    Q_OBJECT
public:
    explicit SystemCmd(QObject *parent = 0);

signals:
    void getMapFile(bool);
    void getTaskFile(bool, QString);

public slots:
    void getMapCmd(QByteArray, QString, QString, QString);
    void getTaskCmd(QByteArray, QString, QString);
};

#endif // SYSTEMCMD_H
