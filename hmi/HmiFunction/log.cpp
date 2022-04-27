#include "log.h"

Log* Log::hmiLog = NULL;
QMutex Log::initMutex;
QMutex Log::classMutex;
QMutex Log::inMutex;
;bool Log::initFlag = true;
bool Log::inFlag = false;

Log::Log()
{

}

Log* Log::instance()
{
    if(hmiLog == NULL) {
        QMutexLocker locker(&classMutex);
        if(hmiLog == NULL) {
            hmiLog = new Log();
        }
    }
    return hmiLog;
}

//日志生成
void Log::messageOutput(QtMsgType type, const char *msg)
{
    {
        if(initFlag == true) {
            QMutexLocker locker(&initMutex);
            if(initFlag == true) {
                initFlag = false;
                inFlag = true;
                initLog();
            }
        }
    }

    if(inFlag == false)
        return;

    QString txtMessage;
    txtMessage.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss "));
    switch(type)
    {
    case QtDebugMsg:
        txtMessage += QString("[Debug] %1").arg(msg);
        break;
    case QtWarningMsg:
        txtMessage += QString("[Warning] %1").arg(msg);
        break;
    case QtCriticalMsg:
        txtMessage += QString("[Critical] %1").arg(msg);
        break;
    case QtFatalMsg:
        txtMessage += QString("[Fatal] %1").arg(msg);
        abort();
    default:
        return;
    }
    txtMessage += QString("\r\n");

    {
        QMutexLocker locker(&inMutex);
        QString filename;
        filename.clear();
        filename.append(LOG_BASE_DIR);
        filename.append(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QString(".log"));
        QFile file(filename);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream out(&file);
            out<<txtMessage;
        }
        file.flush();
        file.close();
    }
}

void Log::initLog()
{
    QDir logdir;
    logdir.setPath(LOG_BASE_DIR);

    if(logdir.exists())//存在日志目录
    {
        if(logdir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Time).size() >= 2)//存在2个文件
        {
            QStringList namelist = logdir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Time);
            QString firstfile = namelist.at(0);//最新文件
            QDateTime firstfiledate = QDateTime::fromString(firstfile.left(firstfile.size() - 4), "yyyy-MM-dd");
            if(firstfiledate.isValid())
            {
                if(firstfiledate.date() == QDateTime::currentDateTime().date())//当日文件存在
                {
                    for(int count = 2; count < namelist.size(); count++)//保存当日和日前文件
                    {
                        logdir.remove(namelist.at(count));
                    }
                }
                else//当日文件不存在
                {
                    for(int count = 1; count < namelist.size(); count++)//保存前日的文件
                    {
                        logdir.remove(namelist.at(count));
                    }
                }
            }
        }
    }
    else//不存在日志目录
    {
        logdir.mkdir(LOG_BASE_DIR);
    }
}
