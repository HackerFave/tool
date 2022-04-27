#ifndef DATAINTERFACEHMI_H
#define DATAINTERFACEHMI_H

#include "dataInterfaceHmi_global.h"
#include "define_zmqmodule.h"

//class DATAINTERFACEHMI_EXPORT dataInterFaceHmi
//{
//public:
//     dataInterFaceHmi();
//     dataInterface * initInstance();

//};
extern "C"
{

DATAINTERFACEHMI_EXPORT dataInterface * initInstance();
}
#endif // DATAINTERFACEHMI_H
