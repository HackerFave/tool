#include "QsLog.h"
#include "QsLogDest.h"
#include "gatewayserver.h"
#include <QCoreApplication>
#include <QDir>

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
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log/gateway.txt"));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
        sLogPath, EnableLogRotation, MaxSizeBytes(20 * 1024 * 1024), MaxOldLogCount(30))); //设置log文件大小20M，保存30个文件
    logger.addDestination(fileDestination);

    DestinationPtr consoleDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(consoleDestination);

    qInstallMessageHandler(outputLog);
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<uint8_t>("uint8_t");

    qDebug() << "Start GateWayServer !!!";

    auto gateway = new GateWayServer();
    gateway->Start(8886);
    int ret = a.exec();
    Logger::destroyInstance();
    return ret;
}
