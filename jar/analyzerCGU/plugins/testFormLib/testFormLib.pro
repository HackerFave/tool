QT += gui core
QT += widgets sql
TEMPLATE = lib
DEFINES += PLUGIN_TESTFORMLIB_LIBRARY

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
    CGU_Widget.cpp \
    form.cpp \
    thread/baseThread.cpp \
    thread/testThread.cpp \
    thread/workThread.cpp

HEADERS += \
    CGU_Widget.h \
    form.h \
    form_global.h \
    thread/baseThread.h \
    thread/testThread.h \
    thread/workThread.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
    INCLUDEPATH +=/usr/local/include/fastdds/rtps
    INCLUDEPATH +=/usr/local/include/fastdds/dds/domain
    DEPENDPATH += /usr/local/include/fastdds/rtps
    INCLUDEPATH += $$PWD/../sqlModule
    INCLUDEPATH += $$PWD/../../public/baseWidget
    LIBS +=-L/usr/local/lib -lfastrtps
    LIBS +=-L$$PWD/../../LINUX/lib -lsqlModule -lbaseWidget

    CONFIG(debug, debug|release){
        DESTDIR =$$PWD/../../build/debug/Plugins
    }else{
        DESTDIR =$$PWD/../../build/release/Plugins
    }
}
win32{
    INCLUDEPATH += $$PWD/../../public/baseWidget
#    LIBS +=-L$$PWD/../../WIN/lib  -lbaseWidget
    LIBS +=-L$$PWD/../../WIN/lib -lbaseWidget

    CONFIG(debug, debug|release){/
         DESTDIR =$$PWD/../../build/debug/Plugins
    }else{
         DESTDIR =$$PWD/../../build/release/Plugins
    }
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    form.ui



