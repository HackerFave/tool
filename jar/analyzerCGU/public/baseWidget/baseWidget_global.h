#ifndef BASEWIDGET_GLOBAL_H
#define BASEWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

   #ifdef Q_OS_WIN
    #if defined(BASEWIDGET_LIBRARY)
    #  define BASEWIDGET_EXPORT /*Q_DECL_EXPORT*/ __declspec(dllexport)
    #else
    #  define BASEWIDGET_EXPORT /*Q_DECL_IMPORT*/ __declspec(dllimport)
    #endif
  #endif

  #ifdef Q_OS_LINUX

    #if defined(BASEWIDGET_LIBRARY)
    #  define BASEWIDGET_EXPORT Q_DECL_EXPORT
    #else
    #  define BASEWIDGET_EXPORT Q_DECL_IMPORT
    #endif
  #endif


#endif // BASEWIDGET_GLOBAL_H
