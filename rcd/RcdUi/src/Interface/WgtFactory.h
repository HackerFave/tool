#ifndef WGTFACTORY_H
#define WGTFACTORY_H

#include "IFunWidget.h"

class WgtFactory
{
public:
    static void     initWidget();
    static IFunWidget* getFunWidget(ModuleType type);
};

#endif // WGTFACTORY_H
