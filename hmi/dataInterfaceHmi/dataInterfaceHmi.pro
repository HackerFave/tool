QT -= gui

TEMPLATE = lib
DEFINES += DATAINTERFACEHMI_LIBRARY

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

LIBS += -L/usr/local/MCGS/sysroot-mcgs/usr/lib -lmcgs-hal -lsqlite3

LIBS += -lgcov
LIBS += -lzmq
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
    datainterfacehmi.cpp \
    zmqmodule/baseObject.cpp \
    zmqmodule/dataInterface.cpp \
    zmqmodule/factory/Factory.cpp \
    zmqmodule/factory/factoryObject.cpp \
    zmqmodule/product/baseProduct.cpp \
    zmqmodule/product/dataSend.cpp

HEADERS += \
    dataInterfaceHmi_global.h \
    datainterfacehmi.h \
    define_zmqmodule.h \
    zmqmodule/baseObject.h \
    zmqmodule/dataInterface.h \
    zmqmodule/factory/Factory.h \
    zmqmodule/factory/factoryObject.h \
    zmqmodule/product/baseProduct.h \
    zmqmodule/product/dataSend.h \
    zmqmodule/topicCommon.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=/usr/local/MCGS/sysroot-mcgs/usr/include/
INCLUDEPATH +=/usr/local/MCGS/sysroot-mcgs/usr/include/boost/
