#ifndef SQLMODULE_H
#define SQLMODULE_H

#include "sqlModule_global.h"
#include "sqlOperation.h"
class SQLMODULE_EXPORT SqlModule
{
public:
    SqlModule();
    sqlOperation *getSqlInstance();
private:
};

#endif // SQLMODULE_H
