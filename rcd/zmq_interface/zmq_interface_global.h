#ifndef ZMQ_INTERFACE_GLOBAL_H
#define ZMQ_INTERFACE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <iostream>

#if defined(ZMQ_INTERFACE_LIBRARY)
#  define ZMQ_INTERFACE_EXPORT Q_DECL_EXPORT
#else
#  define ZMQ_INTERFACE_EXPORT Q_DECL_IMPORT
#endif

#endif // ZMQ_INTERFACE_GLOBAL_H
