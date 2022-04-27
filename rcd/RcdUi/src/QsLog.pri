INCLUDEPATH += $$PWD/QsLog
#DEFINES += QS_LOG_SEPARATE_THREAD # messages are queued and written from a separate thread

SOURCES += $$PWD/QsLog/QsLogDest.cpp \
    $$PWD/QsLog/QsLog.cpp \
    $$PWD/QsLog/QsLogDestConsole.cpp \
    $$PWD/QsLog/QsLogDestFile.cpp

HEADERS += $$PWD/QsLog/QsLogDest.h \
    $$PWD/QsLog/QsLog.h \
    $$PWD/QsLog/QsLogDestConsole.h \
    $$PWD/QsLog/QsLogLevel.h \
    $$PWD/QsLog/QsLogDestFile.h

