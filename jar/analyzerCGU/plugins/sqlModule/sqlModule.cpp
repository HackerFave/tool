#include "sqlModule.h"

SqlModule::SqlModule()
{
}
sqlOperation *SqlModule::getSqlInstance()
{
    return  new sqlOperation(NULL);
}
