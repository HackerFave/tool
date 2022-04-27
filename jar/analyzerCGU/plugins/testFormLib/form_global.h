#ifndef FORM_GLOBAL_H
#define FORM_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef Q_OS_WIN
    #if defined(FORM_LIBRARY)
    #  define FORM_EXPORT /*Q_DECL_EXPORT*/ __declspec(dllexport)
    #else
    #  define FORM_EXPORT /*Q_DECL_IMPORT*/ __declspec(dllimport)
    #endif
#else
    #if defined(FORM_LIBRARY)
    #  define FORM_EXPORT Q_DECL_EXPORT
#else
    #  define FORM_EXPORT Q_DECL_IMPORT
#endif

#endif
#if defined(FORM_LIBRARY)
#  define FORM_EXPORT Q_DECL_EXPORT
#else
#  define FORM_EXPORT Q_DECL_IMPORT
#endif

#endif // FORM_GLOBAL_H
