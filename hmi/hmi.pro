#-------------------------------------------------
#
# Project created by QtCreator 2020-03-09T09:56:47
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

TARGET = hmi
TEMPLATE = app

target.path = /root
INSTALLS += target

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


#LIBS += -lboost
#LIBS += -L/usr/local/MCGS/sysroot-mcgs/usr/lib/libboost_iostreams.so
#LIBS += -L/usr/local/MCGS/sysroot-mcgs/usr/lib/libboost_log.so
#LIBS += -L/usr/local/MCGS/sysroot-mcgs/usr/lib/libboost_log.so.1.76.0
#LIBS += -L/usr/local/MCGS/sysroot-mcgs/usr/lib/libboost_log_setup.so.1.76.0
#LIBS += -lavcodec -lavdevice -lavfilter \
#        -lavformat -lavutil -lpostproc \
#        -lswresample -lswscale -lzmq

#INCLUDEPATH += /usr/include \

#INCLUDEPATH +=C:\sqlite3
#LIBS += C:\sqlite3\sqlite3.dll
DEFINES +=  QT_DEPRECATED_WARNINGS

# 移植HMI的
DEFINES += MAINPATHEXCAVATORID_FEATURE
DEFINES += PROTOCAL_TYPE=2

SOURCES += main.cpp \
    HmiFunction/boundarycatch.cpp \
    HmiFunction/cmddealclass.cpp \
    HmiFunction/filemanager.cpp \
    HmiFunction/jt808.cpp \
    HmiFunction/log.cpp \
    HmiFunction/md5.cpp \
    HmiFunction/paraparsing.cpp \
    HmiFunction/posinfo.cpp \
    HmiFunction/resendclass.cpp \
    HmiFunction/stopCalClass.cpp \
    HmiFunction/stoplistdeal.cpp \
    HmiFunction/systemcmd.cpp \
    HmiFunction/udpdealclass.cpp \
    HmiFunction/worklistdeal.cpp \
    HmiUI/HmiMap/graphicsscene.cpp \
    HmiUI/HmiMap/graphicsview.cpp \
    HmiUI/HmiMap/hmimap.cpp \
    HmiUI/HmiMap/mapconsole.cpp \
    HmiUI/HmiMap/mapdatathread.cpp \
    HmiUI/HmiMap/pathboundary.cpp \
    HmiUI/HmiMap/pathcollection.cpp \
    HmiUI/HmiMap/pathframe.cpp \
    HmiUI/HmiMap/pathroad.cpp \
    HmiUI/HmiMap/pathtask.cpp \
    HmiUI/HmiMap/vehiclecrush.cpp \
    HmiUI/HmiMap/vehicleexcavator.cpp \
    HmiUI/HmiMap/vehicleframe.cpp \
    HmiUI/HmiMap/vehiclemine.cpp \
    HmiUI/delaychoice.cpp \
    HmiUI/uiCrush/abnormalcrush.cpp \
    HmiUI/uiExcavator/abnormalexcavator.cpp \
    HmiUI/uiExcavator/materchoice.cpp \
    HmiUI/uiExcavator/stopheadingchoice.cpp \
    HmiUI/uiExcavator/uiexcavator.cpp \
    HmiUI/uiExcavator/vehicleitem.cpp \
    HmiUI/uiMine/faultminechoice.cpp \
    HmiUI/uiMine/uimine.cpp \
#    Protocol/jt808.cpp \
#    Protocol/warnning.cpp \
    pub.cpp \
    HmiUI/keyboard.cpp \
    HmiUI/uiMine/workstatechoice.cpp \
    HmiUI/uiCrush/uicrush.cpp \
    HmiFunction/warnning.cpp \
    HmiUI/uiMineBaiYun/uiminebaiyun.cpp \
    HmiUI/uiDigger/abnormaldigger.cpp \
    HmiUI/uiDigger/uidigger.cpp \
    HmiFunction/roadcatch.cpp \
    HmiUI/uiCollector/abnormalcollector.cpp \
    HmiUI/uiCollector/uicollector.cpp \
    HmiUI/uiDigger/fixsetdigger.cpp \
    HmiUI/HmiMap/pathrealtime.cpp \
    HmiFunction/commonuifunction.cpp \
    HmiUI/HmiMap/pathpermission.cpp \
    HmiUI/HmiMap/stoppoints.cpp \
    HmiUI/screensaver.cpp \
    HmiUI/HmiMap/vehicleparking.cpp \
    zmqmodule/baseObject.cpp \
    zmqmodule/dataInterface.cpp \
    zmqmodule/factory/Factory.cpp \
    zmqmodule/factory/factoryObject.cpp \
    zmqmodule/product/baseProduct.cpp \
    zmqmodule/product/dataSend.cpp

RESOURCES += \
   resource.qrc

HEADERS += \
    HmiFunction/boundarycatch.h \
    HmiFunction/cmddealclass.h \
    HmiFunction/filemanager.h \
    HmiFunction/hmicmdst.h \
    HmiFunction/jt808.h \
    HmiFunction/log.h \
    HmiFunction/md5.h \
    HmiFunction/paraparsing.h \
    HmiFunction/posinfo.h \
    HmiFunction/resendclass.h \
    HmiFunction/stopCalClass.h \
    HmiFunction/stoplistdeal.h \
    HmiFunction/systemcmd.h \
    HmiFunction/udpdealclass.h \
    HmiFunction/worklistdeal.h \
    HmiUI/HmiMap/graphicsscene.h \
    HmiUI/HmiMap/graphicsview.h \
    HmiUI/HmiMap/hmimap.h \
    HmiUI/HmiMap/mapconsole.h \
    HmiUI/HmiMap/mapdatathread.h \
    HmiUI/HmiMap/pathboundary.h \
    HmiUI/HmiMap/pathcollection.h \
    HmiUI/HmiMap/pathframe.h \
    HmiUI/HmiMap/pathroad.h \
    HmiUI/HmiMap/pathtask.h \
    HmiUI/HmiMap/vehiclecrush.h \
    HmiUI/HmiMap/vehicleexcavator.h \
    HmiUI/HmiMap/vehicleframe.h \
    HmiUI/HmiMap/vehiclemine.h \
    HmiUI/delaychoice.h \
    HmiUI/uiCrush/abnormalcrush.h \
    HmiUI/uiExcavator/abnormalexcavator.h \
    HmiUI/uiExcavator/materchoice.h \
    HmiUI/uiExcavator/stopheadingchoice.h \
    HmiUI/uiExcavator/uiexcavator.h \
    HmiUI/uiExcavator/vehicleitem.h \
    HmiUI/uiMine/faultminechoice.h \
    HmiUI/uiMine/uimine.h \
#    Protocol/jt808.h \
#    Protocol/warnning.h \
    pub.h \
    HmiUI/keyboard.h \
    HmiUI/uiMine/workstatechoice.h \
    HmiUI/uiCrush/uicrush.h \
    HmiFunction/warnning.h \
    HmiUI/uiMineBaiYun/uiminebaiyun.h \
    HmiUI/uiDigger/abnormaldigger.h \
    HmiUI/uiDigger/uidigger.h \
    HmiFunction/roadcatch.h \
    HmiUI/uiCollector/abnormalcollector.h \
    HmiUI/uiCollector/uicollector.h \
    HmiUI/uiDigger/fixsetdigger.h \
    HmiUI/HmiMap/pathrealtime.h \
    HmiUI/HmiMap/pathpermission.h \
    HmiUI/HmiMap/stoppoints.h \
    System/common.h \
    HmiFunction/commonuifunction.h \
    HmiUI/screensaver.h \
    HmiUI/HmiMap/vehicleparking.h \
    zmqmodule/baseObject.h \
    zmqmodule/dataInterface.h \
    zmqmodule/dataSend.h \
    zmqmodule/factory/Factory.h \
    zmqmodule/factory/factoryObject.h \
    zmqmodule/product/baseProduct.h \
    zmqmodule/product/dataSend.h \
    zmqmodule/topicCommon.h

FORMS += \
    HmiUI/HmiMap/hmimap.ui \
    HmiUI/HmiMap/mapconsole.ui \
    HmiUI/delaychoice.ui \
    HmiUI/uiCrush/abnormalcrush.ui \
    HmiUI/uiExcavator/abnormalexcavator.ui \
    HmiUI/uiExcavator/materchoice.ui \
    HmiUI/uiExcavator/stopheadingchoice.ui \
    HmiUI/uiExcavator/uiexcavator.ui \
    HmiUI/uiMine/faultminechoice.ui \
    HmiUI/uiMine/uimine.ui \
    HmiUI/keyboard.ui \
    HmiUI/uiMine/workstatechoice.ui \
    HmiUI/uiCrush/uicrush.ui \
    HmiUI/uiMineBaiYun/uiminebaiyun.ui \
    HmiUI/uiDigger/abnormaldigger.ui \
    HmiUI/uiDigger/uidigger.ui \
    HmiUI/uiCollector/abnormalcollector.ui \
    HmiUI/uiCollector/uicollector.ui \
    HmiUI/uiDigger/fixsetdigger.ui \
    HmiUI/screensaver.ui

INCLUDEPATH +=/usr/local/MCGS/sysroot-mcgs/usr/include/
INCLUDEPATH +=/usr/local/MCGS/sysroot-mcgs/usr/include/boost/


