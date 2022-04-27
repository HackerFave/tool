#include "CGU_Widget.h"
#include "form.h"
void* CreateGui(const void *node)
{
    return (void*)new Form(NULL,node);
}

void ReleaseGui(void* gui)
{
    if (gui != NULL) {
        Form* ui = (Form*)gui;
        delete ui;
        gui = NULL;
    }
}
std::string getPluginName()
{
    return "CGU_Widget";
}
