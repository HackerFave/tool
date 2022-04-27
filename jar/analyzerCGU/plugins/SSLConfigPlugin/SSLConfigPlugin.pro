QT       += core gui

TEMPLATE = lib
DEFINES += PLUGIN_SSLCONFIGPLUGIN_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

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
    SSLConfigPlugin.cpp \
    SSLConfigPluginInterface.cpp \
    idl/SignatureSSL.cxx \
    idl/SignatureSSLPubSubTypes.cxx \
    idl/SignatureSSLTypeObject.cxx

HEADERS += \
          SSLConfigPlugin.h \
          SSLConfigPluginInterface.h \
          SSLConfigPlugin_global.h \
          idl/SignatureSSL.h \
          idl/SignatureSSLPubSubTypes.h \
          idl/SignatureSSLTypeObject.h
FORMS += \
    SSLConfigPlugin.ui

# Default rules for deployment.
unix{
    target.path = /usr/lib
    INCLUDEPATH += $$PWD/../../public/baseWidget
    LIBS +=-L$$PWD/../../LINUX/lib -lbaseWidget #-lsqlModule
    INCLUDEPATH += $$PWD/../../LINUX/include
    LIBS +=-L$$PWD/.././LINUX/lib -lfastrtps
    CONFIG(debug, debug|release){
            DESTDIR =$$PWD/../../build/debug/Plugins
           }else{
            DESTDIR =$$PWD/../../build/release/Plugins
    }
}
win32{
      INCLUDEPATH += $$PWD/../../WIN/include
      INCLUDEPATH += $$PWD/../../WIN/include/fastrtps/include
      INCLUDEPATH += $$PWD/../../WIN/include/fastcdr/include
      INCLUDEPATH += $$PWD/../../WIN/include/FOONATHAN_MEMORY/include/foonathan_memory

      LIBS += -L$$PWD/../../WIN/lib -lfastrtpsd-2.5 -llibfastcdrd-1.0 -lfoonathan_memory-0.7.1-dbg
    CONFIG(debug, debug|release){
            DESTDIR =$$PWD/../../build/debug/Plugins
           }else{
            DESTDIR =$$PWD/../../build/release/Plugins
    }
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
