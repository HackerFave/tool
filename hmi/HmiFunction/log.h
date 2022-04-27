#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QMutex>
#include <QString>
#include <QDir>
#include <QDate>
#include <QTextStream>
#include <stdio.h>

#define LOG_BASE_DIR "/hmilog/"
#define LOG_DEBUG(format, ...) \
{ \
    char logBuf[1024] = {0}; \
    sprintf(logBuf, format, ##__VA_ARGS__); \
    Log::instance()->messageOutput(QtDebugMsg, (logBuf)); \
}
#define LOG_DEBUG_HEX(arr, arrLen, format, ...) \
{ \
    char logBuf[1024] = {0}; \
    sprintf(logBuf, format, ##__VA_ARGS__); \
    for(int k = 0; k < (arrLen); k++) \
    { \
        char tmp[4] = {0}; \
        sprintf(tmp, "%02x ", (arr)[k]); \
        strcat(logBuf, tmp); \
    } \
   Log::instance()->messageOutput(QtDebugMsg, (logBuf)); \
}
/*
#define LOG_DEBUG_HEX(buf, arr, arrLen) \
{ \
    char logBuf[1024] = {0}; \
    strcat(logBuf, (buf)); \
    for(int k = 0; k < (arrLen); k++) \
    { \
        char tmp[4] = {0}; \
        sprintf(tmp, "%02x ", (arr)[i]); \
        strcat(logBuf, tmp); \
    } \
    Log::instance()->messageOutput(QtDebugMsg, (logBuf)); \
}
*/

class Log
{
public:
    ~Log();
    static Log* instance();
    void initLog();
    void messageOutput(QtMsgType type, const char *msg);
private:
    Log();
    static Log *hmiLog;
    static QMutex initMutex;
    static QMutex classMutex;
    static QMutex inMutex;
    static bool initFlag;
    static bool inFlag;
};

#endif // LOG_H
