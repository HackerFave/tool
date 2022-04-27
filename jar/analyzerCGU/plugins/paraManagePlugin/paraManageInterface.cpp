#include "paraManageInterface.h"
#include "paraManage.h"
void* CreateGui(const void *node)
{
    return (void*)new paraManage(NULL,node);
}

void ReleaseGui(void* gui)
{
    if (gui != NULL) {
        paraManage* ui = (paraManage*)gui;
        delete ui;
        gui = NULL;
    }
}
std::string getPluginName()
{
    return "paraManage";
}

std::string getPluginVersion()
{
    return "1.0.0";
}
