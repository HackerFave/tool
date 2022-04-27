TEMPLATE = subdirs

SUBDIRS += \
    CGUMainFrame \
#    plugins/sqlModule \
#    plugins/dataTestPlugin \
    plugins/SSLConfigPlugin \
    plugins/paraManagePlugin \
    plugins/systemInfoPlugin \
    plugins/testFormLib \
#    qBreakpad/crashApp/crash \
    public/baseWidget


############# for qBreakpad ############
## qBreakpad中需要使用到network模块
#QT += network

## 启用多线程、异常、RTTI、STL支持
#CONFIG += thread exceptions rtti stl

## without c++11 & AppKit library compiler can't solve address for symbols
#CONFIG += c++11
#macx: LIBS += -framework AppKit

## 配置头文件搜索路径和链接库路径
#INCLUDEPATH += $$PWD/qBreakpad/include
#unix {
#LIBS += -L$$PWD/qBreakpad/lib/linux -lqBreakpad
#}

#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

############# for qBreakpad ############

#HEADERS += \
#    ../../../testWork/testdata/test.h \
#    CGUMainFrame/public/procIdMethord.h \
#    CGUMainFrame/public/procIdMethord.h \
#    plugins/systemInfoPlugin/thread/workThread.h

#SOURCES += \
#    ../../../testWork/testdata/test.cpp \
#    CGUMainFrame/public/procIdMethord.cpp \
#    CGUMainFrame/public/procIdMethord.cpp \
#    plugins/systemInfoPlugin/thread/workThread.cpp

