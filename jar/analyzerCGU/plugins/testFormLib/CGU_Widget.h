#ifndef CGU_WIDGET_H
#define CGU_WIDGET_H

#include <QtCore/qglobal.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

#ifdef Q_OS_WIN
    #if defined(TEST_LIBRARY)
    #  define TEST_EXPORT /*Q_DECL_EXPORT*/ __declspec(dllexport)
    #else
    #  define TEST_EXPORT /*Q_DECL_IMPORT*/ __declspec(dllimport)
    #endif
#else
    #if defined(TEST_LIBRARY)
    #  define TEST_EXPORT Q_DECL_EXPORT
    #else
    #  define TEST_EXPORT Q_DECL_IMPORT
    #endif
#endif

extern"C"
{
    TEST_EXPORT void* CreateGui(const void *node);
    TEST_EXPORT void ReleaseGui(void* gui);
    TEST_EXPORT std::string getPluginName();
}

#endif // CGU_WIDGET_H
