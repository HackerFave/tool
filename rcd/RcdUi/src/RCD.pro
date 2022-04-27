#-------------------------------------------------
#
# Project created by QtCreator 2020-07-24T13:30:15
#
#-------------------------------------------------
include(QsLog.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  opengl gui network  concurrent quick sql webenginewidgets
TARGET = RCD
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES +=  QT_NO_WARNING_OUTPUT \
#            QT_NO_DEBUG_OUTPUT \
#            QT_MESSAGELOGCONTEXT
DEFINES +=  QT_DEPRECATED_WARNINGS

# 移植HMI的
DEFINES += MAINPATHEXCAVATORID_FEATURE
DEFINES += PROTOCAL_TYPE=2
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

INCLUDEPATH += $$PWD/../../zmq_interface/
INCLUDEPATH += $$PWD/../../zmq_interface/zmqmodule
INCLUDEPATH += $$PWD/../../zmq_interface/zmqmodule/product
INCLUDEPATH += $$PWD/../../zmq_interface/zmqmodule/factory
INCLUDEPATH += $$PWD/../../zmq_interface/zmqmodule/thread
INCLUDEPATH += $$PWD/../../zmq_interface/platform/framework/common/include
INCLUDEPATH += $$PWD/../../zmq_interface/platform/framework/common/include
INCLUDEPATH += $$PWD/../../zmq_interface/platform/framework/fbs/out
INCLUDEPATH += $$PWD/../../zmq_interface/platform/framework/zmq_wrapper/include


#LIBS += -L$$PWD/../../zmq_interface/lib

#LIBS += -lgcov
#LIBS += -lzmq
#LIBS += -lboost_log_setup
#LIBS += -lboost_system
#LIBS += -lboost_log
#LIBS += -lboost_graph
#LIBS += -lzmq_wrapper
#LIBS += -lzmq_interface

SOURCES += \
        Component/AnalogClock.cpp \
        Component/CameraWidget.cpp \
        Component/Dashboard.cpp \
        Component/GearWgt.cpp \
        Component/OpenglWidget.cpp \
        Component/PageWgt.cpp \
        Component/ReversingTrackLine.cpp \
        Component/RingDial.cpp \
        Component/SteerWheel.cpp \
        Component/StreamThread.cpp \
        Component/TGFrame.cpp \
        Component/TitleWgt.cpp \
        Component/TriangleFill.cpp \
        Component/WebEngine.cpp \
        Component/backTrackLine.cpp \
#        Component/frontPlayThread.cpp \
        FunModule/InteractiveManager.cpp \
        FunModule/IpcManager.cpp \
        Interface/IController.cpp \
        Interface/IFunWidget.cpp \
        Interface/IModel.cpp \
        Interface/WgtFactory.cpp \
        MainFile/LeftWgt.cpp \
        MainFile/MainProcessor.cpp \
        MainFile/RightWgt.cpp \
        MainFile/MidWgt.cpp \
        MainFile/Mainwindow.cpp \
        Protocol/HmiCmdPackage.cpp \
        Protocol/ZmqMsgPackage.cpp \
        System.cpp \
        Widget/AllInfoWgt/AllInfoCL.cpp \
        Widget/AllInfoWgt/AllInfoModel.cpp \
        Widget/AllInfoWgt/AllInfoWgt.cpp \
        Widget/CloudWebWgt/CloudWebCL.cpp \
        Widget/CloudWebWgt/CloudWebModel.cpp \
        Widget/CloudWebWgt/CloudWebWgt.cpp \
        Widget/DriveCameraWgt/DriveCameraCL.cpp \
        Widget/DriveCameraWgt/DriveCameraWgt.cpp \
        Widget/LeftCameraWgt/LeftCameraCL.cpp \
        Widget/LeftCameraWgt/LeftCameraModel.cpp \
        Widget/LeftCameraWgt/LeftCameraWgt.cpp \
        Widget/MainCameraWgt/MainCameraCL.cpp \
        Widget/MainCameraWgt/MainCameraModel.cpp \
        Widget/MainCameraWgt/MainCameraWgt.cpp \
        Widget/MainMonitorWgt/MainMonitorCL.cpp \
        Widget/MainMonitorWgt/MainMonitorModel.cpp \
        Widget/MainMonitorWgt/MainMonitorWgt.cpp \
        Widget/RightCameraWgt/RightCameraCL.cpp \
        Widget/RightCameraWgt/RightCameraModel.cpp \
        Widget/RightCameraWgt/RightCameraWgt.cpp \
        Widget/VehicleInfoWgt/VehicleInfoCL.cpp \
        Widget/VehicleInfoWgt/VehicleInfoModel.cpp \
        Widget/VehicleInfoWgt/VehicleInfoWgt.cpp \
        Widget/WarnInfoWgt/WarnInfoCL.cpp \
        Widget/WarnInfoWgt/WarnInfoModel.cpp \
        Widget/WarnInfoWgt/WarnInfoWgt.cpp \
        Widget/messageBox/messageBox.cpp \
        Widget/messageBox/messageBoxForm.cpp \
        main.cpp \
#        map/QtSimpleMap.cpp \
#        map/model/HDPolygonManager.cpp \
#        map/model/HDRoadManager.cpp \
#        map/model/LaneInfo.cpp \
#        map/model/LaneInfoManager.cpp \
#        map/model/PolyInfo.cpp \
#        map/model/SqliteReader.cpp \
#        map/osg/CalcInterPoint.cpp \
#        map/osg/EngineProxy.cpp \
#        map/osg/FlatModel.cpp \
#        map/osg/GisModel.cpp \
#        map/osg/GraphicWindowQt.cpp \
#        map/osg/GridLayer.cpp \
#        map/osg/HDBoundaryLayer.cpp \
#        map/osg/HDBoundaryLayerList.cpp \
#        map/osg/HDLaneLayer.cpp \
#        map/osg/HDLaneLayerList.cpp \
#        map/osg/HDRoadLayer.cpp \
#        map/osg/HDRoadLayerList.cpp \
#        map/osg/Layer.cpp \
#        map/osg/LayerList.cpp \
#        map/osg/Marker.cpp \
#        map/osg/MarkerShape.cpp \
#        map/osg/MoveThread.cpp \
#        map/osg/OrthoManipulator.cpp \
#        map/osg/PointShape.cpp \
#        map/osg/RenderGroup.cpp \
#        map/osg/RouteAndTaskLayer.cpp \
#        map/osg/VehicleLayer.cpp \
#        map/osg/VehicleLayerList.cpp \
#        map/osg/poly2tri/common/shapes.cc \
#        map/osg/poly2tri/sweep/advancing_front.cc \
#        map/osg/poly2tri/sweep/cdt.cc \
#        map/osg/poly2tri/sweep/sweep.cc \
#        map/osg/poly2tri/sweep/sweep_context.cc \
        public/frmgifwidget.cpp \
        public/gifwidget.cpp \
        public/movieplayer.cpp \
        public/publicclass.cpp \
        public/saveGifForm.cpp

HEADERS += \
        Component/AnalogClock.h \
        Component/CameraWidget.h \
        Component/Dashboard.h \
        Component/GearWgt.h \
        Component/OpenglWidget.h \
        Component/PageWgt.h \
        Component/ReversingTrackLine.h \
        Component/RingDial.h \
        Component/SteerWheel.h \
        Component/StreamThread.h \
        Component/TGFrame.h \
        Component/TitleWgt.h \
        Component/TriangleFill.h \
        Component/WebEngine.h \
        Component/backTrackLine.h \
#        Component/frontPlayThread.h \
        FunModule/InteractiveManager.h \
        FunModule/IpcManager.h \
        Interface/IController.h \
        Interface/IFunWidget.h \
        Interface/IModel.h \
        Interface/WgtFactory.h \
        MainFile/LeftWgt.h \
        MainFile/MainProcessor.h \
        MainFile/Mainwindow.h \
        MainFile/RightWgt.h \
        MainFile/MidWgt.h \
        Protocol/HmiCmdPackage.h \
        Protocol/ZmqMsgPackage.h \
        System.h \
        Widget/AllInfoWgt/AllInfoCL.h \
        Widget/AllInfoWgt/AllInfoModel.h \
        Widget/AllInfoWgt/AllInfoWgt.h \
        Widget/CloudWebWgt/CloudWebCL.h \
        Widget/CloudWebWgt/CloudWebModel.h \
        Widget/CloudWebWgt/CloudWebWgt.h \
        Widget/DriveCameraWgt/DriveCameraCL.h \
        Widget/DriveCameraWgt/DriveCameraWgt.h \
        Widget/LeftCameraWgt/LeftCameraCL.h \
        Widget/LeftCameraWgt/LeftCameraModel.h \
        Widget/LeftCameraWgt/LeftCameraWgt.h \
        Widget/MainCameraWgt/MainCameraCL.h \
        Widget/MainCameraWgt/MainCameraModel.h \
        Widget/MainCameraWgt/MainCameraWgt.h \
        Widget/MainMonitorWgt/MainMonitorCL.h \
        Widget/MainMonitorWgt/MainMonitorModel.h \
        Widget/MainMonitorWgt/MainMonitorWgt.h \
        Widget/RightCameraWgt/RightCameraCL.h \
        Widget/RightCameraWgt/RightCameraModel.h \
        Widget/RightCameraWgt/RightCameraWgt.h \ \
        Widget/VehicleInfoWgt/VehicleInfoCL.h \
        Widget/VehicleInfoWgt/VehicleInfoModel.h \
        Widget/VehicleInfoWgt/VehicleInfoWgt.h \
        Widget/WarnInfoWgt/WarnInfoCL.h \
        Widget/WarnInfoWgt/WarnInfoModel.h \
        Widget/WarnInfoWgt/WarnInfoWgt.h \
        Widget/messageBox/messageBox.h \
        Widget/messageBox/messageBoxForm.h \
#        map/QtSimpleMap.h \
#        map/model/HDPolygonManager.h \
#        map/model/HDRoadManager.h \
#        map/model/LaneInfo.h \
#        map/model/LaneInfoManager.h \
#        map/model/PolyInfo.h \
#        map/model/SqliteReader.h \
#        map/osg/CalcInterPoint.h \
#        map/osg/EngineProxy.h \
#        map/osg/FlatModel.h \
#        map/osg/GisModel.h \
#        map/osg/GraphicWindowQt.h \
#        map/osg/GridLayer.h \
#        map/osg/HDBoundaryLayer.h \
#        map/osg/HDBoundaryLayerList.h \
#        map/osg/HDLaneLayer.h \
#        map/osg/HDLaneLayerList.h \
#        map/osg/HDRoadLayer.h \
#        map/osg/HDRoadLayerList.h \
#        map/osg/Layer.h \
#        map/osg/LayerList.h \
#        map/osg/Marker.h \
#        map/osg/MarkerShape.h \
#        map/osg/MoveThread.h \
#        map/osg/OrthoManipulator.h \
#        map/osg/PointShape.h \
#        map/osg/RenderGroup.h \
#        map/osg/RouteAndTaskLayer.h \
#        map/osg/VehicleLayer.h \
#        map/osg/VehicleLayerList.h \
#        map/osg/ViewChangeListener.h \
#        map/osg/poly2tri/common/shapes.h \
#        map/osg/poly2tri/common/utils.h \
#        map/osg/poly2tri/poly2tri.h \
#        map/osg/poly2tri/sweep/advancing_front.h \
#        map/osg/poly2tri/sweep/cdt.h \
#        map/osg/poly2tri/sweep/sweep.h \
#        map/osg/poly2tri/sweep/sweep_context.h \
#        map/osg/proj/geodesic.h \
#        map/osg/proj/proj_api.h \
#        map/osg/proj/projects.h \
#        map/osg/proj/sqlite3.h \
#        map/osg/proj/sqlite3ext.h \
        public/frmgifwidget.h \
        public/gif.h \
        public/gifwidget.h \
        public/movieplayer.h \
        public/publicclass.h \
        public/saveGifForm.h

FORMS += \
        MainFile/LeftWgt.ui \
        MainFile/Mainwindow.ui \
        MainFile/RightWgt.ui \
        MainFile/MidWgt.ui \
        Widget/AllInfoWgt/AllInfoWgt.ui \
        Widget/CloudWebWgt/CloudWebWgt.ui \
        Widget/DriveCameraWgt/DriveCameraWgt.ui \
        Widget/LeftCameraWgt/LeftCameraWgt.ui \
        Widget/MainCameraWgt/MainCameraWgt.ui \
        Widget/MainMonitorWgt/MainMonitorWgt.ui \
        Widget/RightCameraWgt/RightCameraWgt.ui \
        Widget/VehicleInfoWgt/VehicleInfoWgt.ui \
        Widget/WarnInfoWgt/WarnInfoWgt.ui \
        Widget/messageBox/messageBoxForm.ui \
        public/frmgifwidget.ui \
        public/saveGifForm.ui

RESOURCES += \
    res.qrc

LIBS += -lavcodec -lavdevice -lavfilter \
        -lavformat -lavutil -lpostproc \
        -lswresample -lswscale -lzmq -lsqlite3
#        -lSDL2
#    INCLUDEPATH += $$PWD/third/libsqlite3/include
#    LIBS += -L$$PWD/third/libsqlite3/lib
#    INCLUDEPATH += /usr/local/include/libsqlite3
#    LIBS += = /usr/local/lib/libsqlite3
#    -lsqlite3

#LIBS +=-L$$PWD/third/libsqlite3/lib
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH += $$PWD/map/
#INCLUDEPATH += $$PWD/map/model
#INCLUDEPATH += $$PWD/map/osg
#INCLUDEPATH += $$PWD/map/osg/poly2tri
#INCLUDEPATH += $$PWD/map/osg/poly2tri/common
#INCLUDEPATH += $$PWD/map/osg/poly2tri/sweep
#INCLUDEPATH += $$PWD/map/osg/proj


#INCLUDEPATH += /usr/local/include \
#        /usr/local/include/opencv2 \
#        /usr/local/include/osg
#INCLUDEPATH += /usr/include \

#LIBS += /usr/local/lib/libopencv_calib3d.so \
#/usr/local/lib/libopencv_core.so \
#/usr/local/lib/libopencv_features2d.so \
#/usr/local/lib/libopencv_flann.so \
#/usr/local/lib/libopencv_highgui.so \
#/usr/local/lib/libopencv_imgcodecs.so \
#/usr/local/lib/libopencv_imgproc.so \
#/usr/local/lib/libopencv_ml.so \
#/usr/local/lib/libopencv_objdetect.so \
#/usr/local/lib/libopencv_photo.so \
#/usr/local/lib/libopencv_shape.so \
#/usr/local/lib/libopencv_stitching.so \
#/usr/local/lib/libopencv_superres.so \
#/usr/local/lib/libopencv_videoio.so \
#/usr/local/lib/libopencv_video.so \
#/usr/local/lib/libopencv_videostab.so
#/usr/local/lib/libopencv_calib3d.so \

#LIBS +=/usr/local/lib
#LIBS += -lproj

#LIBS +=/usr/local/lib/libosg.so
#LIBS +=/usr/local/lib/libosgDB.so
#LIBS +=/usr/local/lib/libosgFX.so
#LIBS +=/usr/local/lib/libosgGA.so
#LIBS +=/usr/local/lib/libosgUI.so
#LIBS +=/usr/local/lib/libosgViewer.so
#LIBS +=/usr/local/lib/libosgText.so
#LIBS +=/usr/local/lib/libosgUtil.so
#LIBS +=/usr/local/lib/libosgSim.so
#LIBS +=/usr/local/lib/libosgShadow.so
LIBS +=/usr/local/lib/libproj.so
LIBS +=/usr/local/lib/libproj.so.12
LIBS +=/usr/local/lib/libproj.so.12.0.0
LIBS +=/usr/local/lib/libOpenThreads.so

#LIBS +=$$PWD/lib/libosg.so \
