#include <QDir>
#include <QApplication>
#include "MainFile/Mainwindow.h"
#include "MainFile/MainProcessor.h"
#include "QsLog.h"
#include "QsLogDest.h"

using namespace QsLogging;
//log日志
void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    Level logLevel = QsLogging::InfoLevel;
    switch(type) {
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
    Logger& logger = Logger::instance();
    if(logger.loggingLevel() <= logLevel){
        logger.writeToLog(logLevel,msg);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString logFileName("./log/RcdUiLog");
    System::setProcessType(PT_All); 
    // 根据参数设置当前进程类型
    if(argc > 1){
        QString Argument(QString::fromLocal8Bit(argv[1]));
        if(Argument == "-R"){
            System::setProcessType(PT_RightProc);
        }else if(Argument == "-M"){
            System::setProcessType(PT_MidProc);
        }else if(Argument == "-L"){
            System::setProcessType(PT_LeftProc);
        }else{
            return 0;
        }
        logFileName += Argument;
    }

    logFileName += ".log";
    // 日志类单例和配置
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::DebugLevel); //设置日志等级
    const QString sLogPath(QDir(a.applicationDirPath()).filePath(logFileName));

    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
     sLogPath, QsLogging::EnableLogRotation, MaxSizeBytes(20*1024*1024), MaxOldLogCount(30))); //设置log文件大小20M，保存30个文件
    logger.addDestination(fileDestination);

    DestinationPtr consoleDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(consoleDestination);
    // 调用QT安装消息处理函数
    qInstallMessageHandler(messageHandler);

    // main 启动
    MainProcessor app;
    a.installEventFilter(&app);//注册事件过滤
    app.run();

    int ret = 0;
    ret = a.exec();
    Logger::destroyInstance();
    delete System::getInstance();
    return ret;
}
