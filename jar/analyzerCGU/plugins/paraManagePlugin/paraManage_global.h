#ifndef PARAMANAGE_GLOBAL_H
#define PARAMANAGE_GLOBAL_H

#include <QtCore/qglobal.h>

   #ifdef Q_OS_WIN
    #if defined(PLUGIN_PARAMANAGE_LIBRARY)
    #  define PLUGIN_PARAMANAGE_EXPORT /*Q_DECL_EXPORT*/ __declspec(dllexport)
    #else
    #  define PLUGIN_PARAMANAGE_EXPORT /*Q_DECL_IMPORT*/ __declspec(dllimport)
    #endif
  #endif

  #ifdef Q_OS_LINUX

    #if defined(PLUGIN_PARAMANAGE_LIBRARY)
    #  define PLUGIN_PARAMANAGE_EXPORT Q_DECL_EXPORT
    #else
    #  define PLUGIN_PARAMANAGE_EXPORT Q_DECL_IMPORT
    #endif
  #endif


#endif // PARAMANAGE_GLOBAL_H
