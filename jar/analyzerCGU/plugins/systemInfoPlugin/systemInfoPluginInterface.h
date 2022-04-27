#ifndef SYSTEMINFOPLUGININTERFACE_H
#define SYSTEMINFOPLUGININTERFACE_H
#include <QtCore/qglobal.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
#if defined(PLUGIN_LIBRARY)
#  define PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_EXPORT Q_DECL_IMPORT
#endif

extern"C"
{
    PLUGIN_EXPORT void* CreateGui(const void *node);
    PLUGIN_EXPORT void ReleaseGui(void* gui);
    PLUGIN_EXPORT std::string getPluginName();
    PLUGIN_EXPORT std::string getPluginVersion();
}

#endif // SYSTEMINFOPLUGININTERFACE_H
