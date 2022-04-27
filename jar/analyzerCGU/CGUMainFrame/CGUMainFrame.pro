QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
#TEMPLATE = subdirs

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend/dataInfoThread.cpp \
    backend/initThread.cpp \
    backend/workThread.cpp \
    baseInterface/baseThread.cpp \
    idl/commonIdlInfo.cxx \
    idl/commonIdlInfoTypeObject.cxx \
    idl/systemInfo.cxx \
    idl/systemInfoPubSubTypes.cxx \
    idl/systemInfoTypeObject.cxx \
    main.cpp \
    mainForm.cpp \
    manager/PluginManager.cpp \
    message/messageLable.cpp \
    message/outputForm.cpp \
    public/publicClass.cpp
HEADERS += \
    backend/dataInfoThread.h \
    backend/initThread.h \
    backend/workThread.h \
    baseInterface/PluginInterface.h \
    baseInterface/baseThread.h \
    idl/commonIdlInfo.h \
    idl/commonIdlInfoTypeObject.h \
    idl/systemInfo.h \
    idl/systemInfoPubSubTypes.h \
    idl/systemInfoTypeObject.h \
    mainForm.h \
    manager/PluginManager.h \
    message/messageLable.h \
    message/outputForm.h \
    public/publicClass.h


FORMS += \
    mainForm.ui \
    message/messageLable.ui \
    message/outputForm.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{
    CONFIG(debug, debug|release){
      INCLUDEPATH += $$PWD/../include
      INCLUDEPATH += $$PWD/../WIN/include
      INCLUDEPATH += $$PWD/../WIN/include/fastrtps/include
      INCLUDEPATH += $$PWD/../WIN/include/fastcdr/include
      INCLUDEPATH += $$PWD/../WIN/include/FOONATHAN_MEMORY/include/foonathan_memory

      LIBS += -L$$PWD/../WIN/lib -lfastrtpsd-2.5 -llibfastcdrd-1.0 -lfoonathan_memory-0.7.1-dbg
      DESTDIR =$$PWD/../build/debug
    } else {
       DESTDIR =$$PWD/../build/release
    }
}

unix{
     INCLUDEPATH +=$$PWD/../plugins/backendData
     INCLUDEPATH += $$PWD/../LINUX/include
     LIBS +=-L$$PWD/../LINUX/lib -lDynamicDataSubscriber -lfastrtps  -lfastcdr
     CONFIG(debug, debug|release){/
          DESTDIR =$$PWD/../build/debug
    }else{
          DESTDIR =$$PWD/../build/release
    }

############ for qBreakpad ############
# qBreakpad中需要使用到network模块
QT += network

# 启用多线程、异常、RTTI、STL支持
CONFIG += thread exceptions rtti stl

# without c++11 & AppKit library compiler can't solve address for symbols
macx: LIBS += -framework AppKit

# 配置头文件搜索路径和链接库路径
INCLUDEPATH += $$PWD/../qBreakpad/include
unix {
LIBS += -L$$PWD/../qBreakpad/lib/linux -lqBreakpad
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

############ for qBreakpad ############

}


