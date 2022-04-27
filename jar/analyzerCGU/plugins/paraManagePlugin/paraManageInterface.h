#ifndef PARAMANAGEINTERFACE_H
#define PARAMANAGEINTERFACE_H

#include <QtCore/qglobal.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

#ifdef Q_OS_WIN
    #if defined(TEST_LIBRARY)
    #  define PLUGIN_EXPORT /*Q_DECL_EXPORT*/ __declspec(dllexport)
    #else
    #  define PLUGIN_EXPORT /*Q_DECL_IMPORT*/ __declspec(dllimport)
    #endif
#else
    #if defined(PLUGIN_LIBRARY)
    #  define PLUGIN_EXPORT Q_DECL_EXPORT
    #else
    #  define PLUGIN_EXPORT Q_DECL_IMPORT
    #endif
#endif
extern"C"
{
    PLUGIN_EXPORT void* CreateGui(const void *node);
    PLUGIN_EXPORT void ReleaseGui(void* gui);
    PLUGIN_EXPORT std::string getPluginName();
    PLUGIN_EXPORT std::string getPluginVersion();
}

#endif // PARAMANAGEINTERFACE_H
