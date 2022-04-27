QT -= gui

TEMPLATE = lib
DEFINES += ZMQ_INTERFACE_LIBRARY

CONFIG += c++11

LIBS += -L$$PWD/lib/ #-lsqlite3
LIBS += -lzmq
LIBS += -lgcov
LIBS += -lboost_log_setup
LIBS += -lboost_system
LIBS += -lboost_log
LIBS += -lboost_graph
LIBS += -lzmq_wrapper

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
    zmq_interface.cpp \
    zmqmodule/baseObject.cpp \
    zmqmodule/dataInterface.cpp \
    zmqmodule/factory/Factory.cpp \
    zmqmodule/factory/factoryObject.cpp \
    zmqmodule/product/baseProduct.cpp \
    zmqmodule/product/dataPost.cpp \
    zmqmodule/product/dataSend.cpp \
    zmqmodule/thread/IPC_Thread.cpp \
    zmqmodule/thread/baseThread.cpp \
    zmqmodule/thread/subThreadPool.cpp \
    zmqmodule/thread/workThread.cpp

HEADERS += \
    zmq_interface_global.h \
    zmq_interface.h \
    zmqmodule/baseObject.h \
    zmqmodule/dataInterface.h \
    zmqmodule/define_zmqmodule.h \
    zmqmodule/factory/Factory.h \
    zmqmodule/factory/factoryObject.h \
    zmqmodule/product/baseProduct.h \
    zmqmodule/product/dataPost.h \
    zmqmodule/product/dataSend.h \
    zmqmodule/thread/IPC_Thread.h \
    zmqmodule/thread/baseThread.h \
    zmqmodule/thread/subThreadPool.h \
    zmqmodule/thread/workThread.h \
    zmqmodule/topicCommon.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ./zmqmodule
INCLUDEPATH += ./zmqmodule/product
INCLUDEPATH += ./zmqmodule/factory
INCLUDEPATH += ./zmqmodule/thread
INCLUDEPATH += ./platform/framework/common/include
INCLUDEPATH += ./platform/framework/common/include
INCLUDEPATH += ./platform/framework/fbs/out
INCLUDEPATH += ./platform/framework/zmq_wrapper/include
