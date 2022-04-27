#include "QsLog.h"
#include "QsLogDest.h"
#include "controlpub.h"
#include "iodev.h"
#include "tr300rs.h"
#include "trgearshift.h"
#include "zhongzhi.h"
#include "zmq/zmqwrap.hpp"
#include <functional>
#include <hidapi/hidapi.h>
#include <QCoreApplication>
#include <QDir>
#include <QThread>

using namespace QsLogging;

void outputLog(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    Level logLevel = QsLogging::DebugLevel;
    switch (type)
    {
    case QtDebugMsg:
        logLevel = QsLogging::DebugLevel;
        break;
    case QtInfoMsg:
        logLevel = QsLogging::InfoLevel;
        break;
    case QtWarningMsg:
        logLevel = QsLogging::WarnLevel;
        break;
    case QtCriticalMsg:
        logLevel = QsLogging::ErrorLevel;
        break;
    case QtFatalMsg:
        logLevel = QsLogging::FatalLevel;
        break;
    default:
        break;
    }
    Logger &logger = Logger::instance();
    if (logger.loggingLevel() <= logLevel)
    {
        logger.writeToLog(logLevel, msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger &logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::DebugLevel); //设置日志等级
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log/control.txt"));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
        sLogPath, EnableLogRotation, MaxSizeBytes(20 * 1024 * 1024), MaxOldLogCount(30))); //设置log文件大小20M，保存30个文件
    logger.addDestination(fileDestination);

    DestinationPtr consoleDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(consoleDestination);

    qInstallMessageHandler(outputLog);

    qDebug() << "Start Control !!!";
#if 1
    ControlPub pubObj;
    zhongzhi zhzObj;
    IODev iodevice;
    QObject::connect(&zhzObj,
                     SIGNAL(EmitControlData(const QByteArray &)),
                     &pubObj,
                     SLOT(HandleZhongZhiControlDataSlot(const QByteArray &)));

    QObject::connect(&zhzObj,
                     SIGNAL(EmitStatus(quint8)),
                     &pubObj,
                     SLOT(HandleZhongZhiStatusSlot(quint8)));

    QObject::connect(&zhzObj,
                     SIGNAL(EmitButton(quint32)),
                     &pubObj,
                     SLOT(HandleTr300rsButtonSlot(quint32)));

    QObject::connect(&zhzObj,
                     SIGNAL(EmitControlLight(const QByteArray &)),
                     &pubObj,
                     SLOT(HandleZhongZhiControlLightSlot(const QByteArray &)));

    QObject::connect(&iodevice, SIGNAL(EmitEstopCtrl(quint8)), &pubObj, SLOT(HandleIoDevEstopCtrlSlot(quint8)));
    QObject::connect(&iodevice, SIGNAL(EmitBucketCtrl(quint8)), &pubObj, SLOT(HandleIoDevBucketCtrlSlot(quint8)));
    QObject::connect(&iodevice, SIGNAL(EmitIoDevStatus(quint8)), &pubObj, SLOT(HandleIodevStatusSlot(quint8)));

    zhzObj.StartRun();
    int ret = a.exec();
#else
    hid_init();
    ControlPub pubObj;

    Tr300rs tr300;
    TrGearShift trGearObj;
    IODev iodevice;

    QObject::connect(&tr300, SIGNAL(EmitControlData(const QByteArray &)), &pubObj, SLOT(HandleTr300rsControlDataSlot(const QByteArray &)));
    QObject::connect(&tr300, SIGNAL(EmitButton(quint32)), &pubObj, SLOT(HandleTr300rsButtonSlot(quint32)));
    QObject::connect(&tr300, SIGNAL(EmitStatus(quint8)), &pubObj, SLOT(HandleTr300rsStatusSlot(quint8)));
    QObject::connect(&trGearObj, SIGNAL(EmitGearShift(quint8)), &pubObj, SLOT(HandleGearShiftSlot(quint8)));
    QObject::connect(&trGearObj, SIGNAL(EmitGearShiftStatus(quint8)), &pubObj, SLOT(HandleGearShiftStatusSlot(quint8)));
    QObject::connect(&iodevice, SIGNAL(EmitEstopCtrl(quint8)), &pubObj, SLOT(HandleIoDevEstopCtrlSlot(quint8)));
    QObject::connect(&iodevice, SIGNAL(EmitBucketCtrl(quint8)), &pubObj, SLOT(HandleIoDevBucketCtrlSlot(quint8)));
    QObject::connect(&iodevice, SIGNAL(EmitIoDevStatus(quint8)), &pubObj, SLOT(HandleIodevStatusSlot(quint8)));

    QThread *tr300T = QThread::create(&Tr300rs::StartRecvRawData, &tr300);
    tr300.moveToThread(tr300T);
    tr300T->start();

    QThread *trGearT = QThread::create(&TrGearShift::StartRecvRawData, &trGearObj);
    trGearObj.moveToThread(trGearT);
    trGearT->start();

    int ret = a.exec();
    hid_exit();
    delete trGearT;
    delete tr300T;
#endif
    return ret;
}
