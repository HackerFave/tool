#include "systemInfoPluginInterface.h"
#include "systemInfoPlugin.h"
void* CreateGui(const void *node)
{
    return (void*)new systemInfoPlugin(NULL,node);
}

void ReleaseGui(void* gui)
{
    if (gui != NULL) {
        systemInfoPlugin* ui = (systemInfoPlugin*)gui;
        delete ui;
        gui = NULL;
    }
}
std::string getPluginName()
{
    return "systemInfoPlugin";
}

std::string getPluginVersion()
{
    return "1.0.0";
}
