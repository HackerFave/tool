#include "SSLConfigPluginInterface.h"
#include "SSLConfigPlugin.h"
void* CreateGui(const void *node)
{
    return (void*)new SSLConfigPlugin(NULL,node);
}

void ReleaseGui(void* gui)
{
    if (gui != NULL) {
        SSLConfigPlugin* ui = (SSLConfigPlugin*)gui;
        delete ui;
        gui = NULL;
    }
}
std::string getPluginName()
{
    return "SSLConfigPlugin";
}

std::string getPluginVersion()
{
    return "1.0.0";
}
